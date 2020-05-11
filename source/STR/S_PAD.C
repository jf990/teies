/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_pad()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   out         string*  OUTPUT   string to be created     |*/
/*|  :PARAM.   in          string   INPUT    incoming string          |*/
/*|  :PARAM.   size        tint     INPUT    final size for output    |*/
/*|  :PARAM.   padder      byte     INPUT    padding character        |*/
/*|  :PARAM.   side r      byte     INPUT    L=left, R=right, C=center|*/
/*|                                                                   |*/
/*|  :PURPOSE. Create output string by padding input string on the    |*/
/*|            left, right, or boths sides with the given character.  |*/
/*|            Pads (or truncates) according to the given size.       |*/
/*|            Does no checking for corrupt or complex strings.       |*/
/*|            Does not check the return codes of called functions.   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR RC to indicate status     |*/
/*|                                                                   |*/
/*|  :CALLS.   s_alloc(), s_cat, s_copy(), s_drop()                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.  If difference between the source length and the desired|*/
/*|            length is odd during the CENTER operation, a character |*/
/*|            is padded or truncated to/from the start of the string.|*/
/*|            It might be desireable to handle this case differently.|*/
/*|                                                                   |*/
/*|  :MODS.    03_Jun_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    06_Jul_88   K.J.Walsh   Name changed from s_pad().     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


sint s_pad (out, in, size, padder, side)
string  *out      ;               /* String to be created              */
string  in        ;               /* Incoming string                   */
tint    size      ;               /* Final size of output              */
byte    padder    ;               /* Padding character                 */
byte    side      ;               /* Side, L, R, or C                  */
{
   tint     diff  =  0     ;      /* Calculated differences in sizes   */
   tint     count =  0     ;      /* A counter for creating addon str's*/
   tint     in_len=  0     ;      /* Length of input string            */
   string   addon =  S_NUL ;      /* Padding string to be added on     */
   sint     rc    =  0     ;      /* RC from stuff                     */
   tint     fudge =  0     ;      /* If diff is odd during center,     */
                                  /* this is set to 1 a pad or truncate*/
                                  /* to the beginning of the string is */
                                  /* done.                             */
   bool     same  =  FALSE;       /* Is inputstr = outputstr (for C)?  */
   string   dummy =  S_NUL;       /* Holder STR for when same==TRUE    */


   in_len = s_len(in);

   side = OS_TOUPPER(side);
   if ( in_len == size )
   {
      rc = s_copy(out,in,0,0);
      if ( side != 'R' && side != 'L' && side != 'C' )
      {
         rc = S_BAD_PARM;
      }
   }
   else
   {
      switch(side)
      {
         case 'R':
            if ( in_len > size )       /* Just truncate it            */
            {
               rc = s_copy(out,in,1,size);
                rc = 0;
            }
            else
            {                          /* Figgure out what to add,     */
               diff = size - in_len;   /* and then do it               */
               rc = s_alloc(&addon,diff);
               s_smp_ul(addon) = diff;
               for( count = 0 ; count < diff ; count++ )
               {
                  s_char_s(addon,count) = padder;
               }

               rc = s_cat(out,in,addon,0,0);
               rc = s_drop(&addon);
               rc = 0;
            }
            break;

         case 'L':
            if ( in_len >  size )      /* Gotta truncate it            */
            {
               rc = s_copy(out,in,in_len-size+1,0);
                rc = 0;
            }
            else
            {
               diff = size - in_len;
               rc = s_alloc(&addon,diff);
               s_smp_ul(addon) = diff;
               for( count = 0 ; count < diff ; count++ )
                  s_char_s(addon,count) = padder;
               rc = s_cat(out,addon,in,0,0);
               rc = s_drop(&addon);
               rc = 0;
            }
            break;

         case 'C':
         case 'B':
            if ( in_len >  size )      /* Gotta trim both sides        */
            {
               diff  = ( in_len - size ) / 2 ;
               fudge = ( in_len - size ) % 2 ;
               rc = s_copy(out,in,diff+1+fudge,size );
            }
            else                       /* else gotta pad both sides    */
            {
               if ( *out == in )       /* Input string == output ?     */
               {
                  s_alloc(&dummy,size);
                  same = TRUE;
               }
               else
               {
                  dummy = *out;
               }
               diff = ( size - in_len ) / 2;   /* Add this on each side*/
               fudge= ( size - in_len ) % 2 + diff;
               rc = s_alloc(&addon,fudge);
               s_smp_ul(addon) = fudge;
               for( count = 0 ; count < fudge ; count++ )
                  s_char_s(addon,count) = padder;
               rc = s_copy(&dummy,addon,1,fudge); /* put on the front  */
               rc = s_cat(&dummy,dummy,in,  0,0);  /* put on the middle*/
               rc = s_cat(&dummy,dummy,addon,1,diff);
               rc = s_drop(&addon);
               if ( same == TRUE )
               {
                  s_drop(out);
                  *out = dummy;
               }
            }
            break;

         default:
            s_nulout(out);
            rc = S_BAD_PARM;

      }
   }
   return(rc);
}
