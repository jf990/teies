/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    PSI_ADD.C                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 9, 1987                                        |
  |  <LEVEL>   OBJ/TM - outside the normal system                     |
  |  <PARAM>   obj_obj     hword     INPUT   Object type number       |
  |  <PARAM>   obj_name    string    INPUT   Object name to put in PSI|
  |  <PURPOSE> Create a system index object and place it in the PSI.  |
  |  <OUTPUT>  Always return 0.                                       |
  |  <CALLS>   os_memc(), dms_obj(), dms_ce(), dms_sd(), dms_sdp()    |
  |            odt_ptr(), dms_cls(), oid_new()                        |
  |            except(),  obj_rep(), obj_get()                        |
  |  <ERRORS>          PSI does not exist.                            |
  |  <LIMITS>  Before this routine is called the ODT's for SOIDI,     |
  |            PSI, and ODT object must be available. Therefore,      |
  |            the TM_master structure must be active.                |
  |            LFD 1 is assumed.                                      |
  |            The DMS segment allocation/formatting is all "hardcode"|
  |            here. If the Object Components of the PSI, SOIDI, ODT  |
  |            ever change this code MUST be updated.                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"





sint   psi_add( obj_obj, obj_name )

hword   obj_obj;
string  obj_name;


{


byte      key_len        =0x80;
OID      *new_oid        = NIL;
sint      obj_hdr_size   =   0,
          goid           =   0,
          psi_hdr_size   =   0,
          rc             =   0;
struct    dms_cb  *dmscb = NIL;
hword     lfd            =   1; /* PSI MUST BE ON LFD ONE             */
string    db_path        = S_NULL,
          dta_string     = S_NULL;
ODT       *obj_odt       = NIL,
          *psi_odt       = NIL;
boolean   already_open   = FALSE;









    key_len += s_len( obj_name );          /* Set the length of the   */
                                           /* Key                     */
    goid  = dms_obj( lfd, goid );          /*                         */
    if( goid == 0 )                        /* If no goid is in the DMS*/
    {                                      /*then we create it here.  */
                                           /*                         */
       psi_odt     = odt_ptr( PSI_OBJ );   /*                         */
       psi_hdr_size= psi_odt->level;       /* Now create the PSI      */
       goid  = dms_ce( lfd,psi_hdr_size);  /*                         */
       rc    = dms_obj(  lfd,  goid  );    /* Save the PSI in dms blk0*/
                                           /*                         */
    }                                      /*                         */
    if( TM_master->OBJcb == NIL )          /*                         */
    {                                      /*                         */
                                           /*                         */
       dmscb = dms_opn(lfd,goid,DMSOPNU);  /*                         */
                                           /*                         */
    }                                      /*                         */
    else                                   /*                         */
    {                                      /*                         */
                                           /*                         */
       dmscb = TM_master->OBJcb->dmscb;    /*                         */
       already_open = TRUE;                /*                         */
                                           /*                         */
    }                                      /*                         */
                                           /* Open the DMS object for */
    if ( dmscb == NIL )                    /*+IF ERROR, check if the  */
    {                                      /* dmscb is already open,  */
                                           /* and if not we cannot    */
       dmscb = obj_dms( lfd, goid );       /* continue.               */
       if( dmscb == NIL )                  /*                         */
       {                                   /*                         */
                                           /*                         */
           rc = dms_qgs( lfd, goid );      /*Find out what status the */
                                           /*                         */
           except( rc, FATAL_ERROR, S_NUL, S_NUL, S_NUL, S_NUL, S_NUL );
                                           /*                         */
       }                                   /*                         */
       already_open = TRUE;                /*Flag that PSI already opn*/
                                           /*                         */
    }                                      /*                         */
    s_alloc(&dta_string, 16  );            /*  Init Data strings      */
    s_alloc(&db_path,    16  );           /*+ Now that the objects   */
                                          /*  are out there we need  */
    s_put( db_path, '\001', 1 );          /*  to fill in the PSI so  */
    s_put( db_path, '\001', 2 );          /*                         */
    s_put( db_path, key_len,3 );          /*  it has the proper ptrs */
    s_put( db_path, '\000', 4 );          /*  to the SOIDI and ODT   */
    s_put( db_path, '\000', 5 );          /*  indexes. Do this by set*/
    s_put( db_path, '\000', 6 );          /*  ing the database paths */
    s_put( db_path, '\000', 7 );          /*  and then calling dms   */
                                          /*  replace. Store under   */
                                          /*  the key "SOIDI"        */
                                          /*                         */
                                          /* - - - - - - - - - - - - */
    os_memc(  s_buffer(obj_name),         /*                         */
              s_buffer(db_path)+7,        /*Format of a PSI record:  */
              s_len(   obj_name)      );  /*  +----+---+---+----+    */
                                          /*  |Type|OID|LFD|GOID|    */
                                          /*  |hwrd|oid|hwd| int|    */
                                          /*  +----+---+---+----+    */
                                          /*                         */
    s_smp_ul(db_path) = s_len( obj_name)  /*+Here we are formatting  */
                            + 7;          /* a data string to contain*/
                                          /* what we are going to    */
                                          /* store in the PSI for the*/
                                          /* SOIDI. Then we will do  */
                                          /* the same for the ODT ndx*/
                                          /* The ODT ndx needs an OID*/
                                          /* but the SOIDI is OID 0. */
                                          /* - - - - - - - - - - - - */
    obj_odt     = odt_ptr( obj_obj );
    if( obj_odt == NIL )
    {

   return(ODT_DOES_NOT_EXIST);

    }
                                            /* - - - - - - - - - - - - */
    obj_hdr_size= obj_odt->level;           /* Now create the object   */
    goid        = dms_ce( lfd,obj_hdr_size);/*  and get its new GOID.  */
                                            /*                         */
                                            /*                         */
                                            /* - - - - - - - - - - - - */
    if( goid != 0 )
    {
                                        /* - - - - - - - - - - - - - - */
                                        /* Get a unique OID for the    */
                                        /* index and store that in the */
                                        /* PSI.                        */
                                        /*                             */
                                        /* - - - - - - - - - - - - - - */
       new_oid = (OID *)
                  os_mema( sizeof( OID ));
       rc      = oid_new ( new_oid );


       os_memc( & (obj_odt->type),  s_buffer( dta_string ),        2 );
       os_memc(   new_oid,          s_buffer( dta_string )+2,      8 );
       os_memc( & lfd,              s_buffer( dta_string )+2+8,    2 );
       os_memc( & goid,             s_buffer( dta_string )+2+8+2,  4 );

       s_smp_ul(dta_string) = 16;

                                                   /* and data. DO IT! */
                                                   /* - - - - - - - - -*/

       rc = dms_sdp ( dmscb, s_buffer( db_path    ), s_len(db_path)    );
       rc = dms_sd  ( dmscb, s_buffer( dta_string ), 16                );

       if( (rc = dms_rpl ( dmscb )) != DMS_REPLACE_DATA_STORED )
       {

   return(rc);

       }                                          /*********************/
       rc = 0;                                    /* Close the PSI only*/
       if( ! already_open )                       /* if we just opened */
       {                                          /* it.               */
                                                  /* All done, clean   */
          dms_cls( dmscb,DMSCLSC );               /* up. Close the GOID*/
                                                  /* and commit it to  */
       }                                          /* the DMS.          */
                                                  /* - - - - - - - - - */
       if( (obj_obj != SOIDI_OBJ) && (obj_obj != PSI_OBJ) )
       {

          rc = soidi_ad( new_oid, lfd, goid, obj_obj );
          if( rc != 0 )
          {


          }
       }
       os_memd(  new_oid    );

    }
    else
    {

       rc = CANNOT_CREATE_OBJECT;
       except( rc, FATAL_ERROR, obj_name, S_NULL, S_NULL, S_NULL, S_NULL );

    }
    s_drop( & dta_string );
    s_drop( & db_path    );

                                                 /* - - - - - - - - - */
                                                 /* End of LFD=1      */
                                                 /* dependent code.   */
   return(rc);

}
