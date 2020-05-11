
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"

#define WIDTH  16                      /* How many bytes across        */

extern printf();

sint dump(prefix,from,length)

char  *prefix  ;        /* The string to be printed -- null terminated */
byte  *from    ;        /* The 1st byte they want                      */
tint  length   ;        /* How much from 1st that they want            */

{

   char *printable = 0 ;/* Here we create printable version of data    */
   char *old_printable = 0 ;/* A copy of the above                     */
   char *hexverson = 0 ;/* Here we create hex translation version of it*/
   char *oldverson = 0 ;/* Here we create hex translation version of it*/
   char *holder    = 0 ;/* Holds value of a string while we swap 'em   */
   char *format    = 0 ;/* Format adjusts according to length of dump  */
   byte *start     = 0 ;/* Remembers actual "from" location            */
   byte *until     = 0 ;/* Address at which we stop                    */
   byte cp         = 0 ;/* counter                                     */
   byte *c1        = 0 ;/* counter                                     */
   byte *c2        = 0 ;/* counter                                     */
   tint dups       = 0 ;/* duplicate lines found so far                */
   byte *old_from  = 0 ;/* holds "from" value                          */

/*+-------------------------------------------------------------------+*/
/*|Now we figgure out what format to use, based on the length of the  |*/
/*|dump desired, so that staggering does not occur in the columns when|*/
/*|the displacemnt enntry increases by a multiple of 10.              |*/
/*+-------------------------------------------------------------------+*/
   if      ( length < 0x100     )  format = "%s%06X/%02X: %s*%s*\n";
   else if ( length < 0x1000    )  format = "%s%06X/%03X: %s*%s*\n";
   else if ( length < 0x10000   )  format = "%s%06X/%04X: %s*%s*\n";
   else if ( length < 0x100000  )  format = "%s%06X/%05X: %s*%s*\n";
   else if ( length < 0x1000000 )  format = "%s%06X/%06X: %s*%s*\n";
   else
   {
      format = "%s%06X?%X: %s*%s*\n";
      printf("WARNING: dump():Length is too fucking big\n");
   }

   printable = os_mema(40);
   old_printable = os_mema(40);
   hexverson = os_mema(40);
   oldverson = os_mema(40);

   start = from;
   until =  from + (sint)length ;

   hexverson(:0:) = '\0';   /* init */
   oldverson(:0:) = '\0';   /* init */

   for ( ; from < until ; from += WIDTH )       /* make lines          */
   {
      cp = 0;                            /* point to the 1st character */
      hexverson(:0:) = '\0';
      for ( c1 = from ; c1 < from + WIDTH ; c1 += 4 ) /* make words    */
      {
         for ( c2 = c1 ; c2 < c1 + 4 ; c2++, cp++ )   /* make bytes   */
         {
            sprintf(hexverson,"%s%02X",hexverson,*c2);
            if( *c2 < 32 || *c2 > 126)
               printable(:cp:) = '.';
            else
               printable(:cp:) = *c2;
         }
         sprintf(hexverson,"%s ",hexverson);
      }
      printable(:cp:) = '\0';
      printf(format,
            prefix,from,from-start,hexverson,printable);
   }
   os_memd(printable);
   os_memd(old_printable);
   os_memd(hexverson);
   os_memd(oldverson);
   return(0);
}
