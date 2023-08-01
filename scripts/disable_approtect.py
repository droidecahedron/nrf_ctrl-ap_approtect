from pynrfjprog import LowLevel
from time import sleep
# FILL IN YOUR INFO
my_snr = 1050788780
approtect_key = 0x00000001
target_cp = LowLevel.CoProcessor.CP_APPLICATION
# FROM DATASHEET
# CTRL-AP ID according to datasheet debug and trace (2 = app, 3 = net)
ap_id = 2 if target_cp == LowLevel.CoProcessor.CP_APPLICATION else 3
# CTRL-AP addr offsets.
approtect_disable_addr = 0x010 # DEBUGGER SIDE. CPU SIDE IS 0x544. https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf5340%2Fctrl-ap.html&cp=4_0_0_7_9&anchor=register.APPROTECT.DISABLE
eraseall_status_addr = 0x008

print("Start")
with LowLevel.API("NRF53", log=True) as nrf:
    print("Connecting to debugger")
    nrf.connect_to_emu_with_snr(my_snr)

    # Write values
    print("writing key: 0x", approtect_key, " to approtect_disable")
    nrf.write_access_port_register(ap_id, approtect_disable_addr, approtect_key)

"""Are you able to do the below to read the approtect disable addr? there isnt a status equivalent."""
#     print("Wait for eraseall to be finished")
#     finished_writing = False
#     counter = 0
#     while( nrf.read_access_port_register(ap_id, eraseall_status_addr) == True):
#             counter+=1
#             print(".",end="")
#             sleep(0.01)
#     if(counter == 0):
#             print("Failed to disable eraseprotect. Try to reset nRF5340DK and run this script again.")
#     else:
#         print("\nDone. Run read_eraseprotect_status.py to test.")
