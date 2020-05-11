#include "xmodem.h"
/* #include "con.h"
*/
getbyte(ch, delay)
   byte *ch;
   int  delay;
{
   extern FILE *errfile;
   int rc = -1,
       i = 0;
   byte *datptr;

   for ( ; i < delay; i++ )
   {
      rdinbuf();
err("byfptr:%d    rdptr:%d",inpdat->bufptr,inpdat->rdptr);


      if ( inpdat->bufptr != inpdat->rdptr )
      {
/*       *ch = *(inpdat->(data + rdptr));    */
         datptr = inpdat->data;
         *ch = *(datptr + inpdat->rdptr);
         inpdat->rdptr += 1;
         rc = 0;
         break;
      }
      sleep(1);
   }

   return( rc );
}
int rdinbuf()
{
   extern struct rdbufstr *inpdat;
   int datlen = RAWSIZE,
       rc = 0;
   byte *datptr,
        *rawptr,
        *str = CLEAR_CHAR;
   extern FILE *errfile;

   datptr = inpdat->data + inpdat->bufptr;
   rawptr = inpdat->rawdata;
   rc = Inter(CONREAD, inpdat->rawdata, &datlen);
   if ( datlen != 0)
   {
#if 1
      rc = Inter(CONCLEA);  /* clear screen */
#endif
err("datlen:%d",datlen);
dump( rawptr, datlen, errfile);

/* strncpy(inpdat->(data + bufptr), inpdat->(rawdata + 3), datlen); */
      strncpy( datptr, rawptr + 3, datlen - 3);
      inpdat->bufptr += (datlen - 3);

      rc = datlen - 3 ;
   }
   return( rc );

}
