/********************************************************************

:Name.   T_delsp.c
:author. JF
:date.   March 3, 1988
:level.  TM
:purpose.
         Clear the Scratchpad.

:parm.   spad_file  string  INPUT    Scratchpad filename to clear. If NIL
                                     then clear * * TEIES $SP.
:ERRORS:
:GLOBAL:
:calls.  os_gfdd()
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "exc.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint     t_delsp( spad_file )

string            spad_file;


{

sint             rc =    0  ;
tmentry       *temp =  NIL  ;
string   scratchpad =  S_NUL;




if( spad_file == S_NULL )
{

    rc                 =  s_init ( & scratchpad, "A * TEIES $SP" );

}
else
{

    rc                 =  s_copy( & scratchpad, spad_file, 0, 0 );

}
rc = os_gfdd( scratchpad );


s_drop( &scratchpad );


   return(rc);

}
