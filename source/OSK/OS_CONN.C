#include "defs.h"
#include "str.h"
#include "debug.h"
#include "pcomm.h"
#include "osk_i.h"
#include "osk_x.h"

sint os_conn(func, devi, bufptr, buflenptr, flags)
   sint      func, devi,                    flags;
   hword                        *buflenptr;
   byte                 *bufptr;
{
   sint rc = 0, rrc = 0;
   byte *pathptr = CONPATH,
        *tmpbuf = NULL,
        *lasbuf = NULL,
        *orgtmpbuf = NULL;
   sint pathlen = CONPATHLEN,
        tmplen = 0;


   if( (condata != NULL) && (condata->pktlen != 0) )
   {
   }

   orgtmpbuf = tmpbuf = os_mema( *buflenptr + 30 );

   switch( func )
   {
      case CON_ON:
         condata = (struct construc *)
                          os_mema(sizeof(struct construc)+PC_MAX_PKTLEN);
         condata->pktlen = 0;
         break;
      case CON_OFF:
         os_memd( condata );
         condata = NULL;
         break;
      case CON_OPEN:
         rc = Inter(CONOPEN, devi, pathptr, pathlen, bufptr, buflenptr);
         break;
      case CON_CLOSE:

         *tmpbuf++ = 0xC3;        /* WCC */
         *tmpbuf++ = 0x11;        /* SBA */
         *tmpbuf++ = 0x5D;
         *tmpbuf++ = 0x7F;
         *tmpbuf++ = 0x11;        /* SBA */
         *tmpbuf++ = 0x40;
         *tmpbuf++ = 0x40;
         *tmpbuf++ = 0x13;

         tmplen = tmpbuf - orgtmpbuf;
         rc = Inter(CONWRIT, pathptr, pathlen, orgtmpbuf, tmplen);

         rc = Inter(CONCLO, pathptr, pathlen);
         break;
      case CON_READ:
         *tmpbuf++ = 0xC3;        /* Damn WCC that I forgot  */
#if 0
         *tmpbuf++ = 0x28;
         *tmpbuf++ = 0x00;
         *tmpbuf++ = 0x00;
#endif
         *tmpbuf++ = 0x11;
         *tmpbuf++ = 0x5d;
         *tmpbuf++ = 0x7f;
         *tmpbuf++ = 0x11;
         *tmpbuf++ = 0x00;
         *tmpbuf++ = 0x05;    /* used to be 09 when 280000 was in */
#if 0
         *tmpbuf++ = 0x13;       /* put cursor at begining */
#endif
         tmplen = tmpbuf - orgtmpbuf;

         rc = Inter(CONWRIT, pathptr, pathlen, orgtmpbuf, tmplen);
         if( rc != 0 )
            break;

         rc = Inter(CONREAD, pathptr, pathlen, bufptr, buflenptr);

         if( *bufptr != AID_CR )   /* Something is WRONG.  Abort!!     */
   return(PC_ERR_ABORT);
         *buflenptr -= 3;          /* Strip AID and cursor postion info*/
         os_memc((bufptr+3), bufptr, *buflenptr);
         break;
      case CON_WRITE:
         *tmpbuf++ = 0xC3;        /* WCC */
         *tmpbuf++ = 0x11;        /* SBA */
         *tmpbuf++ = 0x5D;
         *tmpbuf++ = 0x7F;
         *tmpbuf++ = 0x11;        /* SBA */
         *tmpbuf++ = 0x00;        /* 12 bit addressing */
         *tmpbuf++ = 0x05;        /* start column 5    */

         os_memc( bufptr, tmpbuf, *buflenptr);   /* rest of the data */
         tmpbuf += *buflenptr;
         *buflenptr = tmpbuf - orgtmpbuf;

         rc = Inter(CONWRIT, pathptr, pathlen, orgtmpbuf, *buflenptr);
         break;
      case CON_CLEAR:
         rc = Inter(CONCLEA, pathptr, pathlen);
         break;
      case CON_QUERY:
         rc = Inter(CONQRY, pathptr, pathlen, bufptr, buflenptr);
         break;
      case CON_WAIT:
         rc = Inter(CONWAIT, pathptr, pathlen);
         break;
      default:
         rc = -OSK_ERR_FUNC_NOT_DEF;/* general error; invalid function */
         break;
   }  /* switch(func) */

   os_memd( orgtmpbuf);          /* de-allocated temp memory */

   if ( rc != 0 )
   {
      *buflenptr = 80;
      flags = -1;
      rrc = Inter(CONQRY, pathptr, pathlen, bufptr, buflenptr);
      rc = rc * -1;  /* Turn RC positive */
      if ( rc > 99 )
         rc = rc - 10;  /* bring error code in range 0..99  */
      rc = OSK_ERR + ((func - 1) * 100) + rc;  /* figure out err code */
   }
   if( bufptr != NULL )
   {
   }

   return(rc);
}
