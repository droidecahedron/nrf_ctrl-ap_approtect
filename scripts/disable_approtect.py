from pynrfjprog import LowLevel
from time import sleep
# FILL IN YOUR INFO
my_snr = 1050788780
approtect_key = 0x00000055
target_cp = LowLevel.CoProcessor.CP_APPLICATION
# FROM DATASHEET
# CTRL-AP ID according to datasheet debug and trace (2 = app, 3 = net)
ap_id = 2 if target_cp == LowLevel.CoProcessor.CP_APPLICATION else 3
# CTRL-AP addr offsets.
approtect_disable_addr = 0x010 # DEBUGGER SIDE. CPU SIDE IS 0x544. https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf5340%2Fctrl-ap.html&cp=4_0_0_7_9&anchor=register.APPROTECT.DISABLE
secureapprotect_disable_addr = 0x014 # cpu side is 0x54c


print("Start")
with LowLevel.API("NRF53", log=True) as nrf:
    print("Connecting to debugger")
    nrf.connect_to_emu_with_snr(my_snr)

    # Write key value
    print("writing key: 0x", approtect_key, " to approtect_disable")
    nrf.write_access_port_register(ap_id, approtect_disable_addr, approtect_key)

    #print("writing key: 0x", approtect_key, " to secureapprotect_disable")
    #nrf.write_access_port_register(ap_id, approtect_disable_addr, approtect_key)