/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                 T_INIT.C
<LEVEL>                TM
<AUTHOR>               John L. Foster
<DATE>                 June 14, 1987
<PARAM>   sys_file     String   INPUT   TEIES String for LFD 1 name. If
                                        S_NULL, will be * * TEIES SYS.
<PARAM>   version      String   INPUT   String for version identifier
<PARAM>   release      sint     INPUT   release id.
<OUTPUT>           sint
<GLOBAL>           TM_master

<PURPOSE>
    This function is called to start up the Transaction Manager.
    It is only called once when the server initializes.
    After this has run new processes are started with the T_START
    function.

<CALLS>          obj_beg()  : Start-up the Object, DMS levels.
                 mem_init() : Initialize the memory manager
                 t_mini()   : Initialize the TM structures.
                 isf_start(): Initialize the ISCF
                 isv_init() : Initialize the ISV tables
                 ti_init()  : Start-up the terminal interface.
                 os_mema()  : Allocate control block memory space.
                 t_config() : Configure dynamic features from local file
                 s_drop(), s_comp(), scv_off()!, var_beg().

<ERRORS>      0, 28000 : TM started.
                 28001 : TM started but not connected to a master server
                 28003 : TM already initialized.
                 28008 : TM not started.
</DOC>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isv.h"
#include "tm.h"
#include "ti.h"
#include "form.h"

extern sint isf_start();





sint     t_init(  system_file, version, release  )

string            system_file,
                  version;
sint              release;

                                            /********************/
{                                           /* Set up TM_PID #1 */
                                            /* and init the TM  */
sint    rc        =      0;                 /********************/
sint    max_conn  =     10,
        dummy     =      0,
        ocblen    =      0,
        odtlen    =      0;
string  state     = S_NULL,
        ver_str   = S_NULL,
        rel_str   = S_NULL,
        user_id   = S_NULL,
        sys_file  = S_NULL,
        state_var = S_NULL,
        wild_card = S_NULL;




#D_begin ( t_init , TM )
#D_str( 'system_file',' ... called with system config file name ...');

    /* rc = mem_init( START_UP_MEMORY_SIZE ); */

                                                    /* - - - - - - - - */
    rc   = t_tmini();                               /*                 */
                                                    /* Set the version */
    s_copy( &(TM_master->version), version, 0, 0 ); /* and release ids */
    TM_master->release = release;                   /*                 */
                                                    /*                 */
                                             /*************************/
                                             /*  Set  up the ISV      */
    rc   =   isv_init( );                    /* so we have the message*/
                                             /* defined for initialize*/
                                             /*************************/
    if( system_file == S_NULL )
    {

       s_init( & sys_file, "* * TEIES SYS" );

    }
    else
    {

       s_copy( & sys_file, system_file, 0, 0 );

    }
    #D_str( 'sys_file', 'system file' );       /***********************/
                                               /* First, read the con-*/
    rc   =    t_config(  sys_file  );          /* fig file and set any*/
                                               /* values from there.  */
                                               /***********************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 Important section of code:  Here we determine the type of server this
 is going to be.  If the Masterserver entry int he configuration file
 is the same ID as this user, then this will be a MasterServer.  If not,
 check for DataBaseServer and NetworkServer respectively.  If none of
 these, then we assume this is a regular user.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    s_init( & wild_card, "*" );
    s_init( & (TM_master->ucb->os_id), "%this is me%" );
    rc   = os_uid( &(TM_master->ucb->os_id) );
    TM_master->MS         = TM_master->sys_init->MS;
    s_trim( & (TM_master->MS),         'B' );
    s_trim( & (TM_master->ucb->os_id), 'B' );
    #D_str(   'TM_master->ucb->os_id',  'Me');
    #D_str(   'TM_master->MS',          'Master Server' );

    if( s_comp( wild_card, TM_master->MS, 0, 0, 'M', NIL )  == 0  )
    {

       TM_master->usrtype = MASTER_SERVER;
       s_copy( & (TM_master->MS), TM_master->ucb->os_id, 0, 0 );
       #D_say( 'Using myself as a Master Server.' );

    }
    else
    if( ! s_comp( TM_master->ucb->os_id, TM_master->MS, 0, 0, 'M', NIL ))
    {

       TM_master->usrtype = MASTER_SERVER;
       #D_say( 'I think I am going to be a Master Server.' );

    }
    else
    {
       /* NOTE: eventually checks for databaseserver and networkserver
                go here                                                */

       TM_master->usrtype = REAL_USER;
       #D_say( 'I think I am going to be a TEIES User.' );
       rc = t_objchk();

    }
    s_drop( & wild_card );


if ( rc == 0 )
{                                                  /********************/
                                                   /*                  */
                                                   /*Initialize the de-*/
                                                   /*fault help request*/
    s_init( &(TM_master->ucb->help_rqc), "?"    ); /*characeter and    */
    s_init( &(TM_master->ucb->help_rqs), "HELP" ); /*command string.   */
    os_dttm( &(TM_master->ucb->time_on)         ); /*                  */
                                                   /********************/
    TM_master->ucb->level = CASUAL_USER;                /**************/
                                                        /*Check to see*/
    s_init( &sys_file, "* * ENGLISH ISV" );             /*if the user */
    if( s_comp( TM_master->sys_init->ISV_name,          /*specified a-*/
                sys_file, 0, 0, 'M', &rc       ) != 0 ) /*nother ISV. */
    {                                                   /*If so add it*/
                                                        /*            */
       rc =  isv_add( TM_master->sys_init->ISV_name );  /*            */
                                                        /*            */
    }                                                   /**************/
                                               /***********************/
                                               /*                     */
                                               /* Start up the object */
    rc   = obj_beg(  SYS_LFD  );               /* level, which in turn*/
                                               /* starts the DMS. Tell*/
                                               /* Obj to use LFD 1 as */
                                               /* main LFD.           */
                                               /***********************/
                                               /* Initialize the ISCF.*/
    rc   = isf_start( &max_conn );             /*                     */
    if( rc == 30104 )                          /*                     */
    {                                          /*                     */
                                               /* A little tweek until*/
        scv__off();                            /* the system runs good*/
        rc=isf_start( &max_conn );             /*                     */
                                               /*                     */
    }                                          /*                     */
                                               /*                     */
    #D_show('allowing %d conns.','max_conn');  /*                     */
                                               /***********************/

}
if ( rc == 0 )
{
                                                 /**********************/
                                                 /* Set up the Global  */
    s_init( &state, "SIGNON"      );             /* system Interface   */
    var_beg(     );                              /* variables. Next,   */
                                                 /*determine what type */
                                                 /*of server we are.   */
                                                 /**********************/
    var_init( & state_var, "%STATE",    state,       OC_STR );
    var_init( & user_id,   "%SYSTEMID", TM_master->ucb->os_id, OC_STR );


    TM_master->release = release;
    s_copy(   & (TM_master->version), version, 0, 0        );
    var_init( & ver_str, "%VERSION", rel_str = t_ver(), OC_STR );

    #D_str( 'rel_str', 'TEIES version ID'                );
    #D_str( 'state',   'Initial State value.'            );
    #D_str( 'TM_master->ucb->os_id', 'User system ID'    );

    s_drop(  &rel_str );
    s_drop(  &ver_str );


    if( TM_master->usrtype != REAL_USER || ti_init() != 15000 )
    {                        /****************************************/
                             /*                                      */
                             /* Not an Interactive user, therefore   */
                             /* the TI will set up an Object level   */
                             /* trace file and we do not need to set */
                             /* up the forms or interactive features.*/
                             /*                                      */
                             /****************************************/
   #D_say( 'Cannot open a display station for this user - log to file.');

    }
    else
    {

       f_minit();
       t_cmdini( ( (struct memnode **)&(TM_master->uicmdtab) ) );

    }
}
s_drop( &  state     );
s_drop( &  state_var );
s_drop( &  user_id   );
s_drop( &  sys_file  );

t_siend();



#D_return(    rc )

}
