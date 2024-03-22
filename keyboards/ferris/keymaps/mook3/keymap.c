#include QMK_KEYBOARD_H
#include "mook3_layers.h"
#include "mook3_combos.h"
#include "mook3_td2.h"

#ifdef CONSOLE_ENABLE
#include "mook3_console.h"
#endif

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE
#include "mook3_oled.h"
#endif

#define SYM_SPC LT(_SYMBOLS,KC_SPC)
#define NUM_ENT LT(_NUMBERS,KC_ENT)
#define TG_GAM TG(_GAMING)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,					 KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   ,
//LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G,	KC_H, RSFT_T(KC_J), RCTL_T(KC_K), RALT_T(KC_L), RGUI_T(KC_QUOT),
KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,					 KC_H   , KC_J   , KC_K   , KC_L   , KC_QUOT,
KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , 			 		 KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH,
									CTL_TAB, SFT_BSP,					 SYM_SPC, NUM_ENT),

	[_GAMING] = LAYOUT(
_______, _______, _______, _______, _______, 					 _______, _______, _______, _______, MY_RESET_KC,
_______, _______, _______, _______, _______,					 _______, _______, _______, _______, _______,
_______, _______, _______, _______, _______,					 _______, _______, _______, _______, TG_GAM ,
				  KC_TAB , KC_BSPC,					 _______, _______),

	[_SYMBOLS] = LAYOUT(
KC_AMPR, KC_PLUS, KC_UNDS, KC_LCBR, KC_RCBR, 					 _______, KC_COLN, KC_HASH, _______, _______,
KC_EXLM, KC_EQL , KC_MINS, KC_LPRN, KC_RPRN, 					 _______, KC_SCLN, _______, _______, _______,
KC_PIPE, KC_ASTR, KC_TILD, KC_LBRC, KC_RBRC,					 _______, KC_GRV , KC_LT  , KC_GT  , KC_BSLS,
				  _______, _______,					 _______, _______),

	[_NUMBERS] = LAYOUT(
_______, KC_1   , KC_2   , KC_3   , KC_PMNS,					 KC_WH_U, KC_HOME, KC_UP  , KC_END , _______,
KC_0   , KC_4   , KC_5   , KC_6   , _______, 					 KC_WH_D, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL ,
_______, KC_7   , KC_8   , KC_9   , KC_PENT, 					 _______, KC_TAB , KC_ESC , _______, _______,
				  _______, _______,					 _______, _______),

	[_FN] = LAYOUT(
_______, KC_F1  , KC_F2  , KC_F3  , KC_F10 ,					 KC_BRIU, KC_BRID, KC_VOLD, KC_MUTE, KC_VOLU,
_______, KC_F4  , KC_F5  , KC_F6  , KC_F11 , 					 _______, _______, _______, _______, _______,
_______, KC_F7  , KC_F8  , KC_F9  , KC_F12 ,					 _______, _______, _______, _______, TG_GAM,
				  _______, _______,					 _______, _______),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    debug_process_record_user(keycode, record);
#endif 

	if (!process_record_user_tds(keycode, record)) {
		// Return false to indicate that custom TD code already handled it
		return false;
	}

    // Process custom keycodes
    switch (keycode) {
		case MY_RESET_KC:
			reset_keyboard();
			return false;
		default:
			break;
	}

	// Return true to indicate we did not handle this keypress
    return true;
}

void matrix_scan_user(void) {
	matrix_scan_user_tds();
}

/*bool is_home_row_mod(uint16_t keycode) {
    switch (keycode) {
		case RSFT_T(KC_J):
		case RCTL_T(KC_K):
		case RALT_T(KC_L):
		case RGUI_T(KC_QUOT):
		case LGUI_T(KC_A):
		case LALT_T(KC_S):
		case LCTL_T(KC_D):
		case LSFT_T(KC_F):
		    return true;
		default:
		    return false;
	}
}*/

/*
 * Determine whether or not to force the mod-tap key press to be handled as a modifier
 * if any other key was pressed while the mod-tap key is held down.
 */
/*bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return !is_home_row_mod(keycode);
}*/

/*
 * Determine whether holding a key after tapping it should repeat the tapping function
 * (false) or activate the hold function (true).
 */
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		case LT(_NUMBERS,KC_ENT):
		case LT(_SYMBOLS,KC_SPC):
            return false;
        default:
		    return true;
            //return !is_home_row_mod(keycode);
    }
}


