/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QPTH.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            vmid :    TEIES string containing the server ID
                             to check

:OUTPUT.           rc   :    IUCV connection path identifer (sint)
                             -1 if no connection to id requested.

:INPUT_GLOBALS.    conhead: IUCV connection list head pointer
                   contail: IUCV connection list tail pointer
                   acchead: IUCV accept queue head pointer

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *
 *  purpose:  to query whether connection has been made, if so
 *            return pathid.
 *
 *  algorithm:
 *            uses connection list to match against the given
 *            given vm id.  If match succedes the connection
 *            has already been made.

:CALLS.       s_comp()  :  TEIES string comparision function
              strncmp()

:ERRORS.

:LIMITS.

:MODS.

JF       05/23/87            Brought up to specification. Added doc-
                             mentation, p*3.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 700

sint scv_qpth ( vmid, conprev)

string          vmid;               /* vmid whose pathid is to be ret. */
struct scv_conq       **conprev;    /* return ptr to where conn info is*/
{
    sint flag = 0,
         rc   = 0;
    struct scv_conq *contemp = NULL;   /* temporary ptr to the con. q */

#D_start(scv_qpth,OSK_SCV,'VmidPtr = 0x%x, PrevPtr=0x%x',\
                          'vmid, conprev')
#D_str('vmid', 'Looking for...')

   *conprev = contemp = SCVExtrn->conhead;
   for(;contemp;)
   {
      #D_str('contemp->vmid', 'Comparing...')
      if( (flag = s_comp(vmid, contemp->vmid, 0, 0, 'M', &rc)) == 0 )
         break;                  /* get out if we found a match */
      *conprev = contemp;
      contemp = contemp->next;
   }

   #D_show('Found Contemp = 0x%x, conprev = 0x%x','contemp,*conprev')
   #D_show('Flag = 0x%x, Conhead = 0x%x','flag, SCVExtrn->conhead')

   if( flag == 0 )                     /* found something which maches */
   {
      if( contemp == SCVExtrn->conhead ) /* Are we connected to self?? */
      {
         rc = ERR_SELF_CONN;
      }
      else
      {
         #D_show('Entry flags = 0x%x','contemp->flags')
         if( (contemp->flags & SCV_CON_DISCONN) )  /* Conn still there?*/
         {
            rc = ERR_CONN_BUT_SEVER;
         }
         else
         {
            rc = ERR_ALREADY_CONN;
         }
      }
   }
   else
   {
      rc = ERR_NO_PATH;
   }

   #D_return( rc );
}
