/*********************************************************************
  <DOC>
  <Name>     TM Header
  <Level>    TM (Transaction Manager)
  <Date>     November 5, 1986
  <Author>   John L. Foster
  <Purpose>

  This module provides external data constants and structures for the
  programmer calling upon the functionality of the TM. Most important
  is the global TM Process Table and Master Process Control Block.

  Refer to the section on the TM level in the Specification Document.

  </Doc>
*********************************************************************/

                                        /*******************************/
                                        /* Set constants for the       */
#define NUMBER_OF_ODTs         32       /* total allowable number of   */
#define MAX_OCB_LIST          128       /* ODT's and the maximum number*/
                                        /* of OCB's in the system.     */
                                        /*******************************/

                                        /*******************************/
                                        /* Define the data type for a  */
typedef tint            TM_PID;         /* TM process ID.              */
                                        /*******************************/
                                               /***********************/
                                               /* This structure has  */
extern  struct  _init_                         /* values required to  */
        {                                      /* initialize all the  */
                                               /* TEIES levels.       */
                                               /*---------------------*/
        string         lfd_name;               /*"DATABASE" for DMS   */
        sint           buffers;                /*"BUFFERS"            */
        sint           mem_size;               /*"MEMORY"             */
        byte          *mem_start;              /*"LOCATION ="         */
        string         ISV_name;               /*"ISV"                */
        byte           prt_class;              /*"CLASS"              */
        string         printer;                /*"PRINTER"            */
        string         editor;                 /*"EDITOR"             */
        string         MS;                     /*"MASTERSERVER"       */
        string         datefmt;                /*"DATEFORMAT"         */
        string         error;                  /*"ERROR_LOG"          */
        string         access;                 /*"ACCESS="            */
        sint           blk_size;               /*"BLOCK_SIZE"         */
        sint           num_buff;               /*"NUMBER_BUFFERS"     */
        sint           hash_size;              /*"HASH_TABLE"         */
        sint           time_zone;              /*timezone=n           */
        sint           db_size;                /*size=n               */
        string         editor_profile;         /*Profile to invoke    */
                                               /*editor with.         */
        sint           max_num_lfds;           /*Total number of LFDs */
                                               /* this server can acc-*/
                                               /* ess for DMS.        */
#define TM_MAX_NUMBER_OF_LFDS           16     /*Cannot exceed this # */
                                               /*                     */
                                               /*For each DATABASE    */
                                               /*  entry:             */
                                               /*                     */
        string       * _lfd_name;              /*"DATABASE" for DMS   */
        string       * _lfd_access_mode;       /*"ACCESS="            */
        sint         * _lfd_block_size;        /*"BLOCK_SIZE"         */
        sint         * _lfd_number_of_blocks;  /*size=n               */
                                               /*                     */
        };                                     /*                     */
                                               /***********************/


                                               /***********************/
                                               /* This structure is a */
extern  struct  _ISCF_TAB                      /* table of all connect*/
        {                                      /* ed servers to this  */
                                               /* server.             */
                                               /*---------------------*/
        string         serv_name;              /*Name of the server.  */
        string         acc_method;             /*Network access method*/
        tint           status;                 /*Status flags bitmap. */
        datetime       on;                     /*date/time connected. */
        OID            objectID;               /*OID of who connected */
 struct _ISCF_TAB     *next;                   /*Linked list.         */
 struct _ISCF_TAB     *prev;                   /*Back pointer.        */
        PRIVS          privs;                  /*Privileges for user  */
        byte           conn_type;              /*Connect to type of   */
                                               /*server, see USER TYPE*/
                                               /*below in UCB         */
        TM_PID         tm_pid;                 /*TM process ID associ-*/
                                               /*ated to this connect.*/
        sint           created;                /*Objects created and  */
        sint           modified;               /* modified during this*/
                                               /* session.            */
        string         user_name;              /*Formatted users name */
                                               /*who is on (gt_name)  */
        sint           received;               /*Objects received this*/
                                               /* session.            */
        };                                     /*                     */
                                               /***********************/
typedef struct _ISCF_TAB  ISCF_TAB;

#define ISF_TAB_POINTER_OK( entry ) \
  ( (entry != NIL )  &&  ( entry != (ISCF_TAB *) -1 ) )



                                    /* Loop break point on all wait fns*/
#define  WATCHDOG_LIMIT            15
#define  RECEIVE_WATCHDOG_LIMIT    15
#define  SEND_WATCHDOG_LIMIT       15

#define  ISCF_NOT_CONNECTED(rc)       (((rc % 100)==ERR_NO_PATH) || \
                                       ((rc % 100)==ERR_CONN_BUT_SEVER))
#define  SERVER_NOT_CONNECTED(rc)     (((rc % 100)==ERR_NO_PATH) || \
                                       ((rc % 100)==ERR_CONN_BUT_SEVER))
#define  SERVER_CONNECTED( rc )   ( ! (((rc % 100)==ERR_NO_PATH) || \
                                       ((rc % 100)==ERR_CONN_BUT_SEVER)))
#define  CLOSE_CONNECTION          TRUE
#define  KEEP_CONNECTION           FALSE

#define  TM_PACKET_HEADER_SIZE     8
#define  TM_PACKET_COMMAND_OFFSET  0
#define  TM_PACKET_FLAGS_OFFSET    2
#define  TM_PACKET_STATE_OFFSET    4
#define  TM_PACKET_DATA_OFFSET     8


/*+-------------------------------------------------------------------+
  |                                                                   |
  | The following macros are to be used to decode OCD packets send    |
  | and received between servers.  They all take the string containing|
  | the OCD packet.  Each offset is computed.  Each OCD and respective|
  | data can be computed off the first OCD, as they cannot be figured |
  | from a marco as the offsets will be different for each packet.    |
  |                                                                   |
  +-------------------------------------------------------------------+*/
#define  ocd_packet_OID(packet)   ((OID *)s_buffer(packet))

#define  ocd_packet_type(packet)  ((hword *)s_buffer(packet)+8)

#define  ocd_packet_count(packet) ((hword *)s_buffer(packet)+10)

#define  ocd_packet_ocd(packet)   ((string) s_buffer(packet)+12)



extern   struct    TM_entry
                   {
                                               /************************/
                        TM_PID    tm_pid;      /* Process ID           */
                        tint      flags;       /* Process status flags */
                                               /* Valid bit masks are: */
#define  PROCESS_PRINTER_ACTIVE    0x0004
#define  PROCESS_ALARM_PENDING     0x0002
#define  PROCESS_SHUT_DOWN_PENDING 0x0001

                        sint      status;      /* Process status value */
                        sint      except;      /* Current error number */
                        sint      exc_type;    /* Current error type   */
                        string    exc_msg;     /* Ptr to error string  */
              struct    hasht  ** vars;        /*Process interface vars*/
              struct    TM_entry *next;        /* Ptr to next PID entry*/
              struct    TM_entry *prev;        /* Ptr to prev PID entry*/
                        string    cmd_line;    /* Form command line str*/
                        ISCF_TAB *conn_entry;  /*Connection entry of a */
                                               /*user this proc is for.*/
                        sint      type;        /*Type of process(TM_X H*/
                        PRIVS     role;        /*Role privileges for   */
                                               /*  this process.       */
                        string    f_setfld;    /* Current field to keep*/
                        byte      state[4];  /* Current STATE ID     */
                        byte    * forms;       /* Ptr to forms         */
                                               /*  struct form_process */
                                               /************************/
                   };


typedef       struct    TM_entry  tmentry;


extern   struct    user_cb
                   {
                                             /**************************/
                        OID       user_id;   /* User internal ID number*/
                        string    os_id;     /* User local name/ID     */
                        OCB      *directory; /* Ptr to OCB for director*/
                        datetime  time_on;   /* Date signed on (TEIES) */
                        sint      class;     /* Billing class          */
                        string    name;      /* Ptr to name string     */
                        string    editor;    /* Users choice text editr*/
                        string    dtform;    /* String for dttm render */
                        sint      time_zone; /* User's time zone off GM*/
                        byte      level;     /* User interface level   */
                                             /*                        */
#define                 FIRST_TIME_USER    0 /* First signon           */
#define                 CASUAL_USER        1 /* Casual User Interface  */
#define                 EXPERT_USER        2 /* Experienced User       */
#define                 POWER_USER         3 /* "Super TEIES" User     */
                                             /*                        */
                        byte     *uistack;   /* UI State Stack         */
                        string    help_rqc;  /* UI String to get Help  */
                        string    help_rqs;  /* UI String to get Help  */
                        TM_PID    tm_pid;    /* Process ID for this usr*/
                        ISCF_TAB *conn_entry;/* Conn Table pointer     */
                        string    editor_profile; /* See _INIT_ struct.*/
                                             /**************************/
                   };


typedef  struct    user_cb        UCB;

                                              /*************************/
struct   odtlist   {                          /*                       */
                                              /*                       */
                        hword      obj_type;  /* The object def type   */
                        odtrecord *odt_ptr;   /* Ptr to the obj def tab*/
                        OAMT      *actions;   /*                       */
                                              /*                       */
                   };                         /*   Used in TMCB below  */
                                              /*************************/
typedef struct odtlist ODTLIST;


struct   _lfd_tab  {                          /*************************/
                                              /*                       */
                        string     lfd_name;  /* LFD Generic File Desc */
                        hword      flags;     /* status and type flags */
                        hword      lfd_num;   /* LFD number            */
                                              /*                       */
                                              /*                       */
                   };                         /*************************/

typedef struct _lfd_tab LFD_TABLE;

#define  SYS_LFD  1



extern   struct    TMCB
                   {
                                              /*************************/
                     tint         processes;  /* # of active PID's     */
                     tmentry     *current;    /* Ptr to current PID    */
                     tint         currentPID; /* current PID #         */
                     tmentry     *first;      /* Ptr to first PID      */
                     OCB         *OBJcb;      /* Ptr to all Open OCB's */
                     ODTLIST     *odt;        /* Ptr to ODT lookup tab */
                     byte        *ISV;        /* Ptr to I String Vector*/
                     UCB         *ucb;        /* User Control Blk      */
                     byte        *CMD;        /* Ptr to command ndx    */
                     byte         usrtype;    /* type of user (see belo*/
                                               /************************/
                                               /* Values for usrtype:  */
                                               /*                      */
#define       SIGNON_PENDING      0            /* Server not yet       */
#define       PENDING_SIGNON      0            /*   identified.        */
#define       REAL_USER           1            /* Real-live person     */
#define       USER_SERVER         1            /* Real-live person     */
#define       PSEUDO_USER         2            /* "Background" process */
#define       MASTER_SERVER       3            /* Master DBMS server   */
#define       DBMS_SERVER         4            /* DBMS server machine  */
#define       DATABASE_SERVER     4            /* DBMS server machine  */
#define       NET_SERVER          5            /* Network Server machin*/
#define       NETWORK_SERVER      5            /* Network Server machin*/
#define       UNKNOWN_SERVER      6            /* Do not know          */
                                               /*                      */
                                               /* Also used in t_opncon*/
                                               /* and t_iscf_t().      */
                                               /************************/
                                              /*                       */
                     string       MS;         /* Master Server ID      */
                     hword        lfd;        /*LFD of main DMS objects*/
             struct  _init_      *sys_init;   /* System init info      */
                     LFD_TABLE   *lfd_tbl[5]; /*All available LFD's    */
                                              /*                       */
                     tint         flags;      /*State flags, defined as*/
                                              /*                       */
#define      NO_FLAGS_SET                  0  /*                       */
#define      SHUT_DOWN_REQUESTED  0x00000001  /* Mask if a shutdown of */
                                              /*this server is requestd*/
#define      PAUSE_SERVER         0x00000002  /* Put the server in hold*/
#define      RESUME_SERVER        0x00000004  /* Un-Pause this server  */
#define      FLUSH_OPEN_OBJECTS   0x00000008  /* Update the data base  */
#define      CONNECTED_TO_MASTER  0x00000010  /*                       */
#define      IN_SIGNON            0x00000020  /*                       */
#define      IN_SIGNOFF           0x00000020  /*                       */
#define      IN_EXCEPT            0x00000040  /*                       */
                                              /*                       */
             struct  hasht     ** vars;       /*Global UI Variables    */
                     string       version;    /*Version ID name        */
                     sint         release;    /*Release identifier     */
                     ISCF_TAB    *conn_tab;   /*ISCF Connection Table  */
                                              /*                       */
                     byte        *uicmdtab;   /*User interface command */
                                              /*line command table.    */
                                              /*                       */
                     OCB         *sni_ocb;    /*Optimization to keep   */
                                              /*the SNI directory at   */
                                              /*close hand.            */
                                              /*                       */
      struct         ODT_T_R     *odt_t_r;    /*Object Level table for */
                                              /*ODT_MAKE().            */
                                              /*                       */
                     sint         obj_t_size; /*Size of Object Name Tbl*/
                     tint         Next_PID;   /*Next available TM PID. */
                                              /*                       */
#define  TM_MAX_PROCESS_ID      0x8FFFFFFF    /*                       */
#define  TM_MIN_PROCESS_ID      0x00000002    /*                       */
#define  TM_GLOBAL_PROCESS_ID   0x00000001    /*                       */
#define  TM_INITIALIZATION_ID   0x00000000    /*                       */
                                              /*                       */
                     byte       * forms;      /*Ptr to Global forms    */
                                              /*  data (tags, PFKeys,  */
                                              /*  etc. )               */
                                              /*************************/


                   };


                                                   /********************/
extern   struct    TMCB          *TM_master;       /* Pointer to the   */
                                                   /* TM process table */
                                                   /********************/


#define MASTER_SERVER_IS_CONNECTED() \
                                 (TM_master->flags & CONNECTED_TO_MASTER)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                     *
 *     Definitions for TM transaction requests                         *
 *     For use with T_DO.                                              *
 *                                                                     *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define LOWEST_REMOTE_COMMAND   1 /*          Range checking low end   */
#define TM_ON                   1 /* Retrieve list of connections      */
#define SIGNON                  2 /* Connect with a serve              */
#define SIGNOFF                 3 /* Disconnect from the server        */
#define GET_OBJECT              4 /* Retrieve an ObjectID              */
#define SEND_OBJECT             5 /* Send back an ObjectID             */
#define OCD_REQUEST             6 /* Get OCDs from an ObjectID         */
#define CONFIRMATION            7 /*command confirm was sent to verify */
#define MS_SHUT_DOWN            8 /* Shut down this server.            */
#define OCD_SEND                9 /*Send set of OCDs to another server */
#define SIGNON_VERIFY          10 /*Signon verify of name and password */
#define OBJECT_ACTION          11 /* Remote operation request          */
#define UPDATE_SYSTEM_INDEX    12 /*t_fndsys, t_putsys                 */
#define LOCATE_IN_SYSTEM_INDEX 13 /*                                   */
#define SAY_SOMETHING          14 /*See if server is alive             */
#define SEND_SEGMENT           15 /*Multiple obj_get/rep ops across    */
#define RECEIVE_SEGMENT        16 /* servers.                          */
#define RECEIVE_OBJECT_ID      17 /*Get an object id given its OID     */
#define SEND_OBJECT_ID         18 /* that is on another server.        */
#define SEND_SINGLE_SEGMENT    19 /*More efficient than SEND_SEGMENT   */
                                  /* only works for 1 segment at a time*/
#define SESSION_STAT_INFO      20 /*Get User statistic record for cur- */
                                  /* rent signon session.              */
#define HIGHEST_REMOTE_COMMAND 20 /*    Range checking high end        */
                                  /*************************************/

                                     /**********************************/
                                     /* Flag bit maps for the TM level */
                                     /* communications functions.      */
#define TRANSACTION_SUCESSFUL 0x0000 /*                                */
#define ERROR_IN_TRANSACTION  0x0002 /*                                */
#define OK_TO_LINK_OBJECT     0x0080 /*                                */
                                     /**********************************/



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                     *
 *     Definitions for TM user interface states                        *
 *     used by T_JMPST and the TM_SELECT_STATE sub-function.           *
 *                                                                     *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define   TM_UPDATE_USER_COUNTERS         1
#define   TM_DISPLAY_CONNECTIONS          2
#define   TM_SYSTEM_MONITOR_MENU          3
#define   TM_SYSTEM_COMMAND_MENU          4
#define   TM_GET_LAST_USER_STAT_RECORD    5


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * table of days per month for non-leap year & leap year respectly     *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern  byte  day_tab[ 2 ][ 13 ];

/*
 * byte day_tab[2][13] =           <<-- Appears in TEIES.H
 * {
 *     { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
 *     { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
 * };
 *
 */



/* macro returns a flag for leap year check */


#define LEAP(x) (x % 4 == 0 && x % 100 != 0 || x % 400 == 0)


#define  START_UP_MEMORY_SIZE  1312000
