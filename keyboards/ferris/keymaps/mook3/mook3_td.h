#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

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

bool process_record_user_tds(uint16_t keycode, keyrecord_t* record) {
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

void matrix_scan_user_tds(void) {
	process_td_on_matrix_scan(&td1);
	process_td_on_matrix_scan(&td2);
	process_td_on_matrix_scan(&td3);
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