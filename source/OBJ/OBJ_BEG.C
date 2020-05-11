/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_beg                                                |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    January 06, 1988 (slight re-write from July87 version) |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master   TMCB *   IN/OUT                            |
  |  <GLOBAL>  odt_t_r     ODT_T_R  IN/OUT                            |
  |                                                                   |
  |  <PARAM>   lfd         hword    INPUT    the LFD that contains    |
  |                                          the PSI                  |
  |                                                                   |
  |  <PURPOSE> The purpose of obj_beg is to do the initialization     |
  |            that is necessary to use the object level.  This       |
  |            includes loading ODT's and opening the PSI and the     |
  |            SOIDI.                                                 |
  |                                                                   |
  |            This routine also defines, allocates and fills in the  |
  |            Object-Action Matrix Table (see obj_prim.h & obj_act.h)|
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_beg (), os_mema (), odt_load (), odt_ptr (),       |
  |            dms_obj ()                                             |
  |  <ERRORS>  20997 : TEIES database LFD 1 does not exist.           |
  |            20998 : SOIDI is not in the PSI.                       |
  |            20999 : Cannot create a TEIES data base.               |
  |            22pxx : DMS error xx; p is a code position indicator   |
  |                    within this routine.                           |
  |                                                                   |
  |  <LIMITS>  Some portions are currently scaffolded.                |
  |                                                                   |
  |            This routine insures the PSI is the 1st open object &  |
  |            the SOIDI is the 2nd.  None of the object level code   |
  |            will work unless this is the case.                     |
  |  <MODS>                                                           |
  |            Jan 23 '89      JF       Changed PSI OID to new value. |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"


extern void printf();

#define LFD_ONE                1

struct ODT_T_R odt_t_r [ODT_TYPES+1];




sint obj_beg (lfd)

hword lfd                       ; /* Lfd that contains the PSI         */



{



   OCD        *ocdptr  =   NULL ; /* Ptr to OCD for component extract  */
   OCB        *ocb_psi =   NIL  , /* Pointer to the OCB of the PSI     */
              *ocb_soid=   NULL ; /* Pointer to the OCB of the PSI     */
   OID        *soid    =   NULL ; /* OID  of the SOIDI                 */
   ODT        *odt_table=  NIL  ; /* ODT pointer holder for table load.*/
   struct dms_cb *dmscb=   NULL ; /* Pointer to the DMS Control Block  */
   string      dbp     = S_NULL ; /* DBpath to a segment in the PSI    */
   hword       slfd    =   0    ; /* LFD  of the SOIDI                 */
   byte       *segptr  =   NULL ; /* Pointer to segment retrieved      */
   boolean     init_sys=   FALSE; /* If new LFD1, this is true         */
   sint        rc      =   0    ,
               sgoid   =   0    , /* GOID of the SOIDI                 */
               seglen  =   0    , /* Length  of segment retrieved      */
               goid    =   0    , /* GOID of the PSI                   */
               count   =   0    ; /* Counter for for-next loop         */




   obj_ini();
                                  /* Allocate memory for the ODT list  */

   TM_master->odt = (struct odtlist *)
                      os_mema( sizeof( struct odtlist )*NUMBER_OF_ODTs );


                                              /* Null out the odtLIST  */

   for (count = 0 ; count <= NUMBER_OF_ODTs - 1 ; count ++)
   {
      (TM_master->odt+count)->odt_ptr  = NIL  ;
      (TM_master->odt+count)->obj_type = 0    ;
   }
                                  /*  - - - - - - - - - - - - - - - -  */
                                  /* Now load the system ODT's         */
                                  /*  - - - - - - - - - - - - - - - -  */

   odt_table = odt_load( PSI_OBJ );
   if( !  ODT_POINTER_OK( odt_table )  )
   {

      except( OBJ_NECESSARY_ODT_NOT_FOUND, FATAL_ERROR,
                           S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

   }

   (TM_master->odt+ 0)->odt_ptr  = odt_table;
   (TM_master->odt+ 0)->obj_type =              PSI_OBJ  ;

   odt_table = odt_load( SOIDI_OBJ );
   if( ! ODT_POINTER_OK( odt_table ) )
   {

      except( OBJ_NECESSARY_ODT_NOT_FOUND, FATAL_ERROR,
                           S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

   }

   if ( (TM_master->odt + 1)->odt_ptr != NIL )
   {

      os_memd( (TM_master->odt + 1)->odt_ptr );

   }
   (TM_master->odt+ 1)->odt_ptr  = odt_table ;
   (TM_master->odt+ 1)->obj_type =            SOIDI_OBJ  ;

   if ( (TM_master->odt + 2)->odt_ptr != NIL )
   {

      os_memd( (TM_master->odt + 2)->odt_ptr );

   }
   (TM_master->odt+ 2)->odt_ptr  = odt_load(  SERRI_OBJ) ;
   (TM_master->odt+ 2)->obj_type =            SERRI_OBJ  ;

   odt_table = odt_load( USER_OBJ );
   if( ! ODT_POINTER_OK( odt_table ) )
   {

      except( OBJ_NECESSARY_ODT_NOT_FOUND, FATAL_ERROR,
                           S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

   }

   if ( (TM_master->odt + 3)->odt_ptr != NIL )
   {

      os_memd( (TM_master->odt + 3)->odt_ptr );

   }
   (TM_master->odt+ 3)->odt_ptr  = odt_table;
   (TM_master->odt+ 3)->obj_type =             USER_OBJ  ;


                                  /*************************************/
                                  /* Initialize the DMS and attempt to */
                                  /* get the GOID of the PSI which the */
                                  /* DMS has stored for us. If 0, then */
                                  /* the DB probably does not exist yet*/
                                  /* and we must create it.            */
                                  /*                                   */
                                  /*************************************/


   dms_beg ( 0, 0 );

   goid      = dms_obj (lfd, 0) ;
   if( goid != 0 )
   {

       dmscb = dms_opn (lfd, goid, DMSOPNU) ;

   }
                                  /*************************************/
                                  /* If dms_opn returns 0 then there is*/
                                  /* no data base, or the DMS crashed  */
                                  /* last time this user was on. First */
                                  /* determine if we can recover the   */
                                  /* PSI GOID. If not, kill this DB (if*/
                                  /* it was a real one) and create a   */
                                  /* new one.                          */
                                  /*************************************/

   if( dmscb == NIL  )
   {

      rc = dms_qgs( lfd, goid );
      if(   rc  &
                  (DMSQGSU | DMSQGSE | DMSQGSN)  )
      {

         sint lfd_one_blk_size = USER_LFD_SIZE;


         if( TM_master->sys_init->db_size >= OBJ_MINIMUM_LFD_SIZE   &&
             TM_master->sys_init->db_size <= OBJ_MAXIMUM_LFD_SIZE )
         {

            lfd_one_blk_size = TM_master->sys_init->db_size;

         }
         else
         {

            if( TM_master->usrtype != REAL_USER )
            {

               lfd_one_blk_size = MASTERSERVER_LFD_SIZE;

            }
         }
         rc = obj_fmt( LFD_ONE, lfd_one_blk_size );
         if( rc != 0 )
         {

            except( rc, FATAL_ERROR, TM_master->sys_init->lfd_name,
                          S_NULL, S_NULL, S_NULL, S_NULL  );

         }
         else
         {

            init_sys = TRUE;
            goid  = dms_obj (lfd, 0) ;
            dmscb = dms_opn (lfd, goid, DMSOPNU) ;
                                                 /*====================*/
         }                                       /*If the DMS gives a  */
      }                                          /*return code indica- */
      else                                       /*ting the PSI is     */
      {                                          /*locked, force it opn*/
                                                 /*otherwise we cannot */
                                                 /*go any further unles*/
                                                 /*we delete the DB.   */
                                                 /*====================*/

         if( rc & (DMSQGSW | DMSQGSL ))
         {

            dmscb = dms_opn (lfd, goid, DMSOPNF) ;

         }
      }
   }
   if( dmscb == NIL )
   {

      except( PSI_IS_LOCKED, FATAL_ERROR,
                                     S_NUL, S_NUL, S_NUL, S_NUL, S_NUL );

   }                              /*************************************/
                                  /* Allocate space for the first OCB  */
   ocb_psi = ocb_all();           /* and create the OCB table.         */
                                  /*                                   */
                                  /*                                   */
   ocb_psi->lfd      = lfd      ; /* Logical File Descriptor           */
   ocb_psi->goid     = goid     ; /* Group Object Identifier           */
   ocb_psi->dmscb    = dmscb    ; /* Pointer to the DMS control block  */
   ocb_psi->mode     = OBJ_UPDATE;/* Mode of open                      */
   ocb_psi->type     = PSI_OBJ  ; /* Type of odt to use                */
   os_dttm (&ocb_psi->d_t)      ; /* Current date and time             */
                                  /* Pointer to odt to use             */
   ocb_psi->odt      = odt_ptr (PSI_OBJ) ;
   ocb_psi->isvref   = odt_nxto(ocb_psi->odt) ;
   ocb_psi->open_count= 1 ;

   (ocb_psi->oid).one    = 0x88;
   (ocb_psi->oid).two    = 0x99;
   (ocb_psi->oid).three  = 0xAA;
   (ocb_psi->oid).four   = 0xBB;
   (ocb_psi->oid).five   = 0xCC;
   (ocb_psi->oid).six    = 0xDD;
   (ocb_psi->oid).seven  = 0xEE;
   (ocb_psi->oid).eight  = 0xFF;

                                  /*************************************/
                                  /*Make DBpath to fetch SOIDI from PSI*/
                                  /*************************************/
                                  /* Initialize string with a known &  */
                                  /*   Meaningless value               */
                                  /* Put correct data into the  DBpath */
                                  /* Segment 001                       */
                                  /* Segment 1 off R/S is an index     */
                                  /* Indexed segment keylen = 5        */
                                  /* bias is 0 (sint)                  */

   s_init ( & dbp, "123456789012");

   s_put (dbp     , '\001',  1)  ;
   s_put (dbp     , '\001',  2)  ;
   s_put (dbp     , '\205',  3)  ;
   s_put (dbp     , '\0'  ,  4)  ;
   s_put (dbp     , '\0'  ,  5)  ;
   s_put (dbp     , '\0'  ,  6)  ;
   s_put (dbp     , '\0'  ,  7)  ;
   s_put (dbp     , 'S'   ,  8)  ;/* Key string                        */
   s_put (dbp     , 'O'   ,  9)  ;
   s_put (dbp     , 'I'   , 10)  ;
   s_put (dbp     , 'D'   , 11)  ;
   s_put (dbp     , 'I'   , 12)  ;
                                  /* Set DB path & find requested seg  */
   dms_sdp  (dmscb,s_buffer(dbp), s_len (dbp)) ;
   dms_find (dmscb) ;
                                  /* Get address & length of segment   */
   segptr = dms_qd  (dmscb) ;
   seglen = dms_qdl (dmscb) ;


   if( segptr == NIL || seglen == 0 )
   {

       except( SOIDI_NOT_IN_PSI, FATAL_ERROR, S_NULL, S_NULL, S_NULL,
                                                      S_NULL, S_NULL );

   }
   s_drop( &dbp  );               /*  - - - - - - - - - - - - - - - -  */
                                  /* SCAFFOLD: Create the OCB and fill */
                                  /*  in all we know so far so we can  */
                                  /*  call odt_FIND. Then locate the   */
                                  /*  OCDs to get out the OID info from*/
                                  /*  the dms_find() we just did.      */
                                  /*  - - - - - - - - - - - - - - - -  */
                                  /* Allocate space for the OCB of the */
                                  /* SOIDI and link it off the PSI ocb.*/
                                  /*   - - - - - - - - - - - - - - -   */
                                  /*OCB_ALL() should create the 2nd OCB*/
   ocb_soid = ocb_all();          /*and place it in the table.         */
                                   /* - - - - - - - - - - - - - - - -  */
                                   /* Open the SOIDI and fill in OCB   */
   ocb_soid->type     = SOIDI_OBJ; /* Type of odt to use               */
   os_dttm (&ocb_soid->d_t)      ; /* Current date and time            */
   ocb_soid->odt      = odt_ptr (SOIDI_OBJ) ;
                                   /* ISV reference to name of object  */
   ocb_soid->isvref   = odt_nxto(ocb_soid->odt) ;


                                  /*  - - - - - - - - - - - - - - - -  */
                                  /* Get OID, LFD and GOID of SOIDI    */
                                  /* soid  =   (byte  *)(segptr +  2); */
                                  /* slfd  = *((hword *)(segptr + 10));*/
                                  /* sgoid = *((tint  *)(segptr + 12));*/
                                  /*  - - - - - - - - - - - - - - - -  */


   soid         = (OID *) os_mema ( sizeof( OID ));
   ocdptr       = ocd_new( &ocdptr, "PSI.INDEX.OID" );
   s_init( & (ocdptr->key),"SOIDI" );

   obj_get( ocb_psi , ocdptr,   soid );

   s_init( &ocdptr->name, "PSI.INDEX.LFD"   );
   obj_get( ocb_psi , ocdptr, & slfd );

   s_init( &ocdptr->name, "PSI.INDEX.GOID"   );
   obj_get( ocb_psi , ocdptr, & sgoid );



   dmscb = dms_opn (slfd, sgoid, DMSOPNU) ;
                                  /*************************************/
   if( dmscb == NIL  )            /* If dms_opn returns 0 then there is*/
   {                              /* a problem with the SOIDI.  We must*/
                                  /* correct this problem in order to  */
      rc = dms_qgs( slfd, sgoid );/* move on.                          */
      if( rc == DMSQGSU  ||       /*                                   */
          rc == DMSQGSN  ||       /*                                   */
          rc == DMSQGSE )         /* If the SOIDI is locked from a     */
      {                           /* prior crash, try to clear the lock*/
                                  /*************************************/
         except( CANNOT_CREATE_LFD_1,  FATAL_ERROR,
                                    S_NUL, S_NUL, S_NUL, S_NUL, S_NUL );

      }
      else
      {

         dmscb = dms_opn (slfd, sgoid, DMSOPNF) ;
         if( dmscb == NIL  )
         {

            printf(  "\nThe SOIDI is locked and cannot be opened.\n\n" );
            except( SOIDI_IS_LOCKED, FATAL_ERROR,
                                     S_NUL, S_NUL, S_NUL, S_NUL, S_NUL );

         }
      }
   }

                                  /* Copy OID to the OCB               */

   ocb_soid->oid.one   = soid->one   ;
   ocb_soid->oid.two   = soid->two   ;
   ocb_soid->oid.three = soid->three ;
   ocb_soid->oid.four  = soid->four  ;
   ocb_soid->oid.five  = soid->five  ;
   ocb_soid->oid.six   = soid->six   ;
   ocb_soid->oid.seven = soid->seven ;
   ocb_soid->oid.eight = soid->eight ;
   ocb_soid->dmscb     = dmscb    ; /* Pointer to the DMS control block*/
   ocb_soid->mode      = OBJ_UPDATE;/* Mode of open                    */

   ocb_soid->lfd      = slfd     ; /* Logical File Descriptor          */
   ocb_soid->goid     = sgoid    ; /* Group Object Identifier          */
   ocb_soid->open_count=    1    ; /* Set open counter                 */


   if( init_sys && (TM_master->usrtype == MASTER_SERVER) )
   {

      cr_sys();

   }
                                   /* Done - cleanup and return        */
   os_memd( soid    );
   ocd_drp( &ocdptr );


   return(0);
}
