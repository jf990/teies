/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    TM_X   Header                                          |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 24, 1989                                         |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |  <PURPOSE>                                                        |
  |            Define external data constants and structures for the  |
  |            TM. Most important are the process structures and      |
  |            global TM_master for other levels to access.           |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                     *
 *  Transaction Manager function definitions                           *
 *                                                                     *
 * $EXTRACT        $MAKER-PROTO                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern   sint t_init    ( string sys_file , string version , sint release );
                                  /* TM Initialize routine, to be used */
                                  /* only once at  logon.              */

extern   sint t_objchk  ( void );
                                  /* No args - make sure all disks, dir*/
                                  /* ectories, files are accessible.   */

extern   sint t_tmini   ( void );
                                  /* Initialize TM_master AND allocate */
                                  /* TM_master->sys_init               */

extern   sint t_gtlid( sint entry, string * lfd_name,
                                   string * lfd_access,
                                   sint   * lfd_block_size,
                                   sint   * lfd_number_of_blocks
                     );
#define  t_get_lfd_init_data     t_gtlid
                                  /* Get LFD start up information out  */
                                  /* of TM_master->sys_init.  If entry */
                                  /* passed in is -1, returns total #  */
                                  /* entries initialized.  If sys_init */
                                  /* not active, returns error code.   */

extern   sint t_tmend   ( void );
                                  /* Deallocate all TM structures      */
                                  /* except TM_master.  No args.       */

extern   sint t_siend   ( void );
                                  /* Deallocate the TM Sys_Init struct.*/

extern   sint t_shutdn  ( void );
                                  /* TM end routine, to be used        */
                                  /* only once at system shutdown.     */

extern   sint   t_alarm   ( TM_PID process_id );
                                  /* Indicate an alarm is pending on   */
                                  /* a process.  Arg: sint pid; Returns*/
                                  /* 0 if ok, error if bad process.    */

extern   sint   t_strprt  ( TM_PID process_id );
#define tm_start_printer    t_strprt
                                  /* Indicate the system printer as the*/
                                  /* active device for all ti level    */
                                  /* functions.                        */

extern   sint   t_stpprt  ( TM_PID process_id );
#define tm_stop_printer    t_stpprt
                                  /* Deactivate the printer and react- */
                                  /* ivate the console for ti level    */
                                  /* functions.                        */

extern   string t_ver   ( void );
                                  /* Takes no parameters but returns a */
                                  /* string that contains the current  */
                                  /* TEIES version identifier.  The    */
                                  /* caller must s_drop{} this string. */

extern   sint   t_spad  ( string spad_file , string * result );
                                  /* Start a new process for the       */
                                  /* scratchpad editor. Edits the file */
                                  /* A0 * TEIES $SP by default.        */

extern   sint   t_delsp   ( string spad_file );
                                  /* Clear the scratchpad. Arg is str  */
                                  /* of scratchpad file name.          */
                                  /* A0 * TEIES $SP by default.        */

extern sint t_jmpst( hword object, hword action, hword level,
                              string modifier, OCB * ocb, void * params);
                                  /* Jump from the current state to the*/
                                  /* new state specified.  Args:       */
                                  /*                                   */
                                  /*   hword   object, action, level;  */
                                  /*   string  modifier;               */
                                  /*   OCB    *ocb;                    */
                                  /*   string *parameters;             */
                                  /*                                   */
                                  /* Returns sint - error code.        */
                                  /*                                   */

extern sint t_remote( string server, hword object, hword action,
               hword level, OCB * ocb, string data, void * result_str);

                                  /*Invoke a call to t_jmpst from a    */
                                  /*remote server.  This is for execut-*/
                                  /*ing operations on another server.  */
                                  /*Parameters are:                    */
                                  /*                                   */
                                  /*   string  server;                 */
                                  /*   hword   object, action, level;  */
                                  /*   OCB    *ocb;                    */
                                  /*   string *data;                   */
                                  /*                                   */
                                  /* Returns sint - error code.        */
                                  /*                                   */

extern sint t_updcnt( OCB * ocb );
#define t_update_user_counters t_updcnt

                                  /* Go through all the list objects in*/
                                  /* the given user object and update  */
                                  /* the count of index entries.       */
                                  /*                                   */

extern sint t_incic( ISCF_TAB * user_entry, sint what_to_increment );

#define    INCREMENT_ITEMS_CREATED        1
#define    INCREMENT_ITEMS_MODIFIED       2
#define    INCREMENT_ITEMS_RECEIVED       3

#define t_increment_items_created(x)  t_incic(x,INCREMENT_ITEMS_CREATED)
#define t_increment_items_modified(x) t_incic(x,INCREMENT_ITEMS_MODIFIED)
#define t_increment_items_received(x) t_incic(x,INCREMENT_ITEMS_RECEIVED)

                                  /* Declare another item created by   */
                                  /* the specified user given their    */
                                  /* connection entry {therefore, can  */
                                  /* only really work on the MS) and if*/
                                  /* NIL then find the current user by */
                                  /* t_invoker and user that.          */

extern sint t_gtstat( OCB * ocb, string * stat_str, sint flag );

#define t_get_last_stat_record                t_gtstat
#define t_get_last_user_stat_record( result ) t_gtstat(NIL,result,FALSE)

                                  /* Get the last statistic entry in   */
                                  /* the users stat list and send it   */
                                  /* back to the requesting server.    */
                                  /*   Ocb is the user object to get   */
                                  /*     stat record from.  If NIL,    */
                                  /*     then current user.            */
                                  /*   Flag determines for the MS if   */
                                  /*     the info is needed locally {T}*/
                                  /*     or remotely {FALSE}.          */
                                  /*   String is where the stat record */
                                  /*     is returned in a format defind*/
                                  /*     in OBJ_USER.h.                */

extern sint t_spawn( OCB * some_ocb, string packet, sint state,
                                                    void   * result_str);

                                  /*Send an object or a packet from one*/
                                  /*server to the master server and get*/
                                  /*the response.  The object's link   */
                                  /*routine is also called.  Args:     */
                                  /*                                   */
                                  /*   OCB    *object;                 */
                                  /*   string  packet;                 */
                                  /*   hword   flags;                  */
                                  /*   string **result;                */
                                  /*                                   */
                                  /* Returns sint - error code.        */
                                  /*                                   */

extern   string  t_invoker( void );
                                  /*Determine who is the TM connection */
                                  /*that we are currently processing.  */
                                  /*The string returned can be passed  */
                                  /*to any TM communication routine.   */
                                  /*No parameters.                     */

extern sint   t_form( string state, string modifier, OCB * object_cb );
                                  /* Locate and process the form for a */
                                  /* given state of the UI.   Args:    */
                                  /*       string state, modifier;     */
                                  /*       struct OCB    *ocb_data;    */
                                  /* Returns the key value of the fun- */
                                  /* tion requested to end the form.   */

extern sint   t_updfld  ( string form_id );
                                  /* Update the form fields for the    */
                                  /* current process/form.     Args:   */
                                  /*                                   */
                                  /*   string    form_id;              */
                                  /*                                   */
                                  /* Returns 0 if all ok, RC from cmd  */
                                  /* line if it was on the form.       */

extern string t_rstfld  ( string form_id );
                                  /* Reset all fields on a form to the */
                                  /* original values they had at parse */
                                  /* time.  Always returns 0.  Args:   */
                                  /*                                   */
                                  /*   string    form_id;              */
                                  /*                                   */
                                  /*                                   */

extern sint t_nxtfld( string * field_name, string * field_contents,
                      sint * length_of_contents, sint * type_of_contents,
                                                      sint * field_type);
                                  /* Return the next field that has ben*/
                                  /* modified on the last form posted. */
                                  /* All arguments are returned by the */
                                  /*                                   */
                                  /*   string *     field_name         */
                                  /*   string *     field_contents     */
                                  /*   sint   *     field_length       */
                                  /*   sint   *     type_of_contents   */
                                  /*   sint   *     type_of_field      */
                                  /*                                   */
                                  /*Returns sint 0 if a field is ret-  */
                                  /*urned, else NO_MORE_MODIFIED_FIELDS*/
                                  /*                                   */

extern sint t_flderr( string state, string field_name, sint error_code );

#define  t_error_on_field    t_flderr
                                  /* Declare a user input error on a   */
                                  /* given field.  Calls except to post*/
                                  /* the error and f_curpos to place   */
                                  /* the cursor on that field for the  */
                                  /* next time the form is posted the  */
                                  /* user is already at the error.     */

extern   OCB   * t_sysndx( sint index );
                                  /* Locate and return the OCB of a sys*/
                                  /* index given its ISV value.  Args: */
                                  /*       sint index;                 */

extern   sint t_homeba  ( void );
#define       t_homebase          t_homeba
                                  /* Go to HomeBase, do not pass GO, do*/
                                  /* ! collect $200.00. {No parameters}*/
                                  /*                                   */

extern   sint t_valcas  ( void );
                                  /* Validate the fields on the Casual */
                                  /* Homebase screen.                  */
                                  /*                                   */

extern   sint t_valexp  ( void );
                                  /* Validate the fields on the Expert */
                                  /* Homebase screen.                  */
                                  /*                                   */

extern   sint t_valpwr  ( void );
                                  /* Validate the fields on the Power  */
                                  /* Homebase screen.                  */
                                  /*                                   */

extern   sint t_valyn   ( string fld_val );
                                  /* Validate a Yes or a No, returns   */
                                  /* TRUE if "Yes", FALSE if "No", and */
                                  /* -1 if neither.  It does this with */
                                  /* the ISV so it is language dependnt*/
                                  /* Args:                             */
                                  /*                                   */
                                  /*   string    value_to_check;       */
                                  /*                                   */

extern   boolean t_ifquit( hword  value_to_check );
                                  /* Determine if a value is a user req*/
                                  /* quit function, either QUIT or     */
                                  /* ABANDON.  Return TRUE if so, else */
                                  /* returns FALSE.                    */

extern sint t_command( string cmdline, struct memnode * cmd_root,
                                                            hword level);
                                  /* Command Line processer.  Args:    */
                                  /*   string  cmdline;                */
                                  /*   node   *uicmdtab;               */
                                  /*   hword   level;                  */
                                  /*                                   */
                                  /* Returns RC from target command.   */

extern   sint t_help    ( string help_line );
                                  /* Process help for words.  Args:    */
                                  /*   string *str_vec;                */
                                  /*                                   */
                                  /* Returns sint RC.                  */

extern   sint t_edtscr  ( string state_id, hword flags, sint options );

#define  t_edit_screen    t_edtscr

                                  /* Edit a FORM or HELP screen.       */

extern   sint t_cmdini  ( struct  memnode  ** tree_origin );
                                  /* Initialize the command line parse */
                                  /*   tables.   Args:                 */
                                  /*                                   */
                                  /*   node  **uicmdtab;               */
                                  /*                                   */

extern sint t_qwind( string formid, hword flags, real * win_width,
                          real * win_len, real * min_x, real * max_x,
                          real * min_y, real * max_y, boolean * border );
                                  /* Inquire the TM to get viewport    */
                                  /* information for the current proces*/
                                  /* Args:   string   FORMID           */
                                  /*         real    *wind_width,      */
                                  /*                 *wind_len, *x_min,*/
                                  /*                 *x_max, *y_min,   */
                                  /*                 *y_max.           */
                                  /* Returns: Sint   0                 */

extern   sint t_active  ( tint process_id );
                                  /* TM activate process to grap the   */
                                  /* attention of a particular process */
                                  /* that has been put to sleep.       */

extern   sint t_start   ( sint process_type );
                                  /* Start a new process.  Arg:        */
                                  /*                                   */

#define   USER_PROCESS      0x80  /* Process on behalf of a real user  */
#define   SYSTEM_PROCESS    0x40  /* System procedure invoked process  */
#define   VOID_PROCESS      0x20  /* Phony process.                    */

extern   sint t_stop    ( tint process_id );
                                  /* Shut down a TM process, release   */
                                  /* all its associated storage and    */
                                  /* devices, clean up servers. Arg:   */
                                  /*                                   */
                                  /*   tint   process_id_to_stop;      */
                                  /*                                   */

extern   sint t_wait    ( void );
                                  /* Wait for an ISCF event.  No args. */
                                  /* RC = event that has occured.      */

extern   sint t_que     ( hword event, void * result );
                                  /* Queue a process to wait for an    */
                                  /* event to occur before continuing. */
                                  /* sched{} is called to so something */
                                  /* else in the mean time.  Events are*/
#define  TASK_COMPLETE  ( (hword)  1 )
#define  TASK_EXCEPTION ( (hword)  2 )
#define  TASK_PAUSE     ( (hword)  3 )
#define  SHUT_DOWN      ( (hword)  4 )
#define  DISK_READ      ( (hword)  5 )
#define  DISK_WRITE     ( (hword)  6 )
#define  DISK_ERROR     ( (hword)  7 )
#define  CONSOLE_READ   ( (hword)  8 )
#define  CONSOLE_WRITE  ( (hword)  9 )
#define  CONSOLE_ERROR  ( (hword) 10 )
#define  ISCF_SEND      ( (hword) 11 )
#define  ISCF_RECEIVE   ( (hword) 12 )
#define  ISCF_ERROR     ( (hword) 13 )
#define  TIMER          ( (hword) 14 )
#define  NO_WAIT        ( (hword) 00 )

extern   sint ts_beg    ( void );
                                  /* Start the scheduler               */

extern   sint ts_end    ( void );
                                  /* Shutdown the scheduler            */

extern   sint t_s_on    ( sint type_of_signon );
                                  /* Access MS and indicate if this is */
                                  /* is a valid TEIES user.            */

#define  TM_PRIMARY_SIGNON          1
#define  TM_SECONDARY_SIGNON        2
#define  TM_MULTIPLE_SIGNON         2


extern   sint t_s_off   ( sint type_of_signoff );
                                  /* Access MS and indicate if this is */
                                  /* a signoff request.                */

extern   sint t_upduser ( void );
                                  /* Get updated new item counters from*/
                                  /* TEIESMS for this user.  No params.*/
                                  /* Returns 0.                        */

extern   sint t_m_user( string server , string data , hword flags ,
                                                        string * result);
                                  /* MS complement to the above routine*/
                                  /* Packs a segment and sends it back */
                                  /* to the requesting server.  RC is  */
                                  /* 0 if packed ok, else error code of*/
                                  /* communication or database access. */
                                  /* Params:                           */
                                  /*   string   server_to_update;      */
                                  /*   string   data;                  */
                                  /*   byte   **result;                */
extern   sint t_rcvocd( string server , string packet , hword flags ,
                                                        string * result);
                                  /* A server is sending us a dms_pak{}*/
                                  /* and an oid to unpack it into.     */
                                  /* Params:                           */
                                  /*   string   server_to_update;      */
                                  /*   string   data;                  */
                                  /*   hword    flags;                 */
                                  /*   string **result;                */

extern sint t_sndocd( string server, string packet, hword flags,
                                                        string * result);
                                  /* A server is asking us for a dbpath*/
                                  /* with an OID and we are to return a*/
                                  /* DMS_PAK{} packet if we have it.   */

extern   sint t_exobj( OCB ** list_object, hword object_type,
                       string item_ids, hword flags, string * bad_ids );
#define t_exist_object t_exobj

                                  /* Determine if a list of ids, sep-  */
                                  /* arated by commas in item_ids      */
                                  /* string are valid {exist, privs},  */
                                  /* if so return OIDs in the list obj,*/
                                  /* else return bad items in output   */
                                  /* string.                           */

extern   sint t_objid( string server, OID * object_id, string * result );
#define t_object_id  t_objid

                                  /* A server is asking this server for*/
                                  /* the Item ID when it knows the OID.*/
                                  /* Send if back to the server through*/
                                  /* t_csend{}.                        */


extern   sint t_config  ( string filename );
                                  /* Configure the system based on a   */
                                  /* Local Environment file that has   */
                                  /* various keywords. Arg  is FD.     */

extern   sint t_dump    ( void );
                                  /* For debugging purposes, display   */
                                  /* the structure of the TM header    */
                                  /* table and the TM process ID table */

extern   sint t_setst   ( void );
                                  /* Set up the TM state table.        */
                                  /*   --  no parameters  -            */

extern   sint t_opncon  ( string server_id , sint server_type );
                                  /*Open a connection to another server*/
                                  /*Parameters are:                    */
                                  /*   string   server_id;             */
                                  /*   sint     server_type;           */

extern sint t_send( string server, hword command, hword flags,
                                               sint state, string data );

                                  /* Send a TM request to another TEIES*/
                                  /* Server.                           */

extern sint t_recv( string server, hword * command, hword * flags,
                                            sint * state, string * data);

                                  /* Recv a TM request fm another TEIES*/
                                  /* Server.                           */

extern   sint t_syserv  ( void );
                                  /* Wait for a new process request to */
                                  /* come from a client user.  No      */
                                  /* parameters                        */

extern   tmentry   *t_intab( sint pid );
                                  /* Returns a pointer to the entry for*/
                                  /* a given TM_PID in the TM process  */
                                  /* table. If -1 is requested, the    */
                                  /* last entry is returned.           */

extern   sint t_iscf_t  ( string server, byte conn_type );
                                  /*Add a connection to the TM table   */
                                  /*   string server;                  */
                                  /*   byte   type_of_connection;      */

#define     TM_NETWORK_CONNECTION     10
#define     TM_LOCAL_CONNECTION       11
#define     TM_VIRTUAL_CONNECTION     12

extern   sint t_isfend  ( void );
                                  /*Close all ISCF connections.        */

extern   sint t_disp_t  ( void );
                                  /*Debug dump all current connections */

extern   sint t_cls_t   ( string server_id , sint close_iscf_connection);
                                  /*Remove a connection from the TM tab*/
                                  /*   string server;                  */

extern   ISCF_TAB *t_conn( string server );

                                  /*Locate a server in the table       */
                                  /*   string server;                  */

extern sint t_do( string server, hword command, hword flags, sint state,
                                            string data, void * result);

                                  /*Execute a command sent to this     */
                                  /*server from another server; args:  */
                                  /*NOTE:  Result is a pointer to a    */
                                  /*  pointer of any data type deter-  */
                                  /*  mined by the process called.     */

extern   sint      t_ms_off( string server, sint type_of_signoff );

                                  /*A server wants to signoff:         */

extern   sint  t_ms_on( string who , string data , hword type_of_signon);
                                  /*A server wants to signon:          */
                                  /*   string server;                  */
                                  /*   string data;                    */
                                  /*   hword  type_of_signon;          */

extern   sint      t_m_on( string server, string data );
                                  /*A server wants the connection table*/
                                  /*   string server;                  */
                                  /*   string data;                    */

extern   sint      t_g_obj( string server, hword flag, string data );
                                  /*A server wants an object; args:    */
                                  /*   string server;                  */
                                  /*   hword  flags;                   */
                                  /*   string data;  {contains OID}    */
                                  /*the flags are set as follows:      */
                                  /*  Exclusive Read Only              */
                                  /*  Multiple Read Only               */
                                  /*  Exclusive Write/Read             */
                                  /*  Multiple Write/Read              */

extern sint t_m_obj( string server, OCB * ocb, void * result,
                                        boolean ok_to_link, sint state );
                                  /*A server wants an object; args:    */
                                  /*NOTE:  Result is a pointer to a    */
                                  /*  pointer of any data type deter-  */
                                  /*  mined by the process called.     */

extern OCB * t_r_obj( string server, hword mode, OID * oid );
                                  /*We are to receive an object from a */
                                  /*server open in the mode specified. */
                                  /*        args:                      */
                                  /*   string  server;                 */
                                  /*   hword   mode;                   */
                                  /*   OID    *oid;                    */

extern sint t_s_obj( string server, OCB ** ocb, hword flags,
                                               sint state, string data );

                                  /*We are to receive an object from a */
                                  /*server;                            */

extern sint t_m_ocd( string server, string data, OCB ** result );
                                  /*A server wants some OCDs from an   */
                                  /*object; args:                      */
                                  /*   string server;                  */
                                  /*   string data;                    */

extern sint t_s_ocd( string server, string packet, OCB ** result, hword flags);
                                  /*We are to receive an set of OCDs   */
                                  /*from a server; args:               */
                                  /*   string server;                  */
                                  /*   string data;                    */

extern sint t_fndsys( hword sys_index_obj, OCD * ocd, void * result );
                                  /*Locate data in a system index.     */
                                  /*Parameters are:                    */
                                  /*   hword    SYS_INDEX_OBJ;         */
                                  /*   OCD     *ocd;                   */
                                  /*   void    *data;                  */

extern sint t_putsys( hword sys_index_obj, OCD * ocd, string data );
                                  /*Update a system index with data.   */
                                  /*Parameters are:                    */
                                  /*   hword    SYS_INDEX_OBJ;         */
                                  /*   OCD     *ocd;                   */
                                  /*   byte    *data;                  */

extern   sint      t_m_fnd ( string server, string packet );
                                  /*A server is requesting that this   */
                                  /*server locates data in a system ndx*/
                                  /*Parameters are:                    */
                                  /*   string    server;               */
                                  /*   string    packet;               */

extern   sint      t_m_put ( string server, string packet );
                                  /*A server has sent a request to up- */
                                  /*data a system index with some data.*/
                                  /*Parameters are:                    */
                                  /*   string    server;               */
                                  /*   string    packet;               */

extern sint t_cfirm( string server, string data, hword flags,
                                                          void ** dummy);
                                  /*A confirmation has been sent from  */
                                  /*a server related to a command that */
                                  /*server has executed for this server*/
                                  /*Store error code at dummy.         */
                                  /*   string server;                  */
                                  /*   string data;                    */
                                  /*   hword  flags;                   */
                                  /*   byte   **dummy;                 */

extern sint t_cfrmw( string server, hword event );
#define     t_confirm_wait                               t_cfrmw

                                  /*Wait for a given type of TM event  */
                                  /*from a particular server.  If      */
                                  /*Server is NIL then wait for the    */
                                  /*event from any server.  If event is*/
                                  /*NIL then wait for any TM event from*/
                                  /*the given server.  If BOTH are NIL */
                                  /*then wait for any event from any   */
                                  /*server {same as t_waitc{}}         */

extern   sint      t_csend( string server, sint rc, string data );
                                  /*Send a confirmation to another     */
                                  /*server.  It is expected that that  */
                                  /*is waiting for a confirmation with */
                                  /*t_waitc{} or is t_syserv{} mode.   */
                                  /*   string server;                  */
                                  /*   sint   rc;                      */
                                  /*   string data;                    */
                                  /*                                   */
                                  /*returns iscf errors if bad connect.*/
                                  /*Data is anything to go back.       */

extern sint t_waitc( string server_id, void * result_ptr );

                                  /*Once a command request has been    */
                                  /*sent off to a server, wait for a   */
                                  /*confirmation that it was completed.*/
                                  /*NOTE:  Result is a pointer to a    */
                                  /*  pointer of any data type deter-  */
                                  /*  mined by the process called.     */

extern   OID *     t_getnam( string name, sint flag );
                                  /*                                   */
#define  t_getname t_getnam       /*Locate if a name or name part is in*/
                                  /*the data base. Return the OID of   */
                                  /*user object that has the requested */
                                  /*name.             Args:            */
                                  /*   string  name;                   */
                                  /*   hword   flag;                   */
                                  /*                  Flags are:       */
#define ANY_NAME      0xFFFF      /*Any name piece, must derive name   */
#define FULL_NAME     0x0001      /*full name given, ...               */
#define FIRST_NAME    0x0002      /*                                   */
#define LAST_NAME     0x0004      /*                                   */
#define NICK_NAME     0x0008      /*                                   */
#define SYSTEM_NAME   0x0010      /*                                   */
#define SYSTEM_ID     0x0010      /*                                   */
#define PEN_NAME      0x0020      /*                                   */
#define NUMBER_NAME   0x0040      /*                                   */
#define ALIAS_NAME    0x0080      /*                                   */

extern sint t_valnam( string names, OID *** oid_array,
                    sint * array_size, string * bad_names, hword flags );

                                  /*Given a string of any names try to */
                                  /*determine the OID's of all of them.*/
                                  /*Also takes search flag for t_getnam*/
                                  /*and output string to place names   */
                                  /*that do not match.  The return code*/
                                  /*is NAME_NOT_FOUND + the number of  */
                                  /*names not found.                   */
                                  /*                  Args:            */
                                  /*   string  names;                  */
                                  /*   OID     ***result_oids;         */
                                  /*   string  *bad_names;             */
                                  /*   hword   flag;                   */
                                  /*                                   */

extern   sint      t_ckname( string user_name, hword check_level,
                                          sint * invalid_char_position );
                                  /*Check a name string for invalid    */
                                  /*characters.  Levels are:           */

#define T_CKNAME_FULL_CHECK            0
#define T_CKNAME_IGNORE_COMMAS         1
#define T_CKNAME_NONPRINTABLE_ONLY     2


extern   sint      t_fname ( string name_part, string last_name,
                                 string first_name, OCB ** list_object );

                                  /*Find a name given various pieces   */

extern   OCB     * t_author( void );
                                  /*Return the OCB of the TEIES user   */
                                  /*running the current process.       */
                                  /*No args.                           */

extern   boolean   t_ckpswd( OID * oid, string password );
                                  /*Given an OID of a user object and  */
                                  /*a password string, return TRUE if  */
                                  /*the password matches, else FALSE.  */
                                  /*                  Args:            */
                                  /*   OID    *name;                   */
                                  /*   string  password;               */

extern   sint  t_ckkeys( string * result_keys , string keys);
                                  /*Given keyword string format it and */
                                  /*determine if they all fit in the   */
                                  /*index in context.                  */
                                  /*                  Args:            */
                                  /*   string *result_keys;            */
                                  /*   string  keys;                   */

extern   sint  t_ckpriv( TM_PID process_id, PRIVS * privilege );
                                  /*Given a PRIV map, check            */
                                  /*if the role is available.          */

extern   sint  t_gtdate( string * result, datetime * specified_date );
                                  /*Return the date in a string format-*/
                                  /*ted to the users specifications.   */
                                  /*Params:                            */
                                  /*   string   *result;               */
                                  /*   datetime *dt;                   */

extern   sint  t_stopms( void );
                                  /* Stop the Master Server from a     */
                                  /* remote server. No args.           */

extern   sint      t_addfrm( string form_id );
                                  /*Given a form_id, ask the user where*/
                                  /*the text to the form is, then add  */
                                  /*it to the SFI.                     */
                                  /*                  Args:            */
                                  /*   string  form_id;                */
                                  /*                                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *     Define the Date and Time manipulation routines
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


extern  sint    dt_tmdif( datetime * dttm1, datetime * dttm2 );

             /*   FUNCTION: compares date&time structures and returns rc:
              *                  {was dttmdiff{ }}
              *   INPUTS:  datetime     *dttm1, *dttm2;
              *
              *   OUTPUTS: return code
              */
#define DT_EQUAL          0 /* if dttm1 = dttm2   */
#define DT_GREATER_THAN   1 /* if dttm1 > dttm2   */
#define DT_LESS_THAN      2 /* if dttm1 < dttm2   */

extern  sint    dt_epoc( void );
                                  /* Return the current date/time epoc */
                                  /* value.  No args.                  */
                                  /*                                   */
                                  /*                                   */

extern  sint    dt_cepoc( sint epoc1, sint epoc2 );
                                  /* Return the difference between 2   */
                                  /* datetime epoc values.  ARgs:      */
                                  /*                                   */
                                  /*  sint    epoc1, epoc2;            */

extern  sint    t_seterr( sint error_code, string * error_message );
                                  /* Set the TM Interface variable to  */
                                  /* an error message out of the SERRI.*/
                                  /*                                   */
                                  /*  sint    error_code;              */
                                  /*                                   */

extern  sint    t_clrerr( TM_PID process_id  );
                                  /* Clear any pending error conditions*/
                                  /* for the state requested.          */
                                  /*                                   */

extern sint except( sint code, sint type, string arg1, string arg2,
                               string arg3, string arg4, string arg5 );

                                  /* Exception Handler entry point     */
                                  /* {code,type, 5 Teies strings}.  Re-*/
                                  /* turns code.  See EXC.H.           */
                                  /*                                   */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $ENDEXTRACT                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
