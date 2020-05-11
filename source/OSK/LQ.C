/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    LQ                                                     |
  |                                                                   |
  |  :AUTHOR.  Anders Bandholm                                        |
  |                                                                   |
  |  :DATE.    October 10, 1988                                       |
  |                                                                   |
  |  :PARAM.   argc        int       INPUT   number of agrs           |
  |            argv        *char(::) INPUT   the arg-vector           |
  |                                                                   |
  |            The arguments passed in the argv are supposed to be    |
  |            real-addresse of two VMBLOKS.                          |
  |                                                                   |
  |  :PURPOSE. To get the contents of the "last queued"-tag at        |
  |            relative addr. 0x174 in the VMBLOK, and to find the    |
  |            difference of the to tags. If one of the VMBLOKS is    |
  |            one's own, the difference of tags is proportional to   |
  |            the idletime of the other user.                        |
  |                                                                   |
  |  :OUTPUT.  If called from REXX:                                   |
  |                the tag-difference as a string                     |
  |                                                                   |
  |            Otherwise prints the tags to stdout, and returns 0     |
  |                                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  The arguments must be valid hex-numbers.               |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/
#include <libfns>
#include <vmlib>
#include <rexxdef>

#define DISPL 0x174                    /* displacement in VMBLOK */


int main( int argc, char *argv(::) )
  {
    diag_regs     regs;
    int           i, diff;
    unsigned int  start1, start2, inbuf(: 2 :), outbuf(:2:);
    char          result(: 50 :);

    if ( argc != 3 )
      {
        printf( "You must give two VMBLOK-addresses as arguments|" );
        exit( 1 );
      }
    sscanf( argv(:1:), "%x", &start1 );  /* read args */
    sscanf( argv(:2:), "%x", &start2 );
    inbuf(:0:) = start1 + DISPL;         /* setup input list for      */
    inbuf(:1:) = start2 + DISPL;         /*                  DIAGNOSE */
    regs.dr_rx   = (int) &inbuf(:0:);    /* prepare registers for     */
    regs.dr_ry   = 2;                    /*                  xx_diag  */
    regs.dr_ryp1 = (int) &outbuf(:0:);
    _diagxx( 4, &regs );                 /* Call DIAGnose with code 4 */
    diff = outbuf(:0:) - outbuf(:1:);
    if ( isrexxfn() )
      {
         if ( diff < 0 )               /* Negative RC seems to cause   */
         {                             /* problems even if acceptable  */
            diff = 0;
         }
        sprintf( result, "%d", diff);
        rexxit( result );
      }
    else
      {
        printf( "LQ tags: %X %X, difference is %d (decimal)\n",
                outbuf(:0:), outbuf(:1:), diff );
        exit( 0 );
      }
  }
