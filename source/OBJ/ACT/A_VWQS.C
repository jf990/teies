/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             A_VWQS.c
:LEVEL.                            ACTIVITY
:AUTHOR.                           Heidi E. Harting
:DATE.                             Apr 28, 1988

:FORMS.  a_vwqs1c
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


sint a_vwqs( ocb, ocd, act_atch )
OCB   *ocb;
OCD   *ocd;
sint  act_atch;

{
   string  file=S_NULL,      rc_form=S_NULL,      comp=S_NULL;
   string  sigs=S_NULL;

   sint    rc_spad=0,        R=0,                 rc=0;
   sint    fld_err=0,        levs=0,              rc_notf=0;

   #D_begin(A_VWQS,ACTIVITY)


   s_init(&file,"A_VWQS1C");

   while ( 1 ) {
      rc_form = t_form( file, S_NULL, ocb );

      /*          Validate Input Values         */
      if ( isv_match( rc_form, ISV_ABANDON, ISV_ABANDON ) != 0 ) {
         break;
         }

      if ( isv_match( rc_form, ISV_QUIT, ISV_QUIT ) != 0 ) {
         break;
         }
      }                          /* end while loop */

   s_drop( &file );
   s_drop( &rc_form );
   s_drop( &comp );
   s_drop( &sigs );

   #D_return( 1 );
}
