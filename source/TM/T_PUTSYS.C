/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_putsys(hword sys_index_obj, OCD *ocd, string data)   |
  |                                                                   |
  |  :AUTHOR.  Tanmay S. Kumar                                        |
  |                                                                   |
  |  :DATE.    August 12, 1988                                        |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   sys_index_obj hword    INPUT   system index            |
  |  :PARAM.   ocd           *OCD     INPUT   pointer to OCD          |
  |  :PARAM.   data          string   INPUT   pointer to data         |
  |                                                                   |
  |                                                                   |
  |  :PURPOSE. To pack the system index, data, ocd and  send it to the|
  |            master server to put in its database.                  |
  |                                                                   |
  |  :OUTPUT.  sint          0.                                       |
  |                                                                   |
  |  :CALLS.   ocd_pack(), t_send(),                                  |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"

sint  t_putsys( sys_index_obj, ocd, data )

hword    sys_index_obj;
OCD    * ocd;
string   data;

{

 OCB           * ocb = NIL;
 byte      ** result = NIL;
 string  send_packet = S_NULL,
              server = S_NULL,
              packet = S_NULL;
 hword          flag = 0,
             command = 0;
 sint             rc = 0,
               state = 0;



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

    if (TM_master->usrtype != MASTER_SERVER)
    {

       /* since it is not the master server we have to make a packet */
       /* to send to the master server. Sent packet consists of      */
       /* system index object, data, and the packed ocd.             */

       ocd_pack(&packet, ocd);

       s_alloc(&send_packet, 2);
       s_smp_ul(send_packet) = 2;
       os_memc(&sys_index_obj, s_buffer(send_packet), 2);

       s_catx(&send_packet, data, 0, 0);
       s_catx(&send_packet, packet, 0, 0);

       command = UPDATE_SYSTEM_INDEX;
       flag = 0;
       rc = t_send(TM_master->MS, command, flag, state, send_packet);
       rc = t_waitc(TM_master->MS, (void **) result);

       s_drop(&packet);
       s_drop(&send_packet);

    }
    else
    {   /* it is the master server - so do a update */

       ocb = t_sysndx(sys_index_obj);
       rc = obj_rep(ocb, ocd, data, SEG_UPDATE);

    }
}

   return(rc);

}
