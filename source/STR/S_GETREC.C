/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                                          S_GETREC.C
:level.
:AUTHOR                                         sreedhar kosaraju
:DATE                                           Nov 11, 1987
:INPUT                   ocb  : a pointer to object control block

                         line : a pointer to a buffer

:OUTPUT.                 line : fills in the buffer pointed to by
                                line with the contents of object
                                component descriptor.

:GLOBAL.                 -NONE-

:PURPOSE.
        this routine given a pointer to an object control block and a
        pointer to a buffer it fills in the buffer and returns that
        string pointed to by line.

:ERRORS.

:MODS.    25_Jan_88   K.J.Walsh   Updated to use #s_pcheck().

:LIMITS.

:END.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"


#s_c_xtrn();                     /* Externs for #s_check()/#s_pcheck() */


string s_getrec(ocb, ocd, line)

OCB            *ocb;
OCD                 *ocd;
string                   *line;


{
sint rc = 0;


  #D_begin(s_getrec,str);
  #s_pcheck(line);
  #D_str( '*line','String to read into.' );

  if( ocb != NIL )
  {
     if( ocd == NIL )
     {

        #D_say('Warning: NIL object component descriptor!');
        ocd = ocd_new( &ocd, NIL );

     }
     rc = obj_get(ocb, ocd, line);
     if( rc != 0 )
     {

       #D_leave( S_NULL, 'No more records in the Object.' );

     }
     else
     {

       #D_str( '*line', 'results of object get record.' );
       #D_return( '*line' );

     }
   }
#D_leave( S_NULL, 'Bad (NIL) Object Control Block!' );
}

