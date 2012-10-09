; Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
;
; This file is part of Heroes.
;
; Heroes is free software; you can redistribute it and/or modify it under
; the terms of the GNU General Public License as published by the Free
; Software Foundation; either version 2 of the License, or (at your
; option) any later version.
;
; Heroes is distributed in the hope that it will be useful, but WITHOUT
; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
; for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

(define (source-texi)
  (or (getenv "SOURCE_TEXI")
      (error "You should set the environment variable SOURCE_TEXI.")))

(define (import-head marker)
  (string-append
   (shellf "sed -n '1,/@c START == %s == /p' %s" marker (source-texi))
   (sprintf "\n@c The following text, up to `END == %s ==',\n" marker)
   (sprintf "@c is AutoGen'ed from %s and %s.\n" (def-file) (tpl-file))))

(define (import-tail marker)
  (shellf "sed -n '/@c END == %s == /,$p' %s" marker (source-texi)))
