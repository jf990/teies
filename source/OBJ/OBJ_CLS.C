/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_cls                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 03, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb      struct OCB   INPUT  The OCB of the object to |
  |                                          close.                   |
  |                                                                   |
  |  <PARAM>    mode     byte         INPUT  Ignored.                 |
  |                                                                   |
  |  <PURPOSE> To close an object. OBJ_UPDATE and OBJ_COMMIT modes    |
  |            update the data base if the object is open in write    |
  |            mode.  OBJ_CANCEL and OBJ_ABANDON close the object and |
  |            do not update the data base.  OBJ_FLUSH closes the     |
  |            object to update the data base and then reopens the    |
  |            object, assuring recent updates are committed.         |
  |                                                                   |
  |  <OUTPUT>  sint           result code.                            |
  |  <CALLS>   dms_cls(), obj_del(), ocb_drp(), os_memc()             |
  |  <ERRORS>                                                         |
  |  <LIMITS>  If invalid mode is specified OBJ_CANCEL is assumed.    |
  |                                                                   |
  |            The FLUSH flag always takes priority.  DELETE, READ and|
  |            REMOTE are ignored if FLUSH is set concurrently with   |
  |            those flags.  Also, if REMOTE is set and the open count|
  |            is less than 1, the object is deleted from the local   |
  |            server.                                                |
  |  <MODS>    Aug-17-89   JF   Added os-file FLUSH logic.            |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/
#include "stdio.h"
#include "file.h"
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"





sint  obj_cls( ocb )

OCB           *ocb     ;          /* OCB of the object to be closed    */


{

   hword       dmsmode        =     0,
               mode           =     0;
   sint        rc             =     0;
   boolean     going_to_flush = FALSE;
   OID         oid                   ;





if( ! OCB_POINTER_OK( ocb ) )
{

   rc = INVALID_OCB;

}
else
{

                                  /* Check if OCB exists for this OID  */

   os_memc( & (ocb->oid), & oid, sizeof( OID ) );
   if (  ( OID_IS_NULL( (& oid) )      )
    ||   ( ocb_inta( & oid, ocb->mode )) == NIL )
   {


   }
   else
   {

      mode           = ocb->mode;
      going_to_flush = obj_test_mode( ocb, OBJ_FLUSH );

      if( ! ( going_to_flush && (ocb->open_count < 2) ) )
      {

         ocb->open_count -- ;

      }

      if( ( ocb->open_count < 1 )  ||  going_to_flush )
      {

         if( mode & LOCAL_OS_FILE )
         {


            if( ! going_to_flush )
            {

               rc = fclose( (FILE *) ocb->dmscb );

               if( rc == EOF )
               {


               }
               if( (mode  & OBJ_DELETE)  )
               {

                  rc = os_gfdd( ocb->gfd );

               }
               ocb_drp( ocb );

            }
            else
            {

               rc = fflush( (FILE *) ocb->dmscb );
               rewind(      (FILE *) ocb->dmscb );

               if( rc != 0 )
               {

                  rc = FILE_FUNCTION_ERROR;

               }
               obj_reset_flag( ocb, OBJ_FLUSH );
               ocb->open_count     ++ ;

            }
         }
         else
         {

            if( obj_test_mode( ocb, OBJ_CACHE_PENDING ) )
            {

               rc = ocb_segflush( ocb, OCB_CACHE_CLEAR );

            }
            if( (mode  & OBJ_UPDATE) ||  going_to_flush  )
            {

               dmsmode = DMS_CLOSE_COMMIT ;

            }
            else
            if( (mode  & OBJ_ABANDON) )
            {

               dmsmode = DMS_CLOSE_BACKOUT ;

            }
            else
            if( (mode  & OBJ_READ)    )
            {

               dmsmode = DMS_CLOSE_READONLY;

            }
            else
            {

               dmsmode = DMS_CLOSE_BACKOUT;

            }
            rc = dms_cls( ocb->dmscb, dmsmode );
            if( rc == DMS_CLOSE_OK )
            {

               rc = 0 ;

            }
            if(  ! going_to_flush  )
            {

               ocb_drp( ocb );

               if( (mode  & OBJ_DELETE)  ||  (mode  & OBJ_REMOTE) )
               {

                  rc = obj_del( & oid );

               }
            }
            else
            {

               if( mode  & OBJ_UPDATE )
               {
                  dmsmode = DMS_OPEN_UPDATE;
               }
               else
               {
                  dmsmode = DMS_OPEN_READ_ONLY;
               }

               ocb->dmscb = dms_opn( ocb->lfd, ocb->goid, dmsmode );
               ocb->last_seg     = NIL;
               ocb->last_seg_len =   0;
               s_drop( & (ocb->lastdbp)  );

               obj_reset_flag( ocb, OBJ_FLUSH );

            }
         }
      }
      else
      {


      }
   }
}


   return(rc);
}
