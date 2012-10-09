## Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
##
## This file is part of Heroes.
##
## Heroes is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License version 2 as published by
## the Free Software Foundation.
##
## Heroes is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##

noinst_LIBRARIES= lvl/libhlvl.a

lvl_libhlvl_a_SOURCES= \
  lvl/access.h \
  lvl/de_body.c \
  lvl/de_head.c \
  lvl/en_body.c \
  lvl/en_head.c \
  lvl/format.h \
  lvl/free.c \
  lvl/in_body.c \
  lvl/load.c \
  lvl/lvl.h \
  lvl/lvl_priv.h \
  lvl/privacc.c \
  lvl/save.c

EXTRA_DIST += \
  lvl/README
