# APPROTECT SAMPLE
**DISCLAIMER: If you enable both APPROTECT and ERASEPROTECT at the same time without setting the internal ERASEPROTECT.DISABLE register in firmware, you will no longer be able to program the nRF5340! It will be Bricked!**

I am hijacking the eraseprotect sample from @hellesvik-nordic to repurpose for approtect purposes.

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

There is a j-link.exe equivalent
```cmd
J-Link>SWDSelect
J-Link>SWDWriteDP 1 0x50000000
J-Link>SWDWriteDP 2 0x02000010
J-Link>SWDWriteAP 0 <KEY>
```

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
