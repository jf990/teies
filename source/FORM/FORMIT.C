/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    Formit()                                               |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    August 12, 1988                                        |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  None.                                                  |
  |  <PARAM>   f_prof  struct f_prof_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains default & parsed  |
  |                                        qualifier values.          |
  |  <PARAM>   f_fmat  struct f_fmat_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains info required to  |
  |                                        render data properly in    |
  |                                        the GKS world.             |
  |  <PURPOSE> Test routine for displaying forms.                     |
  |  <OUTPUT>  void                                                   |
  |  <CALLS>   form()                                                 |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
  |  <MODS>    JF Nov-7-88         Changed main form from NEW_FORM to |
  |                                FFORMIT FORM.                      |
  |            JF May-3-89         Allowed command line FORM id spec. |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "ti.h"
#include "form.h"
#include "teies.h"





main(  argc,  argv,  envp  )


sint   argc;
byte       ** argv;
byte               * envp;


{


   sint       rc           =      0,
              key_hit      =      0,
              flag         =      0;
   string     holder       =  S_NULL,
              form_id      =  S_NULL,
              formit_id    =  S_NULL,
              read_id      =  S_NULL,
              var_name     =  S_NULL,
              end          =  S_NULL,
              message      =  S_NULL,
              message_area =  S_NULL;
   boolean    interactive  =    TRUE,
              done         =   FALSE;






if( argc > 1 )
{

   interactive = FALSE ;

}
else
{

   interactive = TRUE ;

}

s_init( & formit_id,    "FFORMIT" ) ;
s_init( & form_id,      "FFORMIT" ) ;
s_init( & var_name,     "FORM_ID" ) ;
s_init( & end,          "END"     ) ;
s_init( & message_area, "%EXC"    ) ;

t_init (  S_NULL, S_NULL, 0  );

while( ! done )
{

   if( ! interactive )
   {

      if( -- argc < 1 )
      {

         done = TRUE;
         break;

      }
      ++ argv ;
      s_init( & form_id, * argv );

   }

   s_copy( & message, (string) var_get( message_area ), 0, 0 );
   if( s_len( message ) < 1 )
   {

      s_pad( & message, S_NULL, 79, ' ', 'L' );

   }
   f_setfld( form_id, message_area, message );
   var_clr(  message_area );
   s_drop( & message      );

   f_rstinf( form_id, NO_FLAGS   );

   key_hit = form (  form_id, NULL, NO_FLAGS  ) ;


   if ( key_hit != FORM_NOT_FORMED )
   {

      t_updfld ( form_id ) ;

      if( key_hit == FORM_ABANDON )
      {

         done = TRUE;
         rc   = USER_REQUESTED_ABANDON;

      }
      else
      {

         if( interactive )
         {

            read_id = (string) var_get ( var_name ) ;
            if( s_len( read_id ) > 0 )
            {

               s_upper ( read_id, 0, 0 ) ;
               if( (rc = s_comp ( read_id, end, 0, 0, 'M', &flag )) != 0)
               {
                  rc = (key_hit != FORM_QUIT) ;
               }

               if (rc != 0)
               {

                  rc = 0;
                  key_hit = form( read_id, NULL, NO_FLAGS   ) ;
                  if( key_hit == FORM_NOT_FORMED )
                  {

                     except( FORM_TO_DISPLAY_NOT_FOUND, USER_ERROR,
                              read_id, S_NULL, S_NULL,  S_NULL, S_NULL );

                  }
                  else
                  {

                     f_close (  read_id, NO_FLAGS ) ;

                  }
               }
               else
               {

                  rc = USER_REQUESTED_QUIT;
                  done = TRUE;

               }
            }
         }
      }
   }
   else
   {

      except( FORM_TO_DISPLAY_NOT_FOUND, USER_ERROR, read_id,
                                 S_NULL, S_NULL,  S_NULL, S_NULL );

   }

   if( rc != 0 )
   {

      done = TRUE ;

   }
   if( s_comp( form_id, formit_id, 0, 0, 'M', & flag ) != 0 )
   {

      f_close(   form_id, NO_FLAGS   ) ;

   }
}

s_drop (  & form_id          ) ;
s_drop (  & message_area     ) ;
s_drop (  & var_name         ) ;
s_drop (  & end              ) ;
s_drop (  & holder           ) ;
s_drop (  & formit_id        ) ;
s_drop (  & var_name         ) ;

t_shutdn();


   return(0);
}
