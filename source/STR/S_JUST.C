/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_just()                                               |*/
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
/*|  :CALLS.   s_alloc(), s_cat, s_copy(), s_drop(),                 |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.  If difference between the source length and the desired|*/
/*|            length is odd during the CENTER operation, a character |*/
/*|            is padded or truncated to/from the start of the string.|*/
/*|            It might be desireable to handle this case differently.|*/
/*|                                                                   |*/
/*|  :MODS.    03_Jun_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    25_Jan_88   K.J.Walsh   Updated to use #s_pcheck().    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

#s_c_xtrn();                     /* Externs for #s_check()/#s_pcheck() */

extern   sint  s_drop() ;
extern   sint  s_cat()  ;
extern   sint  s_alloc();
extern   sint  s_copy() ;

s_just (out, in, size, padder, side)
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

   #D_start(s_just,str,'out=0x%X, in=0x%X, size=%d, padder=''%c'', \
      side=''%c''','out,in,size,padder,side');
   #s_pcheck(out);
   #s_check(in);
   #D_str  (*out,'start') ;
   #D_str  (in,'start') ;

   in_len = s_len(in);

   if ( in_len == size )
   {
      rc = s_copy(out,in,0,0);
      #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_copy()');
      #D_leave(0,'they were the same size');
   }

   switch(side)
   {
      case 'R':
      case 'r':
         if ( in_len > size )          /* Just truncate it            */
         {
            rc = s_copy(out,in,1,size);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_copy()');
            #D_leave(0,'output is input trimmed from the left');
         }
         else
         {                             /* Figgure out what to add,     */
            diff = size - in_len;      /* and then do it               */
            rc = s_alloc(&addon,diff);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_alloc()');
            s_smp_ul(addon) = diff;
            for( count = 0 ; count < diff ; count++ )
            {
               s_char_s(addon,count) = padder;
            }

            rc = s_cat(out,in,addon,0,0);
            #D_chekrc('S_BAD_RC(rc)','rc','Bad RC from s_cat()');
            rc = s_drop(&addon);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_drop()');
            #D_leave(0,'output is input with extra trailing chars');
         }
         break;

      case 'L':
      case 'l':
         if ( in_len >  size )         /* Gotta truncate it            */
         {
            rc = s_copy(out,in,in_len-size+1,0);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_copy()');
            #D_leave(0,'output is input trimmed from the right');
         }
         else
         {
            diff = size - in_len;
            rc = s_alloc(&addon,diff);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_alloc()');
            s_smp_ul(addon) = diff;
            for( count = 0 ; count < diff ; count++ )
               s_char_s(addon,count) = padder;
            rc = s_cat(out,addon,in,0,0);
            #D_chekrc('S_BAD_RC(rc)','rc','Bad RC from s_cat()');
            rc = s_drop(&addon);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_drop()');
            #D_leave(0,'output is input with extra leading characters');
         }
         break;

      case 'C':
      case 'c':
         if ( in_len >  size )         /* Gotta trim both sides        */
         {
            diff  = ( in_len - size ) / 2 ;
            fudge = ( in_len - size ) % 2 ;
            #D_show('in_len>size on C; diff=%d; fudge=%d.','diff,fudge');
            rc = s_copy(out,in,diff+1+fudge,size );
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_copy()');
            #D_leave(0,'output is input trimmed from both sides');
         }
         else                          /* else gotta pad both sides    */
         {
            diff = ( size - in_len ) / 2; /* Add this much on each side*/
            fudge= ( size - in_len ) % 2 + diff;
            #D_show('size (requested len)=%d, in_len (input str \
               len)=%d diff=%d; fudge=%d','size,in_len,diff,fudge');
            rc = s_alloc(&addon,fudge);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_alloc()');
            s_smp_ul(addon) = fudge;
            for( count = 0 ; count < fudge ; count++ )
               s_char_s(addon,count) = padder;
            rc = s_copy(out,addon,1,fudge); /* put on the front        */
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_copy()');
            rc = s_cat(out,*out,in,0,0);    /* put on the middle       */
            #D_chekrc('S_BAD_RC(rc)','rc','Bad RC from s_cat()');
            rc = s_cat(out,*out,addon,1,diff);
            #D_chekrc('S_BAD_RC(rc)','rc','Bad RC from s_cat()');
            rc = s_drop(&addon);
            #D_chekrc('S_BAD_RC(rc)',rc,'Bad RC from s_drop()');
            #D_leave(0,'output is input with extra chars on both sides');
         }
         break;

      default:
         #D_leave(S_BAD_PARM,'Invalid justification character');

   }
}
