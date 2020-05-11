/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_homebase()                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    December 11, 1987                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   * none *                                               |
  |                                                                   |
  |  <PURPOSE> Perform the Homebase state based on the user level     |
  |            determined from the user control block.  This can be   |
  |            either CASUAL, EXPERIENCED or POWER.                   |
  |                                                                   |
  |            When this routine is competed, all processes are done  |
  |            for this server and it is time to signoff.             |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_upduser, s_init(), s_drop(), s_put_c(), except(),    |
  |  <ERRORS>  28111 : No user control block                          |
  |  <LIMITS>  None known.                                            |
  |  <MODS>    15_dec_88     JF      Added t_upduser() call to update |
  |                                  the new items of the user for    |
  |                                  presentation on the homebase     |
  |                                  form.                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "isv.h"
#include "vars.h"
#include "form.h"

#define MODIFIER_POSITION 2





sint   t_homeba(  )


{

sint                level     =       0,
                    rc        =       0,
                    count     =       0,
                    exit_key  =       0,
                    mail_count=       0;
string              state     =   S_NUL,
                    field     =   S_NUL,
                    count_var =   S_NUL,
                    response  =   S_NUL,
                    yes       =   S_NUL,
                    no        =   S_NUL,
                    str       =  S_NULL,
                    value     =  S_NULL,
                    state_var =   S_NUL;
boolean             done      =   FALSE,
                    signoff   =   FALSE;
OCB               * userocb   =     NIL;
OCD               * index_ocd =     NIL;



   s_init(  & state,     "0000" );
   isv_get( & yes, ISV_YES,   0 );
   isv_get( & no,  ISV_NO,    0 );

   if( TM_master->ucb == NIL )
   {

      rc = TM_USER_CONTROL_BLOCK_NOT_FOUND;
      except( rc, FATAL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                  S_NULL );

   }

   while( ! done )
   {

      switch( (level = TM_master->ucb->level)  )
      {

         case CASUAL_USER :
                           s_put( state, '1', MODIFIER_POSITION ); break;

         case EXPERT_USER :
                           s_put( state, '2', MODIFIER_POSITION ); break;

         case POWER_USER  :
                           s_put( state, '3', MODIFIER_POSITION ); break;

         default          :
                           s_put( state, '0', MODIFIER_POSITION );

      }


      var_set( state_var, state                          );

      /************************************************************
       * Need to set all necessary variables for the next screen, *
       * like # of waiting items!                                 *
       ************************************************************/

      userocb = TM_master->ucb->directory;

      if( OCB_POINTER_OK( userocb ) )
      {

         obj_set_flag(  userocb, OBJ_FLUSH );
         obj_cls(       userocb            );

         ocd_new( & index_ocd, "INDEXES.MAIL_COUNT" );
         rc = obj_get( userocb, index_ocd, & mail_count );
         s_init(  & count_var, "NUMB_MAIL"  );
         s_init(  & response,  "VIEW_MAIL"  );
         s_sitos( & str,        mail_count  );
         if( mail_count > 0 )
         {

            value = yes;

         }
         else
         {

            value = no;

         }
         var_set(   response,   value       );
         var_set(   count_var,  str         );
         f_setfld(  state, count_var,   str );


         s_init( &(index_ocd->name), "INDEXES.NEW_NOTIF_COUNT" );
         rc = obj_get( userocb, index_ocd, & count );
         s_init(  & count_var, "NUMB_NOTIF" );
         s_init(  & response,  "VIEW_NOTIF" );
         s_sitos( & str,        count       );
         if( count > 0 )
         {

            value = yes;

         }
         else
         {

            value = no;

         }
         var_set(   response,   value       );
         var_set(   count_var,  str         );
         f_setfld(  state, count_var,   str );


         s_init( &(index_ocd->name), "INDEXES.CC_COUNT" );
         rc = obj_get( userocb, index_ocd, & count );
         s_init(  & count_var, "NUMB_CONF"  );
         s_init(  & response,  "VIEW_CONF"  );
         s_sitos( & str,        count       );
         if( count > 0 )
         {

            value = yes;

         }
         else
         {

            value = no;

         }
         var_set(   response,   value       );
         var_set(   count_var,  str         );
         f_setfld(  state, count_var,   str );


         s_init( &(index_ocd->name), "INDEXES.MARKED_COUNT" );
         rc = obj_get( userocb, index_ocd, & count );
         s_init(  & count_var, "NUMB_MARKED");
         s_init(  & response,  "VIEW_MARKED");
         s_sitos( & str,        count       );
         if( count > 0 )
         {

            value = yes;

         }
         else
         {

            value = no;

         }
         var_set(   response,   value       );
         var_set(   count_var,  str         );
         f_setfld(  state, count_var,   str );


         s_init( &(index_ocd->name), "INDEXES.UNDONE_COUNT" );
         rc = obj_get( userocb, index_ocd, & count );
         s_init(  & count_var, "NUMB_UNDONE");
         s_init(  & response,  "VIEW_UNDONE");
         s_sitos( & str,        count       );
         if( count > 0 )
         {

            value = yes;

         }
         else
         {

            value = no;

         }
         var_set(   response,   value       );
         var_set(   count_var,  str         );
         f_setfld(  state, count_var,   str );


         s_drop( & str       );
         s_drop( & count_var );
         s_drop( & response  );
         ocd_drp(& index_ocd );
         value   =      S_NULL;

      }
      exit_key = t_form(  state, NIL, NIL                );

      if( exit_key != TM_FORM_NOT_FORMED )
      {

         if( exit_key == SIGNOFF_REQUESTED
          || exit_key == USER_REQUESTED_ABANDON )
         {

            rc = SIGNOFF_REQUESTED ;
            break;

         }
         switch( level )
         {

         case CASUAL_USER :
                               rc = t_valcas();                 break;

         case EXPERT_USER :
                               rc = t_valexp();                 break;

         case POWER_USER  :
                               rc = t_valpwr();                 break;

         default          :
                               rc = t_valcas();                 break;

         }
      }
      else
      {

         except(  HomeBase_Form_Not_Found, FATAL_ERROR, state, S_NULL,
                                               S_NULL, S_NULL, S_NULL  );

      }

      t_upduser();

   }
   s_drop ( & state     );
   s_drop ( & state_var );

   return(rc);

}
