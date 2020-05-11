/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_m_fnd(string server, string packet)                  |
  |  <AUTHOR>  Tanmay S. Kumar                                        |
  |  <DATE>    August 12, 1988                                        |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server        string   INPUT   server requesting data  |
  |  <PARAM>   packet        string   INPUT   information on what to  |
  |                                           find.                   |
  |                                                                   |
  |  <PURPOSE> To unpack the packet received and look for the         |
  |            system index object and specific OCD and if it exists  |
  |            to send the data for it to the server requesting it.   |
  |                                                                   |
  |  <OUTPUT>  sint          0.                                       |
  |  <CALLS>   ocd_unpack(), t_csend(), obj_get(), t_sysndx()         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"

sint  t_m_fnd( server, packet )

string server;
string packet;

{

 hword            sys_index_obj = 0;
 OCD                       *ocd = NIL;
 OCB                       *ocb = NIL;
 ODT                *odtentry   = NIL;
 string              curpointer = S_NULL,
                           data = S_NULL;
 byte                   *result = NIL;
 sint                        rc = 0,
                    data_length = 0;


 /* unpack the received packet */

 sys_index_obj = *(hword *)(s_buffer(packet));
 curpointer = (string) (s_buffer(packet) + 2);

 ocd_unpack(curpointer, &ocd);

 ocb = t_sysndx(sys_index_obj);

 if( OCB_POINTER_OK( ocb ) )
 {

    odtentry    = odt_find( ocb, ocd    );
    data_length = odt_len(  odtentry, ocb->type );


    if( odt_type( odtentry ) != OC_STR )
    {

       result = os_mema(  data_length );
       os_memn( result, data_length );
       rc = obj_get( ocb, ocd, result );
       s_alloc( & data, data_length );
       os_memc( result, s_buffer( data ), data_length );
       s_smp_ul( data ) = data_length;
       os_memd( result );

    }
    else
    {

        rc = obj_get( ocb, ocd, & data );

    }


 }
 else
 {

    rc = NOT_A_SYSTEM_INDEX;

 }

 rc = t_csend(server, rc, data);

 ocd_drp( & ocd  );
 s_drop(  & data );

   return(rc);

}
