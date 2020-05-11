/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    os_call C                                              |*/
/*|                                                                   |*/
/*|  :Author.  Kevin J. Walsh                                         |*/
/*|                                                                   |*/
/*|  :DATE.    20 Apr 87                                              |*/
/*|                                                                   |*/
/*|  :GLOBALS. None                                                   |*/
/*|                                                                   |*/
/*|  :INPUT.   Command:  a TEIES string containing the local operating|*/
/*|                      command string to be sent for execution      |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Rcode:    What return value of the invokation result   |*/
/*|                                                                   |*/
/*|  :PURPOSE.      Forks over a system call.                         |*/
/*|                                                                   |*/
/*|  :MODifications.                                                  |*/
/*|                                                                   |*/
/*|  11 May 87      JF         Added OSK_I.H                          |*/
/*|                            Fixed return type of system()          |*/
/*|                                                                   |*/
/*|  09 Sep 88      JF         Removed buggy call to s_eton()         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "osk_i.h"

sint os_call(command)

string command ;                       /* The desired task             */

{
   byte     *cmd     =  NIL   ;        /* Command translated into C str*/
   sint     rcode    =  0     ;        /* return code from system()    */
                                       /*                              */
                                       /*                              */



   s_put( command, '\0', s_len( command ) + 1 );

   rcode    = system( s_buffer( command ));


   return(rcode);

}
