/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 T_ver()

:LEVEL.                TM

:AUTHOR.               John L. Foster

:DATE.                 July 14, 1988

:OUTPUT.               string : result

:INPUT_GLOBALS.        TM_master

:OUTPUT_GLOBALS.

:PURPOSE.  Return the TEIES version identifier in the format:

                       mmddyy.vvv

           for example, a version build on this date would be
                       071488.100

:LIMITS.

:MODS.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"



string   t_ver(  )



{


sint    rc        =      0;
string
        ver_str   = S_NULL,
        rel_str   = S_NULL;







   s_sitos(   &rel_str,   TM_master->release                );
   s_init(    &ver_str,   "."                               );
   s_cat(     &rel_str,   ver_str, rel_str, 0, 0            );
   s_cat(     &ver_str,   TM_master->version, rel_str, 0, 0 );



   s_drop( &rel_str );



   return(ver_str);

}
