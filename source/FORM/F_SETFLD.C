/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_setfld.c                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   form_id    string         INPUT  Form to use.          |*/
/*|  :PARAM.   field_id   string         INPUT  Field in that form.   |*/
/*|  :PARAM.   value      string         INPUT  Contents to set to.   |*/
/*|                                                                   |*/
/*|  :PURPOSE. Set the field of a particular form to some contents.   |*/
/*|            Error if the field does not exist.                     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  sint.  0 if field set, -1 if field, form does not exist|*/
/*|                                                                   |*/
/*|  :CALLS.   stk_get(), gs_high(), gs_scolor(), os_memd(), gcls_seg)|*/
/*|            s_copy(), f_render(), gcreat_seg()                     |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    03_Mar_88   J.L.Foster                Written.         |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "debug.h"
#include "form_i.h"




sint f_setfld( form_id, field_id, new_field_contents )

string         form_id, field_id, new_field_contents;

{

   sint         rc            =   0 ,
                justification =   0 ,
                f_len         =   0 ;
   byte         pad_c         =   0 ;
   form_dat    *fcb           =  NIL;
   INF         *current_field =  NIL;
   string       value         =S_NUL;
   boolean      found         = FALSE;






   s_trim(     &form_id, 'B'           );
   s_upper(     form_id,  0,  0        );
   s_trim(     &field_id, 'B'           );
   s_upper(     field_id,  0,  0        );



   fcb = frm_inta( form_id, NO_FLAGS );
   if( fcb != NIL && fcb != (form_dat *) -1 )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( s_comp( current_field->ocd->name,field_id,0,0,'M',&rc ) )
         {

            current_field = current_field->next;

         }
         else
         {

            found = TRUE;
            f_len         = current_field->len;
            pad_c         = current_field->pad;
            justification = current_field->just;
            s_copy( &value, new_field_contents, 0, 0 );
            s_trim( &value, 'B' );
            if( s_len( value ) < 1 )
            {

                s_init( & value, " " );
                * s_buffer( value ) = pad_c;

            }
            switch( justification )
            {

               case L_JUST : s_pad ( & value, value, f_len, pad_c, 'R' );
                             break;
               case R_JUST : s_pad ( & value, value, f_len, pad_c, 'L' );
                             break;
               case C_JUST : s_pad ( & value, value, f_len, pad_c, 'C' );
                             break;
                   default : s_pad ( & value, value, f_len, pad_c, 'L' );

            }
            rc = s_copy( & (current_field->contents), value, 0, 0);
            s_drop( & value );
            break;

         }
      }
   }

   if( ! found )
   {

      rc = -1;

   }
   else
   {

      rc = 0;

   }


   return(0);
}
