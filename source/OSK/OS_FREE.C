/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_FREE.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  Feb 08, 1988

:INPUT.                 Ptr:  Ptr to release
                        Len:  Requested # of bytes to release

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 OSDFRET() assembly routine via INTER()

:ERRORS.                (rc mod 10): see Sys Prog Guide(SC19-6203) Pg417

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk_x.h"
#include "osk_i.h"

#define SUB_ERR 990
#define ERR_INVALID 0

sint os_free( ptr, len)
    sint      *ptr;
    sint           len;


{
    sint   tmpdw;
    sint   rc = 0;


    if ((len > 0) && (ptr != NIL))
    {
       tmpdw = (len + 7) / 8;
       if ((rc = Inter(OSDFRET, ptr, tmpdw)) != 0)
       {
          rc = OSK_ERR + SUB_ERR + rc;
       }
    }
    else
    {
       rc = OSK_ERR + SUB_ERR + ERR_INVALID;  /* invalid args  */
    }

   return(rc);
}
