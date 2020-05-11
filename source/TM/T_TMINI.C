/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>             T_TMINI.C
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             November 09, 1987

<PARAM>            * none *

<OUTPUT>           sint

<GLOBAL>           TM_master
<PURPOSE>
                   Call this function only to initialize the TM master
                   control block pointer TM_master.

<CALLS>            * none *
<ERRORS>
                         TM already initialized.
<LIMITS>
<MODS>
</DOC>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"



struct  TMCB      * TM_master   =   NIL;





sint     t_tmini(  )

                                            /********************/
{                                           /* Set up TM_PID #1 */
                                            /* and init the TM  */
sint    rc        =      0;                 /********************/





    if(  TM_master != NIL  )
    {

         rc  =  TM_MASTER_PROCESS_ALREADY_ACTIVE;


    }

    TM_master = (struct TMCB *) os_mema(  sizeof( struct TMCB ));
    os_memn( TM_master, sizeof( struct TMCB ) );


    TM_master->processes      =   1;        /**************************/
    TM_master->currentPID     =   1;        /*  Initialize TM Control */
                                            /**************************/
    TM_master->Next_PID       =   2;

    TM_master->ucb            =   (UCB *) os_mema( sizeof(   UCB   ));
    os_memn( TM_master->ucb, sizeof( UCB ));

    TM_master->first          =   (tmentry *)os_mema( sizeof( tmentry ));
    os_memn( TM_master->first, sizeof( tmentry ) );

    TM_master->first->tm_pid  =   1;
    TM_master->current        =   TM_master->first;
    TM_master->usrtype        =   REAL_USER;

    TM_master->sys_init = (struct _init_ *)
                                       os_mema( sizeof( struct _init_ ));
    os_memn( TM_master->sys_init, sizeof( struct _init_ ) );



   return(rc);

}
