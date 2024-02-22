#include QMK_KEYBOARD_H

enum layer_number {
  _QWERTY = 0,
  _GAMING,
  _SYMBOLS,
  _NUMBERS,
  _FN,
};

// TD_* are for both, MY_* are for second only
enum custom_keycodes {
    PLACEHOLDER	 = SAFE_RANGE,
	TD_MO_0,
	TD_MO_1,
	TD_MO_2,
	TD_MO_3,
	TD_MO_4,
	MY_RESET_KC,
	SFT_BSP,
	CTL_TAB,
	MY_SPC_SYM,
};

// TEST TAP DANCE STRUCT ARRAY


// END TEST

enum combos {
    COMBO_LCTL,
    COMBO_LALT,
	COMBO_LGUI,
	COMBO_LCTL_ALT,
    COMBO_RCTL,
    COMBO_RALT,
	COMBO_RGUI,
	COMBO_RCTL_ALT,
	
	COMBO_LCTL_SFT,
	COMBO_LCTL_SFT_ALT,
	COMBO_LGUI2,
	COMBO_LGUI_SFT,
	COMBO_LALT2,
	COMBO_LALT_SFT,

    COMBO_LENGTH // nifty trick to avoid manually specifying how many combos you have
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// TODO disable combos for gaming layer - just have it use other keycodes that alias for these?
const uint16_t PROGMEM combo_fd[] = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM combo_fs[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM combo_fa[] = {KC_F, KC_A, COMBO_END};
const uint16_t PROGMEM combo_fds[] = {KC_F, KC_D, KC_S, COMBO_END};
const uint16_t PROGMEM combo_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_jl[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM combo_jq[] = {KC_J, KC_QUOT, COMBO_END};
const uint16_t PROGMEM combo_jkl[] = {KC_J, KC_K, KC_L, COMBO_END};

const uint16_t PROGMEM combo_fctl[] = {KC_F, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_rctl[] = {KC_R, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_dctl[] = {KC_D, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_ectl[] = {KC_E, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_sctl[] = {KC_S, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_wctl[] = {KC_W, CTL_TAB, COMBO_END};

combo_t key_combos[] = {
	// One-shot mods
	[COMBO_LCTL] = COMBO(combo_fd, OSM(MOD_LCTL)),
	[COMBO_LALT] = COMBO(combo_fs, OSM(MOD_LALT)),
	[COMBO_LGUI] = COMBO(combo_fa, OSM(MOD_LGUI)),
	[COMBO_LCTL_ALT] = COMBO(combo_fds, OSM(MOD_LCTL | MOD_LALT)),
	[COMBO_RCTL] = COMBO(combo_jk, OSM(MOD_RCTL)),
	[COMBO_RALT] = COMBO(combo_jl, OSM(MOD_RALT)),
	[COMBO_RGUI] = COMBO(combo_jq, OSM(MOD_RGUI)),
	[COMBO_RCTL_ALT] = COMBO(combo_jkl, OSM(MOD_RCTL | MOD_RALT)),
	
	[COMBO_LCTL_SFT] = COMBO(combo_fctl, LSFT(KC_LCTL)),
	[COMBO_LCTL_SFT_ALT] = COMBO(combo_rctl, LSFT(LCTL(KC_LALT))),
	[COMBO_LGUI2] = COMBO(combo_dctl, KC_LGUI),
	[COMBO_LGUI_SFT] = COMBO(combo_ectl, LSFT(KC_LGUI)),
	[COMBO_LALT2] = COMBO(combo_sctl, KC_LALT),
	[COMBO_LALT_SFT] = COMBO(combo_wctl, LSFT(KC_LALT)),
	// Normal mods
	/*[COMBO_LCTL] = COMBO(combo_fd, KC_LCTL),
	[COMBO_LALT] = COMBO(combo_fs, KC_LALT),
	[COMBO_LGUI] = COMBO(combo_fa, KC_LGUI),
	[COMBO_LCTL_ALT] = COMBO(combo_fds, LCTL(KC_LALT)),
	[COMBO_RCTL] = COMBO(combo_jk, KC_RCTL),
	[COMBO_RALT] = COMBO(combo_jl, KC_RALT),
	[COMBO_RGUI] = COMBO(combo_jq, KC_RGUI),
	[COMBO_RCTL_ALT] = COMBO(combo_jkl, RCTL(KC_RALT)),*/
};

typedef struct {
	// Could probably just make this have some sorta 2d array where row 0 is an array of length 2 with single hold, tap keycodes, row 1 is array of length 2 with double hold, tap, etc.
	// Maybe second struct with just kcHold, kcTap values would be good, and then this holds an array of those structs
	const uint16_t kc;
    const uint16_t kcTap;
    const uint16_t kcHold;
	const uint16_t kcDoubleTap;
	const uint16_t kcDoubleHold;
	const uint16_t maxCount;
	uint16_t timer;
	uint16_t count;
	bool pressed;
	bool otherKeyPressedDuringHold;
	uint16_t kcActive;
} qk_tap_dance_t;

bool process_td_kc_user(bool pressed, uint16_t kc, qk_tap_dance_t* td);

// For custom tap dances. Put it here so it can be used in any keymap
void process_last_td_tap_on_expire_or_interrupt(qk_tap_dance_t* td);

#define TD_CUSTOM_USER(kcTD, kcT, kcH, kcDH) \
{ .kc = kcTD, .kcTap = kcT, .kcHold = kcH, .kcDoubleHold = kcDH, .maxCount = kcDH ? 2 : 1}

#define TD1_KC SFT_BSP
static qk_tap_dance_t td1 = TD_CUSTOM_USER(TD1_KC, KC_BSPC, KC_LSFT, 0);
#define TD2_KC CTL_TAB
static qk_tap_dance_t td2 = TD_CUSTOM_USER(TD2_KC, KC_TAB, KC_LCTL, TD_MO_4);
#define TD3_KC MY_SPC_SYM
static qk_tap_dance_t td3 = TD_CUSTOM_USER(TD3_KC, KC_SPC, TD_MO_2, 0);

// TODO loop through these and use these instead of td1/td2
//#define NUM_TD_ACTIONS 3
/*static qk_tap_dance_t user_tap_dance_actions[] = {
	TD_CUSTOM_USER(SFT_BSP, KC_BSPC, KC_LSFT, KC_BSPC),
	TD_CUSTOM_USER(MY_FN_GUI, 0, TD_MO_4, KC_LGUI),
};*/

#define SYM_SPC LT(_SYMBOLS,KC_SPC)
#define NUM_ENT LT(_NUMBERS,KC_ENT)
#define TG_GAM TG(_GAMING)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
_______, _______, _______, _______, _______, _______, 					 _______, _______, _______, _______, _______, _______,
_______, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,					 KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
//_______, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G,	KC_H, RSFT_T(KC_J), RCTL_T(KC_K), RALT_T(KC_L), RGUI_T(KC_QUOT), _______,
_______, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,					 KC_H   , KC_J   , KC_K   , KC_L   , KC_QUOT, _______,
_______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , _______, 	_______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
				  _______, _______, CTL_TAB, SFT_BSP,					 SYM_SPC, NUM_ENT, _______, _______),

	[_GAMING] = LAYOUT(
_______, _______, _______, _______, _______, _______, 					 _______, _______, _______, _______, _______, MY_RESET_KC,
_______, _______, _______, _______, _______, _______, 					 _______, _______, _______, _______, _______, _______,
_______, _______, _______, _______, _______, _______,					 _______, _______, _______, _______, _______, _______,
_______, _______, _______, _______, _______, _______, _______,	_______, _______, _______, _______, _______, _______, TG_GAM ,
				  _______, _______, KC_TAB , KC_BSPC,					 _______, _______, _______, _______),

	[_SYMBOLS] = LAYOUT(
_______, _______, _______, _______, _______, _______,					 _______, _______, _______, _______, _______, _______,
_______, KC_AMPR, KC_PLUS, KC_UNDS, KC_LCBR, KC_RCBR, 					 _______, KC_COLN, KC_HASH, KC_CIRC, _______, _______,
_______, KC_EXLM, KC_EQL , KC_PMNS, KC_LPRN, KC_RPRN, 					 _______, KC_SCLN, KC_DLR , KC_AT  , KC_PERC, _______,
_______, KC_PIPE, KC_ASTR, KC_TILD, KC_LBRC, KC_RBRC, _______,	_______, _______, KC_GRV , KC_LT  , KC_GT  , KC_BSLS, _______,
				  _______, _______, _______, _______,					 _______, _______, _______, _______),

	[_NUMBERS] = LAYOUT(
_______, _______, _______, _______, _______, _______,					 _______, _______, _______, _______, _______, _______,
_______, _______, KC_1   , KC_2   , KC_3   , KC_PMNS,					 _______, KC_HOME, KC_UP  , KC_END , _______, _______,
_______, KC_0   , KC_4   , KC_5   , KC_6   , KC_DOT , 					 _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL , _______,
_______, _______, KC_7   , KC_8   , KC_9   , KC_PENT, _______,	_______, _______, KC_WH_U, KC_ESC , KC_WH_D, _______, _______,
				  _______, _______, _______, _______,					 _______, _______, _______, _______),

	[_FN] = LAYOUT(
TG_GAM , _______, _______, _______, _______, _______,					 _______, _______, _______, _______, _______, _______,
_______, _______, KC_F1  , KC_F2  , KC_F3  , KC_F10 ,					 KC_BRIU, KC_BRID, KC_VOLD, KC_MUTE, KC_VOLU, _______,
_______, _______, KC_F4  , KC_F5  , KC_F6  , KC_F11 , 					 _______, _______, _______, _______, _______, _______,
_______, _______, KC_F7  , KC_F8  , KC_F9  , KC_F12 , _______,	_______, _______, _______, _______, _______, _______, _______,
				  _______, _______, _______, _______,					 _______, _______, _______, _______),
};

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
	  return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

bool oled_task_user(void) {
	if (is_keyboard_master()) {
		// If you want to change the display of OLED, you need to change here
		oled_write_ln(read_layer_state(), false);
		//oled_write_ln(read_keylog(), false);
		//oled_write_ln(read_keylogs(), false);
		//oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
		//oled_write_ln(read_host_led_state(), false);
		//oled_write_ln(read_timelog(), false);
	} else {
		//oled_write(read_logo(), false);
	}
    return false;
}

char layer_state_str[24];

const char *read_layer_state(void) {
	if (layer_state & (1 << _FN)) {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Functions");
	} else if (layer_state & (1 << _NUMBERS)) {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Numbers");
	} else if (layer_state & (1 << _SYMBOLS)) {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Symbols");
	} else if (layer_state & (1 << _GAMING)) {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Gaming");
	} else if (layer_state) {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
	} else {
		snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Default");
	}
    return layer_state_str;
}

#endif // OLED_ENABLE

#ifdef CONSOLE_ENABLE
void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

// 6 chars + terminating null
char kc_hex_str[7];
char * get_pretty_kc(uint16_t keycode) {
	switch (keycode) {
		case KC_A: return "A";
		case KC_B: return "B";
		case KC_C: return "C";
		case KC_D: return "D";
		case KC_E: return "E";
		case KC_F: return "F";
		case KC_G: return "G";
		case KC_H: return "H";
		case KC_I: return "I";
		case KC_J: return "J";
		case KC_K: return "K";
		case KC_L: return "L";
		case KC_M: return "M";
		case KC_N: return "N";
		case KC_O: return "O";
		case KC_P: return "P";
		case KC_Q: return "Q";
		case KC_R: return "R";
		case KC_S: return "S";
		case KC_T: return "T";
		case KC_U: return "U";
		case KC_V: return "V";
		case KC_W: return "W";
		case KC_X: return "X";
		case KC_Y: return "Y";
		case KC_Z: return "Z";
		case LGUI_T(KC_A): return "A/Gui";
		case LALT_T(KC_S): return "S/Alt";
		case LCTL_T(KC_D): return "D/Ctl";
		case LSFT_T(KC_F): return "F/Sft";
		case RSFT_T(KC_J): return "J/Sft";
		case RCTL_T(KC_K): return "K/Ctl";
		case RALT_T(KC_L): return "L/Alt";
		case RGUI_T(KC_QUOT): return "'/Gui";
		case CTL_TAB: return "Tab/Ctl/Fn";
		case SFT_BSP: return "Back/Sft";
		case LT(_SYMBOLS,KC_SPC): return "Spc/Sym";
		case LT(_NUMBERS,KC_ENT): return "Ent/Num";
		default:
			sprintf(kc_hex_str, "0x%04X", keycode);
			return kc_hex_str;
	}
}

void debug_process_record_user(uint16_t keycode, keyrecord_t* record) {
	static uint16_t last_time = 0;
    uprintf("KL: %11s %4s, %5ums, %ux%u, time: %5u, int: %u, count: %u, layer: %u\n", 
			get_pretty_kc(keycode), record->event.pressed ? "down" : "up",
			record->event.time - last_time, record->event.key.col, record->event.key.row, 
			record->event.time, record->tap.interrupted, record->tap.count, layer_state);
	last_time = record->event.time;
}
#endif // CONSOLE_ENABLE

void reset_td(qk_tap_dance_t* td) {
	td->count = 0;
	td->otherKeyPressedDuringHold = false;
}

void process_td_press(qk_tap_dance_t* td, uint16_t kc, keyrecord_t* record) {
	if (td->count > 0 && timer_elapsed(td->timer) < get_tapping_term(kc, record)) {
		// Pressed again within tapping term, increase count
		td->count += 1;
	} else {
		// TODO any chance we need to wrap up last tap dance here if we are barely past tapping term - or is that guaranteed to be handled? Maybe check if count > 0 and call the finish function
		// Count is 0 or outside tapping term, so first tap
		td->count = 1;
	}
	switch (td->count) {
		case 1:
			if (td->kcHold) {
				// Start hold action immediately
				process_td_kc_user(true, td->kcHold, td);
			} else if (td->kcTap && td->maxCount == 1) {
				// Only a single-tap action exists for this tap dance, so we know we can do that right away (TODO why would this ever be a tap dance though...)
				process_td_kc_user(true, td->kcTap, td);
			}
			break;
		case 2:
		default:
			// Find/process first action available in priority: double-hold, double-tap, single-hold, single-tap
			if (td->kcDoubleHold) {
				// Start double hold action immediately
				process_td_kc_user(true, td->kcDoubleHold, td);
			} else if (td->kcDoubleTap) {
				// No double hold action, so we have to be double tapping
				process_td_kc_user(true, td->kcDoubleTap, td);
			} else if (td->kcHold) { // Remove this block to default to tap instead of hold if desired
				// Start single hold action immediately again
				process_td_kc_user(true, td->kcHold, td);
			} else if (td->kcTap) {
				// No double tap/hold action, so just repeat single tap action
				process_td_kc_user(true, td->kcTap, td);
			}
			break;
	}
	// TODO use record->event.time instead of timer_read? (just for perf)
	td->timer = timer_read();
}

void process_td_release(qk_tap_dance_t* td, uint16_t kc, keyrecord_t* record) {
	uint16_t lastActive = KC_TRNS;
	if (td->kcActive) {
		process_td_kc_user(false, td->kcActive, td);
		lastActive = td->kcActive;
	}
	if (timer_elapsed(td->timer) >= get_tapping_term(kc, record) || td->otherKeyPressedDuringHold) {
		// Released hold, so tap dance is done
		reset_td(td);
	} else if (td->count >= td->maxCount) {
		// Tap dance key was tapped and we don't have to wait for higher count tap/holds, so process it if we didn't already on key press
		switch (td->count) {
			case 1:
				if (td->kcTap && lastActive != td->kcTap) {
					tap_code(td->kcTap);
				}
				break;
			case 2:
			default: {
				if (td->kcDoubleTap) {
					if (lastActive != td->kcDoubleTap) {
						tap_code(td->kcDoubleTap);
						if (td->count == td->maxCount) {
							// When we first hit max count and know that we are just tapping, need to do all taps up until then (-1 for above tap and -1 since double tap starts on second tap, so start at 2)
							for (int i = 2; i < td->count; i++) {
								tap_code(td->kcTap);
							}
						}
					}
				} else if (td->kcTap && lastActive != td->kcTap) {
					tap_code(td->kcTap);
					if (td->count == td->maxCount) {
						// When we first hit max count and know that we are just tapping, need to do all "max count" taps (-1 for above tap so start at 1)
						for (int i = 1; i < td->count; i++) {
							tap_code(td->kcTap);
						}
					}
				}
				break;
			}
		}
		td->timer = timer_read();
	}
}

void user_tap_dance(qk_tap_dance_t* td, uint16_t kc, keyrecord_t* record) {
	td->pressed = record->event.pressed;
	if (record->event.pressed) {
		process_td_press(td, kc, record);
	} else {
		process_td_release(td, kc, record);
	}
}

/*
 * Trying to think through how tap dancing should work if you have a single tap/hold action, what should second
 * hold default to? E.g. shift/backspace. You tap backspace and then hold, should it backspace more or activate shift?
 * Probably start hold, if tapping term expires while held without another key being pressed, then tap and register
 * single tap. Same thing on 3rd+ press. Maybe make configurable like how QMK's tap dancing has get_tapping_force_hold
 */
void process_last_td_tap_on_expire_or_interrupt(qk_tap_dance_t* td) {
	// Tap dance ended while not being held (interrupted or expired), last release was within tapping term - process the tap for that release if we didn't already
	if (td->count < td->maxCount) {
		// TODO may be able to share code between here and process_td_release's tap block above
		switch (td->count) {
		case 1:
			tap_code(td->kcTap);
			break;
		case 2:
		default:
			// Never possible to hit this case currently since 2+ can't be less than max count
			if (td->kcDoubleTap) {
				// In theory could need more taps if maxCount > 2
				tap_code(td->kcDoubleTap);
			} else if (td->kcTap) {
				// In theory could need more taps if maxCount > 2
				tap_code(td->kcTap);
				tap_code(td->kcTap);
			}
			break;
		}
	}
	reset_td(td);
}

void process_td_interrupt_on_process_record(uint16_t keycode, qk_tap_dance_t* td) {
	if (keycode != td->kc && td->count) {
		if (td->pressed) {
			td->otherKeyPressedDuringHold = true;
		} else {
			process_last_td_tap_on_expire_or_interrupt(td);
		}
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    debug_process_record_user(keycode, record);
#endif 

	// Any other key should interrupt tap dances
	process_td_interrupt_on_process_record(keycode, &td1);
	process_td_interrupt_on_process_record(keycode, &td2);
	process_td_interrupt_on_process_record(keycode, &td3);

    // Process custom tap dances
    switch (keycode) {
		case TD1_KC:
			user_tap_dance(&td1, keycode, record);
			return false;
        case TD2_KC:
            user_tap_dance(&td2, keycode, record);
			return false;
        case TD3_KC:
            user_tap_dance(&td3, keycode, record);
			return false;
		case MY_RESET_KC:
			reset_keyboard();
			return false;
		default:
			break;
	}

	// Return true to indicate we did not handle this keypress
    return true;
}

void process_td_on_matrix_scan(qk_tap_dance_t* td) {
	// Check if TD was in progress and term has expired
	if (td->count && timer_elapsed(td->timer) >= get_tapping_term(td->kc, NULL)) {
		if (!td->pressed) {
			// We had released last key and term expired without another being pressed,
			// so handle the last key
			process_last_td_tap_on_expire_or_interrupt(td);
		} else if (td->count >= 2) {
			// Key pressed and count >= 2 (maybe should be count == maxCount + 1?)
			// Tapped and released (any number of times) and then held. TD only has single tap/hold and no double actions and no other keys pressed within term -> switch key from hold to tap. I think this is primarily for backspace/shift key to allow you to hold backspace by tapping it first?
			if (td->maxCount == 1 && !td->otherKeyPressedDuringHold) {
				// Need to only do this if another key hasn't been pressed while it's been held
				if (td->kcHold && td->kcTap && td->kcActive == td->kcHold) {
					process_td_kc_user(false, td->kcHold, td);
					process_td_kc_user(true, td->kcTap, td);
				}
			}
		}
	}
}

void matrix_scan_user(void) {
	process_td_on_matrix_scan(&td1);
	process_td_on_matrix_scan(&td2);
	process_td_on_matrix_scan(&td3);
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

// I think this is used by QMK tap dance code behind the scenes for first tap dance version,
// and is used in here for second version
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		case CTL_TAB:
            return 230;
        default:
		    /*if (is_home_row_mod(keycode)) {
				return 200;
			}*/
            return TAPPING_TERM;
    }
}

// Could this jut go in the normal process_record_user?
bool process_td_kc_user(bool pressed, uint16_t kc, qk_tap_dance_t* td) {
	switch (kc) {
		case KC_TRNS:
			return false;
		case TD_MO_0:
		case TD_MO_1:
		case TD_MO_2:
		case TD_MO_3:
		case TD_MO_4: {
			uint16_t layerNum = kc - TD_MO_0;
			if (pressed) {
				layer_on(layerNum);
			} else {
				layer_off(layerNum);
			}
			break;
		}
		default: {
			if (pressed) {
				register_code(kc);
			} else {
				unregister_code(kc);
			}
			break;
		}
	}

	if (pressed) {
		td->kcActive = kc;
	} else {
		td->kcActive = KC_TRNS;
	}
	
	return true;
}

bool is_mod_loading_combo(uint16_t index) {
	switch (index) {
		case COMBO_LCTL_SFT:
		case COMBO_LCTL_SFT_ALT:
		case COMBO_LGUI2:
		case COMBO_LGUI_SFT:
		case COMBO_LALT2:
		case COMBO_LALT_SFT:
			return true;
		default:
			return false;
	}
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
	if (is_mod_loading_combo(index)) {
		return 300;
	}
    //if (combo->keys[1] == CTL_TAB) {
    //    return 1000;
    //}
    return COMBO_TERM;
}

bool get_combo_must_press_in_order(uint16_t index, combo_t *combo) {
	if (is_mod_loading_combo(index)) {
		return true;
	}
	/*if (combo->keys[1] == CTL_TAB) {
        return true;
    }*/
	return false;
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
	if (layer_state_is(_GAMING)) {
		return false;
	}
	if (is_mod_loading_combo(combo_index) && get_mods()) {
		return false;
	}
	/*if (combo->keys[1] == CTL_TAB && get_mods()) {
		return false;
	}*/
	return true;
}