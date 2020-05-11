/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_fmod()                                               |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 12, 1988                                        |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   form_id     string     INPUT   Form to determine.      |
  |  :PARAM.   form_flags  hword      INPUT   Needed to find the form.|
  |  :PARAM.   field_id    string     INPUT   Field to determine.     |
  |                                                                   |
  |  :PURPOSE. Determine if the field indicated has been modified by  |
  |            the user the last time it has been displayed.  This    |
  |            routine will not reset the modified flag.              |
  |                                                                   |
  |  :OUTPUT.  boolean          TRUE if the field was modified,       |
  |                             FALSE if 1) not modified or 2) form   |
  |                                   does not exist.                 |
  |                                                                   |
  |  :CALLS.   f_inta().                                              |
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
#include "form_i.h"



boolean     f_fmod(  form_id,  form_flags,  field_id  )

string               form_id,               field_id;
hword                          form_flags;


{

   boolean      rc            =FALSE;
   form_dat    *fcb           =  NIL;
   INF         *current_field =  NIL;
   sint         x             =    0;






   if( (form_dat *)0xFFFFFFFF != (fcb = frm_inta( form_id, form_flags )))
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if(( s_comp( current_field->ocd->name, field_id, 0, 0, 'M', &x)
              == 0 )                                     &&
             ( current_field->ftype == FLD_INPUT )       &&
             ( current_field->flags & F_MOD   == F_MOD ) )
         {

            rc = TRUE;
            break;

         }
         current_field = current_field->next;

      }
   }
   else
   {

      rc = FALSE;

   }



   return(rc);

}
