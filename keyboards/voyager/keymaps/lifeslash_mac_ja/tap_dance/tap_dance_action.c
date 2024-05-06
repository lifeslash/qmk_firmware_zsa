// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// tap dance function enum
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance custom keycodes
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
enum tap_dance_codes {
  DANCE_0,
  TD_T_QUOT,  // KC_T, KC_QUOT
  TD_G_EQL,   // KC_G, KC_EQL
  TD_Y_BSLS,  // KC_Y, KC_BSLS
  TD_H_MINS,  // KC_H, KC_MINS
  TD_B_PLUS,  // KC_B, KC_PLUS
  TD_N_UNDS,  // KC_N, KC_UNDS
  TD_1_LEFT,  // KC_1, LCTL(KC_LEFT)
  TD_GRV_RGHT,// KC_GRV, LCTL(KC_RGHT)
};

static tap dance_state[1];

uint8_t dance_step(tap_dance_state_t *state);

uint8_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}


void on_dance_0(tap_dance_state_t *state, void *user_data);
void dance_0_finished(tap_dance_state_t *state, void *user_data);
void dance_0_reset(tap_dance_state_t *state, void *user_data);

void on_dance_0(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_EQUAL);
        tap_code16(KC_EQUAL);
        tap_code16(KC_EQUAL);
    }
    if(state->count > 3) {
        tap_code16(KC_EQUAL);
    }
}

void dance_0_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[0].step = dance_step(state);
    switch (dance_state[0].step) {
        case SINGLE_TAP: register_code16(KC_EQUAL); break;
        case SINGLE_HOLD: register_code16(KC_ESCAPE); break;
        case DOUBLE_TAP: register_code16(KC_EQUAL); register_code16(KC_EQUAL); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_EQUAL); register_code16(KC_EQUAL);
    }
}

void dance_0_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[0].step) {
        case SINGLE_TAP: unregister_code16(KC_EQUAL); break;
        case SINGLE_HOLD: unregister_code16(KC_ESCAPE); break;
        case DOUBLE_TAP: unregister_code16(KC_EQUAL); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_EQUAL); break;
    }
    dance_state[0].step = 0;
}

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance actions
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
tap_dance_action_t tap_dance_actions[] = {
    [DANCE_0] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_0, dance_0_finished, dance_0_reset),
    [TD_T_QUOT] = ACTION_TAP_DANCE_DOUBLE(KC_T, KC_QUOT),
    [TD_G_EQL] =  ACTION_TAP_DANCE_DOUBLE(KC_G, KC_EQL),
    [TD_Y_BSLS] = ACTION_TAP_DANCE_DOUBLE(KC_Y, KC_BSLS),
    [TD_H_MINS] = ACTION_TAP_DANCE_DOUBLE(KC_H, KC_MINS),
    [TD_B_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_B, KC_PLUS),
    [TD_N_UNDS] = ACTION_TAP_DANCE_DOUBLE(KC_N, KC_UNDS),
    [TD_1_LEFT] = ACTION_TAP_DANCE_DOUBLE(KC_1, LCTL(KC_LEFT)),
    [TD_GRV_RGHT] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, LCTL(KC_RGHT)),
};
