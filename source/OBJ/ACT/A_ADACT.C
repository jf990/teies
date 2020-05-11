/************************************************************************

    <DOC>

    <NAME>         a_adact( OCB *act_ocb, sint act_type)

    <LEVEL>        Activity

    <DATE>         March 8, 1988     March 1, 1989

    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar

    <GLOBAL>       None.

    <PARAM>        act_ocb    OCB *   INPUT  The ocb of the activity.
                   act_type   sint    INPUT  Type of the activity

    <OUTPUT>       Sint.

    <PURPOSE>      The purpose of this routine is to fill in the activity
                   object with common necessary information for all types
                   of activities.

    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), Obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr().

    <ERRORS>       None.

    <LIMITS>       None.

    <MODS>         March 1, Rewritten routine to specifications.

    <END>

    </DOC>

************************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "exc.h"
#include "activity.h"

sint a_adact( act_ocb, act_type )

OCB     *act_ocb;
sint    act_type;

{

   OCD         *act_ocd      =       NIL;

   byte        x_byte        =       '\0';

   /* defining the variables for the activity object */

   string      signature     =    S_NULL,
               limit_mand    =    S_NULL,
               num_parts     =    S_NULL,
               view_before   =    S_NULL,
               by_per        =    S_NULL,
               sequed        =    S_NULL,
               to_do         =    S_NULL,
               stat          =    S_NULL;

   sint        x_int         =         0,
               rc            =         0;


   s_init( &signature, "SIGNATURE" );
   s_init( &limit_mand, "LIMIT_MAND" );
   s_init( &num_parts, "NUM_PARTS" );
   s_init( &view_before, "VIEW_BEFORE" );
   s_init( &by_per, "BY_PER" );
   s_init( &sequed, "SEQUED" );
   s_init( &to_do, "TO_DO" );
   s_init( &stat, "STAT" );


   ocd_new( &act_ocd, "ACTIVITY.AUTHOR" );
   obj_rep(act_ocb, act_ocd, TM_master->ucb->name, SEG_UPDATE);

   if ( act_type == QUESTION )
   {
      if ( t_valyn(var_get(view_before)) == 1)
      {
         x_byte |= VW_PER;
      }
      if ( t_valyn(var_get(by_per)) == 1)
      {
         x_byte |= BY_PER;
      }
      s_init( &(act_ocd->name), "ACTIVITY.VIEW_BEFORE");
      obj_rep( act_ocb, act_ocd, &x_byte, SEG_UPDATE );

      s_stosi( &x_int, var_get( num_parts ), 0, 0 );
      s_init( &(act_ocd->name), "ACTIVITY.LEVELS");
      obj_rep( act_ocb, act_ocd, &x_int, SEG_UPDATE);

   }
   if ( (act_type == QUESTION) || (act_type == SELECTION) )
   {
      if (act_type == QUESTION)
      {
         x_byte = (byte) t_valyn( var_get( limit_mand ) );
      }
      else
      {
         s_stosi(&x_int, var_get(limit_mand), 0, 0);
         x_byte = (byte)x_int;
      }
      s_init( &(act_ocd->name), "ACTIVITY.LIMIT_MAND");
      obj_rep( act_ocb, act_ocd, &x_byte, SEG_UPDATE );

      s_init( &(act_ocd->name), "ACTIVITY.SIGNATURE");
      obj_rep(act_ocb,act_ocd,&(s_char_s((string) var_get(signature),0)),
                                                           SEG_UPDATE);
   }
   s_init( &(act_ocd->name), "ACTIVITY.STATUS");
   obj_rep(act_ocb,act_ocd,&(s_char_s( (string) var_get(stat),0)),
                                                            SEG_UPDATE);

   s_init( &(act_ocd->name), "ACTIVITY.REQUIRED");
   obj_rep(act_ocb,act_ocd,&(s_char_s( (string) var_get(to_do),0)),
                                                            SEG_UPDATE);

   x_byte = (byte) t_valyn( var_get( sequed ) );
   s_init( &(act_ocd->name), "ACTIVITY.SEQUENCED");
   obj_rep( act_ocb, act_ocd, &x_byte, SEG_UPDATE );

   ocd_drp( &act_ocd );

   s_drop( &signature );
   s_drop( &limit_mand );
   s_drop( &num_parts );
   s_drop( &view_before );
   s_drop( &by_per );
   s_drop( &stat );
   s_drop( &to_do );
   s_drop( &sequed );


   return(rc);
}
