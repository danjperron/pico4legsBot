
1 - add gamepad_hw.c and hid_app.c in ports/rp2 <br>
2 - insert function poll_gamepad(). in line 84 of main.c<br>

          extern void hid_app_task(void);
               void poll_gamepad(void)
          {
                     tuh_task();
                     hid_app_task();
          }

3 - in main.c line 173. I added the poll_gamepad globally and force the hat to be center until it connects <br>


      // Initialise sub-systems.
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

        // from gamepad import GamePad
        gp = GamePad()
        gp.hat() -> will return (x,y) coordonate of the hat
        gp.get() -> will return the 10 bytes record of the gamepad. X,Y,Z,Rx,Ry,Hat, buttons. 32 bits for buttons
        
