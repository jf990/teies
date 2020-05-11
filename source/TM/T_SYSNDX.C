/********************************************************************
<DOC>
<NAME>             T_sysndx.C
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             December 30, 1987
<PARAM>            index     sint      INPUT    System Index ISV value.

<OUTPUT>           ocb   OCB *                  Pointer to the resulting
                                                object control block.

<PURPOSE>          Attempt to return an open object control block of the
                   system index corresponding to the Object type
                   requested.
<ERRORS>   0 : if the index requested is not a system index.
<LIMITS>   The SOIDI and PSI are special cases.
<MODS>     Jan 23  89      JF            Changed PSI OID value.
<MODS>     Sep 13  89      JF            Added Object locked logic.
</DOC>
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"




OCB            *t_sysndx( index )

sint                      index;


{


sint        rc              =      0,
            goid            =      0,
            open_status     =      0,
            isv_key         =      0;
OCB        *ocb             =    NIL,
           *temp            =    NIL;
OID        *oid             =    NIL;
ODT        *odt             =    NIL;
OCD        *ocd             =    NIL;
string      request         = S_NULL,
            message         = S_NULL;
boolean     ok              =   TRUE;
hword       type            =      0,
            lfd             =      0;
struct      dms_cb *dmscb   =    NIL;






if( index == SOIDI_OBJ )
{

   return(TM_master->OBJcb->nextocb);

}
else if ( index == PSI_OBJ )
{

   return(TM_master->OBJcb);

}

switch( index )
{
                                           /* = = = = = = = = = = = = */
   case   SFI_OBJ   : break;               /* Forms Index             */
   case   SERRI_OBJ : break;               /* Error Index             */
   case   SGI_OBJ   : break;               /* Index of Groups         */
   case   SNI_OBJ   :                      /* Names Index             */
                      if( TM_master->sni_ocb != NIL )
                      {

   return(TM_master->sni_ocb);

                      }
                      break;               /* Names Index             */
   case   SKI_OBJ   : break;               /* Keyword Index           */
   case   SAI_OBJ   : break;               /* Authored Index          */
   case   SDTI_OBJ  : break;               /* Date/Time Index         */
   case   CRI_OBJ   : break;               /* Conference Root Index   */
   case   SHI_OBJ   : break;               /* Help Index              */
   case   SDELI_OBJ : break;               /* Object Deletion Index   */
   case   SOIDI_OBJ : break;               /* System OID Index        */
   case   SMI_OBJ   : break;               /* System Mail Index       */
   default          : ok = FALSE;          /* Not a System Index      */
                                           /* = = = = = = = = = = = = */
}
if( ok )
{

   oid = (OID *) os_mema( sizeof( OID ) );
   (*oid).one    = 0x88;
   (*oid).two    = 0x99;
   (*oid).three  = 0xAA;
   (*oid).four   = 0xBB;
   (*oid).five   = 0xCC;
   (*oid).six    = 0xDD;
   (*oid).seven  = 0xEE;
   (*oid).eight  = 0xFF;
                                             /**************************/
                                             /*                        */
                                             /* Open the PSI, get this */
                                             /* SysNdx's ISV value str */
                                             /* and use that as a key  */
                                             /* to find its entry in   */
                                             /* the PSI.  Then extract */
                                             /* info to open the obj,  */
                                             /* such as LFD, GOID and  */
                                             /* Object Type.           */
   ocb = obj_opn( oid, (hword) 0 );          /**************************/
   if( ! OCB_POINTER_OK( ocb ) )
   {

      except( DATA_BASE_IS_CORRUPT, FATAL_ERROR,
                                 S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);

   }
   ocd = ocd_new( &ocd, "PSI.INDEX.OID" );
   odt = odt_ptr( index );

   ocd->key = obj_name ( index ) ;


   rc  = obj_get( ocb, ocd, oid );

   if( rc != 0 )
   {

      rc = psi_add( index, ocd->key );
      rc  = obj_get( ocb, ocd, oid );

   }
   if( rc == 0 )
   {

      OCB *ocb_sndx = NIL;


      ocb_sndx = ocb_inta( oid, (hword) 0 );
      if( ! OCB_POINTER_OK( ocb_sndx ) )
      {

         s_init( &(ocd->name), "PSI.INDEX.TYPE" );
         rc  = obj_get( ocb, ocd, & type );
         s_init( &(ocd->name), "PSI.INDEX.LFD" );
         rc  = obj_get( ocb, ocd, & lfd );
         s_init( &(ocd->name), "PSI.INDEX.GOID" );
         rc  = obj_get( ocb, ocd, & goid );


         dmscb = dms_opn( lfd, goid, DMSOPNU );

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

               except( rc,INFO, ocd->key, S_NULL,S_NULL,S_NULL,S_NULL) ;

            }
            else if( open_status & (
                            DMS_QUERY_GOID_STATUS_LOCKED_GOID
                          | DMS_QUERY_GOID_STATUS_WRITE_LOCK_SET
                   )      )
            {

               dmscb = dms_opn( lfd, goid, DMSOPNF );
               if( dmscb == NIL )
               {

                  except( rc,INFO,ocd->key,S_NULL,S_NULL,S_NULL,S_NULL);

               }
            }
            else
            {


            }
         }


         if( dmscb != NIL )
         {
                                         /*----------------------------
                                           allocate OCB and fill it in
                                           ----------------------------*/
         ocb            = ocb_all();
         ocb->oid.one   =    oid->one   ; /* Copy oid to the OCB       */
         ocb->oid.two   =    oid->two   ;
         ocb->oid.three =    oid->three ;
         ocb->oid.four  =    oid->four  ;
         ocb->oid.five  =    oid->five  ;
         ocb->oid.six   =    oid->six   ;
         ocb->oid.seven =    oid->seven ;
         ocb->oid.eight =    oid->eight ;
         ocb->lfd       = lfd      ;  /* Logical File Descriptor       */
         ocb->goid      = goid     ;  /* Group Object Identifier       */
         ocb->dmscb     = dmscb    ;  /* Pointer to the DMS control blk*/
         ocb->mode      = OBJ_UPDATE; /* Mode of open                  */
         ocb->flags     = OBJ_UPDATE; /* Mode of open                  */
         ocb->type      = type     ;  /* Type of ODT to use            */
         os_dttm (&ocb->d_t)       ;  /* Current date and time         */
         odt = odt_ptr( index )    ;
         ocb->isvref    = odt_nxto(odt) ;
                                      /*ISV reference to name of object*/
         ocb->odt      = odt      ;   /* Pointer to odt to use         */
         ocb->open_count= 1       ;   /*                               */

         }
         else
         {

            ocb = NIL ;

         }
      }
      else
      {

         ocb = obj_opn( oid, OBJ_UPDATE );

      }
   }
   else
   {

      except(  DATA_BASE_IS_CORRUPT, FATAL_ERROR,
                                 S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
      ocb = NIL;

   }
   if( OCB_POINTER_OK( ocb ) )
   {


      if( index == SNI_OBJ  &&  TM_master->sni_ocb == NIL )
      {

         TM_master->sni_ocb = ocb;

      }
   }
   os_memd( oid  );
   ocd->key = S_NULL;
   ocd_drp( &ocd );

}




   return(ocb);

}
