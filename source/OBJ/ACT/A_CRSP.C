/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             A_CRSP.C
:LEVEL.                            ACTIVITY
:AUTHOR.                           Heidi E. Harting
:DATE.                             Mar 11, 1988

:INPUT.                conf  :  a TEIES string that contains the number
                                of the conference presently active.

                     comm_no :  a TEIES string containing the number of
                                the conference comment presently "active"

:OUTPUT.                        - none -

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
          This routine will traverse the screens necessary and control
          the variables needed to create a SPREADSHEAT activity.

:CALLS.   t_form.c  s_stosi.c  s_init.c  var_ini.c  var_beg.c  var_end.c

:FORMS.   a_crgr1c

:ERRORS.

:LIMITS.

:MODS.

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

sint a_crsp( act_atch )
sint  act_atch;

{
   OCB     *ocb = NIL;
   OID     *act_oid=NIL;

   string  file = S_NULL, sp_file=S_NULL;
   string  act_name=S_NULL;
   string  rc=S_NULL;           /* return code from  t_form            */

   #D_begin(A_CRSP,ACTIVITY)

   var_ini( &act_name, "ACT_NAME", "SPREADSHEET", C_STRING);

   s_init(&file,"A_CRGR1C");          /* first screen for a gradebook  */
   rc = t_form( file, S_NULL, ocb );  /* rc=NIL means fields fine      */
                                      /* else rc = field id that bombed*/

   #D_return( 0 );
}
