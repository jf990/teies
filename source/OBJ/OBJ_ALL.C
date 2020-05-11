/************************************************************************
    <DOC>
    <NAME>               OBJ_ALL.C
    <LEVEL>              OBJ
    <AUTHOR>             Steve Kuo, John L. Foster
    <DATE>               June 30, 1987
    <PARAMS>
            obj_type hword  INPUT  Type of object to allocate - #defined
                                   in obj_comm.h.

            location byte   INPUT  Is this a local or remote object?

            oid      OID *  INPUT  Force OID to this value.

    <OUTPUT>
            struct OCB *       return pointer to the Object Control Block
            -1                 failed
    <PURPOSE>
            Allocate a new object based upon the definition (odt)
            and returns the pointer of object control block of the new
            instance.

            If an OID pointer is passed that can be determined to be a
            valid OID, we will use that value as the OID of the new object
            instead of generating a new unique OID.  This should only be
            used for distributed objects where another server has already
            generated a unique OID and passed the object to another server
            for linkage.  It is also assumed that the OID has already been
            checked in the SOIDI to make certain it does not already des-
            cribe an existing object (possibly of another type).

    <CALLS>
            oid_new();
            ocd_dbp();
            dms_ce();
           *dms_opn();
            dms_sdp();
            dms_sd();
            dms_rpl();
            dms_cls();
    <LIMITS>
            lfd is temporary assumed to be 1.
    <MODS>
            Mar 4, 1989   JF         Added SDELI logic.
    </DOC>
************************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "exc.h"
#include "tm.h"




OCB        *obj_all ( obj_type, location, oid )

hword                 obj_type;
byte                  location;
OID                  *oid;

{


   hword       lfd      = 1     ; /* Logical File Descriptor           */
struct dms_cb *dmscb    = NIL   , /* Pointer to the DMS control block  */
              *dbp      = NIL   ; /* pointer to data base path         */
   ODT        *odt      = NIL   ; /* Pointer to the ODT for the object */
   OCD        *ocd      = NIL   ; /* Pointer to the OCD                */
   OCB        *new_ocb  = NIL   ; /* pointer to the created OCB        */
   OID        *newoid   = NIL   ; /* 8 byte Object Identifier          */
   sint       *len      = 0     , /* pointer to the length of dbp      */
               goid     = 0     , /* Group Object Identifier           */
               rc       = 0     ; /* return code                       */
   boolean     free_oid = TRUE  ; /* True if we allocated the OID here */






odt   = odt_ptr( obj_type );


if( ODT_POINTER_OK( odt ) )
{


   goid  = dms_ce ( lfd, odt_lvl(odt)  );
   if( goid > 0 )
   {

      dmscb = dms_opn( lfd, goid, DMS_OPEN_UPDATE );

   }
   if( dmscb == NIL || goid < 1 )
   {


      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
       |                                                               |
       |  At this point the data base is probably full.  We need to    |
       |  double check that the LFD is ok and that block 0 is usable.  |
       |  If so then the data base is probably full.  Now check the    |
       |  deletion index to see if there are any objects we can delete.|
       |  If not, we cannot continue until the user either deletes some|
       |  material or erases the data base.                            |
       |                                                               |
      \* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


      rc = obj_sdeli( & new_ocb, obj_type, SDELI_FIND_BY_OBJECT_TYPE,
                                                       & oid, NIL, NIL );
      if( rc != 0 )
      {

         rc = except ( CANNOT_CREATE_OBJECT, CONTROL_ERROR, S_NULL,
                                    S_NULL, S_NULL, S_NULL, S_NULL  );
   return(NIL);

      }
   }
   if( OID_POINTER_OK( oid ) )
   {

      newoid   =   oid;
      free_oid = FALSE;

   }
   else
   {

      newoid   = (OID *) os_mema( sizeof( OID ));
      free_oid = TRUE                   ;
      rc       = oid_new( newoid       );

   }


  /*----------------------------
    allocate OCB and fill it in
    ----------------------------*/
   new_ocb            = ocb_all();
   os_memc( newoid, &( new_ocb->oid ), sizeof( OID ) );
   new_ocb->lfd       = lfd      ; /* Logical File Descriptor          */
   new_ocb->goid      = goid     ; /* Group Object Identifier          */
   new_ocb->dmscb     = dmscb    ; /* Pointer to the DMS control block */
   new_ocb->mode      = OBJ_UPDATE;/* Mode of open                     */
   new_ocb->type      = obj_type ; /* Type of ODT to use               */
   os_dttm (&new_ocb->d_t)       ; /* Current date and time            */
   new_ocb->isvref    = odt_nxto(odt) ;
                                   /* ISV reference to name of object  */
   new_ocb->odt       = odt      ; /* Pointer to odt to use            */
   new_ocb->open_count= 1        ; /* Set since object is now open     */

   if( 0  != soidi_ad( newoid, lfd, goid, new_ocb->type ) )
   {

       ocb_drp( new_ocb );
       new_ocb = NIL ;
       except( SOIDI_CANNOT_BE_UPDATED, FATAL_ERROR, S_NULL, S_NULL,
                                             S_NULL, S_NULL, S_NULL );

   }

   if( location == REMOTE_OBJECT   ||
       location == FOREIGN_OBJECT  ||
       obj_test_mode( new_ocb, OBJ_REMOTE ) )
   {

      obj_set_mode( new_ocb, OBJ_REMOTE    );
      location =             REMOTE_OBJECT  ;

   }
   else
   {

      location = LOCAL_OBJECT;

                        /*---------------------------------------------*/
                        /*We now have a data base entry and an OCB for */
                        /*the new object. Now we have to initialize it */
                        /*by filling in all the data we have so far and*/
                        /*writing the segments to the DMS.             */
                        /*---------------------------------------------*/


      ocd    = ocd_new( &ocd, "%SYSTEM.TYPE" );
      obj_rep( new_ocb, ocd, &new_ocb->type,   SEG_HOLD   );
      s_init(  &ocd->name, "%SYSTEM.OID"                  );
      obj_rep( new_ocb, ocd, &new_ocb->oid,    SEG_HOLD   );
      s_init(  &ocd->name, "%SYSTEM.CREATE_DATE"          );
      obj_rep( new_ocb, ocd, &new_ocb->d_t,    SEG_HOLD   );
      s_init(  &ocd->name, "%SYSTEM.MODIFY_DATE"          );
      obj_rep( new_ocb, ocd, &(new_ocb->d_t),  SEG_HOLD   );
      s_init(  &ocd->name, "%SYSTEM.LASTACC_DATE"         );
      obj_rep( new_ocb, ocd, &(new_ocb->d_t),  SEG_HOLD   );
      s_init(  &ocd->name, "%SYSTEM.GRANULARITY"          );
      obj_rep( new_ocb, ocd, &(new_ocb->d_t),  SEG_UPDATE );

      s_init(  &ocd->name, "%LINKS.AUTHOR"                );
      obj_rep( new_ocb, ocd, &(TM_master->ucb->user_id), SEG_HOLD );

      /* Need to add Author reverse link. */

      s_init(  &ocd->name, "%LINKS.LAST_EDITOR"           );
      obj_rep( new_ocb, ocd, &(TM_master->ucb->user_id), SEG_HOLD );
      s_init(  &ocd->name, "%LINKS.MODIFIER"              );
      obj_rep( new_ocb, ocd, &(TM_master->ucb->user_id), SEG_UPDATE );

   }
   new_ocb->location = location;


   ocd_drp( & ocd       );
   if( free_oid )
   {

      os_memd( newoid    );

   }
}
else
{


}



   return(new_ocb);
}
