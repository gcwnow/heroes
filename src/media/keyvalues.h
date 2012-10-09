/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License version 2 as published by   |
| the Free Software Foundation.                                           |
|                                                                         |
| Heroes is distributed in the hope that it will be useful, but WITHOUT   |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   |
| for more details.                                                       |
|                                                                         |
| You should have received a copy of the GNU General Public License along |
| with this program; if not, write to the Free Software Foundation, Inc., |
| 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   |
`------------------------------------------------------------------------*/


#ifndef HEROES__KEYVALUES__H
#define HEROES__KEYVALUES__H

#include "keyb.h"

/* Define keys hard coded into heroes code */

/* modifiers */
enum {
  HK_MOD_None_,
#define HK_MOD_None (key_modifiers[HK_MOD_None_])
  HK_MOD_Shift_,
#define HK_MOD_Shift (key_modifiers[HK_MOD_Shift_])
  HK_MOD_Ctrl_,
#define HK_MOD_Ctrl (key_modifiers[HK_MOD_Ctrl_])
  HK_MOD_Alt_,
#define HK_MOD_Alt (key_modifiers[HK_MOD_Alt_])
  HK_MOD_Meta_,
#define HK_MOD_Meta (key_modifiers[HK_MOD_Meta_])
  /* Not a key.  Must come last.  */
  HK_MOD__MAX
};

extern const a_keycode key_modifiers[HK_MOD__MAX];

/* keys */
enum {
  HK_0_,
#define HK_0 (key_codes[HK_0_])
  HK_1_,
#define HK_1 (key_codes[HK_1_])
  HK_2_,
#define HK_2 (key_codes[HK_2_])
  HK_3_,
#define HK_3 (key_codes[HK_3_])
  HK_4_,
#define HK_4 (key_codes[HK_4_])
  HK_5_,
#define HK_5 (key_codes[HK_5_])
  HK_6_,
#define HK_6 (key_codes[HK_6_])
  HK_7_,
#define HK_7 (key_codes[HK_7_])
  HK_8_,
#define HK_8 (key_codes[HK_8_])
  HK_9_,
#define HK_9 (key_codes[HK_9_])
  HK_BackSpace_,
#define HK_BackSpace (key_codes[HK_BackSpace_])
  HK_CtrlL_,
#define HK_CtrlL (key_codes[HK_CtrlL_])
  HK_CtrlR_,
#define HK_CtrlR (key_codes[HK_CtrlR_])
  HK_D_,
#define HK_D (key_codes[HK_D_])
  HK_d_,
#define HK_d (key_codes[HK_d_])
  HK_E_,
#define HK_E (key_codes[HK_E_])
  HK_Delete_,
#define HK_Delete (key_codes[HK_Delete_])
  HK_Down_,
#define HK_Down (key_codes[HK_Down_])
  HK_End_,
#define HK_End (key_codes[HK_End_])
  HK_Enter_,
#define HK_Enter (key_codes[HK_Enter_])
  HK_Escape_,
#define HK_Escape (key_codes[HK_Escape_])
  HK_F_,
#define HK_F (key_codes[HK_F_])
  HK_f_,
#define HK_f (key_codes[HK_f_])
  HK_F1_,
#define HK_F1 (key_codes[HK_F1_])
  HK_F2_,
#define HK_F2 (key_codes[HK_F2_])
  HK_F3_,
#define HK_F3 (key_codes[HK_F3_])
  HK_F4_,
#define HK_F4 (key_codes[HK_F4_])
  HK_F5_,
#define HK_F5 (key_codes[HK_F5_])
  HK_F6_,
#define HK_F6 (key_codes[HK_F6_])
  HK_F7_,
#define HK_F7 (key_codes[HK_F7_])
  HK_F8_,
#define HK_F8 (key_codes[HK_F8_])
  HK_F9_,
#define HK_F9 (key_codes[HK_F9_])
  HK_F10_,
#define HK_F10 (key_codes[HK_F10_])
  HK_F11_,
#define HK_F11 (key_codes[HK_F11_])
  HK_F12_,
#define HK_F12 (key_codes[HK_F12_])
  HK_Home_,
#define HK_Home (key_codes[HK_Home_])
  HK_i_,
#define HK_i (key_codes[HK_i_])
  HK_I_,
#define HK_I (key_codes[HK_I_])
  HK_Left_,
#define HK_Left (key_codes[HK_Left_])
  HK_NIL_,
#define HK_NIL (key_codes[HK_NIL_])
  HK_o_,
#define HK_o (key_codes[HK_o_])
  HK_O_,
#define HK_O (key_codes[HK_O_])
  HK_p_,
#define HK_p (key_codes[HK_p_])
  HK_P_,
#define HK_P (key_codes[HK_P_])
  HK_PageDown_,
#define HK_PageDown (key_codes[HK_PageDown_])
  HK_PageUp_,
#define HK_PageUp (key_codes[HK_PageUp_])
  HK_Pause_,
#define HK_Pause (key_codes[HK_Pause_])
  HK_PrintScreen_,
#define HK_PrintScreen (key_codes[HK_PrintScreen_])
  HK_Right_,
#define HK_Right (key_codes[HK_Right_])
  HK_S_,
#define HK_S (key_codes[HK_S_])
  HK_s_,
#define HK_s (key_codes[HK_s_])
  HK_ShiftL_,
#define HK_ShiftL (key_codes[HK_ShiftL_])
  HK_ShiftR_,
#define HK_ShiftR (key_codes[HK_ShiftR_])
  HK_Space_,
#define HK_Space (key_codes[HK_Space_])
  HK_SysRq_,
#define HK_SysRq (key_codes[HK_SysRq_])
  HK_t_,
#define HK_t (key_codes[HK_t_])
  HK_T_,
#define HK_T (key_codes[HK_T_])
  HK_Up_,
#define HK_Up (key_codes[HK_Up_])
  /* Not a key.  Must come last.  */
  HK__MAX
};

extern const a_keycode key_codes[HK__MAX];

#endif /* HEROES__KEYVALUES__H */
