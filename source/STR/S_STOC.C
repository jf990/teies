/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stoc()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      byte*    INPUT    target byte              |*/
/*|  :PARAM.   source      source   INPUT    source string            |*/
/*|  :PARAM.   pos         pos      INPUT    pointer to char in source|*/
/*|                                                                   |*/
/*|  :PURPOSE. extracts the indicated character from the source and   |*/
/*|            sets the byte to this value                            |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    18_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    16_Jun_87   K.J.Walsh   "Mothballed"--tells user that  |*/
/*|                                    it should not have been called |*/
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

#s_c_xtrn();

#undef s_stoc
sint s_stoc (target, source, pos)
byte   *target ;            /* Target - where to copy to               */
string  source ;            /* Source - where to copy from             */
sint    pos    ;            /* Position of character to copy           */
{
   tint len    ;            /* Used length of source string            */

   #D_start(s_stoc,str,'tar_str=0x%X, sou_str=0x%X, pos=%d','target,source,pos')
/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING FOLLOWS                                               |*/
/*+-------------------------------------------------------------------+*/

   D_pr("####:WARNING:  OLD VERSION OF s_stoc() IN USE.\n");
    if ( pos == 0 )
         pos = 1;
    *target = (sint) s_char_s(source,pos);
   #D_leave(0,'OLD VERSION CALLED');

/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING END                                                   |*/
/*+-------------------------------------------------------------------+*/
   #s_check(source);        /* Check whether string is corrupt         */
   if (pos == 0)            /* If position indicates whole string take */
      pos = 1;              /*    first character                      */
   len = s_len (source) ;   /* Compute used length of string           */
   if (pos > len )          /* If position is past end                 */
   {
      #D_return (S_BAD_PARM )
   }
   if (pos <  0)            /* If position is negative compute posi-   */
      pos = len + pos + 1 ; /*    tive position                        */
   if (pos <  0)            /* If position still negative then it is   */
   {                        /*    an invalid parameter                 */
      #D_return (S_BAD_PARM )
   }
   if (s_simp(source))
   {
      if (len==0 || pos>len)/* If no character in desired position     */
      {
         *target = '\0' ;   /*    then assign a null to the target     */
         #D_return (0)
      }
      else                  /* Else return the character               */
      {
         *target = s_char_s (source,pos-1) ;
         #D_return (0)
      }
   }
   else                     /* Else string is C O M P L E X            */
   {
      #cmplx () ;
   }
}
