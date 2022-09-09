/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Options controlling how MicroPython is built, overriding defaults in py/mpconfig.h

#include <stdint.h>
#include "hardware/spi.h"
#include "hardware/sync.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "mpconfigboard.h"
#if MICROPY_HW_USB_MSC
#include "hardware/flash.h"
#endif


// Board and hardware specific configuration
#define MICROPY_HW_MCU_NAME                     "RP2040"
#define MICROPY_HW_ENABLE_UART_REPL             (1) // useful if there is no USB
#define MICROPY_HW_ENABLE_USBDEV                (0)

#ifndef MICROPY_CONFIG_ROM_LEVEL
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)
#endif

// Memory allocation policies
#define MICROPY_GC_STACK_ENTRY_TYPE             uint16_t
#define MICROPY_ALLOC_PATH_MAX                  (128)
#define MICROPY_QSTR_BYTES_IN_HASH              (1)

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_THUMB_ARMV7M               (0)
#define MICROPY_EMIT_INLINE_THUMB               (1)
#define MICROPY_EMIT_INLINE_THUMB_FLOAT         (0)

// Optimisations
#define MICROPY_OPT_COMPUTED_GOTO               (1)

// Python internal features
#define MICROPY_TRACKED_ALLOC                   (MICROPY_SSL_MBEDTLS)
#define MICROPY_READER_VFS                      (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_SCHEDULER_DEPTH                 (8)
#define MICROPY_SCHEDULER_STATIC_NODES          (1)
#ifndef MICROPY_USE_INTERNAL_ERRNO
#define MICROPY_USE_INTERNAL_ERRNO              (1)
#endif

// Fine control over Python builtins, classes, modules, etc
#define MICROPY_PY_BUILTINS_HELP_TEXT           rp2_help_text
#define MICROPY_PY_SYS_PLATFORM                 "rp2"
#define MICROPY_PY_THREAD                       (1)
#define MICROPY_PY_THREAD_GIL                   (0)
#define MICROPY_THREAD_YIELD()                  mp_handle_pending(true)

// Extended modules
#define MICROPY_EPOCH_IS_1970                   (1)
#define MICROPY_PY_UOS_INCLUDEFILE              "ports/rp2/moduos.c"
#define MICROPY_PY_UOS_UNAME                    (1)
#define MICROPY_PY_UOS_URANDOM                  (1)
#define MICROPY_PY_URE_MATCH_GROUPS             (1)
#define MICROPY_PY_URE_MATCH_SPAN_START_END     (1)
#define MICROPY_PY_UCRYPTOLIB                   (1)
#define MICROPY_PY_UTIME_MP_HAL                 (1)
#define MICROPY_PY_URANDOM_SEED_INIT_FUNC       (rosc_random_u32())
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_MACHINE_PIN_MAKE_NEW         mp_pin_make_new
#define MICROPY_PY_MACHINE_BITSTREAM            (1)
#define MICROPY_PY_MACHINE_PULSE                (1)
#define MICROPY_PY_MACHINE_PWM                  (1)
#define MICROPY_PY_MACHINE_PWM_DUTY_U16_NS      (1)
#define MICROPY_PY_MACHINE_PWM_INCLUDEFILE      "ports/rp2/machine_pwm.c"
#define MICROPY_PY_MACHINE_I2C                  (1)
#define MICROPY_PY_MACHINE_SOFTI2C              (1)
#define MICROPY_PY_MACHINE_SPI                  (1)
#define MICROPY_PY_MACHINE_SPI_MSB              (SPI_MSB_FIRST)
#define MICROPY_PY_MACHINE_SPI_LSB              (SPI_LSB_FIRST)
#define MICROPY_PY_MACHINE_SOFTSPI              (1)
#define MICROPY_PY_ONEWIRE                      (1)
#define MICROPY_VFS                             (1)
#define MICROPY_VFS_LFS2                        (1)
#define MICROPY_VFS_FAT                         (1)
#define MICROPY_SSL_MBEDTLS                     (1)
#define MICROPY_PY_LWIP_SOCK_RAW                (MICROPY_PY_LWIP)

// fatfs configuration
#define MICROPY_FATFS_ENABLE_LFN                (1)
#define MICROPY_FATFS_LFN_CODE_PAGE             437 /* 1=SFN/ANSI 437=LFN/U.S.(OEM) */
#define MICROPY_FATFS_RPATH                     (2)
#if MICROPY_HW_USB_MSC
#define MICROPY_FATFS_USE_LABEL                 (1)
#define MICROPY_FATFS_MULTI_PARTITION           (1)
// Set FatFS block size to flash sector size to avoid caching
// the flash sector in memory to support smaller block sizes.
#define MICROPY_FATFS_MAX_SS                    (FLASH_SECTOR_SIZE)
#endif

#ifndef MICROPY_BOARD_ENTER_BOOTLOADER
#define MICROPY_BOARD_ENTER_BOOTLOADER(nargs, args)
#endif

// By default networking should include sockets, ssl, websockets, webrepl, dupterm.
#if MICROPY_PY_NETWORK
#ifndef MICROPY_PY_USOCKET
#define MICROPY_PY_USOCKET              (1)
#endif
#ifndef MICROPY_PY_USSL
#define MICROPY_PY_USSL                 (1)
#endif
#ifndef MICROPY_PY_UWEBSOCKET
#define MICROPY_PY_UWEBSOCKET           (1)
#endif
#ifndef MICROPY_PY_UHASHLIB_SHA1
#define MICROPY_PY_UHASHLIB_SHA1        (1)
#endif
#ifndef MICROPY_PY_WEBREPL
#define MICROPY_PY_WEBREPL              (1)
#endif
#ifndef MICROPY_PY_OS_DUPTERM
#define MICROPY_PY_OS_DUPTERM           (1)
#endif
#endif

#if MICROPY_PY_NETWORK_CYW43
extern const struct _mp_obj_type_t mp_network_cyw43_type;
#define MICROPY_HW_NIC_CYW43 \
    { MP_ROM_QSTR(MP_QSTR_WLAN), MP_ROM_PTR(&mp_network_cyw43_type) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_IDLE), MP_ROM_INT(CYW43_LINK_DOWN) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_CONNECTING), MP_ROM_INT(CYW43_LINK_JOIN) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_WRONG_PASSWORD), MP_ROM_INT(CYW43_LINK_BADAUTH) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_NO_AP_FOUND), MP_ROM_INT(CYW43_LINK_NONET) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_CONNECT_FAIL), MP_ROM_INT(CYW43_LINK_FAIL) }, \
    { MP_ROM_QSTR(MP_QSTR_STAT_GOT_IP), MP_ROM_INT(CYW43_LINK_UP) },
#else
#define MICROPY_HW_NIC_CYW43
#endif

#if MICROPY_PY_NETWORK_NINAW10
// This Network interface requires the extended socket state.
#ifndef MICROPY_PY_USOCKET_EXTENDED_STATE
#define MICROPY_PY_USOCKET_EXTENDED_STATE   (1)
#endif
extern const struct _mod_network_nic_type_t mod_network_nic_type_nina;
#define MICROPY_HW_NIC_NINAW10              { MP_ROM_QSTR(MP_QSTR_WLAN), MP_ROM_PTR(&mod_network_nic_type_nina) },
#else
#define MICROPY_HW_NIC_NINAW10
#endif

#if MICROPY_PY_NETWORK_WIZNET5K
#if MICROPY_PY_LWIP
extern const struct _mp_obj_type_t mod_network_nic_type_wiznet5k;
#else
extern const struct _mod_network_nic_type_t mod_network_nic_type_wiznet5k;
#endif
#define MICROPY_HW_NIC_WIZNET5K             { MP_ROM_QSTR(MP_QSTR_WIZNET5K), MP_ROM_PTR(&mod_network_nic_type_wiznet5k) },
#else
#define MICROPY_HW_NIC_WIZNET5K
#endif

#ifndef MICROPY_BOARD_NETWORK_INTERFACES
#define MICROPY_BOARD_NETWORK_INTERFACES
#endif

#define MICROPY_PORT_NETWORK_INTERFACES \
    MICROPY_HW_NIC_CYW43 \
    MICROPY_HW_NIC_NINAW10  \
    MICROPY_HW_NIC_WIZNET5K \
    MICROPY_BOARD_NETWORK_INTERFACES \

#define MP_STATE_PORT MP_STATE_VM

// Miscellaneous settings

// Entering a critical section.
extern uint32_t mp_thread_begin_atomic_section(void);
extern void mp_thread_end_atomic_section(uint32_t);
#define MICROPY_BEGIN_ATOMIC_SECTION()     mp_thread_begin_atomic_section()
#define MICROPY_END_ATOMIC_SECTION(state)  mp_thread_end_atomic_section(state)

// Prevent the "lwIP task" from running when unsafe to do so.
#define MICROPY_PY_LWIP_ENTER   lwip_lock_acquire();
#define MICROPY_PY_LWIP_REENTER lwip_lock_acquire();
#define MICROPY_PY_LWIP_EXIT    lwip_lock_release();

#if MICROPY_HW_ENABLE_USBDEV
#define MICROPY_HW_USBDEV_TASK_HOOK extern void tud_task_ext(uint32_t, bool); tud_task_ext(0, false);
#define MICROPY_VM_HOOK_COUNT (10)
#define MICROPY_VM_HOOK_INIT static uint vm_hook_divisor = MICROPY_VM_HOOK_COUNT;
#define MICROPY_VM_HOOK_POLL if (get_core_num() == 0 && --vm_hook_divisor == 0) { \
        vm_hook_divisor = MICROPY_VM_HOOK_COUNT; \
        MICROPY_HW_USBDEV_TASK_HOOK \
}
#define MICROPY_VM_HOOK_LOOP MICROPY_VM_HOOK_POLL
#define MICROPY_VM_HOOK_RETURN MICROPY_VM_HOOK_POLL
#else
#define MICROPY_HW_USBDEV_TASK_HOOK
#endif

#define MICROPY_EVENT_POLL_HOOK_FAST \
    do { \
        if (get_core_num() == 0) { MICROPY_HW_USBDEV_TASK_HOOK } \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        extern void poll_gamepad();\
        poll_gamepad();\
    } while (0)

#define MICROPY_EVENT_POLL_HOOK \
    do { \
        MICROPY_EVENT_POLL_HOOK_FAST; \
        best_effort_wfe_or_timeout(make_timeout_time_ms(1)); \
    } while (0);

#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void *)((mp_uint_t)(p) | 1))

#define MP_SSIZE_MAX (0x7fffffff)
typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef intptr_t mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define BINARY_INFO_TAG_MICROPYTHON BINARY_INFO_MAKE_TAG('M', 'P')
#define BINARY_INFO_ID_MP_FROZEN 0x4a99d719
#define MICROPY_FROZEN_LIST_ITEM(name, file) bi_decl(bi_string(BINARY_INFO_TAG_MICROPYTHON, BINARY_INFO_ID_MP_FROZEN, name))

extern uint32_t rosc_random_u32(void);
extern void lwip_lock_acquire(void);
extern void lwip_lock_release(void);

extern uint32_t cyw43_country_code;
extern void cyw43_irq_init(void);
extern void cyw43_post_poll_hook(void);

#define CYW43_POST_POLL_HOOK cyw43_post_poll_hook();
#define MICROPY_CYW43_COUNTRY cyw43_country_code
