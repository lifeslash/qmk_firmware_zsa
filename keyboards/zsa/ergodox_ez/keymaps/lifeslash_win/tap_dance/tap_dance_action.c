// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// tap dance function enum
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP = 1,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance custom keycodes
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
enum tap_dance_codes {
    TD_LSPO,
    TD_RSPC,
    TD_E_LCBR,  // KC_E, KC_LCBR
    TD_R_LBRC,  // KC_R, KC_LBRC
    TD_U_RBRC,  // KC_U, KC_RBRC
    TD_I_RCBR,  // KC_I, KC_RCBR
    TD_T_QUOT,  // KC_T, KC_QUOT
    TD_G_EQL,   // KC_G, KC_EQL
    TD_Y_BSLS,  // KC_Y, KC_BSLS
    TD_H_MINS,  // KC_H, KC_MINS
    TD_B_PLUS,  // KC_B, KC_PLUS
    TD_N_UNDS,  // KC_N, KC_UNDS
    TD_1_LEFT,  // KC_1, LCTL(KC_LEFT)
    TD_GRV_RGHT,// KC_GRV, LCTL(KC_RGHT)
    TD_ESC_CAPS,// KC_ESC, CAPSLOCK
};

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance custom functions
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
td_state_t cur_dance(tap_dance_state_t *state);

void lspo_finished(tap_dance_state_t *state, void *user_data);
void lspo_reset(tap_dance_state_t *state, void *user_data);

void rspc_finished(tap_dance_state_t *state, void *user_data);
void rspc_reset(tap_dance_state_t *state, void *user_data);

static td_tap_t lspo_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t rspc_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    } else if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance each custom function behavior
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void lspo_finished(tap_dance_state_t *state, void *user_data) {
    lspo_tap_state.state = cur_dance(state);
    switch (lspo_tap_state.state) {
        case TD_SINGLE_TAP: register_code16(KC_LPRN); break; // (
        case TD_SINGLE_HOLD: register_code16(KC_LSFT); break; // Left shift
        case TD_DOUBLE_TAP: register_code16(KC_LBRC); break; // [
        case TD_DOUBLE_HOLD: register_code16(KC_MEH); break; // Left Control, Shift and Alt
        // case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code16(KC_X); break;
        case TD_TRIPLE_TAP: register_code16(KC_LCBR); break; // {
        case TD_TRIPLE_HOLD: register_code16(KC_HYPR); break; // Left Control, Shift and Alt, GUI
        default: break;
    }
}

void lspo_reset(tap_dance_state_t *state, void *user_data) {
    switch (lspo_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code16(KC_LPRN); break;
        case TD_SINGLE_HOLD: unregister_code16(KC_LSFT); break;
        case TD_DOUBLE_TAP: unregister_code16(KC_LBRC); break;
        case TD_DOUBLE_HOLD: unregister_code16(KC_MEH); break;
        // case TD_DOUBLE_SINGLE_TAP: unregister_code16(KC_X); break;
        case TD_TRIPLE_TAP: unregister_code16(KC_LCBR); break;
        case TD_TRIPLE_HOLD: unregister_code16(KC_HYPR); break;
        default: break;
    }
    lspo_tap_state.state = TD_NONE;
}

void rspc_finished(tap_dance_state_t *state, void *user_data) {
    rspc_tap_state.state = cur_dance(state);
    switch (rspc_tap_state.state) {
        case TD_SINGLE_TAP: register_code16(KC_RPRN); break; // )
        case TD_SINGLE_HOLD: register_code16(KC_RSFT); break; // Right Shift
        case TD_DOUBLE_TAP: register_code16(KC_RBRC); break; // ]
        case TD_DOUBLE_HOLD: register_code16(KC_MEH); break; // Left Control, Shift and Alt
        // case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code16(KC_X); break;
        case TD_TRIPLE_TAP: register_code16(KC_RCBR); break; // }
        case TD_TRIPLE_HOLD: register_code16(KC_HYPR); break; // Left Control, Shift and Alt, GUI
        default: break;
    }
}

void rspc_reset(tap_dance_state_t *state, void *user_data) {
    switch (rspc_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code16(KC_RPRN); break;
        case TD_SINGLE_HOLD: unregister_code16(KC_RSFT); break;
        case TD_DOUBLE_TAP: unregister_code16(KC_RBRC); break;
        case TD_DOUBLE_HOLD: unregister_code16(KC_MEH); break;
        // case TD_DOUBLE_SINGLE_TAP: unregister_code16(KC_X); break;
        case TD_TRIPLE_TAP: unregister_code16(KC_RCBR); break;
        case TD_TRIPLE_HOLD: unregister_code16(KC_HYPR); break;
        default: break;
    }
    rspc_tap_state.state = TD_NONE;
}

// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// define tap_dance actions
// _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
tap_dance_action_t tap_dance_actions[] = {
    [TD_LSPO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lspo_finished, lspo_reset),
    [TD_RSPC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rspc_finished, rspc_reset),
    [TD_E_LCBR] = ACTION_TAP_DANCE_DOUBLE(KC_E, KC_LCBR),
    [TD_R_LBRC] = ACTION_TAP_DANCE_DOUBLE(KC_R, KC_LBRC),
    [TD_U_RBRC] = ACTION_TAP_DANCE_DOUBLE(KC_U, KC_RBRC),
    [TD_I_RCBR] = ACTION_TAP_DANCE_DOUBLE(KC_I, KC_RCBR),
    [TD_T_QUOT] = ACTION_TAP_DANCE_DOUBLE(KC_T, KC_QUOT),
    [TD_G_EQL] =  ACTION_TAP_DANCE_DOUBLE(KC_G, KC_EQL),
    [TD_Y_BSLS] = ACTION_TAP_DANCE_DOUBLE(KC_Y, KC_BSLS),
    [TD_H_MINS] = ACTION_TAP_DANCE_DOUBLE(KC_H, KC_MINS),
    [TD_B_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_B, KC_PLUS),
    [TD_N_UNDS] = ACTION_TAP_DANCE_DOUBLE(KC_N, KC_UNDS),
    [TD_1_LEFT] = ACTION_TAP_DANCE_DOUBLE(KC_1, LCTL(KC_LEFT)),
    [TD_GRV_RGHT] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, LCTL(KC_RGHT)),
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};
