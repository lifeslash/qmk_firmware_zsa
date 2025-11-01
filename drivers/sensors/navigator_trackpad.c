// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// THIS IS A WORK IN PROGRESS, AS THE TRACKPAD IC's FIRMWARE IS STILL IN DEVELOPMENT
// DO NOT USE THIS CODE IN PRODUCTION

#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include "navigator_trackpad.h"
#include "i2c_master.h"
#include "quantum.h"
#include "timer.h"


#ifdef PROTOCOL_LUFA
# error "LUFA is not supported yet"
#endif

const pointing_device_driver_t navigator_trackpad_pointing_device_driver = {.init = navigator_trackpad_device_init, .get_report = navigator_trackpad_get_report, .get_cpi = navigator_trackpad_get_cpi, .set_cpi = navigator_trackpad_set_cpi};

uint16_t    current_cpi = DEFAULT_CPI_TICK;
uint32_t    gpio_offset_addr;
uint8_t     has_motion = 0;
extern bool set_scrolling;
bool        in_motion;
bool        touchpad_init;

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
cgen6_report_t ptp_report;
bool           prev_ptp_flag, prev_tap_clear = false;
uint8_t        last_contact_count = 0;
uint16_t       prev_ptp_x, prev_ptp_y;
uint16_t       tap_timer = 0;
int16_t        ptp_delta_x, ptp_delta_y;
#endif

i2c_status_t cirque_gen6_read_report(uint8_t *data, uint16_t cnt) {
    i2c_status_t res = i2c_receive(NAVIGATOR_TRACKPAD_ADDRESS, data, cnt, NAVIGATOR_TRACKPAD_TIMEOUT);
    wait_us(cnt * 15);
    return res;
}

void cirque_gen6_clear(void) {
    uint8_t buf[CGEN6_MAX_PACKET_SIZE];
    for (uint8_t i = 0; i < 5; i++) {
        wait_ms(1);
        if (cirque_gen6_read_report(buf, CGEN6_MAX_PACKET_SIZE) != I2C_STATUS_SUCCESS) {
            break;
        }
    }
}

uint8_t cirque_gen6_read_memory(uint32_t addr, uint8_t *data, uint16_t cnt) {
    uint8_t  cksum = 0;
    uint8_t  res   = CGEN6_SUCCESS;
    uint8_t  len[2];
    uint16_t read = 0;

    uint8_t preamble[8] = {0x01, 0x09, (uint8_t)(addr & (uint32_t)0x000000FF), (uint8_t)((addr & 0x0000FF00) >> 8), (uint8_t)((addr & 0x00FF0000) >> 16), (uint8_t)((addr & 0xFF000000) >> 24), (uint8_t)(cnt & 0x00FF), (uint8_t)((cnt & 0xFF00) >> 8)};

    // Read the length of the data + 3 bytes (first 2 bytes for the length and the last byte for the checksum)
    // Create a buffer to store the data
    uint8_t buf[cnt + 3];
    if (i2c_transmit_and_receive(NAVIGATOR_TRACKPAD_ADDRESS, preamble, 8, buf, cnt + 3, NAVIGATOR_TRACKPAD_TIMEOUT) != I2C_STATUS_SUCCESS) {
        res |= CGEN6_I2C_FAILED;
    }

    // Read the data length
    for (uint8_t i = 0; i < 2; i++) {
        cksum += len[i] = buf[i];
        read++;
    }

    // Populate the data buffer
    for (uint16_t i = 2; i < cnt + 2; i++) {
        cksum += data[i - 2] = buf[i];
        read++;
    }

    // Check the checksum
    if (cksum != buf[read]) {
        res |= CGEN6_CKSUM_FAILED;
    }

    // Check the length (incremented first to account for the checksum)
    if (++read != (len[0] | (len[1] << 8))) {
        res |= CGEN6_LEN_MISMATCH;
    }

    wait_ms(1);

    return res;
}

uint8_t cirque_gen6_write_memory(uint32_t addr, uint8_t *data, uint16_t cnt) {
    uint8_t res   = CGEN6_SUCCESS;
    uint8_t cksum = 0, i = 0;
    uint8_t preamble[8] = {0x00, 0x09, (uint8_t)(addr & 0x000000FF), (uint8_t)((addr & 0x0000FF00) >> 8), (uint8_t)((addr & 0x00FF0000) >> 16), (uint8_t)((addr & 0xFF000000) >> 24), (uint8_t)(cnt & 0x00FF), (uint8_t)((cnt & 0xFF00) >> 8)};

    uint8_t buf[cnt + 9];
    // Calculate the checksum
    for (; i < 8; i++) {
        cksum += buf[i] = preamble[i];
    }

    for (i = 0; i < cnt; i++) {
        cksum += buf[i + 8] = data[i];
    }

    buf[cnt + 8] = cksum;

    if (i2c_transmit(NAVIGATOR_TRACKPAD_ADDRESS, buf, cnt + 9, NAVIGATOR_TRACKPAD_TIMEOUT) != I2C_STATUS_SUCCESS) {
        res |= CGEN6_I2C_FAILED;
    }

    wait_ms(1);

    return res;
}

uint8_t cirque_gen6_read_reg(uint32_t addr) {
    uint8_t data;
    uint8_t res = cirque_gen6_read_memory(addr, &data, 1);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 8bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return data;
}

uint16_t cirque_gen6_read_reg_16(uint32_t addr) {
    uint8_t buf[2];
    uint8_t res = cirque_gen6_read_memory(addr, buf, 2);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 16bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return (buf[1] << 8) | buf[0];
}

uint32_t cirque_gen6_read_reg_32(uint32_t addr) {
    uint8_t buf[4];
    uint8_t res = cirque_gen6_read_memory(addr, buf, 4);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 32bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
}

uint8_t cirque_gen6_write_reg(uint32_t addr, uint8_t data) {
    return cirque_gen6_write_memory(addr, &data, 1);
}

uint8_t cirque_gen6_write_reg_16(uint32_t addr, uint16_t data) {
    uint8_t buf[2] = {data & 0xFF, (data >> 8) & 0xFF};
    return cirque_gen6_write_memory(addr, buf, 2);
}

uint8_t cirque_gen6_write_reg_32(uint32_t addr, uint32_t data) {
    uint8_t buf[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
    return cirque_gen6_write_memory(addr, buf, 4);
}

uint8_t cirque_gen6_set_relative_mode(void) {
    uint8_t feed_config4 = cirque_gen6_read_reg(CGEN6_FEED_CONFIG4);
    feed_config4 &= 0xF3;
    return cirque_gen6_write_reg(CGEN6_FEED_CONFIG4, feed_config4);
}

uint8_t cirque_gen6_set_ptp_mode(void) {
    uint8_t feed_config4 = cirque_gen6_read_reg(CGEN6_FEED_CONFIG4);
    feed_config4 &= 0xF7;
    feed_config4 |= 0x04;
    return cirque_gen6_write_reg(CGEN6_FEED_CONFIG4, feed_config4);
}

uint8_t cirque_gen6_swap_xy(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG);
    if (set) {
        xy_config |= 0x04;
    } else {
        xy_config &= ~0x04;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_invert_y(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG);
    if (set) {
        xy_config |= 0x02;
    } else {
        xy_config &= ~0x02;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_invert_x(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG);
    if (set) {
        xy_config |= 0x01;
    } else {
        xy_config &= ~0x01;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_enable_logical_scaling(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG);
    if (set) {
        xy_config &= ~0x08;
    } else {
        xy_config |= ~0x08;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

bool cirque_gen6_get_gpio_state(uint8_t num) {
    uint32_t gpio_states = cirque_gen6_read_reg_32(0x43000000 + gpio_offset_addr + 0x0004);
    return ((gpio_states >> num) & 0x000000001);
}

uint32_t cirque_gen_6_read_callback(uint32_t trigger_time, void *cb_arg) {
    if (has_motion) {
        return NAVIGATOR_TRACKPAD_READ;
    }
    uint8_t packet[CGEN6_MAX_PACKET_SIZE];
    if (cirque_gen6_read_report(packet, CGEN6_MAX_PACKET_SIZE) != I2C_STATUS_SUCCESS) {
        return false;
    }

    uint8_t report_id = packet[2];
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    if (report_id == CGEN6_PTP_REPORT_ID) {
        ptp_report.confidence    = packet[3] & 0x01;
        ptp_report.tip           = (packet[3] & 0x02) >> 1;
        ptp_report.id            = (packet[3] & 0xFC) >> 2;
        ptp_report.x             = packet[5] << 8 | packet[4];
        ptp_report.y             = packet[7] << 8 | packet[6];
        ptp_report.ts            = packet[9] << 8 | packet[10];
        ptp_report.contact_count = packet[11];
        ptp_report.buttons       = packet[12];

        has_motion = 1;
    }
#endif
#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    if (report_id == CGEN6_MOUSE_REPORT_ID) {
        ptp_report.buttons           = packet[3];
        ptp_report.xDelta            = packet[4];
        ptp_report.yDelta            = packet[5];
        amree ptp_report.scrollDelta = packet[6];
        ptp_report.panDelta          = packet[7];

        has_motion = 1;
    }
#endif
    return NAVIGATOR_TRACKPAD_READ;
}

void dump_ptp_report(void) {
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    printf("PTP Report:\n");
    printf("  X: %d\n", ptp_report.x);
    printf("  Y: %d\n", ptp_report.y);
    printf("  Timestamp: %d\n", ptp_report.ts);
    printf("  ID: %d\n", ptp_report.id);
    printf("  Confidence: %d\n", ptp_report.confidence);
    printf("  Tip: %d\n", ptp_report.tip);
    printf("  Contact Count: %d\n", ptp_report.contact_count);
    printf("  Buttons: %d\n", ptp_report.buttons);
#endif
}

void navigator_trackpad_device_init(void) {
    i2c_init();

    i2c_status_t status = i2c_ping_address(NAVIGATOR_TRACKPAD_ADDRESS, NAVIGATOR_TRACKPAD_TIMEOUT);

    if (status != I2C_STATUS_SUCCESS) {
        printf("Failed to ping touchpad\n");
        touchpad_init = false;
        return;
    }

    cirque_gen6_clear();

    wait_ms(50);

    uint8_t resSize = cirque_gen6_write_reg(0x2001080C, 16);
    resSize         = cirque_gen6_write_reg(0x2001080D, 16);

    if (resSize != CGEN6_SUCCESS) {
        printf("Failed to set touchpad size\n");
    }

    uint8_t sizeX = cirque_gen6_read_reg(0x2001080C);
    uint8_t sizeY = cirque_gen6_read_reg(0x2001080D);

    printf("Touchpad size: %d x %d\n", sizeX, sizeY);

#if defined(NAVIGATOR_TRACKPAD_DEBUG)
    uint8_t  hardwareId  = cirque_gen6_read_reg(CGEN6_HARDWARE_ID);
    uint8_t  firmwareId  = cirque_gen6_read_reg(CGEN6_FIRMWARE_ID);
    uint16_t vendorId    = cirque_gen6_read_reg_16(CGEN6_VENDOR_ID);
    uint16_t productId   = cirque_gen6_read_reg_16(CGEN6_PRODUCT_ID);
    uint16_t versionId   = cirque_gen6_read_reg_16(CGEN6_FIRMWARE_REV);
    uint32_t firmwareRev = cirque_gen6_read_reg_32(CGEN6_FIRMWARE_REV);

    printf("Touchpad Hardware ID: 0x%02X\n", hardwareId);
    printf("Touchpad Firmware ID: 0x%02X\n", firmwareId);
    printf("Touchpad Vendor ID: 0x%04X\n", vendorId);
    printf("Touchpad Product ID: 0x%04X\n", productId);
    printf("Touchpad Version ID: 0x%04X\n", versionId);

    uint32_t revision           = firmwareRev & 0x00ffffff;
    bool     uncommittedVersion = firmwareRev & 0x80000000;
    bool     branchVersion      = firmwareRev & 0x40000000;
    uint8_t  developerId        = firmwareRev & 0x3f000000;

    printf("Touchpad Firmware Revision: 0x%08X\n", (u_int)revision);
    printf("Touchpad Uncommitted Version: %s\n", uncommittedVersion ? "true" : "false");
    printf("Touchpad Branch Version: %s\n", branchVersion ? "true" : "false");
    printf("Touchpad Developer ID: %d\n", developerId);
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    uint8_t res = cirque_gen6_set_ptp_mode();
#endif
#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    uint8_t res = cirque_gen6_set_relative_mode();
#endif

    if (res != CGEN6_SUCCESS) {
        return;
    }

    // Reset to the default alignment
    cirque_gen6_swap_xy(false);
    cirque_gen6_invert_x(false);
    cirque_gen6_invert_y(false);
    cirque_gen6_swap_xy(true);
    cirque_gen6_invert_x(true);
    cirque_gen6_invert_y(true);
    cirque_gen6_enable_logical_scaling(true);

    touchpad_init = true;
    defer_exec(NAVIGATOR_TRACKPAD_READ, cirque_gen_6_read_callback, NULL);
}

report_mouse_t navigator_trackpad_get_report(report_mouse_t mouse_report) {
    if (!has_motion || !touchpad_init) {
        if (prev_tap_clear) {
            prev_tap_clear       = false;
            mouse_report.buttons = 0;
        }
        return mouse_report;
    }

#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    mouse_report.x       = ptp_report.xDelta;
    mouse_report.y       = ptp_report.yDelta;
    mouse_report.v       = ptp_report.scrollDelta;
    mouse_report.h       = ptp_report.panDelta;
    mouse_report.buttons = ptp_report.buttons;
#endif
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    if (!prev_ptp_flag && ptp_report.tip) { // Beginning of a motion
        prev_ptp_x    = ptp_report.x;
        prev_ptp_y    = ptp_report.y;
        prev_ptp_flag = true;
        tap_timer     = timer_read();
        in_motion     = false;
    } else if (!ptp_report.tip) { // End of a motion
        prev_ptp_flag = false;
        if (in_motion == false) { // Register a tap or double tap
            if (last_contact_count > 0) {
                print("Double tap detected\n");
#    ifdef NAVIGATOR_TRACKPAD_ENABLE_DOUBLE_TAP
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON2);
#    endif
            } else {
                print("Single tap detected\n");
#    ifdef NAVIGATOR_TRACKPAD_ENABLE_TAP
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON1);
#    endif
            }
            prev_tap_clear = true;
        }
        set_scrolling = false;
    } else {
        ptp_delta_x = ptp_report.x - prev_ptp_x;
        ptp_delta_y = ptp_report.y - prev_ptp_y;

        if (timer_elapsed(tap_timer) >= NAVIGATOR_TRACKPAD_TAP_DEBOUNCE) {
            if (ptp_report.contact_count > 0) { // More than one finger, return scroll motions
                set_scrolling = true;
            }

            if (ptp_delta_x < 0) {
                mouse_report.x = -powf(-ptp_delta_x, 1.2);
            } else {
                mouse_report.x = powf(ptp_delta_x, 1.2);
            }
            if (ptp_delta_y < 0) {
                mouse_report.y = -powf(-ptp_delta_y, 1.2);
            } else {
                mouse_report.y = powf(ptp_delta_y, 1.2);
            }

            prev_ptp_x = ptp_report.x;
            prev_ptp_y = ptp_report.y;

            in_motion = true;
        }
        last_contact_count = ptp_report.contact_count;
    }
#endif
    has_motion = 0;
    return mouse_report;
}

void set_cirque_cpi(void) {
    // traverse the sequence by compairing the cpi_x value with the current cpi_x value
    // set the cpi to the next value in the sequence
    switch (current_cpi) {
        case CPI_1: {
            current_cpi = CPI_2;
            break;
        }
        case CPI_2: {
            current_cpi = CPI_3;
            break;
        }
        case CPI_3: {
            current_cpi = CPI_4;
            break;
        }
        case CPI_4: {
            current_cpi = CPI_5;
            break;
        }
        case CPI_5: {
            current_cpi = CPI_6;
            break;
        }
        case CPI_6: {
            current_cpi = CPI_7;
            break;
        }
        case CPI_7: {
            current_cpi = CPI_1;
            break;
        }
        default: {
            current_cpi = CPI_4;
            break;
        }
    }
}

uint16_t navigator_trackpad_get_cpi(void) {
    return current_cpi;
}

void restore_cpi(uint8_t cpi) {
    current_cpi = cpi;
    set_cirque_cpi();
}

void navigator_trackpad_set_cpi(uint16_t cpi) {
    if (cpi == 0) { // Decrease one tick
        if (current_cpi > 1) {
            current_cpi--;
        }
    } else {
        if (current_cpi < CPI_TICKS) {
            current_cpi++;
        }
    }
    set_cirque_cpi();
};
