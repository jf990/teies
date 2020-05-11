/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    soidi_ad                                               |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    January 24, 1988 (rewritten from June 17, 1987 version)|
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   *oid        OID      INPUT    Object Identifier        |
  |  <PARAM>    lfd        hword    INPUT    Logical File Descriptor  |
  |  <PARAM>    goid       tint     INPUT    Group Object Identifier  |
  |  <PARAM>    obj_type   hword    INPUT    Object Type              |
  |  <PURPOSE> This routine will add an entry to the SOIDI give the   |
  |            OID, LFD, GOID, and the OBJ_TYPE.                      |
  |  <OUTPUT>  sint                                                   |
  |  <ERRORS>  20092 : there is no SOIDI                              |
  |  <LIMITS>  Should the ODT of type SOIDI_OBJ ever be modified,     |
  |            this routine requires modification as well.            |
  |            This routine places the new Object in the 2nd open     |
  |            object in the system.  If it is not the SOIDI, then too|
  |            bad!! (this to be fixed soon)                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "exc.h"





sint   soidi_ad (oid, lfd, goid, obj_type)

OID   *oid         ;              /* Pointer to 8 byte Object ID       */
hword  lfd         ;              /* Pointer to 2 byte Logical FD      */
tint   goid        ;              /* Pointer to 4 byte Group Object ID */
hword  obj_type    ;              /* Pointer to 2 byte Object Type     */



{


   byte   *segptr  = NULL   ;     /* Pointer to segment retrieved      */
   sint    seglen  = 0      ,     /* Length  of segment retrieved      */
           offset  = 0      ,     /* Data base path offset amount.     */
           rc      = 0      ;     /* GOID for computation & modificatn */
   string  dbpath  = S_NULL ;     /* TEIES string with DB path         */
struct dms_cb *dmscb =  NIL ;     /* Pointer to DMS control block      */
   OCB        *soidi =  NIL ;     /* OCB of the SOIDI                  */



                                  /* Get the Database Control Block    */
                                  /*ASSUMES SOIDI IS THE 2nd OPEN OBJ! */
   if( TM_master->OBJcb == NIL )
   {

      rc = except( SOIDI_IS_NOT_OPEN, FATAL_ERROR,
                                 S_NUL, S_NUL, S_NUL, S_NUL, S_NUL );

   }
   /* soidi = t_sysndx( SOIDI_OBJ ); */
   soidi = TM_master->OBJcb->nextocb;
   if( soidi->type != SOIDI_OBJ )
   {

      rc = except( SOIDI_NOT_SECOND_ALLOCATED_OCB, FATAL_ERROR,
                                 S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
   return(rc);

   }
   dmscb = soidi->dmscb ;

                                  /* Make room for & set len of DBpath */
                                  /* Init to known meaningless value   */
   s_alloc( &dbpath, 28  );
                                  /* Put correct data into the  DBpath */
   s_put (dbpath  , '\001' ,  1) ;/* Root Segment                      */
   s_put (dbpath  , '\001' ,  2) ;/* First hierarchial segment         */
   s_put (dbpath  , '\210' ,  3) ;/* Indexed Segment keylen = 8        */

   os_memc( &(offset), s_buffer(dbpath)+3, 4 );
   os_memc(   oid    , s_buffer(dbpath)+7, sizeof(OID) );
   s_smp_ul( dbpath ) = 3+4+sizeof(OID);

                                  /* Set DB path                       */
   dms_sdp  ( dmscb, s_buffer(dbpath), 15 );
                                  /* Set length and get pointer to seg */
   seglen = 8                ;    /* Data segment length will be 8 byte*/
                                  /* for the 1/2 wrd LFD, 1/2 wrd Type */
                                  /* and 4 byte GOID.                  */

   segptr = os_mema (seglen) ;

   os_memc( & lfd,      segptr,   2 );
   os_memc( & goid,     segptr+2, 4 );
   os_memc( & obj_type, segptr+6, 2 );


   dms_sd( dmscb, segptr, seglen );

   rc = dms_rpl( dmscb );

   s_drop( & dbpath );
   os_memd(  segptr );

   if( rc == DMS_REPLACE_DATA_STORED )
   {

      rc = 0;
      obj_set_mode( soidi, OBJ_SYNC_PENDING );

   }

   return(rc);
}
