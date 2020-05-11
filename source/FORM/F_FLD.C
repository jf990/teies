/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    f_field.c                                              |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <DATE>    November 30, 1987                                      |*/
/*|  <LEVEL>   FORMS.FFUN                                             |*/
/*|  <GLOBAL>  * none *                                               |*/
/*|                                                                   |*/
/*|  <PARAM>   f_prof  *f_prof_s   INOUT   Pointer to the parser pro- |*/
/*|                                        file structure containing  |*/
/*|                                        parsed and default info.   |*/
/*|                                                                   |*/
/*|  <PARAM>   f_fmat  *f_fmat_s   INOUT   Pointer to formatting info.|*/
/*|                                                                   |*/
/*|  <PURPOSE> Processes <field> markups, creating a new field entry  |*/
/*|            structure and fill it in with values set by the parser.|*/
/*|            Later f_sfield() will use those values to place the    |*/
/*|            fields in the world.  Finally, a set of manipluation   |*/
/*|            routines will be used by the TM/Object level to work   |*/
/*|            with the field input and deallocate when done.         |*/
/*|                                                                   |*/
/*|            The field contents are take from the database or var-  |*/
/*|            able definition *before* the default setting.          |*/
/*|                                                                   |*/
/*|  <OUTPUT>  sint                                                   |*/
/*|  <CALLS>   obj_get(), os_mema(), except(), s_copy(), var_get()    |*/
/*|            var_ini(), s_upper(), s_just()                         |*/
/*|  <ERRORS>                                                         |*/
/*|  <LIMITS>                                                         |*/
/*|  <MODS>                                                           |*/
/*|           07_July_88  JF   Corrected field setting from object    |*/
/*|                            component and conflict with default    |*/
/*|                            setting.                               |*/
/*|                                                                   |*/
/*|           19_Oct_88   JF   ReCorrected the computation of the     |*/
/*|                            field length when coming out of an     |*/
/*|                            object and the component is NIL.       |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "vars.h"
#include "form_i.h"





sint f_fld(f_prof,f_fmat)

struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;

{



   sint        rc          =  0     ,  /* RC from things               */
               f_len       =  0     ;  /* size of the field            */
   string      var_value   =  S_NULL,  /* holder of field contents     */
               blank_field =  S_NULL;  /* hlder string for default fld */
   INF        *current     =  NIL   ;  /* Holder for current inf values*/
   byte        pad_c       =  '\0'  ;  /* Padding character requested  */
   ODT        *odt_ptr     =  NIL   ;  /* Pointer to the current ODT   */
   OCD        *ocd         =  NIL   ;  /* Pointer to the current OCD   */
   OCB       * ocb         =  NIL   ;  /* Pointer to the current OCB   */
   boolean     found       =  FALSE ;  /* TRUE if contents found       */








   current    = fld_new( f_prof );
   ocb        = (OCB *)current->ocb;
   ocd        = (OCD *)current->ocd;
   pad_c      = current->pad;
   f_len      = current->len;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Now decide where the value comes from and set it.               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/



   if( current->id == S_NULL && ocd->name != S_NULL )    /**************/
   {                                                     /*If the field*/
       s_copy( &current->id, ocd->name, 0, 0 );          /*then create */
                                                         /*a dummy     */
   }                                                     /*field.      */
   else if( current->id != S_NULL && ocd->name == S_NULL)/*            */
   {                                                     /*            */
                                                         /*            */
       s_copy( &ocd->name, current->id, 0, 0 );          /**************/

   }
   else if( current->id == S_NULL )
   {

       var_init( & ocd->name, "NULL_FIELD", "NULL_FIELD", OC_CSTR );
       s_init(   & current->id,   "NULL_FIELD_ID" );

   }

   if ( ! PROFILE(io_var) )
   {
      if( PROFILE(io_otype) != S_NULL )
      {


      }
      /*+-------------------------------------------------------------+*/
      /*|   Fetch actual contents from the object and put into INF    |*/
      /*+-------------------------------------------------------------+*/
      if( ocb == NIL || ocd->name == S_NULL )
      {


      }
      else
      {

         odt_ptr = odt_find( ocb, ocd );
         if( odt_type( odt_ptr ) != OC_STR )
         {

            rc = ocd_ctos( ocb, ocd, &(var_value) );

         }
         else
         {

            rc = obj_get( ocb, ocd, &(var_value) );

         }
      }


      if( rc == 0 )
      {

         found = TRUE;

      }
      if(  ( s_len( var_value ) < 1 )   &&   ( f_len < 1 )  )
      {

         if ( current->ftype == FLD_INPUT  )
         {

            f_len = current->len = odt_len(  odt_ptr, ocb->type  );

         }
         else
         {

            s_ctos ( & var_value, pad_c ) ;
            f_len = current->len = 1 ;

         }
      }
   }
   else
   {


      /*+-------------------------------------------------------------+*/
      /*|   Search TM/Object User Interface variables for this one    |*/
      /*|     If a variable, we must make sure the ocb is NIL         |*/
      /*|     so the other routines know not to check an object.      |*/
      /*+-------------------------------------------------------------+*/

      current->ocb = NIL;
      s_copy( &var_value, (string) var_get( ocd->name ), 0, 0 );
      if ( var_value == S_NULL )
      {

         s_ctos( & blank_field, pad_c ) ;
         var_set(  ocd->name, blank_field );
         s_drop( & blank_field );

      }
      else
      {

         found = TRUE;

      }                                  /*+- - - - - - - - - - - - - +*/
   }                                     /*|If there is a default set,|*/
   if( (PROFILE( io_def ) != S_NULL)     /*|make sure the var/obj know|*/
       && ( ! found )               )    /*|about it.                 |*/
   {                                     /*+ - - - - - - - - - - - - -+*/

      s_copy( & var_value,  PROFILE( io_def ), 0, 0 );
      if( PROFILE( io_var ) )
      {

         var_set( ocd->name, var_value );

      }
      else
      {

         ocd_stoc( ocb, ocd, var_value );

      }
   }
   s_trim(  & var_value, 'B' );
   if( s_len( var_value ) < 1 )
   {

      s_ctos ( & var_value, pad_c ) ;

   }



   if( f_len > 0 )
   {


      switch( PROFILE( io_just ) )
      {

        case L_JUST : s_pad( & var_value, var_value, f_len, pad_c, 'R' );
                      break;
        case R_JUST : s_pad( & var_value, var_value, f_len, pad_c, 'L' );
                      break;
        case C_JUST : s_pad( & var_value, var_value, f_len, pad_c, 'C' );
                      break;
        default     : s_pad( & var_value, var_value, f_len, pad_c, 'L' );
                      break;

      }

   }

   rc = s_copy( & (current->contents), var_value, 0, 0 );
   rc = s_copy( & (current->init_val), var_value, 0, 0 );
   rc = s_copy( & (current->prv_cont), var_value, 0, 0 );
   s_drop( & var_value );


   return(0);
}
