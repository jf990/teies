/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    fld_stem.c                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :DATE.    Aug 8, 1989                                            |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   id       string     INPUT   String containing the field|*/
/*|                                        id of a new field to add to|*/
/*|                                        the list of fields.        |*/
/*|                                                                   |*/
/*|  :PARAM.   f_prof   struct *   INPUT   Forms profile structure of |*/
/*|                                        initial field values.      |*/
/*|                                                                   |*/
/*|  :PURPOSE. Adds the current stem value to the field name          |*/
/*|                                                                   |*/
/*|  :OUTPUT.  current  INF *                                         |*/
/*|                                                                   |*/
/*|  :CALLS.              os_mema(), s_copy()                         |*/
/*|                       s_upper()                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.                                                           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "vars.h"
#include "ti.h"
#include "form_i.h"

sint fld_stem( result, source, stem )

string *result;
string  source;
sint    stem;

{
   string temp1  = S_NULL;
   string temp2  = S_NULL;
   string period = S_NULL;


   if ( stem != 0 )
   {
      s_init  ( &period, "."  );
      s_sitos ( &temp1,  stem );
      s_cat   ( &temp2, source, period, 0, 0 );
      s_cat   ( result, temp2,  temp1,  0, 0 );

      s_drop  ( &period );
      s_drop  ( &temp1  );
      s_drop  ( &temp2  );
   } else
   {
      s_copy  ( result, source, 0, 0 );
   }

   return(0);
}
