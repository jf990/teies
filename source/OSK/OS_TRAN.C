/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_TRAN.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  June 15, 1988

:INPUT.                 func:   Type of translation
                        buf :   Ptr to buffer to be traslated
                        len :   Length of the buffer

:OUTPUT.                rc  :   0 if all OK, else error
                        buf :   Ptr to buffer translated.

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               To translate data from one character set
                        to another.

:CALLS.                 OS2EBC and OS2ASC via Inter()

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "osk_x.h"
#include "osk_i.h"

sint os_tran( func, buf, len)
   byte        func;
   byte             *buf;
   sint                   len;
{

   sint rc = 0,
        count = 0;

   for( count = len; count > 0; count -= len)
   {
      if( count > 256 )
         len = 256;
      else
         len = count;

         switch( func )
         {
            case ASC2EBC:    rc = Inter(OS2EBC, buf, len);
                             break;
            case EBC2ASC:    rc = Inter(OS2ASC, buf, len);
                             break;
            default:         rc = OSK_ERR_FUNC_NOT_DEF;
                             break;
         }
   }  /* for */

   return( rc );
}
