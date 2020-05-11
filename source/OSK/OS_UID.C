/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_UID.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 May 04, 1987

:INPUT.
              name:    TEIES string pointer to get the local user id
                       string.

:OUTPUT.        rc:    error condition, 0 if none, floated from s_init.

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
         Returns a TEIES string containing the local operating system
         name of the current user running this code.

:CALLS.
         cuserid(): From the Waterloo C Runtime library
         os_mema(): From the OSK to allocate some space to temp
                    hold the C string used by cuserid().
         os_memd(): From the OSK to free that space
         s_init():  From the String library to convert the C string
                    into a TEIES string.

:ERRORS.
         10010:  Buffer too small to store result.
         10011:  No buffer supplied for result.

:LIMITS.
         Dependent on IBM/VM and Waterloo CRTL.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"



sint     os_uid(  name  )

string           *name;


{

sint     rc   = 0,
         rrc  = 0;
byte    *cptr = NIL;
string  space = S_NULL;




    cptr = ( byte * )  os_mema(  sizeof( byte ) * 20  );

    cptr = cuserid(  cptr  );

    if( name == S_NULL )
    {

         rc   = 10011;

    }
    else if( s_allen( *name )  <  strlen( cptr ) )
    {

         rc   = 10010;

    }
    else
    {

         rc   = s_init( name, cptr  );
         rc = s_init( &space, " ");
         rc = s_copy( name, *name, 1, s_first(space, *name, 0, 0));
         rc = s_drop( &space);

    }

    rrc = os_memd( cptr );


   return(rc);

}
