#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

typedef struct {
	// Could probably just make this have some sorta 2d array where row 0 is an array of length 2 with single hold, tap keycodes, row 1 is array of length 2 with double hold, tap, etc.
	// Maybe second struct with just kcHold, kcTap values would be good, and then this holds an array of those structs
	const uint16_t kc;
    const uint16_t * kcs;
	const uint16_t maxCount;
	const uint16_t maxKcIdx;
	uint16_t timer;
	uint16_t count;
	bool pressed;
	bool otherKeyPressedDuringHold;
	uint16_t kcActive;
} qk_tap_dance_t;

const uint16_t td1_kcs[] = { KC_BSPC, KC_LSFT };
static qk_tap_dance_t td1 = { .kc = SFT_BSP, .kcs = td1_kcs, .maxCount = 1, .maxKcIdx = 1 };
const uint16_t td2_kcs[] = { KC_TAB, KC_LCTL, _______, TD_MO_4 };
static qk_tap_dance_t td2 = { .kc = CTL_TAB, .kcs = td2_kcs, .maxCount = 2, .maxKcIdx = 3 };

#define NUM_TDS 2
qk_tap_dance_t * custom_tds[] = {
	&td1, &td2,
};

// Functions used externally
bool process_record_user_tds(uint16_t keycode, keyrecord_t* record);
void matrix_scan_user_tds(void);

// Internal functions
bool process_td_kc_user(bool pressed, uint16_t kc, qk_tap_dance_t* td);
void reset_td(qk_tap_dance_t* td);
void process_td_press(qk_tap_dance_t* td, keyrecord_t* record);
void process_td_release(qk_tap_dance_t* td, keyrecord_t* record);
void process_last_td_tap_on_expire_or_interrupt(qk_tap_dance_t* td);

void reset_td(qk_tap_dance_t* td) {
	td->count = 0;
	td->otherKeyPressedDuringHold = false;
}

uint16_t idx_to_count(uint16_t idx) {
	return idx / 2 + 1;
}

uint16_t count_to_idx(uint16_t count, bool hold) {
	// hold could just be td->count * 2 - 1
	uint16_t idx = (count - 1) * 2;
	return hold ? idx + 1 : idx;
}

uint16_t get_highest_kc_idx(qk_tap_dance_t* td, bool hold) {
	for (uint8_t index = count_to_idx(td->count, hold); index >= 0; index -= 2) {
		if (index <= td->maxKcIdx && td->kcs[index] != KC_TRNS) {
			return index;
		}
	}
	return -1;
}

void process_td_press(qk_tap_dance_t* td, keyrecord_t* record) {
	if (td->count > 0 && timer_elapsed(td->timer) < get_tapping_term(td->kc, record)) {
		// Pressed again within tapping term, increase count
		td->count += 1;
	} else {
		// TODO any chance we need to wrap up last tap dance here if we are barely past tapping term - or is that guaranteed to be handled? Maybe check if count > 0 and call the finish function
		// Count is 0 or outside tapping term, so first tap
		td->count = 1;
	}
	// Only tap if maxTapIdx > maxHoldIdx && maxTapCount >= count
	for (uint8_t index = count_to_idx(td->count, true); index >= 0; index--) {
		if (index <= td->maxKcIdx && td->kcs[index] != KC_TRNS) {
			if (index % 2 == 1 || index == td->maxKcIdx) {
				// hold, or tap that's highest td action
				process_td_kc_user(true, td->kcs[index], td);
				break;
			}
		}
	}
	// TODO use record->event.time instead of timer_read? (just for perf)
	td->timer = timer_read();
}

void do_catch_up_taps(qk_tap_dance_t* td, uint16_t tap_idx) {
	if (tap_idx >= 0) {
		uint16_t tap_count = idx_to_count(tap_idx);
		for (uint8_t i = tap_count; i <= td->count; i++) {
			tap_code(td->kcs[tap_idx]);
		}
	}
}

void process_td_release(qk_tap_dance_t* td, keyrecord_t* record) {
	uint16_t lastActive = KC_TRNS;
	if (td->kcActive != KC_TRNS) {
		lastActive = td->kcActive;
		process_td_kc_user(false, td->kcActive, td);
	}
	if (timer_elapsed(td->timer) >= get_tapping_term(td->kc, record) || td->otherKeyPressedDuringHold) {
		// Released hold, so tap dance is done
		reset_td(td);
	} else if (td->count >= td->maxCount) {
		// Tap dance key was tapped and we don't have to wait for higher count tap/holds, so process it if we didn't already on key press
		uint16_t highest_tap_idx = get_highest_kc_idx(td, false);
		if (highest_tap_idx >= 0 && td->kcs[highest_tap_idx] != lastActive) {
			// When we first hit max count and know that we are just tapping, need to do all taps up until then
			if (td->count == td->maxCount) {
				do_catch_up_taps(td, highest_tap_idx);
			} else {
				tap_code(td->kcs[highest_tap_idx]);
			}
		}
		td->timer = timer_read();
	}
}

/*
 * Trying to think through how tap dancing should work if you have a single tap/hold action, what should second
 * hold default to? E.g. shift/backspace. You tap backspace and then hold, should it backspace more or activate shift?
 * Probably start hold, if tapping term expires while held without another key being pressed, then tap and register
 * single tap. Same thing on 3rd+ press. Maybe make configurable like how QMK's tap dancing has get_tapping_force_hold
 */
void process_last_td_tap_on_expire_or_interrupt(qk_tap_dance_t* td) {
	// Tap dance ended while not being held (interrupted or expired), last release was within tapping term - process the tap for that release if we didn't already (would have already known to do tap on release if >= maxCount)
	if (td->count < td->maxCount) {
		do_catch_up_taps(td, get_highest_kc_idx(td, false));
	}
	reset_td(td);
}

bool process_record_user_tds(uint16_t keycode, keyrecord_t* record) {
	// Any other key should interrupt tap dances
	for (uint8_t index = 0; index < NUM_TDS; index++) {
		qk_tap_dance_t* td = custom_tds[index];
		if (keycode != td->kc && td->count) {
			if (td->pressed) {
				td->otherKeyPressedDuringHold = true;
			} else {
				process_last_td_tap_on_expire_or_interrupt(td);
			}
		}
	}

	// Process custom tap dances
	for (uint8_t index = 0; index < NUM_TDS; index++) {
		qk_tap_dance_t* td = custom_tds[index];
		if (keycode == td->kc) {
			td->pressed = record->event.pressed;
			if (record->event.pressed) {
				process_td_press(td, record);
			} else {
				process_td_release(td, record);
			}
			return false;

		}
	}

	// Return true to indicate we did not handle this keypress
    return true;
}

void matrix_scan_user_tds(void) {
	for (uint8_t index = 0; index < NUM_TDS; index++) {
		qk_tap_dance_t* td = custom_tds[index];
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
					if (td->kcs[1] != KC_TRNS && td->kcs[0] != KC_TRNS && td->kcActive == td->kcs[1]) {
						process_td_kc_user(false, td->kcs[1], td);
						process_td_kc_user(true, td->kcs[0], td);
					}
				}
			}
		}
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

// I think this is used by QMK tap dance code behind the scenes for first tap dance version,
// and is used in here for second version
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		case CTL_TAB:
            //return 230;
			return 170;
        default:
		    /*if (is_home_row_mod(keycode)) {
				return 200;
			}*/
            return TAPPING_TERM;
    }
}