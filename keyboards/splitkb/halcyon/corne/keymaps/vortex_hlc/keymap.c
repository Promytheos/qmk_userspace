#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _GAME,
    _NUM,
    _NAV,
    _MOUSE,
    _FUN,
    _MISC,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
} td_state_t;

enum {
    TO_BASE,
    TO_GAME,
    CT_MED,
    MS_ENC_CLK,
};

enum custom_keycodes {
    KC_CWRD = SAFE_RANGE,
    MS_ENC_CW,
    MS_ENC_CCW,
};

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

tap_dance_action_t *tap_dance_get(uint16_t index);
td_state_t cur_dance(tap_dance_state_t *state);
void       x_finished(tap_dance_state_t *state, void *user_data);
void       x_reset(tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM caps_word[] = {LCTL_T(KC_F), RCTL_T(KC_J), COMBO_END};

combo_t key_combos[] = {
    COMBO(caps_word, KC_CWRD),
};

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_CWRD:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(CT_MED):
            return 300;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;
    tap_dance_state_t  *state;
    uint8_t             mods = get_mods();
    switch (keycode) {
        case KC_CWRD:
            if (record->event.pressed) {
                caps_word_on();
            }
            break;
        case TD(MS_ENC_CLK):
            action = tap_dance_get(QK_TAP_DANCE_GET_INDEX(keycode));
            state  = tap_dance_get_state(QK_TAP_DANCE_GET_INDEX(keycode));
            if (!record->event.pressed && state != NULL && state->count && !state->finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case MS_ENC_CW:
            if (!record->event.pressed) {
                break;
            }
            if (mods & MOD_MASK_GUI) {
                tap_code(MS_DOWN);
            }
            if (mods & MOD_MASK_ALT) {
                tap_code(MS_RGHT);
            }
            if (!(mods & (MOD_MASK_ALT | MOD_MASK_GUI))) {
                if (mods & MOD_MASK_SHIFT) {
                    tap_code(MS_WHLR);
                } else {
                    tap_code(MS_WHLD);
                }
            }
            return false;
        case MS_ENC_CCW:
            if (!record->event.pressed) {
                break;
            }
            if (mods & MOD_MASK_GUI) {
                tap_code(MS_UP);
            }
            if (mods & MOD_MASK_ALT) {
                tap_code(MS_LEFT);
            }
            if (!(mods & (MOD_MASK_ALT | MOD_MASK_GUI))) {
                if (mods & MOD_MASK_SHIFT) {
                    tap_code(MS_WHLL);
                } else {
                    tap_code(MS_WHLU);
                }
            }
            return false;
    }
    return true;
}
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_corne_hlc(
            KC_NO, KC_Q,         KC_W,         KC_E,          KC_R,            KC_T,                KC_Y,   KC_U,          KC_I,         KC_O,         KC_P,            KC_NO ,
            KC_NO, LGUI_T(KC_A), LALT_T(KC_S), LSFT_T(KC_D),  LCTL_T(KC_F),    KC_G,                KC_H,   RCTL_T(KC_J),  RSFT_T(KC_K), RALT_T(KC_L), RGUI_T(KC_QUOT), KC_NO ,
            KC_NO, KC_Z,         KC_X,         KC_C,          KC_V,            KC_B,                KC_N,   KC_M,          KC_COMM,      KC_DOT,       KC_SLSH,         KC_NO,
                                                     LT(_NAV, KC_DEL), LT(_NUM, KC_BSPC),  LT(_MOUSE, KC_ENT),       KC_TAB, LT(_FUN, KC_SPC), LT(_MISC, KC_ESC) ,
                                                     TD(CT_MED), KC_NO, KC_NO, KC_NO, KC_NO,             TD(CT_MED), KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_GAME] = LAYOUT_corne_hlc(
            KC_ESC,  KC_Q, KC_W, KC_E,    KC_R,   KC_T,          KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    KC_MINUS ,
            KC_TAB,  KC_A, KC_S, KC_D,    KC_F,   KC_G,          KC_H,    KC_J,   KC_K,    KC_L,   KC_QUOT, MO(_NUM),
            KC_LSFT, KC_Z, KC_X, KC_C,    KC_V,   KC_B,          KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH, TD(TO_BASE),
                                 KC_LCTL, KC_SPC, KC_LALT,       MO(_FUN),  KC_ENT, MO(_NAV) ,
                 TD(CT_MED), KC_NO, KC_NO, KC_NO, KC_NO,             TD(CT_MED), KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_NUM] = LAYOUT_corne_hlc(
            KC_NO, KC_NO,   KC_NO,   TO_GAME, KC_NO,   KC_NO,          KC_LBRC,  KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT,   KC_LCTL, KC_NO,          KC_EQL,   KC_4, KC_5, KC_6, KC_SCLN, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,          KC_BSLS,  KC_1, KC_2, KC_3, KC_GRV,  KC_NO,
                                     KC_NO,     KC_NO,   KC_NO,          KC_MINUS, KC_0, KC_DOT,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______
    ),

    [_NAV] = LAYOUT_corne_hlc(
            KC_NO, KC_NO,   KC_NO,   TO_GAME, KC_NO,   KC_NO,          KC_PSTE,   KC_COPY,   KC_CUT,   KC_UNDO,   KC_AGIN,   KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT,   KC_LCTL, KC_NO,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_CAPS, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,          KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,   KC_NO,
                                     KC_NO,     KC_NO,   KC_NO,          KC_NO,   KC_NO,   KC_NO ,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_MOUSE] = LAYOUT_corne_hlc(
            KC_NO, KC_NO,   KC_NO,   TO_GAME,   KC_NO,   KC_NO,          KC_NO,   MS_ACL0, MS_ACL1, MS_ACL2,  KC_NO, KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT,   KC_LCTL, KC_NO,          MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, KC_NO, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,          MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR,  KC_NO, KC_NO,
                                     KC_NO,     KC_NO,   KC_NO,          MS_BTN2, MS_BTN1, MS_BTN3,
             TD(MS_ENC_CLK), _______, _______, _______, _______,         TD(MS_ENC_CLK), _______, _______, _______, _______
    ),

    [_FUN] = LAYOUT_corne_hlc(
            KC_NO, KC_F12, KC_F7, KC_F8,  KC_F9,   KC_NO,          KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
            KC_NO, KC_F11, KC_F4, KC_F5,  KC_F6,   KC_NO,          KC_NO, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, KC_NO,
            KC_NO, KC_F10, KC_F1, KC_F2,  KC_F3,   KC_NO,          KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                        KC_DEL, KC_BSPC, KC_ENT,         KC_NO, KC_NO,   KC_NO,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_MISC] = LAYOUT_corne_hlc(
            RM_FLGN, RM_TOGG, RM_VALD, RM_VALU, RM_NEXT, RM_SPDU,        KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
            RM_FLGP, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, RM_SPDD,        KC_NO, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, KC_NO,
            KC_NO,   RM_HUED, RM_HUEU, RM_SATD, RM_SATU, KC_NO,          KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                             KC_MUTE, KC_MPLY, KC_NO,          KC_NO, KC_NO,   KC_NO,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

/*
 * NOTE: NUM_ENCODERS=NUM_ENCODERS_LEFT + NUM_ENCODERS_RIGHT
 * Which is based on what pins are exposed to qmk for the encoders.
 * This is somewhere in splitkb's base firmware and I'm too lazy to find it,
 *   especially if I may add more encoders later.
 * For now, far right encoder mapping is for the one I have installed
 */
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_GAME] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_NUM] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_NAV] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_PGDN, KC_PGUP)  },
    [_MOUSE] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(MS_ENC_CCW, MS_ENC_CW)  },
    [_FUN] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_MISC] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
};
#endif

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
            && !state->interrupted
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold)                                        \
    {                                                                               \
        .fn        = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, \
        .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}),               \
    }

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicative that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 */
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }
    if (state->count == 3) {
        if (!state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Leave TD_DOUBLE_HOLD and TD_TRIPLE_HOLD undefined until I decide to use them

static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void x_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_MPLY); break;
        case TD_SINGLE_HOLD: register_code(KC_MUTE); break;
        case TD_DOUBLE_TAP: register_code(KC_MNXT); break;
        case TD_TRIPLE_TAP: register_code(KC_MPRV); break;
        default: break;
    }
    reset_tap_dance(state);
}

void x_reset(tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_MPLY); break;
        case TD_SINGLE_HOLD: unregister_code(KC_MUTE); break;
        case TD_DOUBLE_TAP: unregister_code(KC_MNXT); break;
        case TD_TRIPLE_TAP: unregister_code(KC_MPRV); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TO_BASE] = ACTION_TAP_DANCE_LAYER_MOVE(KC_NO, _BASE),
    [TO_GAME] = ACTION_TAP_DANCE_LAYER_MOVE(KC_NO, _GAME),
    [CT_MED] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
    [MS_ENC_CLK] = ACTION_TAP_DANCE_TAP_HOLD(MS_BTN1, MS_BTN2),
};

