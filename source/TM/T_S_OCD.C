/********************************************************************
:NAME.             T_S_OCD.C

:LEVEL.            TM

:AUTHOR.           John L. Foster

:DATE.             January 22, 1988

:PARAM.            server    string    INPUT    Server to receive a set
                                                of OCD's from.

:PARAM.            packet    string    INPUT    Packet of the format
                                                described below.

:PARAM.            result    OCB **    OUTPUT   Where to store the result
                                                OCB.

:PARAM.            flags     hword     INPUT    Flags passed by transact
                                                ion routines.

:OUTPUT.           rc   :

:GLOBALS.          - none -

:PURPOSE.          A server has sent us an OID and a set of OCD's that
                   belong to that object.  Find the object in the SOIDI.
                   IF not found, allocate an object of the type in ques-
                   tion.  Fill in the object with the OCD's in the data
                   packet.  Finally, call the link routine for this
                   object type.

                   If the object is updated and linked sucessfully a
                   confirmation is sent back to teh requesting server.
                   The confirmation will contain the OID of the object
                   if it changes. If the operation fails the confirmation
                   packet will contain the error code.

                   The data packet is described in the specs as well as
                   in the source code to T_OBJENC.c.

:CALLS.

:ERRORS.           20655 : Object type of the OID on this server does not
                           match the Object type requested.

:LIMITS.           This is a big one: If the OCD does not belong to the
                   object type indicated, the entire packet may not be
                   decodable!
:MODS.
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint     t_s_ocd( server,  packet, result, flags  )

string            server,
                  packet;
OCB             **result;
hword             flags;


{


sint       rc         =      0;
hword      obj_type   =      0;
OID       *oid        =    NIL;
string     results    = S_NULL;
boolean    ok_to_link =   TRUE;






   if( TM_master->usrtype == REAL_USER )
   {

      ok_to_link = FALSE;

   }
   else if( ! ( flags & OK_TO_LINK_OBJECT ) )
   {

      ok_to_link = FALSE;

   }

   rc = obj_dec( packet, NIL, & obj_type, & oid, ok_to_link, & results );

   if( TM_master->usrtype != REAL_USER )
   {

      t_csend( server, rc, results );

   }

   if( result != NIL )
   {

      *result = ocb_inta( oid, (hword) 0 );

   }


   return(rc);

}
