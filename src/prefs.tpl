[~ autogen5 template h c
# Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
#
# This file is part of Heroes.
#
# Heroes is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# Heroes is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
~]
[~ (dne "** " "/*\t\t\t\t") ~]
*/
[~ (begin
     (define (sget name)
	     (and (exist? name)
                  (get name)))
     (define (c-doc)
       (let ((doc (sget ".doc")))
	 (if doc
             (sprintf "  /* %s  */\n" doc)
	     "")))
     (define (sh-doc)
       (let ((doc (sget ".doc")))
	 (if doc
	     (sprintf "# %s" doc)
             "")))
     (define (c-attrib)
       (let* ((type (sget ".type"))
              (array (sget ".array"))
              (decl (if type
                      (sprintf "%-20s %s%s" type (get ".attrib")
			       (if array
			           (sprintf "[%s]" array)
				   ""))
		      "")))
          (sprintf (if (sget ".nodec")
                       "/* %s; */"
                       "%s;") decl)))
     (define (display-all . L) ; FIXME debug macro
	(for-each display L))
     (define (print-init-attrib attr val)
        (sprintf "  opt.%s = %s;\n" attr val))
     (define (init-attrib)
	(let ((attr (sget ".attrib"))
	      (array (sget ".array")))
           (if array
              (let ((p (lambda (idx val)
                         (print-init-attrib (sprintf "%s[%d]" attr idx) val))))
                (apply string-append
                  (map p (iota (string->number array)) (stack "default"))))
	      (print-init-attrib attr (sget ".default")))))
     (define *pref-name* '())
     (define (dot-join ls)
 	(let ((a (caar ls))
              (d (cdr ls)))
  	  (if (null? d)
	      a
  	      (string-append a "." (dot-join d)))))
     (define (e-join ls)
 	(let ((a (cdar ls))
              (d (cdr ls)))
  	  (if (null? d)
	      a
  	      (string-append a (e-join d)))))
     (define (pref-name) (dot-join (reverse *pref-name*)))
     (define (pref-args) (e-join (reverse *pref-name*)))
     (define (pref-name-push)
        (set! *pref-name*
  	   (cons (if (exist? ".nameraw")
                     (cons "%s" (string-append ", " (get "name")))
                     (cons (get "name") ""))
                 *pref-name*)))
     (define (pref-name-pop) (set! *pref-name* (cdr *pref-name*)))
     (define (repeat n x)
        (if (zero? n) '()
                      (cons x (repeat (1- n) x))))
     (define (save-pref)
	(let ((attr (sget ".attrib"))
	      (array (sget ".array")))
	  (string-append "  fprintf (fs, \"" (pref-name) ":"
	       (if array
		   (let ((a (string->number array))
			 (p (lambda (idx)
			      (sprintf ",\n\t       opt.%s[%d]" attr idx))))
		      (string-append
			(apply string-append (repeat a " %d"))
			"\\n\"" (pref-args)
			(apply string-append (map p (iota a)))
			");\n"))
  	          (sprintf " %%d\\n\"%s, opt.%s);\n" (pref-args) attr)))))
     (define *get-pref-token* #f)
     (define (get-pref-token)
        (string-append "token = strtok ("
                       (if *get-pref-token*
                           "0" (begin
                                  (set! *get-pref-token* #t)
                                  "buf"))
                       ", \" \\t.:\");"))
     (define *types* '("a_u8"      "parse_unsigned" "0" "255"
                       "a_u16"     "parse_unsigned" "0" "UINT16_MAX"
                       "a_u32"     "parse_unsigned" "0" "UINT32_MAX"
		       "a_keycode" "parse_unsigned" "0" "HKEYCODE_MAX"
		       "bool"      "parse_bool"     "false" "true"))
     (define (parse-pref)
        (let* ((type (sget ".type"))
               (array (sget ".array"))
               (type-info (or (member type *types*)
			      (error (string-append "Unknow type "
                                         type " for preference "
                                         (sget "name")))))
               (func (cadr type-info))
	       (min  (or (sget "min")
			 (caddr type-info)))
	       (max  (or (sget "max")
                         (cadddr type-info)))
	       (att  (get "attrib")))
	   (if array
	       (let ((p (lambda (idx)
			  (parse-pref-array att (number->string idx)
                                            func min max))))
                  (apply string-append
                         (map p (iota (string->number array)))))
               (parse-pref-single att func min max))))

     (define (parse-pref-single att func min max)
        (string-append  "    token = strtok (0, \"\\n\");\n"
                        "    opt." att " = " func
                        " (token, " min ", " max ");\n"))
     (define (parse-pref-array att idx func min max)
        (string-append  "    token = strtok (0, \" \\t\");\n"
                        "    opt." att "[" idx "] = " func
                        " (token, " min ", " max ");\n"))

)
~]
/*
[~ (gpl "Heroes" "** ") ~]
*/
[~
  DEFINE CREATE_STRUCT ~][~
    FOR .verbatim ~]
  [~ (get ".verbatim") ~]
[~
    ENDFOR .verbatim ~][~
    FOR .pref ~][~
      (sprintf "\n%s  %s\n" (c-doc) (c-attrib)) ~][~
    ENDFOR .pref ~][~
    FOR .group ~][~
      CREATE_STRUCT ~][~
    ENDFOR .group ~][~
  ENDDEF CREATE_STRUCT ~][~

  DEFINE REINIT_PREFS ~][~
    FOR .pref ~][~
      (init-attrib) ~][~
    ENDFOR .pref ~][~
    FOR .group ~][~
      REINIT_PREFS ~][~
    ENDFOR .group ~][~
  ENDDEF REINIT_PREFS ~][~
  DEFINE SAVE_PREFS ~][~
    FOR .pref ~][~
      (pref-name-push) ~]  fprintf (fs, "[~ (sh-doc) ~]\n");
[~    (save-pref) ~][~
      (pref-name-pop) ~][~
    ENDFOR .pref ~][~
    FOR .group ~][~
      (pref-name-push) ~][~
      SAVE_PREFS ~][~
      (pref-name-pop) ~][~
    ENDFOR .group ~][~
  ENDDEF SAVE_PREFS ~][~
  DEFINE LOAD_PREFS ~]
    [~ (get-pref-token) ~][~
    FOR .group ~]
    if (!strcasecmp ([~ IF (exist? ".nameraw") ~][~ ELSE ~]"[~ ENDIF ~][~
                        (get ".name")
                   ~][~ IF (exist? ".nameraw") ~][~ ELSE ~]"[~ ENDIF
                   ~], token)) {[~
        LOAD_PREFS ~]
    } else [~
    ENDFOR .group ~][~
    IF (exist? ".pref") ~][~
      FOR .pref ~]
    if (!strcasecmp ("[~ (get ".name") ~]", token)) {
[~ (parse-pref) ~]    } else [~
      ENDFOR .pref ~][~
    ENDIF ~]{ append_ignored (line); }[~
  ENDDEF LOAD_PREFS ~]
[~ CASE (suffix) ~][~
  == h ~][~
   (define (multinc) (sprintf "HEROES__%s__H" (string-upcase! (base-name)))) ~]
#ifndef [~ (multinc) ~]
#define [~ (multinc) ~]

typedef struct {
[~ CREATE_STRUCT ~]
} a_pref_set;

extern a_pref_set opt;

void reinit_preferences (void);
void free_preferences (void);
void output_preferences (FILE* fs);
bool save_preferences (void);
bool load_preferences (void);

#endif /* [~ (multinc) ~] */
[~ == c ~]
#include "system.h"
#include "prefs.h"
#include "debugmsg.h"
#include "rsc_files.h"
#include "getshline.h"
#include "keyvalues.h"
#include "errors.h"
#include "misc.h"

a_pref_set opt;
static char* ignored_lines = 0;
static int firstline;
static char* name = 0;

static char*
preferences_file (void)
{
  if (!name)
    name = get_non_null_rsc_file ("pref-file");
  return name;
}

void
reinit_preferences (void)
{
[~ REINIT_PREFS ~]}

void
free_preferences (void)
{
  dmsg (D_MISC, "free preferences");

  XFREE0 (name);
  XFREE0 (ignored_lines);
}

void
output_preferences (FILE* fs)
{
[~ SAVE_PREFS ~]
  if (ignored_lines)
    fprintf (fs, "# Currently ignored preferences.\n%s", ignored_lines);
}

bool
save_preferences (void)
{
  FILE* fs;

  dmsg (D_MISC, "save preferences to %s", preferences_file ());
  fs = fopen (preferences_file (), "wt");
  if (fs == 0) {
     wmsg ("cannot save preferences to %s", preferences_file ());
     dperror ("fopen");
     return true;
  }
  output_preferences (fs);
  fclose (fs);
  return false;
}

static unsigned long int
parse_unsigned (char *token, unsigned long int min, unsigned long int max)
{
  unsigned long int val = strtoul (token, 0, 10);
  if (val < min || val > max) {
    wmsg ("%s:%d: value %s is out of range (%lu-%lu)",
          preferences_file (), firstline, token, min, max);
    val = (val < min) ? min : max;
  }
  return val;
}

static bool
parse_bool (char *token, bool min, bool max)
{
  while (*token && ISSPACE(*token))
    ++token;

  if (ISDIGIT (*token))
    return parse_unsigned (token, min, max);

  /* FIXME: handle min and max.  */
  if (!strcasecmp (token, "true")) {
    return true;
  } else if (!strcasecmp (token, "false")) {
    return false;
  } else {
    emsg ("%s:%d: %s should be either false or true",
          preferences_file (), firstline, token);
    return false; /* Never reached.  */
  }
}

static void
append_ignored (const char* ign)
{
  if (ignored_lines)
    ignored_lines = strappend (ignored_lines, ign);
  else
    ignored_lines = strdup (ign);
}

bool
load_preferences (void)
{
  FILE* fs;
  int endline = 0;
  char* buf = 0;
  size_t bufsize = 0;

  firstline = 0;

  reinit_preferences ();

  dmsg (D_FILE, "reading option from %s", preferences_file ());
  fs = fopen (preferences_file (), "rb");

  if (fs == NULL) {
    /* Don't warn, the file doesn't exists the first time.  */
    dmsg (D_FILE, "cannot open preferences file %s", preferences_file ());
    dperror ("fopen");
    return true;
  }

  XFREE0 (ignored_lines);

  while (getshline_numbered
	 (&firstline, &endline, &buf, &bufsize, fs) != -1) {
    char* token;
    char* line = xstrdup (buf);
[~ LOAD_PREFS ~]
    free (line);
  }

  free (buf);
  fclose (fs);
  return false;
}
[~ ESAC ~]
