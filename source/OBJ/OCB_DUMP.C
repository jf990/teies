/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocb_dump()                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 09, 1988                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Display information on all open objects.               |
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
#include "obj.h"
#include "tm.h"



sint   ocb_dump()


{


   OCB        *curocb ;           /* Current OCB                       */

#D_begin(  ocb_dump,  OBJ  );


   curocb = TM_master->OBJcb ;    /* Pointer to OCB table              */
   if (curocb != NIL )            /* If table doesn't exist            */
   {

      do
      {

         #D_ocb( 'curocb' );
         curocb = curocb->nextocb ;

      }
      while ( curocb != NIL );

   }

#D_return( 0 );

}
