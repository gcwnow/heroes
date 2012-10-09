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
#include <SDL.h>
#include "keyvalues.h"

const a_keycode key_modifiers[HK_MOD__MAX] = {
  /* HK_MOD_None  */ 0,
  /* HK_MOD_Shift */ KMOD_SHIFT,
  /* HK_MOD_Ctrl  */ KMOD_CTRL,
  /* HK_MOD_Alt   */ KMOD_ALT,
  /* HK_MOD_Meta  */ KMOD_META
};

const a_keycode key_codes[HK__MAX] = {
  /* HK_0		*/ SDLK_0,
  /* HK_1		*/ SDLK_1,
  /* HK_2		*/ SDLK_2,
  /* HK_3		*/ SDLK_3,
  /* HK_4		*/ SDLK_4,
  /* HK_5		*/ SDLK_5,
  /* HK_6		*/ SDLK_6,
  /* HK_7		*/ SDLK_7,
  /* HK_8		*/ SDLK_8,
  /* HK_9		*/ SDLK_9,
  /* HK_BackSpace	*/ SDLK_BACKSPACE,
  /* HK_CtrlL		*/ SDLK_LCTRL,
  /* HK_CtrlR		*/ SDLK_RCTRL,
  /* HK_D		*/ 'D',
  /* HK_d		*/ 'd',
  /* HK_E		*/ 'E',
  /* HK_Delete		*/ SDLK_DELETE,
  /* HK_Down		*/ SDLK_DOWN,
  /* HK_End		*/ SDLK_END,
  /* HK_Enter		*/ SDLK_RETURN,
  /* HK_Escape		*/ SDLK_ESCAPE,
  /* HK_F		*/ 'F',
  /* HK_f		*/ 'f',
  /* HK_F1		*/ SDLK_F1,
  /* HK_F2		*/ SDLK_F2,
  /* HK_F3		*/ SDLK_F3,
  /* HK_F4		*/ SDLK_F4,
  /* HK_F5		*/ SDLK_F5,
  /* HK_F6		*/ SDLK_F6,
  /* HK_F7		*/ SDLK_F7,
  /* HK_F8		*/ SDLK_F8,
  /* HK_F9		*/ SDLK_F9,
  /* HK_F10		*/ SDLK_F10,
  /* HK_F11		*/ SDLK_F11,
  /* HK_F12		*/ SDLK_F12,
  /* HK_Home		*/ SDLK_HOME,
  /* HK_i		*/ 'i',
  /* HK_I		*/ 'I',
  /* HK_Left		*/ SDLK_LEFT,
  /* HK_NIL		*/ SDLK_UNKNOWN,
  /* HK_o		*/ 'o',
  /* HK_O		*/ 'O',
  /* HK_p		*/ 'p',
  /* HK_P		*/ 'P',
  /* HK_PageDown	*/ SDLK_PAGEDOWN,
  /* HK_PageUp		*/ SDLK_PAGEUP,
  /* HK_Pause		*/ SDLK_PAUSE,
  /* HK_PrintScreen	*/ SDLK_PRINT,
  /* HK_Right		*/ SDLK_RIGHT,
  /* HK_S		*/ 'S',
  /* HK_s		*/ 's',
  /* HK_ShiftL		*/ SDLK_LSHIFT,
  /* HK_ShiftR		*/ SDLK_RSHIFT,
  /* HK_Space		*/ SDLK_SPACE,
  /* HK_SysRq		*/ SDLK_SYSREQ,
  /* HK_t		*/ 't',
  /* HK_T		*/ 'T',
  /* HK_Up		*/ SDLK_UP
};
