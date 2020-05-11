/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_mtoxs.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   results                  |*/
/*|  :PARAM.   source      byte*    INPUT    memory to be mapped      |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|                                                                   |*/
/*|  :PURPOSE. To translate the memory pointed to to a hexdump        |*/
/*|            (under the assumption it is packed decimal)            |*/
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
#include "debug.h"


extern   sint  s_nulout () ;
extern   sint  s_alloc  () ;

sint s_pdtoxs (target, source, len)
string *target ;                  /* Target for results                */
byte   *source ;                  /* String that 'end' is appended to  */
sint    len    ;                  /* Length of 'end' to append         */
{
   static byte hex_char[] = {'0','1','2','3','4','5','6','7','8','9',\
                               'A','B','C','D','E','F' };

   sint  real_len =  0  ;              /* Length of outgoing string.   */
   sint  counter  =  0  ;              /* A counter                    */
   sint  part     =  0  ;              /* Part of hex constant         */
   sint  value    =  0  ;              /* Displacement into hex_char   */




   if ( len == 0 )
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

   real_len = len * 2;
   if ( real_len > S_MAXLEN )
   {
      s_nulout(target);
   return(S_BAD_PARM);
   }


   s_alloc(target,real_len);
   s_smp_ul(*target) = real_len;

   len--;
   counter = 0;
   while ( counter < real_len )
   {
      value = ((sint) *source-(sint) *source % 0x10 ) / 0x10;
      if ( value > 9 )
      {
         s_char_s(*target,counter++) = '_';
         s_char_s(*target,counter++) = '_';
      }
      else
      {
         s_char_s(*target,counter++) = 'F';
         s_char_s(*target,counter++) = hex_char[ value ];
      }

      value = (sint) *source % 0x10;
      if ( value > 9 )
      {
         s_char_s(*target,counter++) = '_';
         s_char_s(*target,counter++) = '_';
      }
      else
      {
         s_char_s(*target,counter++) = 'F';
         s_char_s(*target,counter++) = hex_char[ value ];
      }
      source++;
   }
   return(0);
}

