/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_cfrm()                                             |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INPUT  Object Control Block     |
  |                                          of Conference to get     |
  |                                          components from          |
  |                                                                   |
  |  <PARAM>   modifier    string     INPUT  not used.                |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   cr_params   struct *   INPUT  not used.                |
  |                                                                   |
  |  <PURPOSE> Serves the feedback purpose to the user after a        |
  |            new conference is established.                         |
  |            Calls up "3143 form" uses the direct manipulation      |
  |            to pull out the components from the object. User is    |
  |            if he wants to create another conference, if YES       |
  |            the whole conference create process starts.            |
  |  <OUTPUT>  returns 1  if another conference is to be created      |
  |                       so that CFR_INFO() should be called         |
  |                                                                   |
  |            returns 8  if the process is over                      |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_cr.h"



sint cfr_cfrm( conf_ocb, modifier, level, fld_todo )

OCB      *conf_ocb;
string   modifier;
hword    level;
hword   *fld_todo;

{

   string  CREATE_ANOTHER  =  S_NULL,
           form_id         =  S_NULL;

   sint    start_over      =  0,
           junk            =  0,
           rc_form         =  0,
           rc              =  0;

   boolean
           show_form       =  TRUE;





   s_init( & CREATE_ANOTHER,  "CREATE_ANOTHER"    );
   s_init( & form_id,         "3143"              );

   while ( show_form && (rc == NO_ERR) )
   {

      rc_form = t_form( form_id, modifier, conf_ocb );

      if ( rc_form == USER_REQUESTED_ABANDON
      ||   rc_form == USER_REQUESTED_QUIT    )
      {
          rc = rc_form;
      }
      else
      {

         start_over = t_valyn( var_get( CREATE_ANOTHER )  );
         if( start_over == TRUE )
         {

             *fld_todo |= CREATE_ANOTHER_CONFERENCE;
             show_form = FALSE;

         }
         else
         if( start_over == FALSE )
         {

            show_form = FALSE;

         }
         else
         {
             f_curpos( form_id, NO_FLAGS, CREATE_ANOTHER, 0.0, 0.0 );
             except( Yes_Or_No_Was_Expected, USER_ERROR,
               var_get(CREATE_ANOTHER), S_NULL, S_NULL, S_NULL, S_NULL);

         }
      }
   }                                 /* end of while loop with any_err */


   s_drop( &CREATE_ANOTHER  );
   s_drop( &form_id         );

   return(rc);

}
