/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_dis()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman, Tanmay S. Kumar   |
  |                                                                   |
  |  :DATE.    June 23, 1988                        August 15, 1989   |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *      INPUT  Object Control Block     |
  |                                          of Group      to get     |
  |                                          components from          |
  |                                                                   |
  |  :PARAM.   modifier    string     INPUT  not used.                |
  |  :PARAM.   level       hword      INPUT  not used.                |
  |  :PARAM.   to_do_list  hword  *   INPUT  contains state info      |
  |                                                                   |
  |  :PURPOSE. Serves the feedback purpose to the user after a        |
  |            new Group      is established.                         |
  |            Calls up "3143 form" uses the direct manipulation      |
  |            to pull out the components from the object.            |
  |            If User wants to create another Group,      if YES     |
  |            the whole Group      create process starts.            |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
  |                                                                   |
  |  :CALLS.                                                          |
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


sint grp_dis( group_ocb, modifier, level, to_do_list)

OCB      *group_ocb;
string   modifier;
hword    level;
hword   *to_do_list;

{

   string  CREATE_ANOTHER  =  S_NULL,
           form_id         =  S_NULL;

   sint    start_over      =  0,
           junk            =  0,
           rc              =  0;

   boolean
           show_form       =  TRUE;





   s_init( &CREATE_ANOTHER,  "CREATE_ANOTHER"    );
   s_init( &form_id,         "3123"              );

   while ( show_form && (rc == NO_ERR) )
   {
      rc = t_form( form_id, modifier, group_ocb );

      if ( ( rc == USER_REQUESTED_ENTER )
      ||   ( rc == USER_REQUESTED_PROCESS ) )
      {
         rc = 0;
         start_over = t_valyn( var_get( CREATE_ANOTHER )  );
         if( start_over == TRUE )
         {
             *to_do_list |= CREATE_ANOTHER_GROUP;
             show_form = FALSE;
         }
         else if( start_over == FALSE )
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
      else
      {
         continue;
      }
   }                                 /* end of while loop with any_err */


   s_drop( &CREATE_ANOTHER  );
   s_drop( &form_id         );

   return(rc);

}
