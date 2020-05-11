/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_eton()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      byte**   IN/OUT   converstion target C-str |*/
/*|  :PARAM.   source      string*  INPUT    conversion source        |*/
/*|  :PARAM.   beg         sint     INPUT    where to start from      |*/
/*|  :PARAM.   len         sint     INPUT    how much to do           |*/
/*|                                                                   |*/
/*|  :PURPOSE. converts a section of a teies string to a C-string     |*/
/*|            of the desired size                                    |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_alloc()                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_LEN, S_BAD_PARM                                  |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    19_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    put in calls to #s_check()     |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

#s_c_xtrn();                     /* Externs for #s_check()/#s_pcheck() */

extern sint s_alloc();

sint s_eton (target, source, beg, len)
byte       **target ;             /* Conversion target -- C-string     */
string      *source ;             /* Conversion source -- TEIES string */
sint         beg    ;             /* Beginning of indicated section    */
sint         len    ;             /* Length    of indicated section    */
{
   tint      lens   ;             /* Used length of TEIES string       */
   tint      lena   ;             /* Alloc len   of TEIES string       */
   tint      count  ;             /* Counter used for copying          */
   tint      nulat  ;             /* Where in  source do we put the \0 */
   sint      rc     ;             /* RC from stuff for debugging       */

   #D_start (s_eton, str, 'target:0x%X, source:0x%X, B=%d, L=%d', \
      'target, source,beg,len')
   #s_pcheck(source);             /* Check for corrupt source          */
   #os_c_ptr(target);             /* Check for bad-looking pointer     */
   #D_str(*source,'T-str');
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| WARNING TO PROGRAMMER:  In this routine, BEG displaces with 0     |*/
/*| meaning the 1st byte, 1 meaning the 2nd, etc, as it is used to    |*/
/*| locate the C-str pointer.                                         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   if (len < 0)                   /* Check for negative length         */
   {
      #D_return (S_BAD_LEN);
   }
   lens = s_len (*source) ;       /* Get used length of source         */
   lena = s_allen (*source) ;     /* Get aloc length of source         */
   nulat= lens ;                  /* Null will go after last char used */

   if ( nulat > lena || lena == 0)/* Is there room for the \0 in it?   */
   {
      rc = s_alloc(source,nulat+1);    /* Nope, gotta make it bigger   */
      #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_alloc()');
   }

   s_char_s(*source,nulat) = '\0';     /* Stick in the terminator      */

   if ( beg < 0 )                      /* Hafta recalculate beginning  */
      beg = lens + beg ;               /* Actually adding a negative   */
   else if ( beg != 0 )
      beg--;                           /* Compenstate for 0 being 1st  */

#D_show('tar=0x%X;*tar=0x%X;s_buf(*sou)=0x%X;beg=%d;s_buf(*sou)+beg=0x%X',\
        'target, *target,    s_buffer(*source),\
                                             beg,s_buffer(*source)+beg');

   *target = s_buffer(*source) + beg ; /* Point to the NULL            */
   #D_show('Resulting C-str (%s) (at 0x%X) / len=%d, lens=%d, beg=%d',\
      '*target,*target,len,lens,beg');

   if ( len < lens - beg && len != 0 )  /* Don't what to end?          */
   {
      #D_leave(S_BAD_LEN,'You must take the whole remaining portion');
   }

   #D_return (0)
}
