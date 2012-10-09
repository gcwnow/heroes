/*------------------------------------------------------------------------.
| Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>                 |
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

/*
 * This is a generic list.  Since `void*' is evil, all that file is
 * written as a macro that will instanciate the list structures and
 * functions for a given type.
 *
 * Three macros are defined:
 *
 *   NEW_LIST_SPEC(PREFIX,TYPE)
 *       generate the declarations,
 *
 *   NEW_LIST_BODY(PREFIX,TYPE,EQUAL_P,DESTRUCTOR)
 *       generate the corresponding definitions
 *
 *   NEW_LIST(PREFIX,TYPE,EQUAL_P,DESTRUCTOR)
 *       is a shorthand that generates both.
 *
 * PREFIX     is prepended to each function and structure created.
 * TYPE       is the plain type of the data hold by the cells.
 * EQUAL_P    is a binary function used to compare to cells.
 * DESTRUCTOR is a fonction call before a cell is freed.
 *
 * Additionally, NULL_DESTRUCTOR can be used as a DESTRUCTOR argument
 * when none are needed; and STD_EQUAL is the standard equal
 * comparison function (==), meant to be used as a EQUAL_P argument.
 */

#define NULL_DESTRUCTOR(x) ;
#define STD_EQUAL(a, b) ((a) == (b))

/* Indenting rules of most editors require to put a semicolon after
 * the call to these macros, as in
 *    NEW_LIST_SPEC(foo,int);
 *                         ^^^
 * Therefore macros should arrange so that this trailing semicolon is
 * legal (out of functions, a single semicolon left alone is
 * considered as an empty declaration which is not legal).
 */

#define NEW_LIST_SPEC(PREFIX,TYPE)					\
									\
struct PREFIX##_list {							\
  TYPE car;								\
  struct PREFIX##_list* cdr;						\
};									\
									\
typedef struct PREFIX##_list* PREFIX##_list;				\
									\
PREFIX##_list   PREFIX##_cons   (TYPE value, PREFIX##_list tail);	\
void            PREFIX##_delete (PREFIX##_list* list);			\
PREFIX##_list   PREFIX##_member (PREFIX##_list list, TYPE value);	\
void            PREFIX##_push   (PREFIX##_list* list, TYPE value);	\
TYPE            PREFIX##_pop    (PREFIX##_list* list);			\
void		PREFIX##_clear  (PREFIX##_list* list)

#define NEW_LIST_BODY(PREFIX,TYPE,EQUAL_P,DESTRUCTOR)			\
									\
PREFIX##_list								\
PREFIX##_cons (TYPE value, PREFIX##_list tail)				\
{									\
  PREFIX##_list result = malloc (sizeof (*result));			\
  result->car = value;							\
  result->cdr = tail;							\
  return result;							\
}									\
									\
void 									\
PREFIX##_delete (PREFIX##_list* list)					\
{									\
  PREFIX##_list result = *list;						\
  *list = (*list)->cdr;							\
  free (result);							\
}									\
									\
PREFIX##_list								\
PREFIX##_member (PREFIX##_list list, TYPE value)			\
{									\
  while (list) {							\
    if (EQUAL_P (list->car, value))					\
      return list;							\
    list = list->cdr;							\
  }									\
  return 0;								\
}									\
									\
void									\
PREFIX##_push (PREFIX##_list* list, TYPE value)				\
{									\
  *list = PREFIX##_cons (value, *list);					\
}									\
									\
TYPE									\
PREFIX##_pop (PREFIX##_list* list)					\
{									\
  TYPE result = (*list)->car;						\
  PREFIX##_delete (list);						\
  return result;							\
}									\
									\
void									\
PREFIX##_clear (PREFIX##_list* list)					\
{									\
  PREFIX##_list next;							\
									\
  while (*list) {							\
    next = (*list)->cdr;						\
    DESTRUCTOR ((*list)->car);						\
    free (*list);							\
    *list = next;							\
  }									\
}									\
									\
extern void dummy_function_declaration_to_eat_ending_semicolon (void)

#define NEW_LIST(PREFIX,TYPE,EQUAL_P,DESTRUCTOR)	\
NEW_LIST_SPEC(PREFIX,TYPE);				\
NEW_LIST_BODY(PREFIX,TYPE,EQUAL_P,DESTRUCTOR)
