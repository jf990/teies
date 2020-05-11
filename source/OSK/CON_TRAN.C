#include "defs.h"
#include "xmodem.h"

sint con_tran( func, buf, len)
   sint        func;
   byte             *buf;
   sint                   len;
{

   sint rc = 0;

   switch( func )
   {
      case CON_ASC2EBC:    rc = Inter(CON2EBC, buf, len);
                           break;
      case CON_EBC2ASC:    rc = Inter(CON2ASC, buf, len);
                           break;
      default:             rc = CON_ERR_FUNC_NOT_DEF;
                           break;
   }

   return( rc );
}
