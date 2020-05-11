/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cc_gtid                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 27, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   prev_err    sint       INPUT                           |
  |  <PARAM>   conf_ocb    OCB *      INPUT                           |
  |  <PARAM>   memb_ocb    OCB *      INPUT                           |
  |  <PARAM>   id_str      string     INPUT                           |
  |  <PARAM>   flags       hword      INPUT                           |
  |  <PARAM>   list        OCB **    OUTPUT                           |
  |  <PURPOSE> Get comment ID                                         |
  |  <OUTPUT>  sint                                error code.        |
  |  <LIMITS>  Need to close form 1150                                |
  |            Do NOT s_drop() 'id_str'                               |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "bse.h"
#include "tm.h"
#include "vars.h"
#include "form.h"
#include "isv.h"
#include "exc.h"
#include "obj_cc.h"
#include "obj_memb.h"


sint     cc_gtid( prev_err, conf_ocb, memb_ocb, id_str, flags, list )

sint            prev_err;
OCB            *conf_ocb, *memb_ocb, **list;
string         *id_str;
hword          *flags;



{

   sint     rc       = 0,
           *seqnum   = NULL,
           *line_num    = NULL,
           *seqnum_arry = NULL,
            arry_size   = 0,
            bse_value   = 0,
            las_seqnum_read = 0,
            conf_las_seqnum = 0,
            rc_form     = 0,
            junk     = 0;

   byte    *ocd_name = NULL;

   string   requested_comm       = S_NULL,
            form_id              = S_NULL,
            blank                = S_NULL,
            space                = S_NULL,
            big_line             = S_NULL,
            cc_id                = S_NULL,
            cc_title             = S_NULL,
            C_LIST               = S_NULL,
            NAMES                = S_NULL,

            conf_id              = S_NULL,
            conf_label           = S_NULL,
            CONF_ID              = S_NULL,
            CONF_LAB             = S_NULL;

   OID     temp_oid,
           memb_oid,
           cc_oid;

   OCB     *user_ocb             = NULL,
           *text_ocb             = NULL,
           *list_ocb             = NULL,
           *cc_ocb               = NULL;

   OCD     *user_ocd             = NULL,
           *conf_ocd             = NULL,
           *text_ocd             = NULL,
           *bse_ocd              = NULL,
           *seqnum_ocd           = NULL,
           *title_ocd            = NULL,
           *cc_ocd               = NULL,
           *id_ocd               = NULL,
           *tit_ocd              = NULL;

   boolean  NOT_DONE             = TRUE,
            MEMB_OCB_OPENED      = FALSE;

   datetime last_active;


list_ocb = *list;

s_init ( & form_id, "1150"         );
s_init ( & NAMES,   "NAMES"        );
s_init ( & space,   "<space 0>"    );

ocd_new( &conf_ocd, "CONFERENCE.LABEL"     );
obj_get(  conf_ocb,  conf_ocd, &conf_label );
s_init ( &CONF_LAB, "CONF_LAB"             );
var_set(  CONF_LAB,  conf_label            );

s_init( &(conf_ocd->name), "CONFERENCE.ID" );
obj_get(  conf_ocb,  conf_ocd, &conf_id    );
s_init ( &CONF_ID,  "CONF_ID"              );
var_set(  CONF_ID,   conf_id               );

      if( OCB_POINTER_OK(memb_ocb) == FALSE )
      {

         obj_find( conf_ocb, "CONFERENCE.MEMBERS", &memb_oid );
         memb_ocb = obj_opn( &memb_oid, OBJ_UPDATE | OBJ_REMOTE );
         MEMB_OCB_OPENED = TRUE;

      }

if( prev_err != NO_ERR )
{

   except(prev_err, USER_ERROR, *id_str, S_NULL, S_NULL, S_NULL, S_NULL);

}

while( NOT_DONE && (rc == NO_ERR)  )
{

   if( *flags & (LIST_ALL_COMMENT | LIST_NEW_COMMENT) )
   {

      if( *flags & LIST_NEW_COMMENT )
      {

         obj_find( conf_ocb, "CONFERENCE.STATUS.LAST_ITEM_SEQNUM",
                             &conf_las_seqnum );

         rc = memb_gt( memb_ocb, S_NULL, *flags, las_seqnum_read,
                       conf_las_seqnum, &seqnum_arry, &arry_size );
         rc = cc_sortid(conf_ocb,*flags,seqnum_arry,arry_size,&list_ocb);
         if( arry_size == 0 )
         {

            prev_err = NO_COMMENTS_TO_LIST;

         }

         ocd_name = "LIST.LIST.NUMBER";

      }
      else
      {

         list_ocb = conf_ocb;
         ocd_name = "CONFERENCE.ITEM_ID.SEQNUM";

      }

      if( rc == NO_ERR )
      {

         ocd_new( &seqnum_ocd, ocd_name );
         seqnum_ocd->bias = 1;
         ocd_new( &title_ocd, "CONFERENCE.ITEM_SN.TITLE" );
         s_alloc( &title_ocd->key,            sizeof(sint) );
         os_memn( s_buffer(title_ocd->key),   sizeof(sint) );
         s_smp_ul( title_ocd->key )        =  sizeof(sint)  ;
         seqnum  = (sint *) s_buffer(title_ocd->key);


         if( OCB_POINTER_OK( text_ocb ) )
         {

            ocd_new( &text_ocd, "CONTENT.CONTENTS" );
            obj_rmv( text_ocb, text_ocd );

         }
         else
         {

            text_ocb = obj_all( CONTENTS_OBJ, LOCAL_OBJECT, NULL );

         }

         ocd_new( &text_ocd, "CONTENT.CONTENTS.TEXT" );
         ocd_new( &bse_ocd,  "CONTENT.CONTENTS.BSE"  );

         s_alloc( &text_ocd->key,            BSE_MAX_KEY_SIZE );
         os_memn( s_buffer(text_ocd->key),   BSE_MAX_KEY_SIZE );
         s_smp_ul( text_ocd->key )        =  BSE_MAX_KEY_SIZE  ;

         bse_ocd->key = text_ocd->key; /* both OCDs have same key   */

         line_num  = (sint *) s_buffer(text_ocd->key);
         bse_value = B_RFIRST | B_PRINTABLE;

         s_alloc( &big_line, 80 );

         while( obj_get( list_ocb, seqnum_ocd, seqnum ) == 0 )
         {

                                         /* key has the id of the conf*/
            obj_get( conf_ocb, title_ocd, &cc_title );

            s_smp_ul( big_line ) = 0;
            s_pad( &big_line, seqnum_ocd->key, 16,' ', 'R' );
            s_cat( &big_line, big_line, cc_title, 0, 0 );
            s_cat( &big_line, big_line, space,    0, 0 );

            *(line_num) += 1;

            obj_rep( text_ocb, text_ocd, big_line,  SEG_UPDATE );
            obj_rep( text_ocb, bse_ocd, &bse_value, SEG_UPDATE );

            bse_value = B_RCONT | B_PRINTABLE ;

         }

         bse_ocd->key        = S_NULL;
         ocd_drp( &seqnum_ocd );
         ocd_drp( &title_ocd  );
         ocd_drp( &text_ocd       );
         ocd_drp( &bse_ocd        );

         f_close( form_id, NO_FLAGS );

         if( *line_num == 0 )
         {

            prev_err = NO_COMMENTS_TO_LIST;  ;

         }

      }  /* If NO_ERR from CC_SORTID */

      *flags &= ~(LIST_ALL_COMMENT | LIST_NEW_COMMENT);

   }  /* if comments have not been displayed yet? */

   if( prev_err != NO_ERR )
   {

      except(prev_err, USER_ERROR, *id_str, S_NULL, S_NULL, S_NULL,
                                            S_NULL);
      prev_err = NO_ERR;

   }

   rc_form = t_form( form_id, S_NULL, text_ocb    );

   requested_comm = var_get( NAMES );

   switch(rc_form)
   {
   case FORM_NOT_FORMED:
   case USER_REQUESTED_ABANDON:
   case USER_REQUESTED_QUIT:

      rc = rc_form;

   break;

   case USER_REQUESTED_SCAN:

      *flags |= SCAN_ALL_COMMENT;
      NOT_DONE = FALSE;

   break;

   case USER_REQUESTED_REPLY:                /* List New               */

      *flags |= LIST_NEW_COMMENT;

   break;

   case USER_REQUESTED_PRINT:

      *flags |= LIST_ALL_COMMENT;

   break;

   case USER_REQUESTED_ENTER:
   case USER_REQUESTED_PROCESS:

      if( (s_len(requested_comm) < 1) )
      {                                /* User wants to see all cc's   */
                                       /* waiting for him/her          */
         *flags |= LIST_NEW_COMMENT;

      }
      else
      {
         s_copy( id_str, requested_comm, 0, 0 );
         NOT_DONE = FALSE;

      }

   break;

   }  /* rc_form switch */

} /* while not done */

var_clr( NAMES     );
var_clr( CONF_ID  );
var_clr( CONF_LAB );

ocd_drp( & conf_ocd );
ocd_drp( & id_ocd   );
ocd_drp( & tit_ocd  );

s_drop ( & conf_id  );
s_drop ( & conf_label);
s_drop ( & CONF_ID  );
s_drop ( & CONF_LAB );
s_drop ( & form_id   );
s_drop ( & NAMES     );
s_drop ( & space     );


*list = list_ocb;

   return(rc);

}
