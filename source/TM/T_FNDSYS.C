/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_fndsys(hword sys_index_obj, OCD *ocd, byte **result) |
  |  <AUTHOR>  Tanmay S. Kumar and JF                                 |
  |  <DATE>    August 12, 1988                                        |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   sys_index_obj hword    INPUT   object type to find.    |
  |  <PARAM>   ocd           OCD *    INPUT   pointer to OCD          |
  |  <PARAM>   result        void *   OUTPUT  Where to put result data|
  |                                                                   |
  |  <PURPOSE> To find if a specific system indexed object exists     |
  |            at the Master Server by sending a request packet       |
  |            consisting of the system index object, and packed OCD. |
  |            If the system indexed object for the specific OCD      |
  |            exists then the data is sent back by the master server |
  |            to satisfy the request.                                |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocd_pack(), t_send(), t_waitc()                        |
  |  <ERRORS>  Returns error, usually an OBJ level error code if there|
  |            is a problem with the search.                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"

sint  t_fndsys( sys_index_obj, ocd, result )

hword           sys_index_obj;
OCD           * ocd;
void          * result;

{

 hword           command = 0,
                 flag    = 0;
 OCB            *err_ocb = NIL,
                *ocb     = NIL;
 ODT            *odtentry= NIL;
 string     send_packet  = S_NULL,
            recv_data    = S_NULL,
                 packet  = S_NULL;
 sint            rc      =      0,
                 state   =      0,
            data_length  =      0;




 if ((sys_index_obj < START_SYSTEM_INDEX_RANGE) ||
                              (sys_index_obj > END_SYSTEM_INDEX_RANGE))
 {

    rc = NOT_A_SYSTEM_INDEX;

 }
 if (ocd == NIL)
 {

    rc = OCD_POINTER_INVALID;

 }
 if (rc == 0)
 {

    if (  TM_master->usrtype != MASTER_SERVER  )
    {

       /* since it is not the master server we need to make the packet */
       /* to send to the master server                                 */

       ocd_pack(& packet, ocd );

       s_alloc( & send_packet,    2 );
       s_smp_ul(  send_packet ) = 2  ;
       os_memc( & sys_index_obj, s_buffer(send_packet), 2);

       s_catx(  & send_packet, packet, 0, 0);

       command = LOCATE_IN_SYSTEM_INDEX;
       flag = 0;
       rc = t_send(  TM_master->MS, command, flag, state, send_packet );
       rc = t_waitc( TM_master->MS, (void *)  &    recv_data  );

       if( (rc == 0) && (s_len( recv_data ) > 0)  )
       {

          ocb = t_sysndx( sys_index_obj );
          odtentry    = odt_find( ocb, ocd    );
          data_length = odt_len(  odtentry, ocb->type );
          if( odt_type( odtentry ) != OC_STR )
          {

             os_memc( s_buffer( recv_data ), result, data_length );

          }
          else
          {

             s_copy( (string *)result, recv_data, 0, 0 );

          }
       }
       s_drop( & send_packet );
       s_drop( & packet      );

    }
    else
    {

                                           /* We are the master server */

       err_ocb = t_sysndx( SERRI_OBJ );
       rc      = obj_get(  err_ocb, ocd, result );

    }
 }

   return(rc);

}
