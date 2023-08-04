# APPROTECT SAMPLE
**DISCLAIMER: If you enable both APPROTECT and ERASEPROTECT at the same time without setting the internal ERASEPROTECT.DISABLE register in firmware, you will no longer be able to program the nRF5340! It will be Bricked!**

I am hijacking the eraseprotect sample from @hellesvik-nordic to repurpose for approtect purposes.

# Intro
Suppose there is a desire to have lock/unlock type of Debug Access to the application core.

The nRF5340 has a control access port ([CTRL-AP](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf5340%2Fctrl-ap.html&anchor=ctrlap)) that enables control of the device when other debug access ports (DAP) have been disabled by access port protection. 

> "Access port protection (APPROTECT) blocks the debugger access to the AHB-AP, and prevents read and write access to all CPU registers and memory-mapped addresses."

The nRF5340 has a set of registers which can be set by the debugger, even if APPROTECT is enabled. The register set we're interested particularly interested in is called CTRLAP.APPROTECT.DISABLE.

# APPROTECT.DISABLE
[Debug and Trace nRF5340 Specification](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf5340%2Fdebugandtrace.html)
Within the following section's table, we are interested in the following:
![image](https://github.com/droidecahedron/nrf_ctrl-ap_approtect/assets/63935881/87674249-8811-481c-b9cd-ff46f3c29eb6)


The CTRLAP.APPROTECT.DISABLE register acts as a lock. 

Both the debugger and on-chip firmware must write the same non-zero 32-bit key value into their respective CTRLAP.APPROTECT.DISABLE registers.

[CTRLAP.APPROTECT.DISABLE  (CPU side)](https://infocenter.nordicsemi.com/topic/ps_nrf5340/ctrl-ap.html?cp=4_0_0_7_9#unique_1852475219)

[CTRLAP.APPROTECT.DISABLE (Debugger side)](https://infocenter.nordicsemi.com/topic/ps_nrf5340/ctrl-ap.html?cp=4_0_0_7_9#register.APPROTECT.DISABLE)

When the key values match, access port will be set to non-secure mode, so you will be able to debug non-secure code.

> Note: You can do a handshake with the debugger or other comms method (either via UART or CTRL-AP Mailboxes to decide on the key to set in CTRLAP.APPROTECT.DISABLE. You can then write that key to the CPU side register. Then the debugger can use that key to unlock APPROTECT. Although this does not accomplish avoiding sending the key directly from debugger to the debugger side register, this can mitigate avoiding dfu if the key is lost depending on how it's implemented.


# Install this
```py
pip install pynrfjprog
```

## As is
This code/configuration is not thoroughly tested or qualified and should be considered provided “as-is”. Please test it with your application and let me know if you find any issues.
There is also no handshaking to resolve what the key should be beforehand. The FW is just placing a static key in the appropriate register.

# Error
This sample is for the application core.

## Versions
- nRF Connect SDK: v2.4.0
- nRF7002DK or nRF5340DK
- Python 3
- pynrfjprog 10.23.0

> _Note_: You can find your pynrfjprog version with

> Linux: pip freeze | grep pynrfjprog

> Windows: pip freeze | findstr pynrfjprog

## Documentation
[CTRL-AP - Control access port](https://infocenter.nordicsemi.com/topic/ps_nrf5340/ctrl-ap.html?cp=3_0_0_7_9)

## Preparation
Change the snr numbers in the scripts to match the ID printed on the sticker on your DK, or found via:
```
nrfjprog --ids
```

## Building and Running
First, build and flash the code to a nRF5340DK.
Add --recover option. 
```
west build -p -b nrf5340dk_nrf5340_cpuapp
west flash --recover
```

The firmware will place a key in the cpu-side of APPROTECT.DISABLE.


To allow debug access, the firmware has written a key to an internal [APPROTECT.DISABLE](https://infocenter.nordicsemi.com/topic/ps_nrf5340/ctrl-ap.html?cp=4_0_0_7_9#unique_1852475219). Write the same key to another, different [APPROTECT.DISABLE](https://infocenter.nordicsemi.com/topic/ps_nrf5340/ctrl-ap.html?cp=4_0_0_7_9#register.APPROTECT.DISABLE) with the script:
```
python3 scripts/disable_approtect.py
```

### Using j-link instead of python
You can use the following if you do not wish to run the above python script to pass the key, but I prefer the python script using pynrfjprog.

```cmd
J-Link>SWDSelect
J-Link>SWDWriteDP 1 0x50000000
J-Link>SWDWriteDP 2 0x02000010
J-Link>SWDWriteAP 0 0x00000055
```
*note: 0x00000055 in the last command is the key*

For an explanation on on the 0x02000010 value in the third command, check out [the following blog](https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/allowing-debugger-access-to-an-nrf5340).
(And recall the register we are interested in is at offset 0x010, and pay close attention to the APBANKSEL field in the SELECT register)

After running this, you will find yourself able to attach the debugger! (Assuming the key matches the one in FW application.) 

After resetting, you will have to submit key again to debug.

## Fresh devkit, can't flash with the vscode gui!
try the following:
```cmd
nrfjprog --recover
```

## Trusted Firmware-M
This sample does not work with TF-M included (nrf5340dk_nrf5340_ns), since the registers used are secure, and the application is non-secure.
To make it work with TF-M, you will have to make a secure service for setup of ERASEPROTECT.
For hints on how this can be done, see [TF-M secure peripheral partion](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.0.1/nrf/samples/tfm/tfm_secure_peripheral/README.html)
