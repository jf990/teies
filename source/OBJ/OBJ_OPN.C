/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_opn                                                |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    June 15, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <PARAM>   *oid        byte     INPUT    This is a pointer to a 8 |
  |                                          byte Object Identifier.  |
  |             mode       byte     INPUT    This byte will indicate  |
  |                                          whether the object is to |
  |                                          be opened in             |
  |                                          Update -- OBJ_UPDATE     |
  |                                          Read   -- OBJ_READ       |
  |                                                                   |
  |  <PURPOSE> This routine will open an object given its OID.        |
  |            Note - if the high bit of mode is set then we are not  |
  |            going to open a TEIES object.  Instead, recast the oid |
  |            pointer to point to a TEIES string and assume it       |
  |            contains a GFD of a local OS file to use as an object. |
  |                                                                   |
  |  <OUTPUT>  OCB *                                                  |
  |            NIL if OID not found.                                  |
  |            -1 if DMS error on the object.                         |
  |                                                                   |
  |  <CALLS>   os_mema (), ocb_inta (), soidi_gt (),                  |
  |            odt_ptr (), os_qfd(), fopen()                          |
  |                                                                   |
  |  <ERRORS>  20002 : OID is not in the SOIDI                        |
  |            20700 : DMS_OPN failed on the LFD/GOID associated to   |
  |                     the OID found in the SOIDI. Actually 20700 is |
  |                     subtraceted from the RC to get the DMS error. |
  |                                                                   |
  |  <LIMITS>  If mode is not valid OBJ_UPDATE is assumed.            |
  |                                                                   |
  |  <mods>    Jul 26 88     JF     Started to add distributed object |
  |                                 handling.                         |
  |  <mods>    Sept 13, 89   JF     Added query GOID status logic for |
  |                                 locked objects.                   |
  |  </DOC>                                                           |
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

extern FILE * fopen();



OCB       * obj_opn (oid, mode)

OID                 *oid;
hword                mode;



{

   OCB        *new_ocb  = NIL   , /* OCB of newly opened object        */
              *pocb     = NIL   ; /* Pointer to previous OCB           */
   hword       lfd      = 0     , /* Logical File Descriptor           */
               obj_type = 0     , /* Indicates which ODT to use        */
               dmsmode  = 0     ; /* OBJ Mode converted to a DMS mode  */
   tint        goid     = 0     ; /* Group Object Identifier           */
   sint        rc       = 0     , /* Return codes from other routines  */
               open_status = 0  ; /* Result of DMS_QGS                 */
   string      tf_name  = S_NULL, /* Result of os_qfd()                */
               g_f_d    = S_NULL, /* Converted generic file descriptor */
               f_default= S_NULL; /* Default file name for GFD         */
   byte       *fmode    = NIL   ; /* File mode for fopen()             */
struct dms_cb *dmscb    = NIL   ; /* Pointer to the DMS control block  */
   boolean     no_errors_occured = TRUE; /* BREAK for errors.          */






if( OID_POINTER_OK( oid ) )
{

                                  /* Convert OBJ mode into a DMS mode  */
if ( mode & OBJ_UPDATE )
{

    dmsmode = DMS_OPEN_UPDATE ;
    fmode   = "a+"            ;

}
else if( mode & OBJ_READ )
{

    dmsmode = DMSOPNR ;
    fmode   = "r"     ;

}
else
{

    dmsmode = DMS_OPEN_UPDATE ;
    fmode   = "a+"            ;

}

while( no_errors_occured )
{

   if( ! (mode & OBJ_NOT_OBJECT) )
   {

   if( ! OID_IS_NULL( oid ) )
   {


      pocb = ocb_inta (  oid, mode  ) ;

      if ( OCB_POINTER_OK( pocb ) )
      {

         new_ocb = pocb;
         new_ocb->open_count ++ ;

           /* At this point we need to verify that the already open
              OCB is the same MODE -OR- if we need to produce a new
              view of the same object.                                 */

         break;

      }
      rc = soidi_gt (  oid, & lfd, & goid, & obj_type  ) ;

      if (rc == OID_NOT_IN_SOIDI  &&  (mode & OBJ_REMOTE) )
      {

         switch( TM_master->usrtype )
         {

            case REAL_USER      :


                    new_ocb = t_r_obj( TM_master->MS, mode & ~OBJ_REMOTE, oid );
                    if( ! OCB_POINTER_OK( new_ocb ) )
                    {

                       rc = Item_Does_Not_Exist;

                    }
                    else
                    {

                       obj_set_mode( new_ocb, OBJ_REMOTE );

                    }
                    break;

            case PSEUDO_USER    :

                    break;

            case MASTER_SERVER  :

                    break;

            case DBMS_SERVER    :

                    break;

            case NET_SERVER     :

                    break;

            case UNKNOWN_SERVER :

                    break;

            default             :

                    break;


         }

      }
      if( (goid > 0)   &&   (lfd > 0) )
      {


         new_ocb = ocb_all();

         if( OCB_POINTER_OK( new_ocb ))
         {

            dmscb = dms_opn (lfd, goid, dmsmode) ;

            if ( dmscb == NIL )
            {

               open_status = dms_qgs( lfd, goid );
               rc = DMS_LEVEL_ERROR + 700 + open_status ;
               if( open_status & (
                          DMS_QUERY_GOID_STATUS_ERROR_ENCOUNTERED
                        | DMS_QUERY_GOID_STATUS_UNUSED_GOID
                        | DMS_QUERY_GOID_STATUS_NOT_INITIALIZED
                 )      )
               {

                  except( rc,INFO,S_NULL,S_NULL,S_NULL,S_NULL,S_NULL) ;

               }
               else if( open_status & (
                               DMS_QUERY_GOID_STATUS_LOCKED_GOID
                             | DMS_QUERY_GOID_STATUS_WRITE_LOCK_SET
                      )      )
               {

                  dmscb = dms_opn( lfd, goid, DMSOPNF );
                  if( dmscb == NIL )
                  {

                     except( rc,INFO,S_NULL,S_NULL,S_NULL,S_NULL,S_NULL);

                  }
               }
               else
               {


               }
            }

            if( dmscb == NIL )
            {

               ocb_drp( new_ocb );
               new_ocb  = NIL    ;

            }
            else
            {

               new_ocb->oid.one   = oid->one   ;
               new_ocb->oid.two   = oid->two   ;
               new_ocb->oid.three = oid->three ;
               new_ocb->oid.four  = oid->four  ;
               new_ocb->oid.five  = oid->five  ;
               new_ocb->oid.six   = oid->six   ;
               new_ocb->oid.seven = oid->seven ;
               new_ocb->oid.eight = oid->eight ;
               new_ocb->lfd       = lfd        ;
               new_ocb->goid      = goid       ;
               new_ocb->dmscb     = dmscb      ;
               new_ocb->type      = obj_type   ;

               new_ocb->odt       = odt_ptr (obj_type) ;
               new_ocb->isvref    = odt_nxto(new_ocb->odt) ;

            }
         }
         else
         {

            break;

         }
      }
      else
      {


      }
   }
   else
   {


   }
   }
   else
   {

      s_copy(   & g_f_d, (string) oid, 0, 0 );
      if ( s_len( g_f_d ) < 1 )
      {

         new_ocb = NIL;
         break;

      }


      new_ocb = ocb_all();

      s_init( &f_default, "* * * *" );
      rc = os_qfd( &tf_name, g_f_d, f_default );
      if( rc == 0 || s_len( tf_name ) > 1 )
      {

         s_put( tf_name, NIL, s_len(tf_name)+1 );

      }
      else
      {

         new_ocb = NIL;
         break;

      }
      dmscb = (struct dms_cb *)      fopen( s_buffer( tf_name ), fmode );

      s_drop( &f_default );
      s_drop( &tf_name   );


      if (dmscb == NIL)             /* Check if DMS open successful    */
      {

         except ( GFD_DOES_NOT_EXIST, INFO, tf_name, g_f_d, S_NULL,
                                                    S_NULL, S_NULL    ) ;
         ocb_drp( new_ocb );
         new_ocb = NIL;
         s_drop( & g_f_d );
         break;

      }
      new_ocb->oid.one  = 0xFF     ;
      new_ocb->goid     = 0        ; /* Group Object Identifier       */
      new_ocb->dmscb    = dmscb    ; /* Pointer to  DMS control block */
      new_ocb->type     = 0        ; /* Type of ODT to use            */
      new_ocb->actions  = NULL     ; /* Actions upon object    */
      new_ocb->gfd      = g_f_d;   ; /* File descriptor if !obj*/
      new_ocb->mode     = LOCAL_OS_FILE;
      new_ocb->odt      = NULL     ;
      new_ocb->isvref   = 0        ;
                                      /*                              */
   }                                  /*                              */
                                      /*                              */
   if( OCB_POINTER_OK( new_ocb )  )   /*                              */
   {                                  /*                              */
                                      /*                              */
                                      /*Handle Common initialization  */
                                      /*                              */
      os_dttm (&(new_ocb->d_t))    ; /* Current date and time         */
      new_ocb->mode    |= mode     ; /* Mode of open                  */
      new_ocb->open_count = 1      ; /* Mode of open                  */



   }
   break;

}
}

   return(new_ocb);
}
