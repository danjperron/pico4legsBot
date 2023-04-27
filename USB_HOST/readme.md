PRELIMINARY 


Host HID mode for gamepad modification  on micropython version 1.19 & 1.20

N.B. I do have the files modified on the github.

In /micropython/ports/rp2 add files<br>
  gamepad.c<br>
  hid_app.c<br>
  tusb_port.c<br>
<br>

In /micropython/shared/tinyusb  overwrite tusb_config.h 
 ** make backup ** <br>
 
/micropython/ports/rp2/CMakeLists.txt<br>
-  on set(MICROPY_SOURCE_LIB<br> 
   remove  mp_usbd.c<br>
   remove  mp_usbd_descriptor.c<br>
-  on set(MICROPY_SOURCE_PORT<br>
   remove usbd.c<br>
   add  ${PICO_TINYUSB_PATH}/src/host/usbh.c<br>
   add  tusb_port.c<br>
   add  gamepad.c<br>
   add  hid_app.c<br>
-   on set(MICROPY_SOURCE_QSTR<br>
   add at the end   ${PROJECT_SOURCE_DIR}/gamepad.c<br>
-  just before if (MICROPY_PY_LWIP)<br>
   add target_link_libraries(${MICROPY_TARGET} tinyusb_host tinyusb_board)<br>

/micropython/ports/rp2/mpconfigport.h
-  overwrite mpconfigport.h
  ** make backup ** <br>
  
  basically in mpconfigport I enable uart serial, disable tud_task and  create tuh_task

<blockquote>
cd micropython<br>
make -C ports/rp2 BOARD=PICO_W submodules<br>
make -C mpy-cross<br>
cd ports/rp2<br>
make BOARD=PICO_W<br>
</blockquote>

The firmware.elf or firmware.uf2 will be create in build-PICO_W folder

N.B. For Pico just use PICO instead of PICO_W in the make.


Bug in Thonny!  Be aware that Thonny only see /dev/ttyAMA0. You need to flip /dev/ttyS0 to /dev/ttyAMA0.  I used "su mv" when I boot my Pi.
