/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_ALOC.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  Feb 08, 1988

:INPUT.                 Ptr:  Ptr to where to store ptr to alloc mem
                        maxlen:  Requested # of bytes to alloc
                        minlen:  Minimum bytes needed

:OUTPUT.                rc:   0 if all OK, else error
                        ptr:  Pointer allocated
                        maxlen:  Actual length allocated in bytes

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 OSDFREE() assembly routine via INTER()

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

sint os_aloc( ptr, maxlen, minlen)
    sint      *ptr;
    sint           *maxlen,   minlen;


{
    sint   maxdw, mindw;
    sint   rc = 0;


    if ( *maxlen > 0)
    {
       maxdw = (*maxlen + 7) / 8;
       mindw = (minlen + 7) / 8;
       if ((rc = Inter(OSDFREE, ptr, &maxdw, mindw)) != 0)
       {
          rc = OSK_ERR + SUB_ERR + rc;
       }
       else
       {
          *maxlen = maxdw * 8;
       }
    }
    else
    {
       rc = OSK_ERR + SUB_ERR + ERR_INVALID;
    }

   return(rc);
}
