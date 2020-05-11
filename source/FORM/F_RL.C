/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_rl()                                                 |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 7, 1989                                            |
  |  <LEVEL>   FORMS                                                  |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Process the end markup for a repeating list.  Markup   |
  |            format is:  </rl>                                      |
  |                                                                   |
  |            For repeating lists all the hard work is done here.    |
  |            Once we know the text of the list we need to parse it  |
  |            for each entry in the list object.  To do so we can    |
  |            call the parser for each entry and tweek the object to |
  |            parse as the list item text and the object to use for  |
  |            direct manipulation as the list entry.  To do this we  |
  |            step through each list entry, open it, and store the   |
  |            OCB in the profile for this form.  When done, replace  |
  |            the original OCB in the profile.                       |
  |                                                                   |
  |            The form for each list entry can come from one of two  |
  |            places, either the <rl> markup defined a form to use   |
  |            or a <li> was provided after the <rl>.  (This version  |
  |            does not support the latter).                          |
  |                                                                   |
  |            Object to parse:      f_prof->ocb, f_prof->ocd         |
  |            Object to manipulate: f_fmat->ocb                      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>  <li> tag not yet supported.                            |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "stack.h"
#include "vars.h"
#include "form_i.h"




sint   f_rl (   f_prof, f_fmat  )

struct f_prof_s        * f_prof;
struct f_fmat_s        * f_fmat;


{

   sint        rc              =   0,
               stack_point     =   0;
   OCB       * form_ocb        = NIL,
             * next_object     = NIL,
             * list_object     = NIL;
   OCD       * list_component  = NIL,
             * form_ocd        = NIL;
   OID         next_oid             ;




if( f_fmat != NIL && f_prof != NIL )
{

   list_object = f_fmat->ocb;
   if( OCB_POINTER_OK( list_object ) && ( list_object->type == LIST_OBJ ))
   {

      ocd_new( & list_component, "LIST.LIST.OID" );
      list_component->bias = 1;

      while( rc == 0 )
      {

         rc = obj_get( list_object, list_component, & next_oid );
         if( rc == 0 )
         {

            next_object = obj_opn( & next_oid, OBJ_UPDATE );
            if( OCB_POINTER_OK( next_object ) )
            {


               f_fmat->ocb = next_object;

                          ocd_new ( &form_ocd, NIL);
               form_ocb = obj_opn ( (OID *) f_prof->rl_name,
                                                    OBJ_READ | OS_FILE );
               form_ocb->status = 1;
               f_fmat->rl_item += 1;
               f_parse( form_ocb, form_ocd );

               obj_cls( next_object );
               obj_cls( form_ocb    );

            }
         }
      }
      ocd_drp( & form_ocd  );
      ocd_drp( & list_component );
      f_fmat->ocb = list_object;

   }
   else
   {

      rc = IMPROPER_OBJECT_TYPE;

   }
}
else
{


}

f_fmat->rl_item = 0;

   return(rc);

}
