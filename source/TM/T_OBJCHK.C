/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_objchk                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 3, 1989                                       |
  |  <LEVEL>   TM                                                     |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   None                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <PURPOSE> Verify that all external files required to run the     |
  |            system are accessible.  If this routine returns other  |
  |            than 0, TEIES cannot be run probably because the user  |
  |            does not have access to the language directory.        |
  |                                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>  Can only run while initializing the system.            |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "stdio.h"
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   t_objchk()


{

   sint        rc          =      0;
   string      object_name = S_NULL;
   FILE       *fcb         =    NIL;



#if 0
if(  TM_master              != NIL         &&
     TM_master->usrtype     == REAL_USER   &&
     TM_master->sys_init    != NIL            )
{

   s_init( & object_name, "TEIES SYSERR *" );
   if( (fcb = fopen( s_buffer( object_name ), "r" )) == NIL )
   {

      rc = TEIES_SYSTEM_ERROR_TABLE_NOT_FOUND;

   }
   else
   {

      fclose( fcb );

   }
   s_init( & object_name, "TEIES $CMDDATA *" );
   if( (fcb = fopen( s_buffer( object_name ), "r" )) == NIL )
   {

      rc = TEIES_COMMAND_TABLE_NOT_FOUND;

   }
   else
   {

      fclose( fcb );

   }
   s_init( & object_name, "0100 FORM *" );
   if( (fcb = fopen( s_buffer( object_name ), "r" )) == NIL )
   {

      rc = HomeBase_Form_Not_Found;

   }
   else
   {

      fclose( fcb );

   }
   s_drop( & object_name );

}
#endif

   return(rc);

}
