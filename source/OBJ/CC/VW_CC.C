/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    vw_cc                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 27, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INPUT                           |
  |  <PARAM>   modifier    string     INPUT                           |
  |  <PARAM>   flags       hword      INPUT                           |
  |  <PARAM>   memb_ocb    OCB *      INPUT                           |
  |  <PURPOSE> View    Conference Comment                             |
  |   Modifier has too meanings...1st the normal modifier string      |
  |   other one is  list_ocb which was created when the user          |
  |   display NEW waiting cc's in GT_CCID                             |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "obj_cr.h"
#include "obj_cc.h"
#include "obj_memb"
#include "obj_list"
#include "tm.h"



sint vw_cc(  conf_ocb, modifier, flags, memb_ocb  )

OCB        * conf_ocb, * memb_ocb;
string       modifier;
hword        flags;

{

sint      rc          = NO_ERROR,
        * seqnum      =      NIL,
        * seqnum_arry =      NIL,
          arry_size   =        0,
          count       =        0,
          junk        =        0,
          tot_itm     =        0,
          bad_itm_num =        0,
          las_itm_num =        0;
datetime  last_active           ;
OCB     * item_ocb    =      NIL,
        * user_ocb    =      NIL,
        * list_ocb    =      NIL,
        * cc_ocb      =      NIL;
OID       item_oid               ,
        * oid_ptr     = &item_oid,
          memb_oid               ,
          junk_oid               ,
          cc_oid                 ;
OCD     * user_ocd    =       NIL,
        * seqnum_ocd  =       NIL,
        * list_ocd    =       NIL,
        * conf_ocd    =       NIL;
boolean   item_ocb_opened = FALSE,
          conf_ocb_opened = FALSE,
          need_to_close_cc_list = FALSE,
          memb_ocb_opened = FALSE;
string    conf_id         = S_NULL,
          str_seqnum      = S_NULL,
          bad_itm_str     = S_NULL,
          total_itm       = S_NULL,
          TOTAL_ITEMS     = S_NULL,
          ITEM_NUM        = S_NULL,
          ITEM_NUMBER     = S_NULL,
          itm_num         = S_NULL,
          cc_id           = S_NULL;



if( ! OCB_POINTER_OK( conf_ocb ) )
{

   if( s_len(modifier) < 1 )
   {

      rc = vw_cr( NIL, modifier, flags, NIL );

   }
   else
   {

      rc = obj_item( modifier, OBJ_NEED_ONE_OID, & oid_ptr, NIL,
                                    & bad_itm_str, & bad_itm_num, junk );
      item_ocb = obj_opn( oid_ptr, OBJ_UPDATE | OBJ_REMOTE );
      if( OCB_POINTER_OK( item_ocb ) )
      {

         item_ocb_opened = TRUE;
         if( item_ocb->type == CONF_OBJ )
         {

            rc = vw_cr( item_ocb, modifier, flags, NIL );

         }
         else if( item_ocb->type == COMMENT_OBJ )
         {

            cc_ocb = item_ocb;
            obj_find( cc_ocb, "%LINKS.OWNER", & item_oid );
            conf_ocb = obj_opn( & item_oid, OBJ_UPDATE | OBJ_REMOTE );

            if( OCB_POINTER_OK( conf_ocb ) )
            {

               conf_ocb_opened =            TRUE;
               flags          |= VIEWING_COMMENT;
               rc = vw_cr( conf_ocb, modifier, flags, (byte *) cc_ocb );

            }
         }
         else
         {

            rc = IMPROPER_OBJECT_TYPE;

         }
      }
      else
      {

         rc = Item_Does_Not_Exist;

      }
   }
   if( conf_ocb_opened )
   {

      obj_set_flag( conf_ocb, OBJ_DELETE );
      obj_cls(      conf_ocb             );

   }
   if( item_ocb_opened )
   {

      obj_set_flag( item_ocb, OBJ_DELETE );
      obj_cls(      item_ocb             );

   }
}
else
{

   ocd_new( & conf_ocd, "CONFERENCE.STATUS.LAST_ITEM_SEQNUM" );

   if( modifier == NIL )
   {

      obj_get(  conf_ocb, conf_ocd,    & las_itm_num  );

      rc = memb_gt( memb_ocb, S_NIL, flags, 0, las_itm_num,
                                            & seqnum_arry, & arry_size );

      rc = cc_sortid(conf_ocb,flags, seqnum_arry, arry_size, &list_ocb );
      need_to_close_cc_list = TRUE;

   }
   else
   {

      list_ocb = (OCB *) modifier;

   }

   if( OCB_POINTER_OK( list_ocb ) )
   {

      rc = gt_list(   list_ocb, (string) conf_ocb, flags, memb_ocb   );
      if( need_to_close_cc_list )
      {

         obj_set_mode( list_ocb, OBJ_DELETE );
         obj_cls(      list_ocb             );
         need_to_close_cc_list =        FALSE;

      }
   }
   else
   {

      rc = NO_COMMENTS_TO_LIST;

   }
   ocd_drp( & conf_ocd );
   ocd_drp( & list_ocd );

}

   return(rc);

}
