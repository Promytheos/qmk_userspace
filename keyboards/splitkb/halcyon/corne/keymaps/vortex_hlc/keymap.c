// Copyright 2024 splitkb.com (support@splitkb.com)
// SPDX-License-Identifier: GPL-2.0-or-later

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

void caps_word_set_user(bool active) {
    if (active) {
        // Do something when Caps Word activates.
    } else {
        // Do something when Caps Word deactivates.
    }
}

// Tap Dance declarations
enum {
    TO_BASE,
    CT_MED,
};

typedef struct {
    uint16_t single_tap;
    uint16_t double_tap;
    uint16_t triple_tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_multi_hold_t;

void tap_dance_multi_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_multi_hold_t *multi_hold = (tap_dance_multi_hold_t *)user_data;

    if (state->pressed) {
        register_code16(multi_hold->hold);
        multi_hold->held = multi_hold->hold;
    }
    else {
        switch (state->count) {
            case 1:
                register_code16(multi_hold->single_tap);
                multi_hold->held = multi_hold->single_tap;
                break;
            case 2:
                register_code16(multi_hold->double_tap);
                multi_hold->held = multi_hold->double_tap;
                break;
            case 3:
                register_code16(multi_hold->triple_tap);
                multi_hold->held = multi_hold->triple_tap;
                break;
        }
    }
}

void tap_dance_multi_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_multi_hold_t *multi = (tap_dance_multi_hold_t *)user_data;

    if (multi->held) {
        unregister_code16(multi->held);
        multi->held = 0;
    }
}

#define ACTION_TAP_DANCE_MULTI_HOLD(single_tap, double_tap, triple_tap, hold)                                        \
    {                                                                               \
        .fn        = {NULL, tap_dance_multi_hold_finished, tap_dance_multi_hold_reset}, \
        .user_data = (void *)&((tap_dance_multi_hold_t){single_tap, double_tap, triple_tap, hold, 0}),               \
    }

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TO_BASE] = ACTION_TAP_DANCE_LAYER_MOVE(KC_NO, _BASE),
    [CT_MED] = ACTION_TAP_DANCE_MULTI_HOLD(KC_MPLY, KC_MNXT, KC_MPRV, KC_MUTE),
};

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
            KC_NO, KC_NO,   KC_NO,   KC_NO,   TO(_GAME), KC_NO,          KC_LBRC,  KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL,   KC_NO,          KC_EQL,   KC_4, KC_5, KC_6, KC_SCLN, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,          KC_BSLS,  KC_1, KC_2, KC_3, KC_GRV,  KC_NO,
                                     KC_NO,   KC_NO,     KC_NO,          KC_MINUS, KC_0, KC_DOT,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______
    ),

    [_NAV] = LAYOUT_corne_hlc(
            KC_NO, KC_NO,   KC_NO,   KC_NO,   TO(_GAME),   KC_NO,          KC_PSTE,   KC_COPY,   KC_CUT,   KC_UNDO,   KC_AGIN,   KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_NO,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_CAPS, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,          KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,   KC_NO,
                                           KC_NO,   KC_NO,   KC_NO,          KC_NO,   KC_NO,   KC_NO ,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_MOUSE] = LAYOUT_corne_hlc(
            KC_NO, KC_NO,   KC_NO,   KC_NO,   TO(_GAME),   KC_NO,          KC_NO,   MS_ACL0, MS_ACL1, MS_ACL2,  KC_NO, KC_NO,
            KC_NO, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_NO,          MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, KC_NO, KC_NO,
            KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,          MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR,  KC_NO, KC_NO,
                                           KC_NO,   KC_NO,   KC_NO,          MS_BTN2, MS_BTN1, MS_BTN3,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
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

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [2] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [3] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_PGDN, KC_PGUP)  },
    [4] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(MS_WHLD, MS_WHLU)  },
    [5] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [6] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
};
#endif
