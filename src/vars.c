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
#include "hash.h"
#include "vars.h"
#include "cast.h"

typedef struct a_var_entry a_var_entry;
struct a_var_entry {
  char *name;
  char *value;
};

Hash_table *var_hash;

static bool
var_equ (const void *left, const void *right)
{
  const a_var_entry *l = left;
  const a_var_entry *r = right;
  return !strcasecmp (l->name, r->name);
}

static unsigned
var_hasher (const void *data, unsigned size)
{
  const a_var_entry *l = data;
  return hash_string (l->name, size);
}

static void
var_free (void *data)
{
  a_var_entry *l = data;
  XFREE (l->name);
  XFREE (l->value);
  free (l);
}

static a_var_entry *
var_cons (const char *name, const char *value)
{
  NEW (a_var_entry, v);
  v->name = name ? xstrdup (name) : 0;
  v->value = value ? xstrdup (value) : 0;
  return v;
}

void
var_initialize (void)
{
  var_hash = hash_initialize (17, NULL, var_hasher, var_equ, var_free);
  if (!var_hash)
    xalloc_die ();
}

void
var_uninitialize (void)
{
  hash_free (var_hash);
}

void
var_define (const char *name, const char *value)
{
  if (!hash_insert (var_hash, var_cons (name, value)))
    xalloc_die ();
}

const char *
var_get_value (const char *name)
{
  a_var_entry ve;
  ve.name = const_cast_string (name);
  return hash_lookup (var_hash, &ve);
}

void
var_print_all (void)
{
  a_var_entry *cur = hash_get_first (var_hash);
  while (cur) {
    printf ("%s = %s\n", cur->name, cur->value);
    cur = hash_get_next (var_hash, cur);
  }
}
