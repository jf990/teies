/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME.    ex_cr                                                  |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *     INPUT   Pointer to Conference    |
  |                                          Object Control Block     |
  |                                          to get components from   |
  |  <PARAM>   index_key   string    INPUT   String ID to find in CRI |
  |  <PARAM>   search_type hword     INPUT   defined in obj_prm1.h    |
  |                                          and could be one of the  |
  |                                          following                |
  |                                                                   |
  |                                          CHK_CONF_ID              |
  |                                          CHK_CONF_LABEL           |
  |                                          CHK_CONF_OID             |
  |  <PARAM>   params     *OID
  |                                                                   |
  |                                  OUTPUT  IF chking ID | LABEL then|
  |                                          then return OID else if  |
  |                                          chking for OID then OID  |
  |  <PURPOSE> Check for existance of a conference root.              |
  |            Depending upon the above mentioned "search_type"       |
  |            this routine checks for the required component         |
  |            (i.e ID, LABEL, OID) passed to it in the conference    |
  |            object control block (conf_ocb) and tries to find      |
  |            a match in the conference root index (CRI_OBJ). On     |
  |            the basis of this comparison it returns the appropriate|
  |            code defined in the obj_errs. h                        |
  |                                                                   |
  |            Modifier is structured to contain the ID and the LABEL |
  |            in the following s_catx() format:                      |
  |                                                                   |
  |             +------------+---------+------------+------------+    |
  |             | S_OVERHEAD | Conf_id | S_OVERHEAD | Conf_label |    |
  |             +------------+---------+------------+------------+    |
  |                                                                   |
  |            When we're ready to optimize we should consider letting|
  |            this routine handle multiple(conf id and labels) items |
  |            in one shot. So we call master server once rather then |
  |            multiple times for each request.                       |
  |  <OUTPUT>  sint                                                   |
  |            0 = NO_ERROR : If key matches                          |
  |            7662 : CONF_ID_NOT_IN_CRI                              |
  |            7664 : CONF_LABEL_NOT_IN_CRI                           |
  |            7666 : CONF_DOES_NOT_EXIST                             |
  |  <CALLS>   t_sysndx(), s_copy(), ocd_new(), obj_get(),            |
  |            os_mema(), os_memd();                                  |
  |  <ERRORS>  20995 : CRI_MISSING_OR_INVALID                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_cr.h"



sint     ex_cr( conf_ocb, modifier, search_type, params )

OCB           * conf_ocb;
string          modifier;
hword           search_type;
OID          ** params;

{

   OCB      * cri_ocb         =         NIL;
   OCD      * conf_ocd        =         NIL;
   OID      * conf_oid        =         NIL;
   string     params_str      =      S_NULL,
              dot_separator   =      S_NULL;
   hword      org_search_type = search_type;
   sint       rc              =    NO_ERROR,
              trc             =    NO_ERROR,
              id_length       =           0;




if( TM_master->usrtype == MASTER_SERVER )
{

conf_oid = (OID *) os_mema( sizeof( OID ) );
os_memn(   conf_oid,        sizeof( OID ) );

cri_ocb = t_sysndx( CRI_OBJ );

while( search_type && (rc == NO_ERROR) )
{

   if( search_type & CHK_CONF_LABEL )
   {

      ocd_new( &conf_ocd, "CRI.LABEL.CONF_ROOT");
      if( org_search_type & CHK_CONF_ID )
      {
                                 /* Label is 2nd string in modifier    */
         s_copy( &(conf_ocd->key), (string) (s_buffer(modifier)
           + s_allen((string) s_buffer(modifier)) + S_OVERHEAD), 0, 0 );

      }
      else
      {

         s_copy( &(conf_ocd->key), modifier, 0, 0 );

      }

      trc = obj_get( cri_ocb, conf_ocd,  conf_oid );

      if( trc != NO_ERROR )
      {

         rc = CONF_LABEL_NOT_IN_CRI;

      }
      else
      {

         rc = CONF_LABEL_ALREADY_IN_CRI;

      }
      search_type &= ~CHK_CONF_LABEL;

   }


   if( search_type & CHK_CONF_ID )
   {

      ocd_new( & conf_ocd, "CRI.ID.CONF_ROOT" );
      if( org_search_type & CHK_CONF_LABEL )
      {
                                 /* ID is 1st string in modifier    */
         s_copy( & conf_ocd->key, (string) s_buffer( modifier ), 0, 0 );

      }
      else
      {

         s_copy( & (conf_ocd->key), modifier, 0, 0 );

      }
                                 /*+------------------------------------+
                                   | We must now check the conference id|
                                   | for the possibility of left padding|
                                   | 0's if they are left off.          |
                                   +------------------------------------+*/
      s_init( & dot_separator, "." );
      id_length = s_first( dot_separator, conf_ocd->key, 0, 0 ) - 1;
      if( id_length < 4  &&  id_length > 0 )
      {

         sint pad_length = 0;

         pad_length = s_len( conf_ocd->key ) + 4 - id_length;
         s_pad( & (conf_ocd->key), conf_ocd->key, pad_length, '0', 'L' );

      }
      s_drop( & dot_separator );

      trc = obj_get( cri_ocb, conf_ocd,  conf_oid );

      if( trc != NO_ERROR )
      {

         if( rc == CONF_LABEL_NOT_IN_CRI )
         {

            rc = CONF_ID_AND_LABEL_NOT_IN_CRI;

         }
         else if( rc == NO_ERROR )
         {

            rc = CONF_ID_NOT_IN_CRI;

         }
      }
      else
      {

         if( rc == CONF_LABEL_ALREADY_IN_CRI )
         {

            rc = CONF_ID_AND_LABEL_ALREADY_IN_CRI;

         }
         else
         {

            rc = CONF_ID_ALREADY_IN_CRI;

         }
      }
      search_type &= ~CHK_CONF_ID;

   }  /* CHK_CONF_ID */
}  /* while */

s_alloc ( & params_str,      sizeof(OID) );
s_smp_ul(   params_str ) =   sizeof(OID)  ;
os_memc(    conf_oid, s_buffer(params_str), sizeof(OID) );

t_csend( t_invoker(), rc, (string) params_str );

s_drop(  & params_str );
os_memd(   conf_oid   );
ocd_drp( & conf_ocd   );

}
else
{

   rc = t_remote( TM_master->MS, CONF_OBJ, EXIST_ACT, search_type,
                                      NIL, modifier, & params_str  );
   if( (params != NIL) )               /* caller wants OID back        */
   {                                   /* he better provide the space! */

      os_memc( s_buffer(params_str), *params, sizeof(OID) );

   }
}

if(       ( rc == CONF_ID_ALREADY_IN_CRI     )
      ||  ( rc == CONF_LABEL_ALREADY_IN_CRI  )
      ||  ( rc == CONF_ALREADY_EXISTS        )    )
{

   rc = 0;

}

   return(rc);

}
