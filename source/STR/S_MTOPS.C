/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_mtops.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   results                  |*/
/*|  :PARAM.   source      byte*    INPUT    memory to be translated  |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|  :PARAM.   sub_char    byte     INPUT    character to use when    |*/
/*|                                          unprintable found        |*/
/*|                                                                   |*/
/*|  :PURPOSE. To translate the memory pointed to to a printable      |*/
/*|            representation.                                        |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint string rcode or zero                      |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  If the memory pointer and len indicate unallocated     |*/
/*|            memory, this guy bombs out the system!                 |*/
/*|                                                                   |*/
/*|  :MODS.    01_Dec_87   K.Walsh     Written                        |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "osk_cset.h"
#include "debug.h"


extern   sint  s_nulout () ;
extern   sint  s_alloc  () ;

sint s_mtops (target, source, len,sub_char)
string *target    ;               /* Target for results                */
byte   *source    ;               /* String that 'end' is appended to  */
sint    len       ;               /* Length of 'end' to append         */
byte    sub_char  ;               /* Use this when source is unprintble*/
{

   sint  counter  =  0  ;              /* A counter                    */




   if ( len > S_MAXLEN )
   {
      s_nulout(target);
   return(S_BAD_PARM);
   }
   else if ( len == 0 )
   {
      if ( *target == S_NULL )
      {
   return(S_BAD_PARM);
      }
      len = s_smp_al(*target);
   }
   else if ( len < 0 )
   {
      s_nulout(target);
   return(S_BAD_LEN);
   }


   s_alloc(target,len);
   s_smp_ul(*target) = len;

   if ( sub_char == '\0' )
   {
      sub_char = '.';
   }

   len--;
   for ( counter = 0 ; len >= counter ; counter++ )
   {
      if ( OS_CANPRINT( *source ) )
      {
         s_char_s(*target,counter) = *source;
      }
      else
      {
         s_char_s(*target,counter) = sub_char;
      }
      *(source)++;
   }

   return(0);
}
