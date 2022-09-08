PRELIMINARY 

<img src="Pico_gamepad.jpg">
1 - add gamepad_hw.c and hid_app.c in ports/rp2 <br>
2 - insert function poll_gamepad(). in line 84 of main.c<br>

          extern void hid_app_task(void);
               void poll_gamepad(void)
          {
                     tuh_task();
                     hid_app_task();
          }

3 - in main.c line 173. I added the poll_gamepad globally and force the hat to be center until it connects <br>


        Initialise sub-systems.
        readline_init0();
        machine_pin_init();
        rp2_pio_init();
        MP_STATE_VM(PollGamePad)= (void *) &poll_gamepad;
        MP_STATE_VM(GamePadData)[5]=8; // force gamepad hat to be on center

4 - in CMakeLists.txt I added. gamepad_hw.c  and gamepad_hw.c in set(MICROPY_SOURCE_PORT

5 -  in CMakeLists.txt I added gamepad_hw.c   in set(MICROPY_SOURCE_QSTR

6-   in mpconfigport.h I change MICROPY_HW_ENABLE_USBDEV to (0)

7 - in tusbconfig.h I set #define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_HOST)

8- recompile<br>

9- To get the game pad working you need to activate the python module gamepad

        from gamepad import GamePad
        gp = GamePad()
        gp.hat() -> will return (x,y) coordonate of the hat
        gp.get() -> will return the 10 bytes record of the gamepad. X,Y,Z,Rx,Ry,Hat, buttons. 32 bits for buttons
        
N.B. You need to provide 5V on Vsys and Vusb since no power will be delivered from usb port.<br>
communication will be from uart GP0 and GP1.

Some gamepads don't have the dame layout check what the gamepad.get() bytes to see what is what.

Some work to do,<br>

- Find a better way to poll the usb<br>
- Use page usage. I think that the gamepad record is 5. I should check for page usage 5 .<br>
- Set default hat to 9. This will tell the gamepad is not connected yet. I need to modified function to take care of this.
- remove added stuff from main.c  No need to change main.c. I will add them in gamepad_hw.c

Bug in Thonny!  Be aware that Thonny only see /dev/ttyAMA0. You need to flip /dev/ttyS0 to /dev/ttyAMA0.  I used "su mv" when I boot my Pi.
