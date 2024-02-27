#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

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