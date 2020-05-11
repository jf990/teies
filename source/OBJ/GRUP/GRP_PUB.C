/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_pub()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman, Tanmay S. Kumar   |
  |                                                                   |
  |  :DATE.    June 23, 1988                        August 15, 1989   |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *      INOUT  Pointer to Group         |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_GRUP()   |
  |                                                                   |
  |  :PARAM.   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  :PARAM.   level       hword      INPUT  not used.                |
  |                                                                   |
  |  :PARAM.   params      byte   *   INOUT  not used                 |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :PURPOSE. Create a Group.         This routine handles the       |
  |            adding of PUBLIC components of the Group       namely  |
  |            TITLE and KEYS.                                        |
  |            It calls up the "3122 form", validates the user inputs,|
  |            implements function keys like QUIT, ABANDON, etc..     |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
  |                                                                   |
  |  :CALLS.   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_group.h"


sint grp_pub( group_ocb, modifier, level, params )

OCB     *group_ocb;
string   modifier;
hword    level;
byte    *params;

{

string   form_id        =  S_NULL,
         group_id       =  S_NULL,
         group_label    =  S_NULL,
         tmp_str        =  S_NULL,

         GROUPID       =  S_NULL,
         GROUPLABEL    =  S_NULL,
         GROUP_TITLE    =  S_NULL,
         GROUP_KEYS     =  S_NULL;

OCD      *group_ocd     =  NULL;

boolean  done           = FALSE;


sint     rc             =  NO_ERR,
         junk           =  0;


s_init( &form_id,     "3122"    );

s_init( &GROUPID,  "GROUPID" );
s_init( &GROUPLABEL,  "GROUPLABEL" );
s_init( &GROUP_TITLE,  "GROUP_TITLE" );
s_init( &GROUP_KEYS,   "GROUP_KEYS"  );

while (!done)
{
   obj_find(group_ocb, "GROUP.ID", &group_id);
   obj_find(group_ocb, "GROUP.LABEL", &group_label);
   var_set(GROUPID, group_id);
   var_set(GROUPLABEL, group_label);
   f_setfld(form_id, GROUPID, group_id);
   f_setfld(form_id, GROUPLABEL, group_label);
   rc = t_form( form_id, S_NULL, NIL );

   if ( ( rc == USER_REQUESTED_ABANDON )
    ||  ( rc == USER_REQUESTED_QUIT ) )
   {
      break;
   }
   else if ( ( rc == USER_REQUESTED_ENTER )
          || ( rc == USER_REQUESTED_PROCESS ) )
   {
      rc = 0;
      tmp_str = var_get( GROUP_TITLE );
      if( s_len( tmp_str ) != 0 )
      {
         obj_update( group_ocb, "GROUP.TITLE", tmp_str );
      }
      tmp_str = var_get( GROUP_KEYS );
      if( s_len( tmp_str ) != 0 )
      {
         t_ckkeys( &tmp_str, tmp_str );
         obj_update( group_ocb, "GROUP.KEYS", tmp_str );
      }
      done = TRUE;
      break;

   }

   else
   {
      continue;
   }
}

s_drop( &form_id     );

s_drop( &group_id    );
s_drop( &group_label );

s_drop( &GROUPID  );
s_drop( &GROUPLABEL  );
s_drop( &GROUP_TITLE  );
s_drop( &GROUP_KEYS   );

   return(rc);

}
