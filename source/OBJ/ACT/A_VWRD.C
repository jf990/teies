/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             A_VWRD.C
:LEVEL.                            ACTIVITY
:AUTHOR.                           Heidi E. Harting
:DATE.                             Apr 28, 1988

:FORMS.  a_vwrd1c

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


sint a_vwrd( ocb, ocd, act_atch )
OCB   *ocb;
OCD   *ocd;
sint  act_atch;

{
   byte    x_byte=' ';

   sint    rc2=0,             rc_spad=0,             fld_err=0;
   sint    R=0,               rc_notf=0,             x_dash=0;
   sint    n_parts=0,         x_int=0;
   sint    x_comma=0;
   tint    i=0;

   string  file=S_NULL,       rc=S_NULL,             vw=S_NULL;
   string  comma=S_NULL,      result=S_NULL,         sections=S_NULL;
   string  type=S_NULL,       nf=S_NULL,             abstract=S_NULL;
   string  table=S_NULL,      notif_specl=S_NULL,    num_parts=S_NULL;
   string  signature=S_NULL,  l_mand=S_NULL,         limit_opt=S_NULL;
   string  view_before=S_NULL,lmand=S_NULL,          notf=S_NULL;
   string  abs=S_NULL,        tble=S_NULL,           dash=S_NULL;
   string  vw_rng=S_NULL,     all=S_NULL,            sect=S_NULL;
   string  title=S_NULL;
   string  sec_num= S_NULL;
   string  id= S_NULL;

   #D_begin(A_VWRD,ACTIVITY)

   s_init( &title, "TITLE" );
   s_init( &sect, "SECT" );
   s_init( &comma, "," );
   s_init( &dash, "-" );
   s_init( &all, "ALL" );
   s_init( &sections, "SECTIONS" );
   s_init( &vw,       "VW" );

   s_init( &(ocd->name), "ACTIVITY.NUM_PARTS" );
   obj_get( ocb, ocd, &n_parts );
   s_sitos( &num_parts, n_parts );
   #D_str('num_parts','Number of Sections: %s');
   var_set( sections, num_parts );

   s_init( &file, "A_VWRD1C" );         /* first screen for a read act */

   while ( 1 ) {
      rc = t_form( file, S_NULL, ocb );

      if ( isv_match( rc, ISV_ABANDON, ISV_ABANDON ) != 0 )
      {
         #D_say('An Abandon was hit.')
         break;
      }

      if ( isv_match( rc, ISV_QUIT, ISV_QUIT ) != 0 )
      {
         #D_say('A QUIT was hit.')
         break;
      }

      s_copy( &vw_rng, var_get( vw ), 0, 0 );
      s_upper( vw_rng, 0, 0 );

      s_init( &file, "RDVW_TTL" );

      if ( s_comp( all, vw_rng, 0, 0, 'M', &R ) == 0 )
      {
         for ( i=1; i <= n_parts; i++ )
         {
            s_sitos( &sec_num, i );
/*          rc_spad = a_gtpt( file, ocb, i, act_atch, id );      */
            var_set( sect, sec_num );
            f_setfld( file, sect, sec_num );
            rc = t_form( file, S_NULL, ocb );
         }
         break;
      }

      x_comma = s_first( comma, vw_rng, 0, 0 );
      x_dash = s_first( dash, vw_rng, 0, 0 );

/*    if ( (x_comma == 0) && (x_dash == 0) ) {  */   /* single section */
/*         s_stosi( &sec_num, vw_rng, 0, 0 );
         var_set( sect, sec_num );
         f_setfld( file, sect, sec_num );
         i = (tint)vw_rng;
         rc_spad = a_gtpt( file, ocb, i, act_atch, id );
         rc - t_form( file, S_NULL, ocb );                     */
/*       }                                   */
 /*   else {                                             */
 /*      #D_say(' there are commas or dashes in range.');*/
 /*      }                                               */
      }                     /* end of while-loop */

   /* Variables Used in Froms */

   s_drop( &file );
   s_drop( &rc );
   s_drop( &sect );
   s_drop( &sec_num );
   s_drop( &comma );
   s_drop( &dash );
   s_drop( &all );
   s_drop( &sections );
   s_drop( &vw );

   #D_return( 1 );
}
