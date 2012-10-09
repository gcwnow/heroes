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


#define font_pos          30	/* position du premier caractère (n° pixel) */
#define font_first_ascii  32	/* number du premier caractère (ASCII) */
#define font_transp_color 0	/* couleur transparante */
#define font_height       6	/* hauteur de chaque caractère */

static char font_width[62] =	/* largeur de chaque caractère */
{ 2, 1, 3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 2, 3, 2, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 1, 2, 0, 3, 0,
/*      ! " ° $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = >  */

  3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 2, 4, 2
};
/*    ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ]  */
