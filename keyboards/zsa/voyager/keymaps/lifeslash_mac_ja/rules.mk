# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file

# reduce the fiemware size
# this option cause error
# LTO_ENABLE = yes # enable link time optimization

CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = yes
ORYX_ENABLE = yes
RGB_MATRIX_CUSTOM_KB = yes
TAP_DANCE_ENABLE = yes
SPACE_CADET_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = no
SRC = matrix.c

# CONSOLE_ENABLE = no # disable output debug infos
# COMMAND_ENABLE = no # disable boot magic keycode
# MOUSEKEY_ENABLE = no # mouse keys
# ORYX_ENABLE = yes
# RGB_MATRIX_CUSTOM_KB = yes
# TAP_DANCE_ENABLE = yes
# SPACE_CADET_ENABLE = yes # shift when held, ( when tapped
# CAPS_WORD_ENABLE = yes
# COMBO_ENABLE = yes

# EXTRAKEY_ENABLE = no # media keys
# GRAVE_ESC_ENABLE = no # escape when pressed, ` when shift or gui are held
# MAGIC_ENABLE = no # GUI and ALT/CTRL swapping feature
# AVR_USE_MINIMAL_PRINTF = yes # sprintf and snprintf function optimization

# # for keyboard custom settings
# RGBLIGHT_ENABLE = no
# RGB_MATRIX_ENABLE = yes

# DYNAMIC_MACRO_ENABLE = yes

# #RGB_MATRIX_CUSTOM_USER = yes # custom rgb lighting
# #SRC += ../default/keymap.c

# # for more tips for reducing firmware size, see this
# # /docs/squeezing_avr.md
# SRC = matrix.c