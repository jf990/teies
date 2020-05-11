/************************************************************************
     <DOC>
     <NAME>    obj_item()
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    February 13, 1989
     <LEVEL>   OBJ
     <PARAM>   modifier    string     INPUT  String with list to
                                                             convert.
     <PARAM>   level       hword      INPUT  Shows if there is one or
                                             or more items.
     <PARAM>   oid         OID **     INOUT  Where to store the result
                                             if there is one item.
     <PARAM>   list_ocb    OCB **     INOUT  Where to store the result
                                             if there is more than one
     <PARAM>   bad_items   string*    OUTPUT stores string of bad items
                                             seperated by semicolons.
     <PARAM>   num_bad     sint*      OUTPUT stores # of bad items in
                                             string bad_items.
     <PARAM>   type        hword      INPUT  If there is an abiguity in
                                             determining the type of
                                             object in the list this type
                                             is used. If no default is
                                             used it is ANY_OBJ. The
                                             object types are defined in
                                             obj_comm.h

     <PURPOSE> Convert a string with one or more items separated by
               commas to the OID(s) of the item.

     <OUTPUT>  sint                                       0 if successful
               error code depending on the error.
     <LIMITS>  If list_ocb or oid are allocated in this routine then they
               must be deallocated by the caller.
     <MODS>    Apr 18, 1990        JF        Corrected privilege checks &
                                              user index lookup logic to
                                              new user object format.
     </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"
#include "obj_prim.h"

#define MAX_NUMBER_OF_ITEM_IDS              40
#define MAX_NUMBER_OF_ITEM_COMPONENTS        5
#define OBJ_ITEM_LIST_KEY_SIZE               4



sint obj_item( modifier, level, oid, list_ocb, bad_items, num_bad, type )

string              modifier;
hword               level;
OID              ** oid;
OCB              ** list_ocb;
string            * bad_items;
sint              * num_bad;
hword               type;

{

 hword      search_type     =     CHK_CONF_ID,
            mail_type       =               0;
 sint       rc              =               0,
            list_size       =               0,
            item_counter    =               0;
 string     key             =          S_NULL,
            mail_key        =          S_NULL,
            item            =          S_NULL,
            parser          =          S_NULL,
            t_parser        =          S_NULL,
            semi_colon      =          S_NULL,
            extra           =          S_NULL,
          * item_ids        =             NIL,
          * item_components =             NIL,
            prefix          =          S_NULL,
            n_prefix        =          S_NULL,
            m_prefix        =          S_NULL,
            c_prefix        =          S_NULL,
            cc_prefix       =          S_NULL,
            g_prefix        =          S_NULL,
            u_prefix        =          S_NULL,
            t_prefix        =          S_NULL,
            item_prefix     =          S_NULL;
 OID      * item_oid        =             NIL;
 OCB      * user_ocb        =             NIL,
          * item_ocb        =             NIL;
 OCD      * item_ocd        =             NIL;







if ( s_len( modifier ) < 1 )
{

   rc = NOTHING_TO_PROCESS;

}
else if ( (( level == OBJ_NEED_MANY_OID ) && ( list_ocb == NIL) )
       || (( level == OBJ_NEED_ONE_OID  ) && ( oid      == NIL) ) )
{

   rc = CANNOT_STORE_RESULT_OID;

}
else
{

   if (  level == OBJ_NEED_MANY_OID  )
   {


      if ( * list_ocb == NIL )
      {

         * list_ocb = obj_all( LIST_OBJ, LOCAL_OBJECT, NIL );

      }
      s_valloc( & item_ids,        MAX_NUMBER_OF_ITEM_IDS        );
      s_valloc( & item_components, MAX_NUMBER_OF_ITEM_COMPONENTS );

   }
   else if (  level == OBJ_NEED_ONE_OID  )
   {


      if ( * oid == NIL )
      {

         * oid = (OID *) os_mema ( sizeof( OID ) );
         os_memn( * oid,           sizeof( OID ) );

      }
      s_valloc( & item_ids,        4                             );
      s_valloc( & item_components, MAX_NUMBER_OF_ITEM_COMPONENTS );

   }
   else
   {

      rc = CANNOT_STORE_RESULT_OID;

   }

   if (  rc == 0  )
   {

      if( num_bad != NIL )
      {

         * num_bad = 0;

      }
      s_init(  & semi_colon, ";"          );
      s_init(  & parser,     "@~.. @n ,." );
      s_upper(   modifier,           0, 0 );

      s_parse(  & modifier, parser, S_NULL, item_ids, & extra );

      if  ( s_len ( (string) item_ids ) > 0 )
      {

         item_oid = (OID *) os_mema( sizeof( OID ) );

         isv_get( & n_prefix,   ISV_NOTIF,            1 );
         isv_get( & m_prefix,   ISV_MMSG,             1 );
         isv_get( & c_prefix,   ISV_CONF,             1 );
         isv_get( & cc_prefix,  ISV_CC,               2 );
         isv_get( & g_prefix,   ISV_GROUP,            1 );
         isv_get( & u_prefix,   ISV_USER,             1 );
         isv_get( & t_prefix,   ISV_TICKET,           1 );
         s_init(  & parser,     " @$n @n@?:"            );
         s_cat(   & parser,     parser, cc_prefix, 0, 0 );
         s_init(  & t_parser,   ": @n : "               );
         s_cat(   & parser,     parser, t_parser,  0, 0 );
         ocd_new( & item_ocd,   NIL                     );

         for (  item_counter  = 1 ;
                item_counter <= s_len( (string) item_ids );
                item_counter ++  )
         {

            os_memn(  item_oid, sizeof (OID) );
            s_copy( & item,     * (item_ids + item_counter), 0, 0);

            s_parse( &item, parser, S_NULL, item_components, & extra );

            if (  s_len( (string) item_components) >= 2  )
            {

               s_copy( & prefix,  *(item_components + 1), 0, 0 );

               if ( s_len( prefix ) == 0 )
               {

                  switch (  type  )
                  {

                     case MAIL_OBJ:

                               item_prefix = m_prefix;
                               break;

                     case NTF_OBJ:

                               item_prefix = n_prefix;
                               break;

                     case CR_OBJ:

                               item_prefix = c_prefix;
                               break;

                     case CC_OBJ:

                               item_prefix = cc_prefix;
                               break;

                     case USER_OBJ  :

                               item_prefix = u_prefix;
                               break;

                     case GROUP_OBJ :

                               item_prefix = g_prefix;
                               break;

                     case TICKET_OBJ:

                               item_prefix = t_prefix;
                               break;

                     default:

                               item_prefix = S_NULL;
                               break;

                  }
                  if( item_prefix != S_NULL )
                  {

                     s_copy( & prefix, item_prefix, 0, 0 );

                  }
               }

                                          /*---------------------------*
                                           | Check for Conference Root |
                                           | and Conference Comment    |
                                           *---------------------------*/
               if( s_len( prefix ) > 0 )
               {

               if ( ( s_comp( prefix, c_prefix, 0, 0, 'M', NIL ) == 0 ) )
               {

                  s_copy( & key, * (item_components + 2), 0, 0 );

                  rc = ex_cr( item_ocb, key, search_type, & item_oid );

                  if (  ( rc == 0 )   ||  CR_CONFERENCE_EXISTS( rc ) )
                  {

                     if( s_len( (string) item_components ) == 3 )
                     {

                        s_copy( & key, * ( item_components + 3 ), 0, 0 );

                        rc = ex_cc( item_ocb, key, (hword) 0, & item_oid );

                        if (  rc != 0  )
                        {

                           rc = NO_DATA_FOUND;

                        }
                     }
                     else
                     {


                     }
                  }
                  else
                  {

                     rc = NO_DATA_FOUND;

                  }
               }
                                          /*---------------------------*
                                           | Check for Mail message    |
                                           *---------------------------*/

               else if( ( s_comp( prefix, m_prefix, 0, 0, 'M', NIL ) == 0 ) )
               {

                  s_init( & item_ocd->name, "SMI.MESSAGE.OID" );
                  s_copy( & item_ocd->key, * ( item_components + 2 ), 0, 0 );
                  rc = t_fndsys( SMI_OBJ, item_ocd, item_oid );
                  if( rc != 0 )
                  {

                     rc = NO_DATA_FOUND;

                  }
               }
                                          /*---------------------------*
                                           | Check for Notification ID |
                                           *---------------------------*/

               else if( ( s_comp( prefix, n_prefix, 0, 0, 'M', NIL ) == 0 ) )
               {

                  s_init(  & item_ocd->name, "INDEXES.OLD_NOTIF" );
                  s_copy(  & item_ocd->key,  *(item_components + 2), 0, 0 );
                  s_pad(   & item_ocd->key,  item_ocd->key,
                                               NTF_KEY_LENGTH, ' ', 'L');
                  user_ocb = t_author();
                  rc = obj_get( user_ocb, item_ocd, item_oid );
                  if (rc != 0)
                  {

                     s_init(&item_ocd->name,"INDEXES.NOTIFICATIONS.OID");
                     rc = obj_get( user_ocb, item_ocd, item_oid );
                     if (rc != 0)
                     {

                        rc = NO_DATA_FOUND;
                     }
                  }
               }
                                          /*---------------------------*
                                           | Check for Group of users  |
                                           *---------------------------*/

               else if ((s_comp( prefix, g_prefix, 0, 0, 'M', NIL) == 0))
               {

                  s_copy( & key, * ( item_components + 2 ), 0, 0 );
                  rc = NO_DATA_FOUND;

               }
                                          /*---------------------------*
                                           | Check for User            |
                                           *---------------------------*/

               else if ((s_comp( prefix, u_prefix, 0, 0, 'M', NIL) == 0))
               {

                  s_copy(  & key, * ( item_components + 2 ), 0, 0 );
                  rc = NO_DATA_FOUND;

               }
                                          /*---------------------------*
                                           | Check for Ticket          |
                                           *---------------------------*/

               else if ((s_comp( prefix, t_prefix, 0, 0, 'M', NIL) == 0))
               {

                  s_copy( & key, * ( item_components + 2 ), 0, 0 );
                  rc = NO_DATA_FOUND;

               }
               else
               {

                  rc = NO_DATA_FOUND;

               }
               }
            }
            else
            {

               rc = NO_DATA_FOUND;

            }
            if  (level == OBJ_NEED_ONE_OID)
            {

               os_memc( item_oid, * oid, sizeof( OID ) );

            }
            if (level == OBJ_NEED_MANY_OID)
            {

               string     list_key = S_NULL;
               sint       item_num =      0;


               s_alloc( & list_key,             OBJ_ITEM_LIST_KEY_SIZE );
               s_smp_ul(  list_key ) =          OBJ_ITEM_LIST_KEY_SIZE  ;
               os_memc( & item_counter, s_buffer( list_key ),\
                                                OBJ_ITEM_LIST_KEY_SIZE );
               s_stosi( & item_num, * (item_components + 2),      0, 0 );

               md_list( * list_ocb, list_key, OBJ_LIST_ADD,
                                                             item_oid );
               md_list( * list_ocb, list_key, OBJ_LIST_ADD_ENTRY,
                                         * ( item_ids + item_counter ) );
               md_list( * list_ocb, list_key, OBJ_LIST_ADD_NUMBER,
                                                            & item_num );
               s_drop( &  list_key );

            }
            if (rc != 0)
            {

               if (bad_items != NIL)
               {

                  s_cat( bad_items, * bad_items, * (item_ids + item_counter),
                                                                   0, 0);
                  s_cat( bad_items, * bad_items, semi_colon,       0, 0);

               }
               if ( num_bad != NIL )
               {

                  * num_bad += 1;

               }
            }
         }                      /* end for loop */
         if ( bad_items != NIL && s_len(*bad_items) > 0)
         {
            s_smp_ul(*bad_items) = s_len(*bad_items) - 1;
         }
         ocd_drp( & item_ocd);

      }
      else
      {

         rc = NO_DATA_FOUND;

      }
      s_vdrop( & item_ids   );
      s_vdrop( & item_components   );
      s_drop(  & key       );
      s_drop(  & item      );
      s_drop(  & parser    );
      s_drop(  & t_parser  );
      s_drop(  & extra     );
      s_drop(  & prefix    );
      os_memd(   item_oid  );

   }
}

   return(rc);
}
