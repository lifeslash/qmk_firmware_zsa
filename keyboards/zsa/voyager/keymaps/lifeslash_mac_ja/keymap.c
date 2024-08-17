#include QMK_KEYBOARD_H
#include "version.h"
// #include "keymap_jp.h"
#include "tap_dance/tap_dance_action.c"

/*#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)
#define NO_PIPE_ALT KC_GRAVE
#define NO_BSLS_ALT KC_EQUAL
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
#define BP_NDSH_MAC ALGR(KC_8)
#define SE_SECT_MAC ALGR(KC_6)
#define MOON_LED_LEVEL LED_LEVEL //*/

enum custom_keycodes {
  RGB_SLD = ML_SAFE_RANGE,
  HSV_0_255_255,
  HSV_74_255_255,
  HSV_169_255_255,
  HANZEN,
  CMDTAB
};

bool is_hanzen = true;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_voyager(
    TD(TD_1_LEFT),  KC_2,           KC_3,           KC_4,           KC_5,           KC_6,                                           KC_7,           KC_8,           KC_9,           KC_0,           CMDTAB,         TD(TD_GRV_RGHT),       
    KC_TAB,         LT(1,KC_Q),     KC_W,           TD(TD_E_LCBR),  TD(TD_R_LBRC),  TD(TD_T_QUOT),                                  TD(TD_Y_BSLS),  TD(TD_U_RBRC),  TD(TD_I_RCBR),  KC_O,           LT(1,KC_P),     KC_DEL,
    MT(MOD_LCTL,KC_CAPS),LT(2,KC_A),KC_S,           KC_D,           KC_F,           TD(TD_G_EQL),                                   TD(TD_H_MINS),  KC_J,           KC_K,           KC_L,           LT(2,KC_SCLN),  MT(MOD_LCTL,KC_MCTL),
    SC_LSPO,        MT(MOD_LGUI,KC_Z),MT(MOD_LALT,KC_X),KC_C,       KC_V,           TD(TD_B_PLUS),                                  TD(TD_N_UNDS),  KC_M,           KC_COMMA,       MT(MOD_RALT,KC_DOT),MT(MOD_RGUI,KC_SLSH),SC_RSPC,
                                                                    KC_SPC,         KC_BSPC,                                        HANZEN,         KC_ENT
  ),
  [1] = LAYOUT_voyager(
    KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,                                          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,         
    KC_ESC,         KC_NO,          KC_AT,          KC_DLR,         KC_CIRC,        KC_ASTR,                                        KC_NO,          KC_INT1,        KC_INT2,        KC_INT3,        DM_REC1,        KC_PSCR,           
    KC_CAPS,        KC_EXLM,        KC_HASH,        KC_PERC,        KC_AMPR,        KC_NO,                                          KC_HOME,        KC_PGDN,        KC_PGUP,        KC_END,         DM_REC2,        KC_INS,      
    RGB_MOD,        KC_NO,          KC_NO,          KC_LEFT,        KC_DOWN,        DM_PLY1,                                        DM_PLY2,        KC_UP,          KC_RGHT,        KC_NO,          DM_RSTP,        KC_NO,       
                                                                    RGB_VAD,        RGB_VAI,                                        RGB_HUD,        RGB_HUI
  ),
  [2] = LAYOUT_voyager(
    KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          
    KC_NO,          KC_NO,          KC_MS_U,        KC_NO,          KC_NO,          KC_NO,                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          
    KC_NO,          KC_MS_L,        KC_MS_D,        KC_MS_R,        KC_NO,          KC_NO,                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          
    KC_NO,          KC_NO,          KC_NO,          HSV_0_255_255,  HSV_74_255_255, HSV_169_255_255,                                KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          
                                                                    KC_BTN1,        KC_BTN2,                                        KC_BTN1,        KC_BTN2       
  ),
};
/*
const uint16_t PROGMEM combo0[] = { MT(MOD_RSFT, KC_QUOTE), MT(MOD_LSFT, KC_BSPACE), COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo0, KC_CAPSLOCK),
}; //*/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HANZEN:
      if (record->event.pressed) {
        if (is_hanzen) {
          register_code(KC_LNG1);
        } else {
          register_code(KC_LNG2);
        }
      } else {
        if (is_hanzen) {
          is_hanzen = false;
          unregister_code(KC_LNG1);
        } else {
          is_hanzen = true;
          unregister_code(KC_LNG2);
        }
      }
      return false;
    case CMDTAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LGUI);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      return false;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
    case HSV_0_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(0,255,255);
      }
      return false;
    case HSV_74_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(74,255,255);
      }
      return false;
    case HSV_169_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(169,255,255);
      }
      return false;
  }
  return true;
}

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000 ) {
      unregister_code(KC_LGUI);
      is_alt_tab_active = false;
    }
  }
}
