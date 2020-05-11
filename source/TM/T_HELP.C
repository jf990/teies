/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_help()                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 1, 1989                                       |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   help_line       string    INPUT   Help words requested.|
  |                                                                   |
  |  <PURPOSE> Process ?word, ??word, ??? command line features.      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "form.h"
#include "vars.h"

#define    SINGLE_HELP      1
#define    ADDITIONAL_HELP  2
#define    PEOPLE_HELP      3



sint   t_help(  help_line  )

string          help_line;


{

   string    form_id       = S_NULL ,
             command_line  = S_NULL ,
             q_request     = S_NULL ;

   sint      position      =      0 ,
             rc            =      0 ,
             key_hit       =      0 ;

   boolean   edit_help_form= FALSE  ,
             done          = FALSE  ;





if( s_len( help_line ) > 0 )
{

   for (  position = 0;  position < 3;  position ++  )
   {

      if( s_char_s( help_line, position ) != '?' )
      {

         break;

      }
   }
   s_copy( & q_request, help_line, position + 1, 0 );
   s_trim( & q_request, 'B' );

   switch( position )
   {

      case SINGLE_HELP     :

              break;

      case ADDITIONAL_HELP :

              break;

      case PEOPLE_HELP     :

              break;

      default              :

              break;

   }
   s_copy( & form_id, q_request, 0, 0 );

   s_init( & command_line, "%CMD" );

   while( ! done )
   {

      if( s_len( form_id ) > 0 )
      {

         key_hit = form (   form_id, NIL, HELP_FORM   ) ;


      }

      if (  key_hit == FORM_NOT_FORMED  )
      {

         string  help_id   = S_NULL,
                 no_help   = S_NULL;


         s_init (  & help_id, "HELP_ID" ) ;
         var_set(    help_id,  form_id  ) ;
         s_init (  & no_help, "NO_HELP" ) ;

         key_hit = form (  no_help, NIL, HELP_FORM  ) ;

         if( key_hit == FORM_EDIT_HELP )
         {

            edit_help_form = TRUE;

         }

         f_close(  no_help, HELP_FORM );
         var_drp(  help_id            );
         s_drop( & help_id            );
         s_drop( & no_help            );

      }
      else if( key_hit == FORM_EDIT_HELP )
      {

         edit_help_form = TRUE;

      }


      if( edit_help_form )
      {

         rc = t_edtscr( form_id, HELP_FORM, 0 );
         f_close(       form_id, HELP_FORM    );
         done           = FALSE;
         edit_help_form = FALSE;

      }
      else
      {

         done = TRUE;

      }
   }

   f_close(  form_id, HELP_FORM );
   s_drop( & form_id            );
   s_drop( & q_request          );
   s_drop( & command_line       );


}
else
{

   rc = NO_HELP_TO_PROCESS;

}




   return(rc);

}
