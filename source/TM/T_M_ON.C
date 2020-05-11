/********************************************************************
:NAME.             T_M_ON.C

:LEVEL.            TM

:AUTHOR.           John L. Foster

:DATE.             November 29, 1987

:PARAM.            server    string    INPUT    Server requesting the
                                                connection table.

:PARAM.            data      string    INPUT    Table specification.

:OUTPUT.           rc   :

:GLOBALS.          - none -

:PURPOSE.          A server is requesting the connection table detailing
                   all connections to this server.

:CALLS.

:ERRORS.

:LIMITS.

:MODS.
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint     t_m_on( server, data )

string           server;
string           data;


{


sint   rc      = 0;
sint   level   = 0;
string request = S_NULL;




   return(rc);

}
