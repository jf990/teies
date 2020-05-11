/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_rmv                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 03, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb      struct OCB   INPUT  Pointer to the Object's  |
  |                                          Control Block.           |
  |                                                                   |
  |  <PARAM>   *ocd      struct OCD   INPUT  Pointer to the Object    |
  |                                          Component Descriptor to  |
  |                                          delete.                  |
  |                                                                   |
  |  <PURPOSE> This routine deletes the specified component of the    |
  |            object.  The entire object is deleted if the OCD is    |
  |            NIL (but not if the ocd name is S_NULL).  Also handles |
  |            os files and remote objects.                           |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_send(), t_waitc(), os_gfdd(), dms_de(), dms_rmv()    |
  |  <LIMITS>  If OCD==NIL then after this routine returns the OCB    |
  |            is no longer valid.  The caller should NIL it out.     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"





sint        obj_rmv (    ocb, ocd    )


OCB         *ocb ;                /* Pointer to the Objects Cntrl Blk  */
OCD         *ocd ;                /* Ptr to the Component Descriptor   */



{


   sint     rc        =      0;
   string   db_path   = S_NULL;
   ODT     *odt_entry =    NIL;









   if( OCB_POINTER_OK ( ocb ) )
   {

      if( ocd == NIL )
      {


      }
      else
      {

         if( obj_test_mode( ocb, OBJ_UPDATE ) )
         {

            if( (odt_entry = odt_find( ocb, ocd )) != NIL )
            {

               if(  (odt_type( odt_entry ) > OC_SEG)  ||
                     ocd_key(  odt_entry )              )
               {

                  if( obj_test_mode( ocb, OBJ_CACHE_PENDING ) )
                  {

                     rc = ocb_segflush( ocb, OCB_CACHE_CLEAR );

                  }
                  rc = ocd_dbp( ocb, ocd,           & db_path );
                  rc = dms_sdp( ocb->dmscb, s_buffer( db_path ),
                                            s_len(    db_path ) );
                  rc = dms_rmv( ocb->dmscb );
                  if( rc == DMS_RMV_EMPTY )
                  {

                     rc = INDEX_TO_REMOVE_DOES_NOT_EXIST;

                  }
                  else if ( rc != DMS_RMV_OK )
                  {


                  }
                  else
                  {

                     rc = 0;
                     obj_set_mode( ocb, OBJ_SYNC_PENDING );

                  }
                  s_drop( & db_path );

               }
               else
               {

                  rc = obj_rep( ocb, ocd, NIL, SEG_UPDATE );

               }
            }
         }
         else
         {

            rc = OBJECT_IS_READ_ONLY;

         }
      }
   }
   else
   {

      rc = INVALID_OCB ;

   }

   return(rc);
}
