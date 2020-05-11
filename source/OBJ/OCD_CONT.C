/********************************************************************
    <DOC>
    <NAME>               OCD_Contents
    <LEVEL>              OBJ
    <AUTHOR>             John L. Foster
    <DATE>               April 2, 1990
    <PARAMS>
           *ocb OCB             pointer to object control block
           *ocd OCD             pointer to ocdstr and index key
           *gfd OCB             object of the input data.
    <OUTPUT> sint error code.
    <GLOBAL>
    <PURPOSE>
            Figure out if the object / component combination are a
            contents or they point to a Content object.  If the latter,
            open the content object and use that for ocd_bse(). If no
            OID exists there then create a contents object.
    <CALLS>            obj_get(), obj_rep(), s_copy(), s_cat()
    </DOC>
 ********************************************************************/
#include <stdio.h>
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "exc.h"
#include "bse.h"






sint   ocd_cont(  ocb, ocd, gfd  )

OCB             * ocb    ;
OCD             * ocd    ;
OCB             * gfd    ;



{


   sint      rc           = 0     ;
   ODT     * odt_entry    = NIL   ;
   OCD     * contents_ocd = NIL   ;
   OCB     * contents     = NIL   ;
   OID       contents_oid         ;





if( ! OCB_POINTER_OK( gfd ) )
{

   rc = INVALID_OCB;
   except( rc, DATA_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

}
else
{

   if( ! OCB_POINTER_OK( ocb ) )
   {

      rc = INVALID_OCB;
      except( rc, DATA_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

   }
   else
   {


      odt_entry = odt_find( ocb, ocd );

      if(  ODT_POINTER_OK( odt_entry )  )
      {

         if( odt_type( odt_entry ) == OC_NDX )
         {

            rc = ocd_bse( ocb, ocd, gfd );

         }
         else if( odt_type( odt_entry ) == OC_OID )
         {

            rc = obj_get( ocb, ocd, & contents_oid );
            if( rc == 0 )
            {

               if( OID_IS_NULL( & contents_oid ) )
               {

                  contents = obj_all( CONTENTS_OBJ, LOCAL_OBJECT,  NIL );
                  rc = obj_rep( ocb, ocd, & contents->oid,  SEG_UPDATE );

               }
               else
               {

                  contents = obj_opn( & contents_oid, OBJ_UPDATE | OBJ_REMOTE );

               }
               if( OCB_POINTER_OK( contents )  &&
                                   contents->type == CONTENTS_OBJ )
               {

                  ocd_new   ( & contents_ocd, "CONTENT.CONTENTS" );
                  rc = ocd_bse( contents, contents_ocd, gfd );
                  ocd_drp   ( & contents_ocd );

               }
               else
               {

                  rc = INVALID_OCB;

               }
               obj_cls( contents );

            }
            else
            {

               rc = INVALID_OID;

            }
         }
         else
         {

            rc = OCD_NOT_OF_PROPER_TYPE;

         }
      }
      else
      {

         rc = OCD_NOT_IN_ODT;

      }
   }
}

   return(rc);
}
