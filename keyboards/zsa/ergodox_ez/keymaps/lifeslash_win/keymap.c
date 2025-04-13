#include QMK_KEYBOARD_H
#include "version.h"
#include "tap_dance/tap_dance_action.c"

enum layers {
  BASE, // default layer
  SYMB, // symbols
  MDIA, // media keys
};

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

enum custom_keycodes {
  VRSN = QK_USER,
  RGB_SLD,
  ALT_TAB,
  LAYER_KEY
};

#define LAYER_CYCLE_START BASE
#define LAYER_CYCLE_END MDIA

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |CtlWin<-|   1  |   2  |   3  |   4  |   5  |   6  |           |   7  |   8  |   9  |   0  | - _  | = +  |CtlWin->|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    | Q/L1 | W/L2 |   E  |   R  |   T  |  '   |           |   \  |   Y  |   U  |   I  | O/L2 | P/L1 |  DEL   |
 * |--------+------+------+------+------+------|  Cmd |           |  Cmd |------+------+------+------+------+--------|
 * |WordCAPS|  A   |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  | ; :  |APP/Alt |
 * |--------+------+------+------+------+------|   =  |           |   -  |------+------+------+------+------+--------|
 * | LShift(|Z/Ctrl| X/ALT|   C  |   V  |   B  |  MEH |           |HYPER |   N  |   M  |   ,  | ./ALT|//Ctrl| RShift)|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | Grv  |  '"  | Esc  | Left | Down |                                       |  Up  | Right|   [  |   ]  |Layer |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |DM_RC1|       |DM_RC2|      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |DM_STP|       |DM_STP|      |      |
 *                                 | Space|Backsp|------|       |------|  Han |Enter |
 *                                 |      |ace   |DM_PL1|       |DM_PL2|      |      |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox(
  // left hand
  C(G(KC_LEFT)),  KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_6,
  KC_TAB,         LT(SYMB,KC_Q),  LT(MDIA,KC_W),  KC_E,           KC_R,           KC_T,           GUI_T(KC_QUOT),
  CW_TOGG,        KC_A,           KC_S,           KC_D,           KC_F,           KC_G,
  SC_LSPO,        CTL_T(KC_Z),    ALT_T(KC_X),    KC_C,           KC_V,           KC_B,           MEH_T(KC_EQL),
  KC_GRV,         KC_QUOT,        TD(TD_ESC_CAPS),KC_LEFT,        KC_DOWN,
                                                                                  KC_NO,          DM_REC1,
                                                                                                  DM_RSTP,
                                                                  KC_SPC,         KC_BSPC,        DM_PLY1,
  // right hand
  KC_7,           KC_8,           KC_9,           KC_0,           KC_MINS,        KC_EQL,         C(G(KC_RIGHT)),
  GUI_T(KC_BSLS), KC_Y,           KC_U,           KC_I,           LT(MDIA,KC_O),  LT(SYMB,KC_P),  KC_DEL,
                  KC_H,           KC_J,           KC_K,           KC_L,           KC_SCLN,        ALT_T(KC_APP),
  HYPR_T(KC_MINS),KC_N,           KC_M,           KC_COMM,        ALT_T(KC_DOT),  CTL_T(KC_SLSH), SC_RSPC,
                                  KC_UP,          KC_RGHT,        KC_LBRC,        KC_RBRC,        LAYER_KEY,
  DM_REC2,        KC_NO,
  DM_RSTP,
  DM_PLY2,        C(KC_SPC),      KC_ENT
),
/* Keymap 1: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |  Caps   |      | PgUp |      |      |      |      |           |      |      |      |  Up  |  [ { |  ] } | BackSP |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |OS Cntrl | Home | PgDn | End  |      |      |------|           |------|      | Left | Down | Rght |  '"  |  Trans |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |OS Shift | jis_ | jis\ | 半全 |      |      |      |           |      |      |      |      |  `~  |  \|  |OS Shift|
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | EPRM  |      |      | Home | PgDn |                                       | PgUp | End  |      |      | Trans|
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 | Ctrl | Alt  |      |       |      | Ctrl | Ctrl |
 *                                 | Win  | Tab  |------|       |------| Win  | Win  |
 *                                 |  <-  |      |      |       |      |  D   |  ->  |
 *                                 `--------------------'       `--------------------'
 */
[SYMB] = LAYOUT_ergodox(
  // left hand
  KC_NO,          KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,
  KC_CAPS,        KC_NO,          KC_PGUP,        KC_NO,          KC_NO,          KC_NO,          KC_NO,
  QK_LOCK,        KC_HOME,        KC_PGDN,        KC_END,         KC_NO,          KC_NO,
  OSM(MOD_LSFT),  KC_INT1,        KC_INT3,        KC_LNG5,        KC_NO,          KC_NO,          KC_NO,
  EE_CLR,         KC_NO,          KC_NO,          KC_HOME,        KC_PGDN,
                                                                                  KC_NO,          KC_NO,
                                                                                                  KC_NO,
                                                                  C(G(KC_LEFT)),  ALT_TAB,        KC_NO,
  // right hand
  KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,         KC_NO,
  KC_NO,          KC_NO,          KC_NO,          KC_UP,          KC_LBRC,        KC_RBRC,        KC_BSPC,
                  KC_NO,          KC_LEFT,        KC_DOWN,        KC_RGHT,        KC_QUOT,        KC_TRNS,
  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_GRV,         KC_BSLS,        OSM(MOD_RSFT),
                                  KC_PGUP,        KC_END,         KC_NO,          KC_NO,          KC_TRNS,
  KC_NO,          KC_NO,
  KC_NO,
  KC_NO,          C(G(KC_D)),     C(G(KC_RIGHT))
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |Version |      |      |      |      |      |      |           |      |      |      |      |      |      | PRTSC  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Reset |      |MS UP |      |      |      |      |           |      |      |      |      |      |      |  INS   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |MS LF |MS DN |MS RG |      |      |------|           |------|  VAD |  VAI |  HUD |  HUI |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | BT1  | BT3  | BT2  |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      | TRANS|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------| MOD  |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[MDIA] = LAYOUT_ergodox(
  // left hand
  VRSN,           KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,  
  QK_BOOT,        KC_NO,          KC_MS_U,        KC_NO,          KC_NO,          KC_NO,          KC_NO,  
  KC_NO,          KC_MS_L,        KC_MS_D,        KC_MS_R,        KC_NO,          KC_NO,  
  KC_NO,          KC_BTN1,        KC_BTN3,        KC_BTN2,        KC_NO,          KC_NO,          KC_NO,  
  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,
                                                                                  KC_NO,          KC_NO,  
                                                                                                  KC_NO,  
                                                                  KC_NO,          KC_NO,          KC_NO,  
  // right hand
  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_PSCR,  
  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_INS,  
                  RGB_VAD,        RGB_VAI,        RGB_HUD,        RGB_HUI,        KC_NO,          KC_NO,  
  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,  
                                  KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_TRNS,  
  KC_NO,          KC_NO,  
  KC_NO,  
  KC_NO,          RGB_MOD,        KC_NO
),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {
      case VRSN:
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        return false;
      case ALT_TAB:
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
        return false;
      case LAYER_KEY:
        return false;
      #ifdef RGBLIGHT_ENABLE
      case RGB_SLD:
        rgblight_mode(1);
        return false;
      #endif
    }
  } else {
    switch (keycode) {
      case ALT_TAB:
        unregister_code(KC_TAB);
        return false;
      case LAYER_KEY:
      {
        uint8_t current_layer = get_highest_layer(layer_state);
        if(current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
          return false;
        }
        uint8_t next_layer = current_layer + 1;
        if(next_layer > LAYER_CYCLE_END) {
          next_layer = LAYER_CYCLE_START;
        }
        layer_move(next_layer);
      }
      return false;
    }
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
  rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();

  uint8_t layer = biton32(state);
  switch (layer) {
      case 0:
        #ifdef RGBLIGHT_COLOR_LAYER_0
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
        #else
        #ifdef RGBLIGHT_ENABLE
          rgblight_init();
        #endif
        #endif
        break;
      case 1:
        ergodox_right_led_1_on();
        #ifdef RGBLIGHT_COLOR_LAYER_1
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
        #endif
        break;
      case 2:
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_2
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
        #endif
        break;
      case 3:
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_3
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
        #endif
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_4
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
        #endif
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_5
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
        #endif
        break;
      case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_6
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
        #endif
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_7
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
        #endif
        break;//*/
      default:
        break;
    }

  return state;
};

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}
