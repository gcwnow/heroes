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
#include <ggi/ggi.h>
#include "keyvalues.h"

const a_keycode key_modifiers[HK_MOD__MAX] = {
  /* HK_MOD_None  */ 0,
  /* HK_MOD_Shift */ GII_MOD_SHIFT,
  /* HK_MOD_Ctrl  */ GII_MOD_CTRL,
  /* HK_MOD_Alt	  */ GII_MOD_ALT,
  /* HK_MOD_Meta  */ GII_MOD_META
};

const a_keycode key_codes[HK__MAX] = {
  /* HK_0		*/ GIIUC_0,
  /* HK_1		*/ GIIUC_1,
  /* HK_2		*/ GIIUC_2,
  /* HK_3		*/ GIIUC_3,
  /* HK_4		*/ GIIUC_4,
  /* HK_5		*/ GIIUC_5,
  /* HK_6		*/ GIIUC_6,
  /* HK_7		*/ GIIUC_7,
  /* HK_8		*/ GIIUC_8,
  /* HK_9		*/ GIIUC_9,
  /* HK_BackSpace	*/ GIIUC_BackSpace,
  /* HK_CtrlL		*/ GIIK_CtrlL,
  /* HK_CtrlR		*/ GIIK_CtrlR,
  /* HK_D		*/ GIIUC_D,
  /* HK_d		*/ GIIUC_d,
  /* HK_E		*/ GIIUC_E,
  /* HK_Delete		*/ GIIK_Delete,
  /* HK_Down		*/ GIIK_Down,
  /* HK_End		*/ GIIK_End,
  /* HK_Enter		*/ GIIUC_Return,
  /* HK_Escape		*/ GIIUC_Escape,
  /* HK_F		*/ GIIUC_F,
  /* HK_f		*/ GIIUC_f,
  /* HK_F1		*/ GIIK_F1,
  /* HK_F2		*/ GIIK_F2,
  /* HK_F3		*/ GIIK_F3,
  /* HK_F4		*/ GIIK_F4,
  /* HK_F5		*/ GIIK_F5,
  /* HK_F6		*/ GIIK_F6,
  /* HK_F7		*/ GIIK_F7,
  /* HK_F8		*/ GIIK_F8,
  /* HK_F9		*/ GIIK_F9,
  /* HK_F10		*/ GIIK_F10,
  /* HK_F11		*/ GIIK_F11,
  /* HK_F12		*/ GIIK_F12,
  /* HK_Home		*/ GIIK_Home,
  /* HK_i		*/ GIIUC_i,
  /* HK_I		*/ GIIUC_I,
  /* HK_Left		*/ GIIK_Left,
  /* HK_NIL		*/ GIIK_NIL,
  /* HK_o		*/ GIIUC_o,
  /* HK_O		*/ GIIUC_O,
  /* HK_p		*/ GIIUC_p,
  /* HK_P		*/ GIIUC_P,
  /* HK_PageDown	*/ GIIK_PageDown,
  /* HK_PageUp		*/ GIIK_PageUp,
  /* HK_Pause		*/ GIIK_Pause,
  /* HK_PrintScreen	*/ GIIK_PrintScreen,
  /* HK_Right		*/ GIIK_Right,
  /* HK_S		*/ GIIUC_S,
  /* HK_s		*/ GIIUC_s,
  /* HK_ShiftL		*/ GIIK_ShiftL,
  /* HK_ShiftR		*/ GIIK_ShiftR,
  /* HK_Space		*/ GIIUC_Space,
  /* HK_SysRq		*/ GIIK_SysRq,
  /* HK_t		*/ GIIUC_t,
  /* HK_T		*/ GIIUC_T,
  /* HK_Up		*/ GIIK_Up
};
