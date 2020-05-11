/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_MCPY.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  Oct 23, 1987

:INPUT.                 Srcptr, Trgptr, Length

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 OSMEMCPY() assembly routine via INTER()

:ERRORS.

:LIMITS.


:MODS.         02/16/89    Sal      Rename routine name from os_memc
                                    due to prototyping problems

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "osk_i.h"

sint os_mcpy( srcptr, trgptr, length)
    byte      *srcptr,
                     *trgptr;
    sint                      length;


{
    sint   i            ,
           rc   = NO_ERR;


   if ( length > 0 )
   {

      if( Inter(OSMEMCPY, srcptr, trgptr, length) != 0 )
      {

         for ( i = 0; i < length; i++ )
         {

            *trgptr = *srcptr;
             trgptr++;
             srcptr++;

         }

      }

   }
   else
   {

      rc = INVALID_LENGTH;

   }


   return(0);
}
