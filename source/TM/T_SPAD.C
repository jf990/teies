/********************************************************************

:Name.   T_SPAD.C
:author. JF
:date.   November 9, 1987
:level.  TM
:purpose.
    Start a new transaction manager process for the scratchpad to
    edit a file.  By default the file will be A0 * TEIES $SP unless
    the first argument is other than S_NULL.

:parm.   spad_file  string  INPUT  filename to edit
:parm.   *result    string  OUTPUT  Real file that was edited.
:output. result     sint    OUTPUT  Requested ending function.
:ERRORS:

        49098      Not enough memory to start another TM process.
        48001      No process entries.
:GLOBAL:

        TM_master  Pointer to TM process table.
:calls.
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "exc.h"
#include "osk.h"
#include "tm.h"
#include "form.h"



sint     t_spad ( spad_file, result_str )

string            spad_file,
                * result_str;


{

sint             rc =    0  ,
             result =  FORM_PROCESS;
tmentry       *temp =  NIL  ;
string   scratchpad =  S_NUL;
boolean  init_scr   =  TRUE ;





if( spad_file == S_NULL )
{

    rc                 =  s_init ( &scratchpad, "A * TEIES $SP");

}
else
{

    rc                 =  s_copy( &scratchpad, spad_file, 0, 0 );

}

                                       /*+---------------------------+*/
                                       /*| Check the codes returned  |*/
                                       /*| by the editor to determine|*/
                                       /*| how to save the file and  |*/
                                       /*| return to the caller.     |*/
                                       /*+---------------------------+*/

                      os_edtr(  scratchpad, & rc, init_scr );

                                       /*+---------------------------+*/
                                       /*| If the caller supplied a  |*/
if( result_str != NIL )                /*| pointer then copy the real|*/
{                                      /*| SPAD file we used so they |*/
                                       /*| know where their data is. |*/
                                       /*+---------------------------+*/
   s_copy( result_str, scratchpad, 0, 0 );

}
s_drop( & scratchpad );



   return(result);

}
