// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef NAVIGATOR_SCROLL_DIVIDER
    #define NAVIGATOR_SCROLL_DIVIDER 10
#endif

#ifdef POINTING_DEVICE_DRIVER_navigator_trackball
#define NAVIGATOR_TURBO_MULTIPLIER 3
#define NAVIGATOR_AIM_DIVIDER 3
#endif

#ifdef POINTING_DEVICE_DRIVER_navigator_trackpad
#define NAVIGATOR_TURBO_MULTIPLIER 2
#define NAVIGATOR_AIM_DIVIDER 2
#endif
