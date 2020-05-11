/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_valcas.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 11, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   * none *                                               |
  |  <PURPOSE> Validate and process the fields entered on the Casual  |
  |            homebase form.                                         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "isv.h"
#include "form.h"



sint   t_valcas()


{

string   YorN        =  S_NULL,
         VIEW_NOTIF  =  S_NULL,        /* = = = = = = = = = = = = = = */
         VIEW_MAIL   =  S_NULL,        /*                             */
         VIEW_CONF   =  S_NULL,        /* Define all of the fields    */
         VIEW_MARKED =  S_NULL,        /* that appear on the casual   */
         VIEW_UNDONE =  S_NULL,        /* form.                       */
         NUMB_NOTIF  =  S_NULL,        /*                             */
         NUMB_MAIL   =  S_NULL,        /*                             */
         NUMB_CONF   =  S_NULL,        /* = = = = = = = = = = = = = = */
         NUMB_MARKED =  S_NULL,
         NUMB_UNDONE =  S_NULL,
         JOIN_CONF   =  S_NULL,
         CREAT_NOTIF =  S_NULL,
         CREAT_MAIL  =  S_NULL,
         CREAT_CCOMM =  S_NULL,
         CREAT_DRAFT =  S_NULL,
         MODIFY_ITEM =  S_NULL,
         MEMB_GRP_ID =  S_NULL,
         REPLY_ID    =  S_NULL,
         CONF_ID     =  S_NULL,
         CCOMM_ID    =  S_NULL,
         DRAFT_SP    =  S_NULL,
         FIND_USER   =  S_NULL,
         FIND_MMSG   =  S_NULL,
         FIND_GROUP  =  S_NULL,
         FIND_CONF   =  S_NULL,
         NAME_PART   =  S_NULL,
         INDEX_KEY   =  S_NULL,
         MODIFIER    =  S_NULL,
         no_reset    =  S_NULL,
         new         =  S_NULL,
         marked      =  S_NULL,
         form_id     =  S_NULL,
         state_var   =  S_NULL,
         junk        =  S_NULL;

real     x           =     0.0,
         y           =     0.0;

sint     rc          =       0;

byte     in_char     =     ' ';

boolean  answer      =    TRUE,
         still_checking = TRUE;






s_init( & form_id, "0100" );
/* form_id = (string) var_get( state_var ); */

s_init( & YorN,       "yes or no"    ); /* Should come from ISV !      */
s_init( & new,        "NEW"          ); /* Should come from ISV !      */
s_init( & marked,     "MARKED"       ); /* Should come from ISV !      */
s_init( & VIEW_NOTIF, "VIEW_NOTIF"   );
s_init( & VIEW_MAIL,  "VIEW_MAIL"    );
s_init( & VIEW_CONF,  "VIEW_CONF"    );
s_init( & VIEW_MARKED,"VIEW_MARKED"  );
s_init( & VIEW_UNDONE,"VIEW_UNDONE"  );
s_init( & NUMB_NOTIF, "NUMB_NOTIF"   );
s_init( & NUMB_MAIL,  "NUMB_MAIL"    );
s_init( & NUMB_CONF,  "NUMB_CONF"    );
s_init( & NUMB_MARKED,"NUMB_MARKED"  );
s_init( & NUMB_UNDONE,"NUMB_UNDONE"  );
s_init( & JOIN_CONF,  "JOIN_CONF"    );
s_init( & CREAT_NOTIF,"CREAT_NOTIF"  );
s_init( & CREAT_MAIL, "CREAT_MAIL"   );
s_init( & CREAT_CCOMM,"CREAT_CCOMM"  );
s_init( & CREAT_DRAFT,"CREAT_DRAFT"  );
s_init( & MODIFY_ITEM,"MODIFY_ITEM"  );
s_init( & MEMB_GRP_ID,"MEMB_GRP_ID"  );
s_init( & REPLY_ID,   "REPLY_ID"     );
s_init( & CONF_ID,    "CONF_ID"      );
s_init( & CCOMM_ID,   "CCOMM_ID"     );
s_init( & DRAFT_SP,   "DRAFT_SP"     );
s_init( & FIND_USER,  "FIND_USER"    );
s_init( & FIND_MMSG,  "FIND_MMSG"    );
s_init( & FIND_GROUP, "FIND_GROUP"   );
s_init( & FIND_CONF,  "FIND_CONF"    );
s_init( & NAME_PART,  "NAME_PART"    );
s_init( & INDEX_KEY,  "INDEX_KEY"    );
s_init( & MODIFIER,   "MODIFIER"     );
isv_get( & no_reset,  ISV_NO, 0      );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Now check each field for proper input, and act on the field     */
/* according to user input and our interface rules.                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


while( still_checking )
{

   if( (answer = t_valyn( (string) var_get( VIEW_NOTIF ))) == TRUE )
   {

      t_jmpst( NOTIF_OBJ, VIEW_ACT, ISV_NEW, new, NIL, NIL );
      f_setfld( form_id, VIEW_NOTIF, no_reset);

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( VIEW_NOTIF ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, VIEW_NOTIF, x, y );
      break;

   }
   if( (answer = t_valyn( (string) var_get( VIEW_MAIL ))) == TRUE )
   {

      t_jmpst( MAIL_OBJ, VIEW_ACT, ISV_NEW, new, NIL, NIL );
      f_setfld( form_id, VIEW_MAIL, no_reset);

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( VIEW_MAIL ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, VIEW_MAIL, x, y );
      break;

   }
   if( (answer = t_valyn( (string) var_get( VIEW_CONF ))) == TRUE )
   {

      t_jmpst( CONF_OBJ, VIEW_ACT, ISV_NEW, new, NIL, NIL );
      f_setfld( form_id, VIEW_CONF, no_reset);

   }
   else if( answer  !=  FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( VIEW_CONF ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, VIEW_CONF, x, y );
      break;

   }
   if( (answer = t_valyn( (string) var_get( VIEW_MARKED ))) == TRUE )
   {

      /* Get the marked index in the user object, then call view list */
      t_jmpst( LIST_OBJ, VIEW_ACT, 0, marked, TM_master->ucb->directory,
                                              NIL                       );
      f_setfld( form_id, VIEW_MARKED, no_reset        );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( VIEW_MARKED ),YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, VIEW_MARKED, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( VIEW_UNDONE ))) == TRUE )
   {

     /* Get the undone activities in the user object, then call view list */
      t_jmpst( ACTIVITY_OBJ, VIEW_ACT, ISV_NEW, new, NIL, NIL );
      f_setfld( form_id, VIEW_UNDONE, no_reset   );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( VIEW_UNDONE ),YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, VIEW_UNDONE, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( JOIN_CONF ))) == TRUE )
   {

      t_jmpst( CONF_OBJ, JOIN_ACT, 0, (string) var_get( CONF_ID ), NIL, NIL );
      f_rstfld( form_id, NO_FLAGS, JOIN_CONF                  );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( JOIN_CONF ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, JOIN_CONF, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( CREAT_NOTIF ))) == TRUE )
   {

      t_jmpst( NOTIF_OBJ, CREATE_ACT, 0, S_NULL, NIL, NIL );
      f_rstfld( form_id, NO_FLAGS, CREAT_NOTIF       );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( CREAT_NOTIF ),YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, CREAT_NOTIF, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( CREAT_MAIL ))) == TRUE )
   {

      t_jmpst( MAIL_OBJ, CREATE_ACT, 0, (string) var_get( REPLY_ID ), NIL, NIL);
      f_rstfld( form_id, NO_FLAGS, CREAT_MAIL                   );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( CREAT_MAIL ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, CREAT_MAIL, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( CREAT_CCOMM ))) == TRUE )
   {

      t_jmpst( COMMENT_OBJ, CREATE_ACT, 0, (string)var_get( CONF_ID ),NIL,NIL);
      f_rstfld( form_id, NO_FLAGS,  CREAT_CCOMM                     );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( CREAT_CCOMM ),YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, CREAT_CCOMM, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( CREAT_DRAFT ))) == TRUE )
   {

      string scratchpad = S_NULL;


      s_init( & scratchpad, "A * TEIES $SP" );
      s_cat(  & scratchpad, scratchpad, (string) var_get( DRAFT_SP ), 0, 0 );
      t_spad(   scratchpad, NIL );
      s_drop( & scratchpad );
      f_rstfld( form_id, NO_FLAGS, CREAT_DRAFT );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( CREAT_DRAFT ),YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, CREAT_DRAFT, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( MODIFY_ITEM))) == TRUE )
   {

      t_jmpst( ANY_OBJ, MODIFY_ACT, 0, S_NULL, TM_master->ucb->directory,
                                               NIL                       );
      f_rstfld( form_id, NO_FLAGS,   MODIFY_ITEM         );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( MODIFY_ITEM), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, MODIFY_ITEM, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( FIND_USER ))) == TRUE )
   {

      t_jmpst( USER_OBJ, FIND_ACT, 0, (string) var_get( NAME_PART ), NIL, NIL );
      f_rstfld( form_id, NO_FLAGS, FIND_USER      );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( FIND_USER ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, FIND_USER, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( FIND_MMSG ))) == TRUE )
   {

      t_jmpst( MAIL_OBJ, FIND_ACT, 0, S_NULL, NIL, NIL );
      f_rstfld( form_id, NO_FLAGS, FIND_MMSG   );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( FIND_MMSG ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, FIND_MMSG, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( FIND_GROUP ))) == TRUE )
   {

      t_jmpst( GROUP_OBJ, FIND_ACT, 0, S_NULL, NIL, NIL );
      f_rstfld( form_id,  NO_FLAGS,    FIND_GROUP  );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( FIND_GROUP ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, FIND_GROUP, x, y );
      break;

   }


   if( (answer = t_valyn( (string) var_get( FIND_CONF ))) == TRUE )
   {

      t_jmpst( CONF_OBJ, FIND_ACT, 0, S_NULL, NIL, NIL );
      f_rstfld( form_id, NO_FLAGS, FIND_CONF   );

   }
   else if( answer != FALSE )
   {

      except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( FIND_CONF ), YorN,
                                           S_NULL, S_NULL, S_NULL    );
      f_curpos( form_id, NO_FLAGS, FIND_CONF, x, y );
      break;

   }
   still_checking = FALSE;

}


/* + = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = + *\
   |                                                                 |
   | All fields on the Casual homebase have been checked and their   |
   | respective functions called.  Now reset all the Optional Fill-in|
   | fields back to their original (default) settings.               |
   |                                                                 |
\* + = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = + */

if( ! still_checking )
{

   f_rstfld( form_id, NO_FLAGS, MEMB_GRP_ID );
   f_rstfld( form_id, NO_FLAGS, REPLY_ID    );
   f_rstfld( form_id, NO_FLAGS, CONF_ID     );
   f_rstfld( form_id, NO_FLAGS, CCOMM_ID    );
   f_rstfld( form_id, NO_FLAGS, DRAFT_SP    );
   f_rstfld( form_id, NO_FLAGS, NAME_PART   );
   f_rstfld( form_id, NO_FLAGS, INDEX_KEY   );
   f_rstfld( form_id, NO_FLAGS, MODIFIER    );

}
else
{

   rc = USER_ERRORS_ON_FORM;

}



   s_drop( & YorN          );
   s_drop( & new           );
   s_drop( & marked        );
   s_drop( & VIEW_NOTIF    );
   s_drop( & VIEW_MAIL     );
   s_drop( & VIEW_CONF     );
   s_drop( & VIEW_MARKED   );
   s_drop( & VIEW_UNDONE   );
   s_drop( & NUMB_NOTIF    );
   s_drop( & NUMB_MAIL     );
   s_drop( & NUMB_CONF     );
   s_drop( & NUMB_MARKED   );
   s_drop( & NUMB_UNDONE   );
   s_drop( & JOIN_CONF     );
   s_drop( & CREAT_NOTIF   );
   s_drop( & CREAT_MAIL    );
   s_drop( & CREAT_CCOMM   );
   s_drop( & CREAT_DRAFT   );
   s_drop( & MODIFY_ITEM   );
   s_drop( & MEMB_GRP_ID   );
   s_drop( & REPLY_ID      );
   s_drop( & CONF_ID       );
   s_drop( & CCOMM_ID      );
   s_drop( & DRAFT_SP      );
   s_drop( & FIND_USER     );
   s_drop( & FIND_MMSG     );
   s_drop( & FIND_GROUP    );
   s_drop( & FIND_CONF     );
   s_drop( & NAME_PART     );
   s_drop( & INDEX_KEY     );
   s_drop( & MODIFIER      );


   return(rc);

}
