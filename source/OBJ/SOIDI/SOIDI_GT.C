/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    soidi_gt                                               |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    June 17, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   *oid        OID      INPUT    Object Identifier        |
  |  <PARAM>   *lfd        hword   OUTPUT    Logical File Descriptor  |
  |  <PARAM>   *goid       tint    OUTPUT    Group Object Identifier  |
  |  <PARAM>   *obj_type   hword   OUTPUT    Object Type              |
  |  <PURPOSE> This routine will get the LFD, GOID, and the TYPE for  |
  |            a give OID from the SOIDI.                             |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_sdp (), dms_find (), dms_qd (), dms_qdl ()         |
  |  <ERRORS>  0 if found                                             |
  |            OID_NOT_IN_SOIDI if not found.                         |
  |  <LIMITS>  Should the ODT of type SOIDI_OBJ ever be modified,     |
  |            this routine requires modification as well.            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"




sint                        soidi_gt (  oid, lfd, goid, obj_type  )


OID   *oid                  ;     /* Pointer to 8 byte Object ID       */
hword *lfd                  ;     /* Pointer to 2 byte Logical FD      */
tint  *goid                 ;     /* Pointer to 4 byte Group Object ID */
hword *obj_type             ;     /* Pointer to 2 byte Object Type     */


{


   hword   soilfd  = 0      ;     /* SOIDI's Logical File Descriptor   */
   sint    rc      = 0      ,     /* SOIDI's Group Object Identifier   */
           soigoid = 0      ,     /* SOIDI's Group Object Identifier   */
           seglen  = 0      ;     /* Length  of segment retrieved      */
   byte   *segptr    =  NIL ;     /* Pointer to segment retrieved      */
struct dms_cb *dmscb =  NIL ;     /* Pointer to DMS control block      */
   string  dbpath  = S_NULL ;     /* TEIES string with DB path         */



if( TM_master->OBJcb->nextocb->type != SOIDI_OBJ )
{

   rc = SOIDI_MISSING_OR_INVALID;
   except( rc, CONTROL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);

}
else
{

   dmscb = ((TM_master->OBJcb)->nextocb)->dmscb ;
                                  /* Make room for & set len of DBpath */
                                  /* Init to known meaningless value   */
   s_init ( & dbpath, "123456789012345") ;
                                  /* Put correct data into the  DBpath */
   s_put (dbpath  , '\001',   1) ;/* Segment 1                         */
   s_put (dbpath  , '\001',   2) ;/* 1st segment off R/S is an index   */
   s_put (dbpath  , '\210',   3) ;/* Indexed Segment keylen = 8        */
   s_put (dbpath  , '\0'  ,   4) ;/* bias is 0 (sint)                  */
   s_put (dbpath  , '\0'  ,   5) ;
   s_put (dbpath  , '\0'  ,   6) ;
   s_put (dbpath  , '\0'  ,   7) ;
   s_put (dbpath,oid->one  ,  8) ;/* Key -- OID                        */
   s_put (dbpath,oid->two  ,  9) ;
   s_put (dbpath,oid->three, 10) ;
   s_put (dbpath,oid->four , 11) ;
   s_put (dbpath,oid->five , 12) ;
   s_put (dbpath,oid->six  , 13) ;
   s_put (dbpath,oid->seven, 14) ;
   s_put (dbpath,oid->eight, 15) ;
                                  /* Set DB path & find requested seg  */
   dms_sdp  (dmscb,s_buffer(dbpath), s_len (dbpath)) ;
   dms_find (dmscb) ;
                                  /* Get address & length of segment   */
   segptr = dms_qd  (dmscb) ;
   seglen = dms_qdl (dmscb) ;
   if ( segptr == NIL || seglen == 0 )
   {

      rc = OID_NOT_IN_SOIDI;

   }
   else
   {

                                     /* Get LFD, GOID, and OBJ_TYPE    */
      *lfd      = *((hword *)(segptr +  0)) ;
      *goid     = *((sint  *)(segptr +  2)) ;
      *obj_type = *((hword *)(segptr +  6)) ;

   }
   s_drop( & dbpath );

}


   return(rc);
}
