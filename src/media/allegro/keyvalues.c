/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                   |
| This file is part of Heroes.                                      |
|                                                                   |
| Heroes is free software; you can redistribute it and/or modify it |
| under the terms of the GNU General Public License version 2 as    |
| published by the Free Software Foundation.                        |
|                                                                   |
| Heroes is distributed in the hope that it will be useful, but     |
| WITHOUT ANY WARRANTY; without even the implied warranty of        |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU |
| General Public License for more details.                          |
|                                                                   |
| You should have received a copy of the GNU General Public License |
| along with this program; if not, write to the Free Software       |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA          |
| 02111-1307 USA                                                    |
`------------------------------------------------------------------*/

#include "system.h"
#include <allegro.h>
#include "keyvalues.h"

const a_keycode key_modifiers[HK_MOD__MAX] = {
  /* HK_MOD_None  */ 0,
  /* HK_MOD_Shift */ KB_SHIFT_FLAG,
  /* HK_MOD_Ctrl  */ KB_CTRL_FLAG,
  /* HK_MOD_Alt   */ KB_ALT_FLAG,
  /* HK_MOD_Meta  */ KB_LWIN_FLAG | KB_RWIN_FLAG
};

const a_keycode key_codes[HK__MAX] = {
  /* HK_0		*/ KEY_0,
  /* HK_1		*/ KEY_1,
  /* HK_2		*/ KEY_2,
  /* HK_3		*/ KEY_3,
  /* HK_4		*/ KEY_4,
  /* HK_5		*/ KEY_5,
  /* HK_6		*/ KEY_6,
  /* HK_7		*/ KEY_7,
  /* HK_8		*/ KEY_8,
  /* HK_9		*/ KEY_9,
  /* HK_BackSpace	*/ KEY_BACKSPACE,
  /* HK_CtrlL		*/ KEY_LCONTROL,
  /* HK_CtrlR		*/ KEY_RCONTROL,
  /* HK_D		*/ KEY_D,
  /* HK_d		*/ KEY_D,
  /* HK_E		*/ KEY_E,
  /* HK_Delete		*/ KEY_DEL,
  /* HK_Down		*/ KEY_DOWN,
  /* HK_End		*/ KEY_END,
  /* HK_Enter		*/ KEY_ENTER,
  /* HK_Escape		*/ KEY_ESC,
  /* HK_F		*/ KEY_F,
  /* HK_f		*/ KEY_F,
  /* HK_F1		*/ KEY_F1,
  /* HK_F2		*/ KEY_F2,
  /* HK_F3		*/ KEY_F3,
  /* HK_F4		*/ KEY_F4,
  /* HK_F5		*/ KEY_F5,
  /* HK_F6		*/ KEY_F6,
  /* HK_F7		*/ KEY_F7,
  /* HK_F8		*/ KEY_F8,
  /* HK_F9		*/ KEY_F9,
  /* HK_F10		*/ KEY_F10,
  /* HK_F11		*/ KEY_F11,
  /* HK_F12		*/ KEY_F12,
  /* HK_Home		*/ KEY_HOME,
  /* HK_i		*/ KEY_I,
  /* HK_I		*/ KEY_I,
  /* HK_Left		*/ KEY_LEFT,
  /* HK_NIL		*/ 0,
  /* HK_o		*/ KEY_O,
  /* HK_O		*/ KEY_O,
  /* HK_p		*/ KEY_P,
  /* HK_P		*/ KEY_P,
  /* HK_PageDown	*/ KEY_PGDN,
  /* HK_PageUp		*/ KEY_PGUP,
  /* HK_Pause		*/ KEY_PAUSE,
  /* HK_PrintScreen	*/ KEY_PRTSCR,
  /* HK_Right		*/ KEY_RIGHT,
  /* HK_S		*/ KEY_S,
  /* HK_s		*/ KEY_S,
  /* HK_ShiftL		*/ KEY_LSHIFT,
  /* HK_ShiftR		*/ KEY_RSHIFT,
  /* HK_Space		*/ KEY_SPACE,
  /* HK_SysRq		*/ KEY_PRTSCR,
  /* HK_t		*/ KEY_T,
  /* HK_T		*/ KEY_T,
  /* HK_Up		*/ KEY_UP
};
