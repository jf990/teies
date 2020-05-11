/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_MFREE.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.

:INPUT.

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 -none-

:ERRORS.

:LIMITS.

:MODS.         02/16/89    Sal      Rename routine name from os_memd
                                    due to prototyping problems


* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "osk_i.h"

sint  os_mfree   (   point   )

byte                *point;        /* Pointer to memory to deallocate */

{

   byte buffer [ 200 ]; /* buffer for tracing functions */

   if( point != (byte *) NIL && point != (byte *) MINUS_ONE_PTR    ){
       free (point) ;       }


   return(0);

}
