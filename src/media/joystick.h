/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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

#ifndef HEROES__JOYSTICK__H
#define HEROES__JOYSTICK__H

extern int joystick_x[2];
extern int joystick_y[2];
extern char joystick_b[2];
extern char joystick_detected;

char joyinit (void);

void get_joystick_state (void);

#define is_joystick_up(j)    (joystick_y[j]<-8000)
#define is_joystick_down(j)  (joystick_y[j]>8000)
#define is_joystick_left(j)  (joystick_x[j]<-8000)
#define is_joystick_right(j) (joystick_x[j]>8000)
#define is_joystick_button_a(j)  (joystick_b[j]&1)
#define is_joystick_button_b(j)  (joystick_b[j]&2)

#endif /* HEROES__JOYSTICK__H */
