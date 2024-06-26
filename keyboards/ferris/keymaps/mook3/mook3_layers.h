#pragma once

enum layer_number {
  _QWERTY = 0,
  _GAMING,
  _SYMBOLS,
  _NUMBERS,
  _FN,
};

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

static uint16_t custom_idle_timer = 0;