/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     LIST_FUN C                                             |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <AUTHOR>   Kevin Walsh                                            |*/
/*| <DATE>     November 11, 1988                                      |*/
/*| <PARAM>    operation  OCB        IN   What is to be done.         |*/
/*| <PARAM>    list_ocb   OCB        IN   OCB of list to do it to.    |*/
/*| <PARAM>    modifier   string     IN   Passed through to t_jmpst.  |*/
/*| <PARAM>    level      hword      IN             "                 |*/
/*| <PARAM>    params     *byte      OUT  Passed back from t_jmpst.   |*/
/*|                                                                   |*/
/*| <PURPOSE>  Performs the indicated t_jmpstate() action on all      |*/
/*|            elements of list indicated.                            |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "debug.h"

sint list_fun( operation, list_ocb, modifier, level, params )

   hword    operation;                 /* jumpstate function to do     */
   OCB     *list_ocb;                  /* OCB of list to dothis to     */
   string   modifier;                  /* Not used                     */
   hword    level;                     /* Not used                     */
   byte    *params;                    /* Not used                     */
{

   OCD     *list_ocd    =  NIL   ;     /* OCD to manipulate list       */
   sint     rc          = 0      ,     /* Anticipated return code      */
            total_items_in_list  = 0,
            current_item_in_list = 0;
   boolean  more_in_list=  TRUE  ,     /* Indicates if list all shown  */
            do_not_drop_modifier = FALSE;
   OID      oper_oid             ;     /* A list item                  */
   OCB     *oper_ocb             ;     /* OCB of the list item         */
   hword    oper_type            ;     /* Type of list item.           */
   string   TOTAL_ITEMS = S_NULL ,
            CURRENT_ITEM= S_NULL ,
            pass_through_modifier = S_NULL,
            convert     = S_NULL ;





   if ( OCB_POINTER_OK(list_ocb))
   {

      ocd_new(&list_ocd,"LIST.LIST.OID");
      list_ocd->bias = 1;
      s_init( & CURRENT_ITEM, "%CURRENT_ITEM" );
      s_init( & TOTAL_ITEMS,  "%TOTAL_ITEMS"  );


      do /* While there is anything left in list or users wants out: */
      {

         rc = obj_get( list_ocb, list_ocd, & oper_oid );

           /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
            * Set interface variables to keep track of the list size*
            * and where we are currently in the list.  THese are    *
            * standard list processing variables available to forms *
            * and interface coding.                                 *
            * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

         total_items_in_list  = obj_qns( list_ocb, "LIST.LIST" );
         current_item_in_list = obj_pos( list_ocb, list_ocd    );
         s_sitos( & convert,    total_items_in_list            );
         var_set( TOTAL_ITEMS,  convert                        );
         s_sitos( & convert,    current_item_in_list           );
         var_set( CURRENT_ITEM, convert                        );

         if ( rc == 0 )                /* Apparently valid obj found   */
         {

            oper_ocb  = obj_opn(  & oper_oid, OBJ_READ | OBJ_REMOTE );
            oper_type = obj_type( & oper_oid );
            if ( oper_ocb == NIL || oper_type == 0 )
            {


            }
            else
            {

               if( ( oper_ocb->type == NOTIFICATION_OBJ )
                || ( oper_ocb->type ==         MAIL_OBJ ) )
               {

                  s_copy( & pass_through_modifier, list_ocd->key, 0, 0 );

               }
               else if( ( oper_ocb->type == COMMENT_OBJ )
                     && ( operation      == GET_ACT     ) )
               {

                  pass_through_modifier = modifier;
                  do_not_drop_modifier  =     TRUE;

               }
               else
               {

                  s_copy( & pass_through_modifier, modifier, 0, 0 );

               }
               rc = t_jmpst( oper_type, operation, level,
                               pass_through_modifier, oper_ocb, params );
               if ( rc != 0 )
               {
               }
               obj_cls( oper_ocb );
               if( ! do_not_drop_modifier )
               {

                  s_drop( & pass_through_modifier );

               }
            }
         }
         else if ( rc == NO_DATA_FOUND )
         {
            more_in_list = FALSE;
            rc = 0;                    /* Not an error for caller      */
         }
         else
         {
            more_in_list = FALSE;
         }
      }
      while( more_in_list                      &&
             (rc != USER_REQUESTED_QUIT)       &&
             (rc != USER_REQUESTED_ABANDON)    );

      s_sitos( & convert,    0       );
      var_set( TOTAL_ITEMS,  convert );
      var_set( CURRENT_ITEM, convert );
      ocd_drp( & list_ocd     );
      s_drop(  & convert      );
      s_drop(  & CURRENT_ITEM );
      s_drop(  & TOTAL_ITEMS  );

   }
   else
   {
      rc = INVALID_OCB;
   }


   return(rc);

}
