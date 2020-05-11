/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_del                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July  2, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master->flags              Read bit flag to deter-  |
  |                                          mine real object removal |
  |                                          or queing in SDELI.      |
  |                                                                   |
  |  <PARAM>   oid       OID *        INPUT  Pointer to the Object's  |
  |                                          Identifier to delete.    |
  |                                                                   |
  |  <PURPOSE> This routine deletes the specified object.  If it is   |
  |            open then delete will return an error code.            |
  |                                                                   |
  |            Remove the object from the SOIDI.                      |
  |                                                                   |
  |            Enter the object on the SDELI.                         |
  |                                                                   |
  |            Mark the object for deletion (%SYSTEM.STATUS).         |
  |                                                                   |
  |            Unlink is not fully done until real delete is requested|
  |                                                                   |
  |  <OUTPUT>  sint            result code.                           |
  |  <CALLS>   dms_de(), obj_inta(), soidi_de()                       |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>                                                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"



sint        obj_del (    oid    )


OID                     *oid ;



{


   hword    lfd         =      0,
            object_type =      0;
   tint     goid        =      0;
   sint     rc          =      0;
   OCB     *ocb         =    NIL;
   boolean  real_delete =   TRUE;







if( OID_POINTER_OK( oid ) )
{


   ocb = ocb_inta( oid, (hword) 0  );
   if( ! OCB_POINTER_OK( ocb ) )
   {

      if( soidi_gt( oid, & lfd, & goid, & object_type ) == 0 )
      {


         if( ! real_delete )
         {

            rc = obj_sdeli( NIL, object_type, SDELI_ADD_TO_INDEX, & oid,
                                                              NIL, NIL );
         }
         else
         {

            rc = dms_de( lfd, goid );
            if( rc == DMS_DELETE_ENTRY_DONE )
            {

               rc = 0;

            }
         }
         if( rc == 0 )
         {

            rc = soidi_de( oid );

         }
      }
      else
      {

         rc = OID_NOT_IN_SOIDI;

      }
   }
   else
   {

       rc = OID_IS_OPEN;

   }
}
else
{

   rc = INVALID_OID_POINTER;

}



   return(rc);
}
