/********************************************************************
:NAME.             T_M_OCD.C

:LEVEL.            TM

:AUTHOR.           John L. Foster

:DATE.             November 29, 1987

:PARAM.            server    string    INPUT    Server to receive OCD
                                                 packet.

:PARAM.            data      string    INPUT    Data containing the OCDs
                                                 requested.

:PARAM.            result    OCB **    OUTPUT   Where to store the result
                                                 OCD if needed.

:OUTPUT.           rc   :

:GLOBALS.          - none -

:PURPOSE.          The server is asking this server for some OCD's from a
                   particular object.  This routine will locate the
                   object, find the OCDs in question, and format a data
                   packet to return to the requesting server.

                   Format of the input data packet:


           +------+-----+--------+------+-------+-----+
           |Length|     | Number |      |       |     |
           |  of  | OID |   of   | OCD 1| OCD 2 | ... |
           | OID  |     |  OCDs  |      |       |     |
           +------+-----+--------+------+-------+-----+

                   Format of the return packet:

           +------+-----+--------+------+-----------+-------+-----+
           |Length|     | Number |      |           |       |     |
           |  of  | OID |   of   | OCD 1| Contents 1| OCD 2 | ... |
           | OID  |     |  OCDs  |      |           |       |     |
           +------+-----+--------+------+-----------+-------+-----+

                   All OCDs and Contents are TEIES strings, therefore
                   they contain length information.  All length data
                   elements are single byte values (8 bits).  A packet
                   cannot have a total length greater than 4K bytes.

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




sint     t_m_ocd( server,  data,  result  )

string            server,
                  data;
OCB             **result;


{


sint   rc      = 0;
sint   level   = 0;
string request = S_NULL;




   return(rc);

}
