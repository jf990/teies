/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             DBM_IXP.C
:LEVEL.                            independent
:AUTHOR.                           Ajaz-Ur-Rehman Rana
:DATE.                             March 16, 1990

:INPUT.                         - none -

:OUTPUT.                        - none -

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
          This routine starts testing procrss for
          obj_expo and obj_impo
:CALLS.

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "debug.h"
#include "isv.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "exc.h"
#include "form.h"
#include "vars.h"
#include "teies.h"

main()

{
   string   modifier          = S_NULL,
            form_id           = S_NULL,

            CHOICE            = S_NULL,
            quit              = S_NULL,
            abandon           = S_NULL;

   hword    rc_form           = 0,
            selection         = 0;

   sint     rc                = 0,
            R                 = 0;

   boolean  DONE              = FALSE;

   #D_main(DBM_IXP, RANA);
   t_init( S_NULL, S_NULL, NIL );

   t_s_on ( 0 );

   s_init ( &form_id,  "DBM_IXP"      );
   s_init ( &CHOICE,   "CHOICE"       );

   isv_get( &quit,     ISV_QUIT,    0 );
   isv_get( &abandon,  ISV_ABANDON, 0 );

   while( ! DONE )
   {

      rc_form = t_form( form_id, S_NULL, NIL );
      if( ! t_ifquit( rc_form ) )
      {

         selection = 0;
         s_stoh( & selection, var_get( CHOICE ), 0, 0 );
         #D_show( 'Selection = %d', 'selection'       );

         switch( selection )
         {

            case 1 :

                 #D_say( 'Calling dbm_xind' );
                 rc = dbm_xind( );
                 #D_show( 'rc of dbm_xind = %d', 'rc' );

            case 3 :

                 #D_say( 'Calling dbm_xall' );
                 rc = dbm_xall( );
                 #D_show( 'rc of dbm_xall = %d', 'rc' );


            case 5 :

                 #D_say( 'Calling tobjexpo' );
                 rc = tobjexpo();

                 break;

            case 6 :

                 #D_say( 'Calling tobjimpo' );
                 rc = tobjimpo();

                 break;

            case 7 :

                 #D_say( 'Stop the test process' );
                 rc = 0;
                 DONE = TRUE;

                 var_clr( CHOICE );
                 f_rstfld( form_id, NO_FLAGS, CHOICE );

                 break;

            default :

                 #D_say( 'Invalid choice made' );
                 except( INVALID_CHOICE, USER_ERROR, var_get( CHOICE ),
                                      S_NULL, S_NULL, S_NULL, S_NULL );
                 break;


         } /* end switch */

      }
      else
      {

         #D_say('time to stop' );
         rc = rc_form;
         DONE = TRUE;

      }

   } /* end while */

   f_close( form_id, NO_FLAGS );

   s_drop ( & form_id );
   s_drop ( & CHOICE  );

   t_shutdn( );
   #D_return(rc)
}
