// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "report.h"
#include "pointing_device.h"

#    ifndef CIRQUE_PINNACLE_X_LOWER
#        define CIRQUE_PINNACLE_X_LOWER 127 // min "reachable" X value
#    endif
#    ifndef CIRQUE_PINNACLE_X_UPPER
#        define CIRQUE_PINNACLE_X_UPPER 1919 // max "reachable" X value
#    endif
#    ifndef CIRQUE_PINNACLE_Y_LOWER
#        define CIRQUE_PINNACLE_Y_LOWER 63 // min "reachable" Y value
#    endif
#    ifndef CIRQUE_PINNACLE_Y_UPPER
#        define CIRQUE_PINNACLE_Y_UPPER 1471 // max "reachable" Y value
#    endif
#    ifndef CIRQUE_PINNACLE_X_RANGE
#        define CIRQUE_PINNACLE_X_RANGE (CIRQUE_PINNACLE_X_UPPER - CIRQUE_PINNACLE_X_LOWER)
#    endif
#    ifndef CIRQUE_PINNACLE_Y_RANGE
#        define CIRQUE_PINNACLE_Y_RANGE (CIRQUE_PINNACLE_Y_UPPER - CIRQUE_PINNACLE_Y_LOWER)
#    endif

#define NAVIGATOR_TRACKPAD_READ 7
#define NAVIGATOR_TRACKPAD_TAPPING_TERM 100
#define NAVIGATOR_TRACKPAD_TAP_DEBOUNCE 100

#ifndef NAVIGATOR_TRACKPAD_ADDRESS
#    define NAVIGATOR_TRACKPAD_ADDRESS 0x58
#endif

#ifndef NAVIGATOR_TRACKPAD_TIMEOUT
#    define NAVIGATOR_TRACKPAD_TIMEOUT 100
#endif

#define NAVIGATOR_TRACKPAD_PTP_MODE
#if !defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE) && !defined(NAVIGATOR_TRACKPAD_PTP_MODE)
#    define NAVIGATOR_TRACKPAD_PTP_MODE
#endif

#define CGEN6_MAX_PACKET_SIZE 17
#define CGEN6_PTP_REPORT_ID 0x01
#define CGEN6_MOUSE_REPORT_ID 0x06
#define CGEN6_ABSOLUTE_REPORT_ID 0x09

// C3 error codes when reading memory
#define CGEN6_SUCCESS 0x00
#define CGEN6_CKSUM_FAILED 0x01
#define CGEN6_LEN_MISMATCH 0x02
#define CGEN6_I2C_FAILED 0x03

// C3 register addresses
#define CGEN6_REG_BASE 0x20000800
#define CGEN6_HARDWARE_ID CGEN6_REG_BASE + 0x08
#define CGEN6_FIRMWARE_ID CGEN6_REG_BASE + 0x09
#define CGEN6_FIRMWARE_REV CGEN6_REG_BASE + 0x10
#define CGEN6_VENDOR_ID CGEN6_REG_BASE + 0x0A
#define CGEN6_PRODUCT_ID CGEN6_REG_BASE + 0x0C
#define CGEN6_VERSION_ID CGEN6_REG_BASE + 0x0E
#define CGEN6_FEED_CONFIG4 0x200E000B
#define CGEN6_FEED_CONFIG3 0x200E000A
#define CGEN6_SYS_CONFIG1 0x20000008
#define CGEN6_XY_CONFIG 0x20080018
#define CGEN6_SFR_BASE 0x40000008
#define CGEN6_GPIO_BASE 0x00052000

#define CPI_TICKS 7
#define DEFAULT_CPI_TICK 4
#define CPI_1 200
#define CPI_2 400
#define CPI_3 800
#define CPI_4 1024
#define CPI_5 1400
#define CPI_6 1800
#define CPI_7 2048

#ifndef NAVIGATOR_TRACKPAD_SCROLL_DIVIDER
#    define NAVIGATOR_TRACKPAD_SCROLL_DIVIDER 10
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
#    ifndef MOUSE_EXTENDED_REPORT
#        define MOUSE_EXTENDED_REPORT
#    endif
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t ts;
    uint8_t  id;
    uint8_t  confidence;
    uint8_t  tip;
    uint8_t  contact_count;
    uint8_t  buttons;
} cgen6_report_t;
#endif

#if defined(NAVIGATOR_TRACKPAD_ABSOLUTE_MODE)
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t  palm;
    uint8_t  z;
} finger_data_t;

typedef struct {
    finger_data_t fingers[3]; // Cirque support 5 fingers, we only need 3 for our application
    uint8_t       contact_flags;
    uint8_t       buttons;
} cgen6_report_t;
#endif

#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
typedef struct {
    uint8_t buttons;
    int8_t  xDelta;
    int8_t  yDelta;
    int8_t  scrollDelta;
    int8_t  panDelta;
} cgen6_report_t;
#endif

const pointing_device_driver_t navigator_trackpad_pointing_device_driver;
void           navigator_trackpad_device_init(void);
report_mouse_t navigator_trackpad_get_report(report_mouse_t mouse_report);
uint16_t       navigator_trackpad_get_cpi(void);
void           navigator_trackpad_set_cpi(uint16_t cpi);
void           restore_cpi(uint8_t cpi);
