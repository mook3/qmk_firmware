/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Select hand configuration */

//#define MASTER_LEFT
#define MASTER_RIGHT
// #define EE_HANDS

#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define TAPPING_FORCE_HOLD_PER_KEY
#define TAPPING_TERM_PER_KEY
#define TAPPING_TERM 100

#define COMBO_TERM 45        // how quickly all combo keys must be pressed in succession to trigger
//#define COMBO_MUST_HOLD_MODS // if a combo triggers a modifier, only trigger when the combo is held
//#define COMBO_HOLD_TERM 175  // how long at least one of the combo keys must be held to trigger
#define COMBO_TERM_PER_COMBO
#define COMBO_MUST_PRESS_IN_ORDER_PER_COMBO	
#define COMBO_SHOULD_TRIGGER
