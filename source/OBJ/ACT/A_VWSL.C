/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             A_VWSL.C
:LEVEL.                            ACTIVITY
:AUTHOR.                           Heidi E. Harting
:DATE.                             Apr 28, 1988

:FORMS.  a_vwsl1c

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "isv.h"
#include "activity.h"


sint a_vwsl( ocb, ocd, act_atch )
OCB   *ocb;
OCD   *ocd;
sint  act_atch;

{
   string  file = S_NULL, rc=S_NULL, comp=S_NULL;
   string  sigs=S_NULL, sp_file=S_NULL;
   sint    rc_spad=0, R=0, fld_err=0;


   #D_begin(A_VWSL,ACTIVITY)

   s_init(&file,"A_VWSL1C");

   while ( 1 ) {
      rc = t_form( file, S_NULL, ocb );

      if ( isv_match( rc, ISV_ABANDON, ISV_ABANDON ) != 0 ) {
         #D_say('Abandon was hit.')
         break;
         }

      if ( isv_match( rc, ISV_QUIT, ISV_QUIT ) != 0 ) {
         #D_say('Quit was hit');
         break;
         }
      }                          /* end the while-loop of creating    */

   s_drop( &file );
   s_drop( &comp );
   s_drop( &rc );

   #D_return( 0 );
}
