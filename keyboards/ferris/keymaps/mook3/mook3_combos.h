#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

enum combos {
	/*
    COMBO_LCTL,
    COMBO_LALT,
	COMBO_LGUI,
	COMBO_LCTL_ALT,
    COMBO_RCTL,
    COMBO_RALT,
	COMBO_RGUI,
	COMBO_RCTL_ALT,
	*/
	
	COMBO_LCTL_SFT,
	COMBO_LCTL_SFT_ALT,
	COMBO_LGUI2,
	COMBO_LGUI_SFT,
	COMBO_LALT2,
	COMBO_LALT_SFT,
	COMBO_FN,
	
	COMBO_LALT_SFT2,
	COMBO_LGUI_SFT2,
	COMBO_LCTL_SFT2,

    COMBO_LENGTH // nifty trick to avoid manually specifying how many combos you have
};
uint16_t MOD_LOADING_COMBOS_START_IDX = COMBO_LCTL_SFT;
uint16_t COMBO_LEN = COMBO_LENGTH;

/*
const uint16_t PROGMEM combo_fd[] = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM combo_fs[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM combo_fa[] = {KC_F, KC_A, COMBO_END};
const uint16_t PROGMEM combo_fds[] = {KC_F, KC_D, KC_S, COMBO_END};
const uint16_t PROGMEM combo_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_jl[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM combo_jq[] = {KC_J, KC_QUOT, COMBO_END};
const uint16_t PROGMEM combo_jkl[] = {KC_J, KC_K, KC_L, COMBO_END};
*/

const uint16_t PROGMEM combo_fctl[] = {KC_F, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_rctl[] = {KC_R, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_dctl[] = {KC_D, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_ectl[] = {KC_E, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_sctl[] = {KC_S, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_wctl[] = {KC_W, CTL_TAB, COMBO_END};
const uint16_t PROGMEM combo_actl[] = {KC_A, CTL_TAB, COMBO_END};

const uint16_t PROGMEM combo_ssft[] = {KC_S, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_dsft[] = {KC_D, SFT_BSP, COMBO_END};
const uint16_t PROGMEM combo_fsft[] = {KC_F, SFT_BSP, COMBO_END};

combo_t key_combos[] = {
	// One-shot mods
	/*
	[COMBO_LCTL] = COMBO(combo_fd, OSM(MOD_LCTL)),
	[COMBO_LALT] = COMBO(combo_fs, OSM(MOD_LALT)),
	[COMBO_LGUI] = COMBO(combo_fa, OSM(MOD_LGUI)),
	[COMBO_LCTL_ALT] = COMBO(combo_fds, OSM(MOD_LCTL | MOD_LALT)),
	[COMBO_RCTL] = COMBO(combo_jk, OSM(MOD_RCTL)),
	[COMBO_RALT] = COMBO(combo_jl, OSM(MOD_RALT)),
	[COMBO_RGUI] = COMBO(combo_jq, OSM(MOD_RGUI)),
	[COMBO_RCTL_ALT] = COMBO(combo_jkl, OSM(MOD_RCTL | MOD_RALT)),
	*/
	
	[COMBO_LCTL_SFT] = COMBO(combo_fctl, MO(_FN)),
	[COMBO_LCTL_SFT_ALT] = COMBO(combo_rctl, LSFT(LCTL(KC_LALT))),
	[COMBO_LGUI2] = COMBO(combo_dctl, KC_LGUI),
	[COMBO_LGUI_SFT] = COMBO(combo_ectl, LSFT(KC_LGUI)),
	[COMBO_LALT2] = COMBO(combo_sctl, KC_LALT),
	[COMBO_LALT_SFT] = COMBO(combo_wctl, LSFT(KC_LALT)),
	[COMBO_FN] = COMBO(combo_actl, MO(_FN)),
	
	[COMBO_LALT_SFT2] = COMBO(combo_ssft, LSFT(KC_LALT)),
	[COMBO_LGUI_SFT2] = COMBO(combo_dsft, LSFT(KC_LGUI)),
	[COMBO_LCTL_SFT2] = COMBO(combo_fsft, LSFT(KC_LCTL)),
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
	if (combo_index != COMBO_LCTL_SFT2 && timer_elapsed(custom_idle_timer) < 150) {
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