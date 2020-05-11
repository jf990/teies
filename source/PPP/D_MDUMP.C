/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_mdump()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   prefix      byte*    INPUT    C-string prefix for each;|*/
/*|  :PARAM.   from        byte*    INPUT    Pointer to start of dump;|*/
/*|  :PARAM.   length      tint     INPUT    Desired dump size in byte|*/
/*|                                                                   |*/
/*|  :PURPOSE.                                                        |*/
/*+-------------------------------------------------------------------+*/
/*|  This guy prints the given section of memory in a semisensible    |*/
/*|  manner.  Each line is prefixed by the given string.  Based       |*/
/*|  on Ken Ng's mdump().                                             |*/
/*|                                                                   |*/
/*|  Note that the table os_char(::) indicates if a given byte can be |*/
/*|  printed.     Unprintable and fancy (typically unprintable)       |*/
/*|  characters are translated into a dot (.) as is usually done with |*/
/*|  BAL dumps.                                                       |*/
/*|                                                                   |*/
/*|  The output of this routine is superior to that of IBM's CP       |*/
/*|  DISPLAY command in that (1) it will display uppercase; (2) it    |*/
/*|  shows both address and displacement; (3) it begins at the        |*/
/*|  requested address, not at the preeceding full-word alignment;    |*/
/*|  (4) it interfaces nicely with the rest of the debugging support  |*/
/*|  because it uses the D_pr() output function.                      |*/
/*|                                                                   |*/
/*|  It is inferior in that it depends on 'C' and will attempt do     |*/
/*|  display memory that it may not view, which, of course, would     |*/
/*|  result in memory fault error.  (Most likely "paging error".)     |*/
/*|  Also, it does not notice identical lines like DISPLAY does.      |*/
/*|  It will cheerfully print N thousand identical lines.             |*/
/*|                                                                   |*/
/*|  Future considerations:  (1) get around the 2 inferiority complexs|*/
/*|  mentioned in the preeceding paragraph; (2) advantage #3 above    |*/
/*|  might be considered a disadvantage by some because it makes      |*/
/*|  scanning the output by eye difficult.  Perhaps this could be     |*/
/*|  gotten around by rounding but not displaying the rounded portion.|*/
/*|  For example, a dump from X'37AAB2' of length X'25' is requested; |*/
/*|  currently the output would be:                                   |*/
/*|                                                                   |*/
/*| :37AAB2/00: 00000000 00000000 00000000 00000000 *................*|*/
/*| :37AAC2/10: C1C2C3C4 00000000 C1C2C3C4 00000000 *ABCD....ABCD....*|*/
/*| :37AAD2/20: 00000000 00000000 00000000 00000000 *................*|*/
/*|                                                                   |*/
/*|  but it might be more desirable to instead do:                    |*/
/*|                                                                   |*/
/*| :37AAB2/00:     0000 00000000 00000000 00000000 ***..............*|*/
/*| :37AAC2/0E: C1C2C3C4 00000000 C1C2C3C4 00000000 *ABCD....ABCD....*|*/
/*| :37AAD0/1E: 00000000 00000000                   *........*********|*/
/*|                                                                   |*/
/*|  note that the displacement column gives displacements from the   |*/
/*|  requested dump start, and that it is hex, not decimal.  Either   |*/
/*|  might be undesirable.                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero                                           |*/
/*|                                                                   |*/
/*|  :CALLS.   D_pr()                                                 |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Does not check to see if it may look at requested      |*/
/*|            memory                                                 |*/
/*|                                                                   |*/
/*|  :MODS.    05_May_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    22_Jun_87   K.J.Walsh   Moved from debugi.h to the     |*/
/*|                                    str txtlib                     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"
#include <stdlib.h>
#include "str.h"
#include "osk.h"

#define WIDTH  16                      /* How many bytes across        */

sint D_mdump(prefix,from,length)

byte  *prefix  ;        /* The string to be printed -- null terminated */
byte  *from    ;        /* The 1st byte they want                      */
tint  length   ;        /* How much from 1st that they want            */

{

   byte *printable = 0 ;/* Here we create printable version of data    */
   byte *old_printable = 0 ;/* A copy of the above                     */
   byte *hexverson = 0 ;/* Here we create hex translation version of it*/
   byte *oldverson = 0 ;/* Here we create hex translation version of it*/
   byte *holder    = 0 ;/* Holds value of a string while we swap 'em   */
   byte *format    = 0 ;/* Format adjusts according to length of dump  */
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
     D_pr("****:D_mdump(A): WARNING: len > 0x1000000?  Check datatype.\n"
         );
      D_pr("****:D_mdump(B): got (%d) / 0x%X; truncated to 0x%X\n",\
         length,length,0x1000000);
      length = 0x1000000;
   }

   printable = D_ALLOCATE(40);
   old_printable = D_ALLOCATE(40);
   hexverson = D_ALLOCATE(40);
   oldverson = D_ALLOCATE(40);

   start = from;
   until =  from + (sint)length ;

   hexverson(:0:) = '\0';   /* init */
   oldverson(:0:) = '\0';   /* init */

   if ( from == NIL )
   {
      D_pr("*   :D_mdump(C): Pointer is NIL; not done.\n");
   }
   else if ( from == (byte*) -1 )
   {
      D_pr("****:D_mdump(D): Pointer is -1; not done.\n");
   }
   else if ( length == 0 )
   {
      D_pr("*   :D_mdump(E): Length is zero; not done.\n");
   }
   else
   {

      for ( ; from < until ; from += WIDTH )       /* make lines       */
      {
         cp = 0;                         /* point to the 1st character */
         hexverson(:0:) = '\0';
         for ( c1 = from ; c1 < from + WIDTH ; c1 += 4 ) /* make words */
         {
            for ( c2 = c1 ; c2 < c1 + 4 ; c2++ )   /* make bytes       */
            {
               sprintf(hexverson,"%s%02X",hexverson,*c2);
               printable(:cp++:) = OS_PRINT_AS(*c2);
            }
            sprintf(hexverson,"%s ",hexverson);
         }
         printable(:cp:) = '\0';
   #if 0
         if ( strcmp(hexverson,oldverson ) && !(from < until+WIDTH) )
         {
            dups++ ;
      D_pr("{%s}\n{%s}\n",hexverson,oldverson);
         }
         else
         {
            if ( dups > 0 || from < until+WIDTH)
            {
               dups = 0;
               D_pr(format,
                  prefix,old_from,old_from-start,oldverson,old_printable);
               D_pr("%s%06X/%X to %06X/%X suppressed lines(s) same as above\n",
                  prefix,old_from-WIDTH,old_from-WIDTH-start,from-WIDTH\
                  ,from-WIDTH-start);
            }
      #endif
            D_pr(format,
               prefix,from,from-start,hexverson,printable);
      #if 0
            old_from = from;

            holder = old_printable;
            old_printable = printable;
            printable = holder;

            holder = oldverson;
            oldverson = hexverson;
            hexverson = holder;
         }
      #endif
      }
   }
   D_DEALLOCATE(printable);
   D_DEALLOCATE(old_printable);
   D_DEALLOCATE(hexverson);
   D_DEALLOCATE(oldverson);
   return(0);
}
