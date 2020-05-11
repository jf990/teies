/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stob()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      byte*    INPUT    target byte              |*/
/*|  :PARAM.   source      source   INPUT    source string            |*/
/*|  :PARAM.   pos         pos      INPUT    pointer to char in source|*/
/*|                                                                   |*/
/*|  :PURPOSE.  Copies the indicated byte of a given string to a      |*/
/*|             byte pointed to by the given byte pointer.            |*/
/*|                                                                   |*/
/*|  :OUTPUT.   Returns zero or a standard STR error code.            |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  This dummy version is provided to facilitate headerfile|*/
/*|            and txtlib generation.                                 |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|  :MODS.    20_Oct_87   K.J.Walsh   Implemented.                   |*/
/*|  :MODS.    20_Oct_87   K.J.Walsh   "Mothballed"--tells user that  |*/
/*|                                    it should not have been called |*/
/*|                                    (replaced by PPC macro in      |*/
/*|                                    "str_x h")                     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

#s_c_xtrn();

#undef s_stob
sint s_stob (target, source, pos)
byte   *target ;            /* Target - where to copy to               */
string  source ;            /* Source - where to copy from             */
sint    pos    ;            /* Position of character to copy           */
{
   tint len    ;            /* Used length of source string            */

   #D_start(s_stob,str,'tar_str=0x%X, sou_str=0x%X, pos=%d',\
   'target,source,pos')
/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING FOLLOWS                                               |*/
/*+-------------------------------------------------------------------+*/

   D_pr("####:WARNING:  OLD VERSION OF s_stob() IN USE.\n");
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
