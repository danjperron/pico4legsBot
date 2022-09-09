PRELIMINARY 

<img src="Pico_gamepad.jpg">
1 - Add gamepad.c and hid_app.c in ports/rp2 <br>
2 - In CMakeLists.txt I added. gamepad.c  and hid_app.c in set(MICROPY_SOURCE_PORT<br>
3 - In CMakeLists.txt I added gamepad.c   in set(MICROPY_SOURCE_QSTR <br>
4-  In mpconfigport.h I change MICROPY_HW_ENABLE_USBDEV to (0) and (1) for UART<br>
5-  In mpconfigport.h add poll_gamepad() 
	    	#define MICROPY_EVENT_POLL_HOOK_FAST \
    		do { \
        	if (get_core_num() == 0) { MICROPY_HW_USBDEV_TASK_HOOK } \
        	extern void mp_handle_pending(bool); \
        	mp_handle_pending(true); \
        	extern void poll_gamepad();\
        	poll_gamepad();\
    		} while (0)

6-  In tusbconfig.h I set #define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_HOST)

7- recompile<br>

9- To get the game pad working
        import gamepad as gp
        gp.hat() -> will return (x,y) coordonate of the hat
        gp.get() -> will return the 10 bytes record of the gamepad. X,Y,Z,Rx,Ry,Hat, buttons. 32 bits for buttons
 
N.B. You need to provide 5V on Vsys and Vusb since no power will be delivered from usb port.<br>
communication will be from uart GP0 and GP1.

Some gamepads don't have the dame layout check what the gamepad.get() bytes to see what is what.

Some work to do,<br>

- Find a better way to poll the usb<br> Done!
- Use page usage. I think that the gamepad record is 5. I should check for page usage 5 .<br>
- Set default hat to 9. This will tell the gamepad is not connected yet. Done!
- remove added stuff from main.c  No need to change main.c. Done!

Bug in Thonny!  Be aware that Thonny only see /dev/ttyAMA0. You need to flip /dev/ttyS0 to /dev/ttyAMA0.  I used "su mv" when I boot my Pi.
