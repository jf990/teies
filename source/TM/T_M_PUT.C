/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_m_put(string server, string packet)                  |
  |                                                                   |
  |  :AUTHOR.  Tanmay S. Kumar                                        |
  |                                                                   |
  |  :DATE.    August 12, 1988                                        |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   server        string   INPUT   server requesting data  |
  |  :PARAM.   packet        string   INPUT   information on what to  |
  |                                           find.                   |
  |                                                                   |
  |  :PURPOSE. To unpack the packet received and look for the         |
  |            system index requested and replace the data for that   |
  |            index.                                                 |
  |                                                                   |
  |  :OUTPUT.  sint          0.                                       |
  |                                                                   |
  |  :CALLS.   ocd_unpack(), t_csend(), obj_rep(), t_sysndx()         |
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

sint  t_m_put(server, packet)

string server;
string packet;

{

 hword     sys_index_obj = 0;
 byte       *current_ptr = NIL;
 OCD                *ocd = NIL;
 OCB                *ocb = NIL;
 string             data = S_NULL;
 string       curpointer = S_NULL;
 sint                 rc = 0;


 current_ptr = (byte *) packet;

 sys_index_obj = *(hword *)(s_buffer(packet));
 current_ptr += 6;

 data = (string)(current_ptr);

 current_ptr += (s_smp_al(data) + S_OVERHEAD);
 curpointer = (string)(current_ptr);

 ocd_unpack(curpointer, &ocd);

 ocb = t_sysndx(sys_index_obj);
 rc = obj_rep(ocb, ocd, data, SEG_UPDATE);
 rc = t_csend(server, rc, NIL);

 ocd_drp( & ocd );


   return(rc);
}
