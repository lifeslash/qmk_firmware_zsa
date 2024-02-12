// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// clang-format off

// reactive
#if defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_SIMPLE) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_WIDE) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTIWIDE) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_CROSS) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTICROSS) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_NEXUS) || \
    defined(ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTINEXUS) || \
    defined(ENABLE_LED_MATRIX_SOLID_SPLASH) || \
    defined(ENABLE_LED_MATRIX_SOLID_MULTISPLASH)
#    define LED_MATRIX_KEYPRESSES
#endif