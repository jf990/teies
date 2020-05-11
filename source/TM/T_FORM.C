/********************************************************************
<doc>
<name>   t_form.c
<author> John L. Foster
<level>  TM
<date>   December 28, 1987
<purpose>
         Given a TEIES state code identifier, locate the form, call
         forms to process the form, check the fields updated by the user
         are ok, update the objects and variables requested, and
         return.

         If t_alarm() was called or a process set the alarm bit then
         the display screen is "beeped" when form formats the display.
         The alarm is turned off once processed.

         The form message area is queried for possible display on the
         form.  If the field is undefined or empty then the TM variable
         is set blank.  Once processed the variable is blanked out in
         case no one sets it up until the next form is processed.

<param>  state     string INPUT   State identifier.
<param>  modifier  string INPUT   State modifier or additional data.
<param>  ocb       struct INPUT   Object Control Blk of inuse object.
<output> end_key   string OUTPUT  Function requested on exit of form.
<global>
<errors> 28091     No Form in the SFI for the state requested.
<calls>  form(), s_init(), s_comp(), f_updfld()
<limits>
<mods>   July 20, 1989     JF     Added FORM editor interface.
</doc>
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"



sint     t_form(  state, modifier, object_cb  )



string            state, modifier;
OCB              *object_cb;



{


OCB        *psi_ocb           =    NIL,
           *sfi_ocb           =    NIL;
string
           form_id            = S_NULL,
           form_var           = S_NULL,
           message_area       = S_NULL,
           message            = S_NULL,
           command_line       = S_NULL,
           last_field         = S_NULL;

sint                 mode     =      0,
                     rc       =      0,
                     finish_key =    0;

boolean              done     =  FALSE,
           edit_form          =  FALSE;

hword      flags              = NO_FLAGS;








s_copy(   &form_id, state, 0, 0 );
s_trim(   &form_id, 'B'         );
s_upper(   form_id, 0, 0        );


s_init( & form_var, "%FORM" );

if( s_len( form_id )   > 1      )
{

   while( ! done )
   {

      flags = NO_FLAGS;
      if( TM_master->current->flags & PROCESS_ALARM_PENDING )
      {

         TM_master->current->flags &= ~(PROCESS_ALARM_PENDING);
         flags |= SOUND_ALARM;

      }


       /*************************************************************\
       *                                                             *
       * Set the exception message area with the message from the    *
       * exception variable.  Reset the field back to its original   *
       * contents (should be blank) after the form is posted.        *
       *                                                             *
       \*************************************************************/

      s_init( & message_area, "%EXC" );
      s_copy( & message, (string) var_get( message_area ), 0, 0 );
      if( s_len( message ) < 1 )
      {

         s_pad( & message, S_NULL, 79, ' ', 'L' );

      }
      f_setfld( form_id, message_area, message );
      var_clr(  message_area );
      s_drop( & message      );

      f_rstinf( form_id,  flags   );
      var_set(  form_var, form_id );

      finish_key = form( form_id, object_cb, flags );


      f_rstfld( form_id, flags, message_area );
      t_clrerr( TM_master->current->tm_pid   );

      /***************************************************************
       *                                                             *
       *Check over the form fields only if abandon was not requested *
       *AND forms returned indicating no errors occurred.            *
       *                                                             *
       ***************************************************************/

      if( finish_key == FORM_NOT_FORMED )
      {

         string    no_state  =  S_NULL,
                   state_id  =  S_NULL;


         /* Put up NOSTATE form with variable %STATE_ID as the form_id */

         s_init( & no_state, "NOSTATE"   );
         s_init( & state_id, "%STATE_ID" );
         var_set(  state_id,  form_id    );

         finish_key = form( no_state, NIL, NO_FLAGS );

         if( finish_key == FORM_NOT_FORMED )
         {

            done = TRUE;
            finish_key = TM_FORM_NOT_FORMED ;
            except( finish_key, FATAL_ERROR,  S_NULL, S_NULL,
                                     S_NULL,  S_NULL, S_NULL  );
            break;

         }
         else if( finish_key == FORM_EDIT_FORM )
         {

            edit_form = TRUE;

         }
         f_close(  no_state, NO_FLAGS );
         s_drop( & state_id           );
         s_drop( & no_state           );

         finish_key = TM_FORM_NOT_FORMED ;

      }
      else if( finish_key != FORM_ABANDON )
      {

         rc = f_updfld( form_id, NO_FLAGS );
         s_init( & command_line, "%CMD" );
         if( f_rstfld( form_id, NO_FLAGS, command_line ))
         {                                         /********************
                                                    * Only attempt to  *
                                                    * check the command*
                                                    * line if one was  *
                                                    * on the form.  The*
                                                    * field is cleared *
                                                    * so we dont execut*
                                                    * it again next tme*
                                                    ********************/

            string   user_request = S_NULL ;


            s_copy(  & user_request, (string) var_get( command_line ), 0, 0 );
            s_ctrim( & user_request, 'B', ' ' );
            s_ctrim( & user_request, 'B', '_' );
            var_clr(   command_line           );
            if( s_len( user_request ) )
            {

               rc = t_command( user_request,
                              (struct memnode *)TM_master->uicmdtab, 0 );

               if(  ( rc >= LOWEST_PROGRAM_FUNCTION_KEY  )
                &&  ( rc <= HIGHEST_PROGRAM_FUNCTION_KEY ) )
               {

                  finish_key = rc - TM_CONTROL_MENU_SELECTION ;
                  rc         =  0;

               }
            }
            if( rc == SIGNOFF_REQUESTED )
            {

               finish_key = -1 ;
               done = TRUE;

            }
            s_drop( & user_request );

         }
         if ( rc != 0 )
         {

            done = FALSE;

         }
         else
         {

            done = TRUE;

         }

         if( finish_key == FORM_EDIT_FORM )
         {

            edit_form = TRUE;
            done      = FALSE;

         }
         s_drop( & command_line );

      }
      else
      {

         done = TRUE;

      }

      if( edit_form )
      {

         rc = t_edtscr( form_id, NO_FLAGS, 0 );
         f_close( form_id, NO_FLAGS );
         edit_form = FALSE ;

      }
   }
}
else
{

   finish_key = TM_FORM_NOT_FORMED ;
   except( finish_key, CONTROL_ERROR,  S_NULL, S_NULL,
                              S_NULL,  S_NULL, S_NULL  );

}

s_drop( & form_id      );
s_drop( & message_area );
s_drop( & command_line );
s_drop( & form_var     );

if( finish_key != FORM_NOT_FORMED )
{

   finish_key += TM_CONTROL_MENU_SELECTION ;

}

   return(finish_key);


}
