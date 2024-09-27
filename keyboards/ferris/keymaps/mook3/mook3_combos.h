#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

enum combos {
	COMBO_FN,
	COMBO_G,
	COMBO_GC,
	COMBO_A,
	COMBO_AC,
	COMBO_GA,
	COMBO_GAC,
	
	COMBO_CS,
	COMBO_GS,
	COMBO_GCS,
	COMBO_AS,
	COMBO_ACS, // MEH
	COMBO_GAS,
	COMBO_GACS, // HYPR

    COMBO_LENGTH // nifty trick to avoid manually specifying how many combos you have
};
uint16_t MOD_LOADING_COMBOS_START_IDX = COMBO_FN;
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM combo_fctl[] = {KC_F, CTL_TAB, COMBO_END};
//const uint16_t PROGMEM combo_rctl[] = {KC_R, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_dctl[] = {KC_D, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_ectl[] = {KC_E, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_sctl[] = {KC_S, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_wctl[] = {KC_W, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_actl[] = {KC_A, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_qctl[] = {KC_Q, CTL_TAB, COMBO_END};

const uint16_t PROGMEM combo_fsft[] = {KC_F, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_dsft[] = {KC_D, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_esft[] = {KC_E, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_ssft[] = {KC_S, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_wsft[] = {KC_W, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_asft[] = {KC_A, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_qsft[] = {KC_Q, SFT_BSP, COMBO_END};


combo_t key_combos[] = {
	[COMBO_FN] = COMBO(combo_fctl, MO(_FN)),
	[COMBO_G] = COMBO(combo_dctl, KC_LGUI),
	[COMBO_GC] = COMBO(combo_ectl, C(KC_LGUI)),
	[COMBO_A] = COMBO(combo_sctl, KC_LALT),
	[COMBO_AC] = COMBO(combo_wctl, C(KC_LALT)),
	[COMBO_GA] = COMBO(combo_actl, A(KC_LGUI)),
	[COMBO_GAC] = COMBO(combo_qctl, C(A(KC_LGUI))),

	[COMBO_CS] = COMBO(combo_fsft, S(KC_LCTL)),
	[COMBO_GS] = COMBO(combo_dsft, S(KC_LGUI)),
	[COMBO_GCS] = COMBO(combo_esft, S(C(KC_LGUI))),
	[COMBO_AS] = COMBO(combo_ssft, S(KC_LALT)),
	[COMBO_ACS] = COMBO(combo_wsft, KC_MEH),
	[COMBO_GAS] = COMBO(combo_asft, S(A(KC_LGUI))),
	[COMBO_GACS] = COMBO(combo_qsft, KC_HYPR),
};

bool is_mod_loading_combo(uint16_t index) {
	//return index >= MOD_LOADING_COMBOS_START_IDX && index < COMBO_LEN;
	return true;
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
	if (is_mod_loading_combo(index)) {
		return 250;
	}
    return COMBO_TERM;
}

bool get_combo_must_press_in_order(uint16_t index, combo_t *combo) {
	if (is_mod_loading_combo(index)) {
		return true;
	}
	return false;
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
	if (layer_state_is(_GAMING)) {
		return false;
	}
	if (combo_index != COMBO_CS && timer_elapsed(custom_idle_timer) < 150) {
		// Keyboard must have been idle for configured amount of time for combos to work.
		// This disables combo when fast typing, mostly to remove input delay by allowing
		// keys to be registered on keydown rather than keyup.
		return false;
	}
	if (is_mod_loading_combo(combo_index) && get_mods()) {
		return false;
	}
	return true;
}
