/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_gtid                                               |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 23, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   prev_err    sint       INPUT  description              |
  |  <PARAM>   id_str      string *   INPUT  description              |
  |  <PARAM>   flags       hword  *   INPUT  description              |
  |                                                                   |
  |  <PURPOSE> Get conference ID(or label)                            |
  |                                                                   |
  |  <OUTPUT>  sint          0 = Everything is well                   |
  |                              otherwise some error                 |
  |  <LIMITS>  Need to close form 1140                                |
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
#include "obj_cr.h"


sint     cfr_gtid( prev_err, id_str, flags )

sint               prev_err;
string         *   id_str;
hword          *   flags;



{

   sint     rc        = 0,
            junk      = 0,
            bse_value = 0,
            cc_count  = 0,
            linenum   = 0,
            rc_form   = 0,
            *line_num = &linenum;

   string   requested_confs      = S_NULL,
            space                = S_NULL,
            form_id              = S_NULL,
            conf_label           = S_NULL,
            count_str            = S_NULL,
            big_line             = S_NULL,
            NAMES                = S_NULL,
            NAMES2               = S_NULL;

   OID     temp_oid;

   OCB     *user_ocb             = NULL,
           *text_ocb             = NULL;

   OCD     *user_count_ocd       = NULL,
           *user_label_ocd       = NULL,
           *text_ocd             = NULL,
           *bse_ocd              = NULL;

   boolean  NOT_DONE             = TRUE;


s_init ( & form_id, "1140"         );
s_init ( & NAMES,   "NAMES"        );
s_init ( & NAMES2,  "NAMES2"       );
s_init ( & space,   "<space 0>"    );


while( NOT_DONE && (rc == NO_ERR)  )
{

   if((*flags & DISPLAY_ALL_CONFS)&&((*flags & ALL_CONFS_DISPLAYED)==0))
   {


                                     /* If we already displayed all    */
                                     /* confs then there is no need to */
                                     /* display them all over again    */

      user_ocb = TM_master->ucb->directory;
      ocd_new( &user_count_ocd, "MEMBERSHIPS.CONFERENCES.NEW_CC_COUNT" );
      ocd_new( &user_label_ocd, "MEMBERSHIPS.CONFERENCES.LABEL"        );
      user_count_ocd->bias = 1;

      text_ocb = obj_all( CONTENTS_OBJ, LOCAL_OBJECT, NULL );

      ocd_new( &text_ocd, "CONTENT.CONTENTS.TEXT" );
      ocd_new( &bse_ocd,  "CONTENT.CONTENTS.BSE"  );

      s_alloc( &text_ocd->key,            BSE_MAX_KEY_SIZE );
      os_memn( s_buffer(text_ocd->key),   BSE_MAX_KEY_SIZE );
      s_smp_ul( text_ocd->key )        =  BSE_MAX_KEY_SIZE  ;

      bse_ocd->key = text_ocd->key; /* both OCDs have same key      */

      line_num  = (sint *) s_buffer(text_ocd->key);
      bse_value = B_RFIRST | B_PRINTABLE;

      s_alloc( &big_line, 80 );

      while( obj_get( user_ocb, user_count_ocd, &cc_count ) == 0 )
      {
                                      /* key has the id of the conf    */
         user_label_ocd->key = user_count_ocd->key;
         obj_get( user_ocb, user_label_ocd, &conf_label );
         s_sitos( &count_str, cc_count );

         s_smp_ul( big_line ) = 0;
         s_pad( &big_line, user_count_ocd->key, 16,' ', 'R' );
         s_cat( &big_line, big_line, conf_label, 0, 0 );
         s_pad( &big_line, big_line, 48,' ', 'R'      );
         s_cat( &big_line, big_line, count_str,  0, 0 );
         s_cat( &big_line, big_line, space,      0, 0 );

         *(line_num) += 1;

         obj_rep( text_ocb, text_ocd, big_line,  SEG_UPDATE );
         obj_rep( text_ocb, bse_ocd, &bse_value, SEG_UPDATE );

         bse_value = B_RCONT | B_PRINTABLE ;

      }

      bse_ocd->key        = S_NULL;
      user_label_ocd->key = S_NULL;
      ocd_drp( &user_count_ocd );
      ocd_drp( &user_label_ocd );
      ocd_drp( &text_ocd       );
      ocd_drp( &bse_ocd        );

      f_close( form_id, NO_FLAGS );

      if( *line_num == 0 )
      {

         prev_err = CONF_NOT_MEMBER_OF_ANY;

      }

      *flags &= ~DISPLAY_ALL_CONFS;    /* Turn bit off                 */
      *flags |=  ALL_CONFS_DISPLAYED;  /* Turn bit on                  */

   }  /* if conference have not been displayed yet? */

   if( prev_err != NO_ERR )
   {

      except(prev_err, USER_ERROR, *id_str, S_NULL, S_NULL, S_NULL, S_NULL);
      prev_err = NO_ERR;

   }

   rc_form = t_form( form_id, S_NULL, text_ocb    );

   requested_confs = var_get( NAMES );


   switch(rc_form)
   {

      case FORM_NOT_FORMED:
      case USER_REQUESTED_ABANDON:
      case USER_REQUESTED_QUIT:

         rc = rc_form;

      break;

      case USER_REQUESTED_SCAN:

         *flags |= SCAN_ALL_CONFS;
          NOT_DONE = FALSE;

      break;

      case USER_REQUESTED_PRINT:

         *flags |= DISPLAY_ALL_CONFS;

      break;

      case USER_REQUESTED_ENTER:
      case USER_REQUESTED_PROCESS:

         if( (s_len(requested_confs) < 1) )
         {                             /* User wants to see all confs  */
                                    /* sh/e belongs to              */
            *flags |= DISPLAY_ALL_CONFS;

         }
         else
         {
            s_copy( id_str, requested_confs, 0, 0 );
            s_upper( *id_str, 0, 0 );
            NOT_DONE = FALSE;

         }

      break;

      default:

         break;

   }


} /* while not done */

var_clr( NAMES     );
var_clr( NAMES2    );

s_drop ( & form_id   );
s_drop ( & NAMES     );
s_drop ( & NAMES2    );
s_drop ( & space     );
s_drop ( & big_line  );



return( rc );

}
