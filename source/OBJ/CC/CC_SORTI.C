/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cc_sortid                                              |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    August 7, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conference  OCB *      INPUT  description              |
  |  <PARAM>   level       hword      INPUT  description              |
  |  <PARAM>   array       sint *     INPUT  description              |
  |  <PARAM>   array_size  sint       INPUT  description              |
  |  <PARAM>   list_object OCB **    OUTPUT  description              |
  |  <PURPOSE> Sort by comment id's for a given conference given their|
  |             respective sequence numbers.                          |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_cc.h"
#include "obj_list.h"


sint  cc_sortid( conference, level, array, array_size, list_object )

OCB            * conference,                        ** list_object;
hword                        level;
sint                              * array, array_size;

{

   sint      rc       = NO_ERROR,
           * seqnum   = NIL,
             i        = 0;
   OCD     * list_ocd = NIL,
           * conf_ocd = NIL;
   string    list_ocd_number = S_NULL,
             list_ocd_oid    = S_NULL,
             conf_ocd_id     = S_NULL,
             conf_ocd_oid    = S_NULL;
   OID       comment_oid             ;



if( array == NIL )
{

   rc = INVALID_DATA_POINTER;

}
else if( array_size > 0 )
{

   if( (list_object != NIL) && (NOT OCB_POINTER_OK( * list_object ))  )
   {

      * list_object = obj_all( LIST_OBJ, LOCAL_OBJECT, NIL );

   }

   ocd_new( & list_ocd,        NIL                     );
   s_init(  & list_ocd_number, "LIST.LIST.NUMBER"      );
   s_init(  & list_ocd_oid,    "LIST.LIST.OID"         );
   ocd_new( & conf_ocd,        NIL                     );
   s_init(  & conf_ocd_id,     "CONFERENCE.ITEM_SN.ID" );
   s_init(  & conf_ocd_oid,    "CONFERENCE.ITEM_SN.OID");
   s_alloc( & conf_ocd->key,    sizeof(sint)           );
   s_smp_ul(  conf_ocd->key ) = sizeof(sint)            ;
   seqnum = (sint *)           s_buffer( conf_ocd->key );
   os_memn( & comment_oid,     sizeof( OID )           );

   for( i = 0; i < array_size; i++ )
   {

      * seqnum = array[i];
      conf_ocd->name = conf_ocd_id;
      obj_get(   conference,  conf_ocd, & (list_ocd->key)  );
      conf_ocd->name = conf_ocd_oid;
      obj_get(   conference,  conf_ocd, & comment_oid      );
      list_ocd->name = list_ocd_number;
      obj_rep( * list_object, list_ocd,   seqnum,      SEG_HOLD   );
      list_ocd->name = list_ocd_oid;
      obj_rep( * list_object, list_ocd, & comment_oid, SEG_UPDATE );

   }
   s_drop(  & list_ocd_number );
   s_drop(  & list_ocd_oid    );
   s_drop(  & conf_ocd_id     );
   s_drop(  & conf_ocd_oid    );
   conf_ocd->name = S_NULL;
   ocd_drp( & conf_ocd );
   list_ocd->name = S_NULL;
   ocd_drp( & list_ocd );

}

   return(rc);
}
