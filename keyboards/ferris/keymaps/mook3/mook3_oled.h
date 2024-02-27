#pragma once

#include QMK_KEYBOARD_H
#include "mook3_layers.h"

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
