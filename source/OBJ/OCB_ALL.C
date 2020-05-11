/********************************************************************

    :NAME.     OCB_ALL.C
    :LEVEL.    OBJ
    :AUTHOR.   John L. Foster
    :DATE.     January 25, 1988

    :PURPOSE.  Allocate a new object control block, link it into the
               table of open objects, and initialize all elements to NIL.

    :INPUT.    * none *

    :OUTPUT.   struct OCB *    Return a pointer to the new OCB.

    :INPUT_GLOBALS.  * none *
    :OUTPUT_GLOBALS. * none *
    :CALLS.    os_mema()
    :ERRORS.
    :LIMITS.
    <MODS>
             Mar 28 89    JF      Added table full checking.
    </DOC>

 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"


extern OCB        * ocb_inta();




OCB        *ocb_all ( )




{


   OCB        *new_ocb  = NULL  ; /* pointer to the created OCB        */
   OCB        *temp     = NULL  ; /* temporary pointer                 */






   temp = ocb_inta( ( OID * ) -1, (hword) 0  );

   if ( temp == OCB_TABLE_FULL )
   {

      except( OBJ_OBJECT_TABLE_FULL, CONTROL_ERROR, S_NULL, S_NULL,
                                            S_NULL, S_NULL, S_NULL );
      new_ocb = NIL;

   }
   else
   {

      new_ocb            = (OCB *) os_mema (sizeof (OCB)) ;
      os_memn( new_ocb, sizeof( OCB ) );

      if ( ! OCB_POINTER_OK( temp ) )
      {

         TM_master->OBJcb = new_ocb;
         new_ocb->prevocb = NIL;
         new_ocb->nextocb = NIL;

      }
      else
      {

         new_ocb->prevocb  =    temp;
         temp->nextocb     = new_ocb;
         new_ocb->nextocb  =     NIL;

      }
   }


   return(new_ocb);
}
