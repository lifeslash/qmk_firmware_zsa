// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "navigator.h"

float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

bool set_scrolling = false;
bool navigator_turbo = false;
bool navigator_aim = false;

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    // Turbo mode is used to increase the speed of the mouse cursor
    // by multiplying the x and y values by a factor.
    if (navigator_turbo) {
        mouse_report.x *= NAVIGATOR_TURBO_MULTIPLIER;
        mouse_report.y *= NAVIGATOR_TURBO_MULTIPLIER;
    }
    // Aim mode is used to slow down the mouse cursor
    // by dividing the x and y values by a factor.
    if (navigator_aim) {
        mouse_report.x /= NAVIGATOR_AIM_DIVIDER;
        mouse_report.y /= NAVIGATOR_AIM_DIVIDER;
    }
    if (set_scrolling) {
        scroll_accumulated_h += (float)mouse_report.x / NAVIGATOR_SCROLL_DIVIDER;
        scroll_accumulated_v += (float)mouse_report.y / NAVIGATOR_SCROLL_DIVIDER;
        mouse_report.h = (int8_t)scroll_accumulated_h;
#ifdef NAVIGATOR_SCROLL_INVERT
        mouse_report.v = (int8_t)-scroll_accumulated_v;
#else
        mouse_report.v = (int8_t)scroll_accumulated_v;
#endif

        scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int8_t)scroll_accumulated_v;

        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}
