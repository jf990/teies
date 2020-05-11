/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_MEMP.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  Jun 14, 1988

:INPUT.                 Ptr1, Ptr2, Length

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               Compare memory byte-by-byte
                   compare Srcptr and Trgptr for Len bytes
                   See CLCL instruction for more details in
                   any 370 assembly manual
         returns   0   If Data is same
                   1   First ptr data is smaller
                   2   Second ptr data is smaller
                   3   Either length or ptr's are BAD

:CALLS.                 OSMEMCMP() assembly routine via INTER()

:ERRORS.

:LIMITS.


:MODS.         02/16/89    Sal      Rename routine name from os_mcmp
                                    due to prototyping problems

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk_i.h"

sint os_memp( ptr1, ptr2, length)
    byte      *ptr1,
                   *ptr2;
    sint                  length;

{
    sint rc = 0;


#if 0
   check if ptr's are valid and length is greater than 0
#endif


    rc = Inter(OSMEMCMP, ptr1, ptr2, length);

   return(rc);
}
