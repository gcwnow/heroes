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
#include "keyvalues.h"

const a_keycode key_modifiers[HK_MOD__MAX] = {
  HK_MOD_None_,
  HK_MOD_Shift_,
  HK_MOD_Ctrl_,
  HK_MOD_Alt_,
  HK_MOD_Meta_
};

const a_keycode key_codes[HK__MAX] = {
  HK_0_,
  HK_1_,
  HK_2_,
  HK_3_,
  HK_4_,
  HK_5_,
  HK_6_,
  HK_7_,
  HK_8_,
  HK_9_,
  HK_BackSpace_,
  HK_CtrlL_,
  HK_CtrlR_,
  HK_D_,
  HK_d_,
  HK_E_,
  HK_Delete_,
  HK_Down_,
  HK_End_,
  HK_Enter_,
  HK_Escape_,
  HK_F_,
  HK_f_,
  HK_F1_,
  HK_F2_,
  HK_F3_,
  HK_F4_,
  HK_F5_,
  HK_F6_,
  HK_F7_,
  HK_F8_,
  HK_F9_,
  HK_F10_,
  HK_F11_,
  HK_F12_,
  HK_Home_,
  HK_i_,
  HK_I_,
  HK_Left_,
  HK_NIL_,
  HK_o_,
  HK_O_,
  HK_p_,
  HK_P_,
  HK_PageDown_,
  HK_PageUp_,
  HK_Pause_,
  HK_PrintScreen_,
  HK_Right_,
  HK_S_,
  HK_s_,
  HK_ShiftL_,
  HK_ShiftR_,
  HK_Space_,
  HK_SysRq_,
  HK_t_,
  HK_T_,
  HK_Up_
};
