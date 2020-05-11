/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    MAIN == STK_TST.C                                      |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14 1987                                        |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   - none -                                               |
  |                                                                   |
  |  :PURPOSE. Test the stack level routines.                         |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "debugi.h"
#include "teies.h"
#include "stack.h"



main( argc, argv )

sint argc;
byte **argv;

{


STACK *tmstack = NIL;
sint   rc      = 0;
sint  *ptr     = NIL;

#D_main(  stk_tst,  STACK  );

#D_say('Starting Stack test');

tmstack = stk_ini( 10, sizeof( sint ) );

#D_show( 'New stack at x%x;','tmstack' );

rc = 55;
rc = stk_psh( tmstack, &rc );

#D_show( 'Results of push RC=%d','rc' );

#D_show('Stack size=%d; tos=x%x; base=x%x; Esize=%d','tmstack->used,\
tmstack->tos,tmstack->base,tmstack->size');

ptr = &rc;
ptr = stk_pop( tmstack );

#D_show( 'Pop gave us %d','*ptr' );


rc = 67;
rc = stk_psh( tmstack, &rc );
rc = 77;
rc = stk_psh( tmstack, &rc );
rc = 88;
rc = stk_psh( tmstack, &rc );
rc = 99;
rc = stk_psh( tmstack, &rc );
rc = 01;
rc = stk_psh( tmstack, &rc );
rc = 11;
rc = stk_psh( tmstack, &rc );
rc = 22;
rc = stk_psh( tmstack, &rc );
rc = 33;
rc = stk_psh( tmstack, &rc );
rc = stk_dup( tmstack );
#D_show('%d remaining on stack','stk_qry(tmstack)');
#D_show('Stack size=%d; tos=x%x; base=x%x; Esize=%d','tmstack->used,\
tmstack->tos,tmstack->base,tmstack->size');
rc = stk_dsp(tmstack);

ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );

#D_show('Stack size=%d; tos=x%x; base=x%x; Esize=%d','tmstack->used,\
tmstack->tos,tmstack->base,tmstack->size');

ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );
#D_show( 'Pop gave us %d','*ptr' );
ptr = stk_pop( tmstack );

#D_show('%d remaining on stack','stk_qry(tmstack)');
stk_end( tmstack );



#D_return( 0 );

}
