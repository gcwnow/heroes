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

#include "system.h"
#include "rsc_files.h"
#include "rsc_files_hash.h"
#include "errors.h"
#include "debugmsg.h"

static void
check_rsc_spelling_error (const char *rsc_name)
{
  const char *approx = get_rsc_approx (rsc_name);
  if (approx)
    wmsg (_("%s: no such resource, possible misspelling of '%s'."),
	  rsc_name, approx);
  else
    wmsg (_("%s: no such resource."), rsc_name);
}

int
set_rsc_file (const char* rsc_name, const char* file_name, bool secure)
{
  struct rsc_file* res = in_rsc_set (rsc_name, strlen(rsc_name));

  if (res == 0) {
    check_rsc_spelling_error (rsc_name);
    return 1;
  }
  dmsg (D_RESOURCE, "set resource $(%s)=%s", rsc_name, file_name);
  if (res->modified_value)
    free (res->modified_value);
  res->modified_value = xstrdup (file_name);
  res->secure = secure;
  return 0;
}

/*  Search value for strings of the form `$(name)' to expand.  */
char*
rsc_expand_secure (char *value, bool *secure)
{
  int size = strlen (value) + 1;
  char *result;
  int dest;

  XMALLOC_ARRAY (result, size);
  for (dest = 0; *value; ++value)
    if (*value != '$' || value[1] != '(')
      result[dest++] = *value;
    else {
      char* end;

      value += 2;
      /* look for the closing parenthethis */
      for (end = value; *end && *end != ')'; ++end)
	/* NOP */;

      if (*end == 0) {		/* no closing parenthethis found */
	free (result);
	return 0;
      }
      *end = 0;

      /* insert the expanded value into the result string */
      {
	char* expanded;

	expanded = get_rsc_file_secure (value, secure);
	if (expanded) {
	  char* src;

	  size += strlen (expanded) - (end - value + 3);
	  XREALLOC_ARRAY (result, size);
	  for (src = expanded; *src;)
	    result[dest++] = *src++;
	  free (expanded);
	}
	value = end;
      }
    }
  result[dest] = 0;
  return result;
}

char*
rsc_expand (char *value)
{
  return rsc_expand_secure (value, 0);
}


char *
get_rsc_file_secure (const char *rsc_name, bool *secure)
{
  char *result;
  char *tmp;
  struct rsc_file *res = in_rsc_set (rsc_name, strlen(rsc_name));

  if (res == 0) {
    check_rsc_spelling_error (rsc_name);
    return 0;
  }
  if (res->expanded)		/* prevent infinite recursion */
    return 0;

  if (secure)
    *secure &= res->secure;
  res->expanded = 1;

  /* duplicate the value because rsc_expand will modify it */
  tmp = xstrdup (res->modified_value ? res->modified_value : res->value);
  dmsg (D_RESOURCE, "get resource $(%s)=%s [%s]", rsc_name, tmp,
	res->secure ? "trusted" : "untrusted");
  result = rsc_expand_secure (tmp, secure);
  dmsg (D_RESOURCE, "expanded resource $(%s)=%s [%s]", rsc_name, result,
	secure ? (*secure ? "trusted" : "untrusted") : "don't care");
  free (tmp);
  res->expanded = 0;
  return result;
}

char *
get_rsc_file (const char *rsc_name)
{
  return get_rsc_file_secure (rsc_name, 0);
}

char *
get_non_null_rsc_file_secure (const char *rsc_name, bool *secure)
{
  char* tmp = get_rsc_file_secure (rsc_name, secure);
  if (tmp == 0)
    emsg (_("%s: null resource"), rsc_name);
  if (!strcmp(tmp, ""))
    emsg (_("%s: empty resource"), rsc_name);
  return tmp;
}

char *
get_non_null_rsc_file (const char *rsc_name)
{
  return get_non_null_rsc_file_secure (rsc_name, 0);
}
