/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_ON.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.                max_con   max connection requested by ISCF

:OUTPUT.               max_con   max connection possible
                       rc        returns non-zero value if error occurs

:INPUT_GLOBALS.    SCVExtrn :  Chk if it is already init

:OUTPUT_GLOBALS.   SCVExtrn :  SCV info; init it

:PURPOSE.
                   Initialize the IUCV / SCV interface to
                   all startup values, allocate and initialize
                   structures, pointers, and buffers, and finally
                   set initial servier IUCV interrupts.

:CALLS.            SCVINIT  :  Assembly initalize routine
                   s_copy() :  c routine to copy TEIES strings
                   s_alloc():  allocate string space for sysid
                   os_uid() :  to pick up this system name
                   os_mema():  allocate space for head list

:ERRORS.           SCV_ERR + SUB_ERR +
                   00   :  General error
                   01   :  SCV already initialized.
                   02   :  SCV can not run

:LIMITS.

:MODS.

JF       05/23/87            Documentation, standardized programming,
                             put up to current specification level,
                             P*3, and SCV level compatibility.

JF       06/15/87            Added call to SCVSETD routine to set up
                             a data section for the IUCV for global
                             access to a save area for interrupts.

Sal      07/06/88            Re-write of SCV level; Got rid of
                             ACCQ and DSCQ list.
Sal      02/14/89            s_just() ---->  s_pad()


* ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
                                                 /*                   */
#define SUB_ERR 100

sint     scv_on( max_con )
sint            *max_con;

{
sint  rc  =   0;
struct scv_conq *contail;                       /* temp ptrs          */
struct scv_recq *rqtail;                        /* sort of WITH       */

extern struct scv_xstr *SCVExtrn;



    if ( SCVExtrn == NULL )
    {                     /* scv_on has NOT been called before */

      SCVExtrn = (struct scv_xstr *) os_mema(sizeof(struct scv_xstr));
      SCVExtrn->sysid = S_NULL;
      rc = s_alloc(&(SCVExtrn->sysid), IDLEN);
      rc = os_uid(&(SCVExtrn->sysid));
      rc = s_pad( &(SCVExtrn->sysid), SCVExtrn->sysid, IDLEN, ' ', 'R');
             /* pad with spaces to make sure sysid it is IDLEN long  */

                                  /* Init IUCV and allocate space for  */
                                  /* scvdata which has to be on double */
                                  /* word boundry. Its SCVDLEN long,   */
                                  /* defined in SCVMCRO MACRO          */

      rc = Inter(SCVON);          /* Call assembly code                */

      if( rc < 0 )                   /* if negative rc, then error */
      {

           rc = SCV_ERR + SUB_ERR - rc; /* format error the right way */
           s_drop(&(SCVExtrn->sysid));  /* now drop sysid             */
           os_memd( SCVExtrn );
           SCVExtrn = NULL;

      }
      else
      {
         *max_con = rc;
         rc = SCV_NO_ERR;
                                        /* now init 1st element on list*/
         contail = (struct scv_conq *) os_mema(sizeof(struct scv_conq));
         contail->vmid = S_NULL;
         s_copy( &(contail->vmid), SCVExtrn->sysid, 0, 0);
         contail->pathid = -1;
         contail->msghead = NULL;
         contail->msgtail = NULL;
         contail->flags = 0;
         contail->next   = NULL;

         rqtail =  (struct scv_recq *) os_mema(sizeof(struct scv_recq));
         rqtail->msgid = -1;
         rqtail->trgcls = -1;
         rqtail->msglen = -1;
         rqtail->vmpath = (hword) 0xFFFF;
         rqtail->flags = 0;
         rqtail->next = NULL;


         SCVExtrn->conhead = contail;
         SCVExtrn->contail = contail;
         SCVExtrn->rqhead = rqtail;
         SCVExtrn->rqtail = rqtail;
      }    /* else rc < 0  */
    }
    else
    {

      rc =  SCV_ERR + SUB_ERR + ERR_ALREADY_INIT;
    }

   return(rc);
}
