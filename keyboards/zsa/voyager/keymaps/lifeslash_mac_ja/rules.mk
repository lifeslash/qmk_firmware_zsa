# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file

# reduce the fiemware size
# this option cause error
# LTO_ENABLE = yes # enable link time optimization

# MCU_LDSCRIPT = voyager

# CONSOLE_ENABLE = no
# COMMAND_ENABLE = no
MOUSEKEY_ENABLE = yes
# ORYX_ENABLE = yes
# RGB_MATRIX_CUSTOM_KB = yes
TAP_DANCE_ENABLE = yes
SPACE_CADET_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = no
DYNAMIC_TAPPING_TERM_ENABLE = yes
# KEY_LOCK_ENABLE = yes

VPATH += drivers/gpio
SRC += matrix.c mcp23018.c
I2C_DRIVER_REQUIRED = yes

# # for more tips for reducing firmware size, see this
# # /docs/squeezing_avr.md
# SRC = matrix.c
