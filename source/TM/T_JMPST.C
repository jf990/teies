/********************************************************************
<NAME>             T_JMPST.c
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             December 27, 1987
<PARAM>            object    hword     INPUT    Object type for next
                                                state>
<PARAM>            action    hword     INPUT    Action type for next
                                                state.
<PARAM>            level     hword     INPUT    Sub-level of the state
<PARAM>            modifier  string    INPUT    Modifiers passed to
                                                target routine.
<PARAM>            ocb       OCB *     INPUT    On-hand object for use by
                                                the target routine.
<PARAM>            parameters byte *   INPUT    Parameter structure to
                                                pass to target routine.
<OUTPUT>           rc        sint
<PURPOSE>          A new User Interface state is requested, find it and
                   jump to it.  The input string is a teies state id,
                   with object, action, and modifier information. Format
                   is XYZ.xxyyzz where X is the object code, Z is the
                   action code, Y is a modifier for data base searching,
                   xxyyzz are sub-level refinements for that state. This
                   routine should also (eventually) handle object/action
                   code in dynamically loadable object files, interpreted
                   and non-teies compiled routines.  For now, must be
                   properly resolved by the Linker.

                   If both Object and Action are 0, the Level indicates a
                   TM state to jump to.  If either Object or Action are 0
                   then that is a valid (class ANY) state.

<ERRORS>  28121 :  State not defined
          28122 :  System Form Index not found
<LIMITS>           Only can handle code linked in the system, C code only
<MODS>
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"
#include "obj_prim.h"



#define    TM_SELECT_OBJECT       1
#define    TM_SELECT_ACTION       2
#define    TM_SELECT_STATE        3
#define    TM_SELECT_AO_MATRIX    4




sint     t_jmpst( object, action, level, modifier, ocb, params )


hword             object, action, level;
string            modifier;
OCB              *ocb;
byte             *params;


{


sint       rc                   =  0,
           index                =  0,
           selection            =  0,
           function_matrix[ ] = {  TM_SELECT_STATE,
                                     TM_SELECT_ACTION,
                                     TM_SELECT_OBJECT,
                                     TM_SELECT_AO_MATRIX  };
boolean    selection_ok = FALSE;
hword      key_pressed  =     0;
string
           SELECT_VAR= S_NULL,
           state     = S_NULL,
           state_var = S_NULL;
OBJ_ACT    oa                ;






if( (action > NUMBER_OF_ACTIONS)  || (action < 0)    ||
    (object > NUMBER_OF_OBJECTS)  || (object < 0)      )
{

   rc = except( STATE_NOT_DEFINED, CONTROL_ERROR, S_NULL, S_NULL, S_NULL,
                                                  S_NULL, S_NULL );
   if( TM_master->usrtype == REAL_USER )
   {

      s_init( & state_var, "NOSTATE"   );
      t_form(   state_var,  NIL, NIL    );
      f_close(  state_var,  STATE_FORM  );

   }
}
else
{

   if( action != 0 )
   {

      index += 2;

   }
   if( object != 0 )
   {

      index += 1;

   }

   s_init( & state_var, "%STATE"   );
   state = (string) var_get( state_var     );
   if( s_len( state ) > 3 )
   {

      byte   * state_position = NIL ;


      state_position = s_buffer( state );

      * (state_position + 0) = (byte) action + 0xF0 ;
      * (state_position + 1) = 0xF0 ;
      * (state_position + 2) = (byte) object + 0xF0 ;
      * (state_position + 3) = (byte) level  + 0xF0 ;
      * (state_position + 4) = NIL ;

   }

   switch( function_matrix[ index ] )
   {

   case TM_SELECT_AO_MATRIX :

      oa = AOM[ action-1 ][ object-1 ] ;
      if( oa.linked_code != NIL )
      {

          rc = (*oa.linked_code)( ocb, modifier, level, params );

      }
      else
      {

         rc =except( STATE_NOT_CODED, CONTROL_ERROR, S_NULL, S_NULL,
                                             S_NULL, S_NULL, S_NULL );
         if( TM_master->usrtype == REAL_USER )
         {

            s_init( & state_var, "NOSTATE"  );
            t_form(   state_var,  NIL, NIL  );
            f_close(  state_var,  STATE_FORM);

         }
      }
      break;

   case TM_SELECT_STATE :

      switch( level )
      {

      case TM_UPDATE_USER_COUNTERS :

         rc = t_update_user_counters( ocb );
         break;

      case TM_DISPLAY_CONNECTIONS  :

         break;

      case TM_SYSTEM_MONITOR_MENU  :

         break;

      case TM_SYSTEM_COMMAND_MENU  :

         break;

      case TM_GET_LAST_USER_STAT_RECORD :

         rc = t_get_last_stat_record( ocb, (string *) params, FALSE );
         break;

      default :

         break;

      }
      break;

   case TM_SELECT_ACTION :

      s_init( & SELECT_VAR, "SELECTION" );

      while( ! selection_ok )
      {

         key_pressed = t_form( state, NIL, NIL );

         if( t_ifquit( key_pressed ) )
         {

            selection_ok = FALSE;
            break;

         }
         s_stosi( & selection, var_get( SELECT_VAR ), 0, 0 );
         if( selection > 0 && selection <= NUMBER_OF_ACTIONS )
         {

            selection_ok = TRUE;
            break;

         }
         else
         {

            except( INVALID_CHOICE, USER_WARNING, var_get( SELECT_VAR ),
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( state, STATE_FORM, SELECT_VAR, 0.0, 0.0 );

         }
      }
      if( selection_ok )
      {

         rc = t_jmpst( object, selection, level, modifier, ocb, params );

      }
      s_drop( & SELECT_VAR );
      break;

   case TM_SELECT_OBJECT :

      s_init( & SELECT_VAR, "SELECTION" );

      while( ! selection_ok )
      {

         key_pressed = t_form( state, NIL, NIL );

         if( t_ifquit( key_pressed ) )
         {

            selection_ok = FALSE;
            break;

         }
         s_stosi( & selection, var_get( SELECT_VAR ), 0, 0 );
         if( selection > 0 && selection <= NUMBER_OF_OBJECTS )
         {

            selection_ok = TRUE;
            break;

         }
         else
         {

            except( INVALID_CHOICE, USER_WARNING, var_get( SELECT_VAR ),
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( state, STATE_FORM, SELECT_VAR, 0.0, 0.0 );

         }
      }
      f_close( state, STATE_FORM );
      if( selection_ok )
      {

         rc = t_jmpst( object, selection, level, modifier, ocb, params );

      }
      s_drop( & SELECT_VAR );
      break;

   default :

      break;

   }
}
s_drop( & state_var );



   return(rc);

}
