/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_end                                                |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    July  2, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> To terminate all activites associated with OBJ level.  |
  |            Must go through the OCB table and close all open objs. |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocb_inta (), obj_cls (), dms_end ()                    |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "obj_table.h"
#include "tm.h"





sint   obj_end (  )


{


   OCB   * ocb      =  NIL;       /* Pointer to the PSI's OCB          */
   sint    position =    1,
           rc       =    0,
           i        =    0;





                                             /**************************/
                                             /*                        */
                                             /*   First, close all     */
                                             /*   open objects.        */
   ocb = ocb_inta( (OID *) -1, (hword) 0 );  /*                        */
   while( OCB_POINTER_OK( ocb ) )            /**************************/
   {

      ocb->open_count = 0;
      obj_reset_mode( ocb, OBJ_FLUSH );
      obj_cls(  ocb  );
      ocb = ocb_inta( (OID *) -1, (hword) 0 );

   }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
   |                                                                   |
   |                    Remove all ODT's in the system                 |
   |                                                                   |
   * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   if( TM_master != NIL )
   {

      if( TM_master->odt != NIL )
      {

         for( i=0; i < NUMBER_OF_ODTs; i++ )
         {

            if( (((TM_master->odt) + i)->odt_ptr ) != NIL )
            {

               os_memd( ((TM_master->odt) + i)->odt_ptr );

            }
         }
         os_memd( TM_master->odt );
         TM_master->odt = NIL;

      }

      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
       |                                                               |
       |                Remove the Object Table Information            |
       |                                                               |
       * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      for( position = 0; position < TM_master->obj_t_size; position++ )
      {

         s_drop( &(objtable[ position ].object_name) );

      }
      TM_master->obj_t_size = 0;

   }

   rc = dms_end ( 0, 0 ) ;



   return(0);
}
