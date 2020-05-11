/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_m_obj                                                |
  |  <AUTHOR>  Tanmay S Kumar                                         |
  |  <DATE>    June 21, 1988                                          |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   ocb        OCB *     INPUT    Pointer to the OCB that  |
  |                                          contains the pointer to  |
  |                                          the ODT                  |
  |            server     string      INPUT  ID to whom to send data  |
  |            result     void *      INPUT  Result pointer.          |
  |            ok_to_link boolean     INPUT  If respective LINK logic |
  |                                          is to be invoked on the  |
  |                                          other server.            |
  |            state      sint        INPUT  TM state code for hand-  |
  |                                          shaking on the other     |
  |                                          server.                  |
  |                                                                   |
  |  <PURPOSE> To send an object to a specific user ID.  Calls        |
  |            obj_pack() on the ocb to packetize the object.  Then   |
  |            send the object to the destination server and wait for |
  |            a confirmation.  The destination server calls t_s_obj()|
  |            to unpack the object and link it into the data base.   |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>   s_alloc(), s_smp_ul(), s_drop(), except()              |
  |            ocd_new (), obj_pack (), t_send ().                    |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    May 16, 1989      JF      Added STATE parameter.       |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"



sint t_m_obj( server, ocb, result, ok_to_link, state )

string        server;
OCB         * ocb;
void        * result;
boolean       ok_to_link;
sint          state;


{


   sint    rc       =           0;
   OCD    *ocd      =         NIL;
   string  senddata =      S_NULL;
   hword   command  = SEND_OBJECT,
           flags    =           0;






 s_alloc( & senddata,    12 );
 s_smp_ul(  senddata ) = 12  ;

 ocd_new( & ocd,    "%LINKS" );
 rc = obj_pack( & senddata, ocb, ocd );


 if ( ( rc == 0 )  &&  ( s_len( senddata ) > 12 ) )
 {

    if( ok_to_link )
    {

       flags |= OK_TO_LINK_OBJECT;

    }
    rc = t_send( server, command, flags, state, senddata );
    if ( rc != 0 )
    {

       except( rc, CONTROL_ERROR, server, S_NULL, S_NULL, S_NULL,S_NULL);

    }
    else
    {

       rc = t_waitc( server, (void **) result );

    }
 }
 else
 {

    t_csend( server, rc, S_NULL );

 }

 ocd_drp( & ocd      );
 s_drop(  & senddata );



   return(rc);
}
