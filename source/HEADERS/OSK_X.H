/*********************************************************************
 *                                                                   *
 *  File Name:  OSK     H                                            *
 *                                                                   *
 *  Purpose:    To keep all the EXTERNAL definitions required to     *
 *              use the operating system interface routines.         *
 *                                                                   *
 *  Author:     Sal Johar                                            *
 *                                                                   *
 *  Version:    1.0                                                  *
 *                                                                   *
 *  Date:       15 August 1986                                       *
 *                                                                   *
 *  Modification History:                                            *
 *       Date           Author              Change                   *
 *   Jan 25   1988       Sal             Added OS_CP & OS_CSP extern *
 *   Mar 20   1988       Sal/JF          Added OS_GFDD extern        *
 *   Dec 03   1988       Sal             Phased out os_con   and     *
 *                                       chnaged return type for     *
 *                                       os_edtr   from sint->string *
 *   Jan 24   1989       Kevin           Moved datetime def to DEFS.H*
 *                                                                   *
 *   Feb 13   1989       Sal             Prototyped and removed      *
 *                                       dependency on STR.H         *
 *                                                                   *
 *********************************************************************/


/*********************************************************************/
/*                                                                   */
/*      Date  and  Time  functions                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
extern struct ttime                    /* to call time routines      */
       {

              byte     hour;
              byte     minute;
              byte     second;
              byte     fract;
       };

extern struct tdate                    /* to call date routines      */
       {
              byte     century;
              byte     year;
              byte     month;
              byte     day;
       };

typedef    struct  tdate        date;
typedef    struct  ttime        time;

extern struct cqysect                  /* os_qydev for 3270 devices. */
       {                               /*                            */
                                       /*                            */
              sint      cqyhplen;      /* Length of path section     */
              sint      cqyhdlen;      /* Length of device section   */
              sint      cqydusct;      /* Number of open paths       */
              sint      cqydnumb;      /* Virtual Device Number      */
              byte      cqydvcls;      /* VD Class                   */
              byte      cqydvtyp;      /* VD Type                    */
              byte      cqydvsst;      /* VD Status                  */
              byte      cqydvflg;      /* VD Flags                   */
              byte      cqydrcls;      /* Real Device type class     */
              byte      cqydrtyp;      /* RD type                    */
              byte      cqydrmdl;      /* RD model number            */
              byte      cqydrftr;      /* RD features code           */
              byte      cqydllen;      /* Virtual Console line length*/
              byte      cqydtmcd;      /* VC terminal code           */
              byte      cqyattr;       /* Device attribute flag mask */
              byte      cqydstat;      /* Device status flag         */
              byte      cqydqrfl;      /* Additional Device Flags    */
              byte      cqydqrpn;      /* Number of Partitions       */
              hword     cqydqrcl;      /* Number of columns {Width}  */
              hword     cqydqrrw;      /* Number of rows    {Height} */
              byte      cqydqycd;      /* Device query code          */
              byte      cqydresA;      /*  -----Reserved-----        */
              sint      cqyd8cl;       /* Length of Diag'8c' add.info*/
              byte     *cqyd8cp;       /* Ptr to 8C add. buffer      */
              sint      cqydresB;      /* Reserved                   */
              byte     *cqypexit;      /* User exit address          */
              sint      cqypxwrd;      /* User exit word             */
              byte      cqypflg;       /* Path flag                  */
              byte      cqydresC[7]; /*  -----Reserved-----        */
              byte      cqypslcc;      /* Logout pending CC codes    */
              byte      cqydresD[3]; /*  -----Reserved-----        */
              tint      cqypccw;       /* Last CCW executed          */
              byte      cqypust;       /* Unit status code           */
              byte      cqypcst;       /* Channel status byte        */
              hword     cqyprct;       /* Residual count             */
              sint      cqypscnt;      /* Sense count                */
              byte      cqypsdta[32];/* Sense data                 */
       };                              /*                            */
                                       /*                            */
typedef  struct cqysect CQYSECT;       /* - - - - - - - - - - - - -  */

                             /* Following macros were used to get    */
                             /* around prototyping problems. These   */
                             /* macros cast args to the type expected*/

#define os_memc( srcptr, trgptr, len)      \
        os_mcpy( (byte *) (srcptr), (byte *) (trgptr), (sint) (len) )

#define os_mcmp( memptr1, memptr2, len )   \
        os_memp( (byte *) (memptr1), (byte *) (memptr2), (sint) (len) )

#define os_memn( memptr, len )             \
        os_mnul( (byte *) (memptr), (sint) (len) )

#define os_memd( freeptr )                 \
        os_mfree( (byte *) (freeptr) )

/*
$maker-proto
$extract */

extern sint os_date( date *ptr); /* returns date in the structure.
                               input: ptr: struct tdate *ptr;        */

extern sint os_time( time  *ptr); /* returns time in the structure.
                               input: ptr: struct ttime *ptr;        */

extern sint os_dttm( datetime *ptr); /* returns datetime in structure
                                  input: ptr: struct tdttm *ptr;     */

extern sint os_dtadd( datetime **date_in, datetime *date_to_add);

extern sint os_dtsub( datetime **date_in, datetime *date_to_sub);

/*********************************************************************/
/*                                                                   */
/* Terminal Device I/O handling functions.                           */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
                                       /* The following structure is */
                                       /* for use with the Query of  */
                                       /* Now define the device      */
                                       /* access routines:           */
                                       /*                            */
extern sint os_opdev( string device); /* open console device     */
extern sint os_cldev( string device); /* Close device            */
extern sint os_read( string vaddr, byte *buffer, sint *length);
                                       /* wait for keyboard input    */
extern sint os_write( string vaddr, byte *buffer, sint length,
                          sint flag);       /* update terminal display */
extern sint os_qydev( string device, CQYSECT *buffer);
                          /* Query a device for its atts*/
                                       /******************************/

extern sint os_conn( sint func, sint devi, byte *bufptr,
                                hword *buflenptr, sint flags);
                                         /* interface to file transfer*/

                     /* following functions are used in OS_CONN      */
#define CON_ON    1
#define CON_OFF   2
#define CON_OPEN  3
#define CON_CLOSE 4
#define CON_READ  5
#define CON_WRITE 6
#define CON_CLEAR 7
#define CON_QUERY 8
#define CON_WAIT  9

#define CON_DEVICE -1 /* default device to do i/o                    */
#define CONFLAGS 0
                     /* following functions are used in OS_TRAN      */
extern sint os_tran( byte func, byte *buf, sint len);
#define ASC2EBC 1    /* Translate  ascii buffer to ebcdic            */
#define EBC2ASC 2    /* vis-a-vis                                    */

/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*   M e m o r y   M a n a g e m e n t   F u n c t i o n s           */
/*                                                                   */
/*********************************************************************/

extern sint os_aloc( sint *ptr, sint *maxlen, sint minlen );
 /* Calls OSDFREE.ass to allocate memo                  ry
                                 input: Ptr: where to store address
                                        maxl: sint ptr, max bytes needed
                                        minl: min bytes requested
                                  returns: 0 for success, else err   */

extern  sint     os_free( sint *ptr, sint len); /*  de-allocates memory
                                   input: ptr: address to be freed
                                          len: number of bytes to free
                                   returns: 0 for success, else err */

extern  byte    *os_mema( sint size); /* allocates memory given
                                  input: tint size;
                                  returns: NIL if no memory          */


extern  sint     os_mfree( byte *point);
                        /* deallocates memory pointed by
                                   input: byte * ptr;
                                   returns: 0 for success            */

extern  sint     os_memk( byte *pid);
                                  /* deallocates all memory as sociated
                                   to PID as the first arguement.    */

extern  sint     os_mcpy( byte *srcptr, byte *trgptr, sint length);
                                /* copy memory                       */

extern  sint     os_memp( byte *ptr1, byte *ptr2, sint length);
                   /* compare memory                    */

extern void os_mnul( byte *pointer, sint length);
                        /* null out the memory */
                        /* pointer by 1st arg, len long{2nd arg} */

extern bool os_cp( byte *pointer);  /* chk memory pointer */

extern bool os_csp( string str_ptr); /* chk pointer to see if it is a*/
                                /*  valid teies string pointer       */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| The os_edtr.c function accepts a file descriptor in the form      |*/
/*| "FM FP FN FT", inwhich  {in IBM implementation}:                  |*/
/*|                                                                   |*/
/*|  FM = File mode                                                   |*/
/*|  FP = Not used {use "*" wildcard as placeholder}                  |*/
/*|  FN = File name                                                   |*/
/*|  FT = File type                                                   |*/
/*|                                                                   |*/
/*|  This implementation calls XEDIT.  The default file descriptor is |*/
/*|  "* * SPAD *".                                                    |*/
/*|                                                                   |*/
/*| format:       os_edtr{filedescriptor};                            |*/
/*|                                                                   |*/
/*|                       filedescriptor is a string                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern string os_edtr( string file, sint *caller_rc, bool init);

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_parse.c extracts portions of FD's.  It also does character     |*/
/*| validations.                                                      |*/
/*|                                                                   |*/
/*| format:       os_parse{&result,given,&pointer,maxlen};            |*/
/*|                                                                   |*/
/*|                       result is a string address                  |*/
/*|                       given is the input string                   |*/
/*|                       &pointer an address of a tint, points into  |*/
/*|                           given                                   |*/
/*|                       maxlen is a sint indicating max len of      |*/
/*|                           &result to be accepted                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint os_parse( string *output, string input, sint *ptr,
                      sint maxlen, sint glen);

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_gfde.c assemples portions of a resulting FD by checking the    |*/
/*| given and provided portions and then selecting on to append       |*/
/*| {gfde=generate file descriptor element}                           |*/
/*|                                                                   |*/
/*| format:       os_gfde{&result,given,default};                     |*/
/*|                                                                   |*/
/*|                       given and default are strings               |*/
/*|                       result is a string address                  |*/
/*|                                                                   |*/
/*|   Note:  check source for important rules if interested.          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint os_gfde( string *result, string given, string defalt);

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_qfd.c will "qualify file descriptor".  Passed a given and a    |*/
/*| default, it attempts to generate system dependant file descriptors|*/
/*| from a system independant file descriptor                         |*/
/*|                                                                   |*/
/*| format:       os_qfd{&result,given,default};                      |*/
/*|                                                                   |*/
/*|                       given and default are strings               |*/
/*|                       result is a string address                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint os_qfd( string *results, string given_i, string defalt_i)           ;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_gfdd.c will remove a TEIES generic file from the local file    |*/
/*| system.  See the description of GFD's in the specs.  Calls CRTL   |*/
/*| unlink.c {v5} or remove.c {ANSI}.                                 |*/
/*|                                                                   |*/
/*| format:   rc = os_gfdd( generic_file_name };                      |*/
/*|                                                                   |*/
/*|                  sint   rc;                                       |*/
/*|                  string generic_file_name;                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern  sint os_gfdd( string gfd);

/* Copy CMS files */
extern sint os_fcpy( string source, string target );

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_call.c executes the supplied system command.                   |*/
/*|                                                                   |*/
/*| format:       os_call{command};                                   |*/
/*|                                                                   |*/
/*|                       command is a string                         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   sint     os_call( string command);

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_uid.c     : Local  OS User ID                                  |*/
/*|                                                                   |*/
/*| Returns the USer ID in a TEIES string of the current executing    |*/
/*| user.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   sint     os_uid( string *name);

extern sint os_bsub( byte *ptr, sint len, byte subconst);
                /*  subtract byte out str  */
                /*  byte  *ptr;   string ptr
                    sint  len;    length of the string to modify
                    byte  const;  byte to subtract
                */



extern sint os_wait( sint period);  /* request for wait in seconds. */
                /*      oswait{waitperiod}
                 *      sint waitperiod;
                 *     return code:
                 *                    0:  successfull wait
                 *                   -1:  wait failed
                 */

extern sint os_wint( sint event, sint interval); /* wait for interrupt */
                /*
                 *      sint event, waitperiod;
                 *                             waitperiod in 100th secs
                 *     return code:
                 *                    0:  successfull wait
                 *                   -1:  interrupt did not occur.
                 */

extern void os_ctrak( void);    /* used by D_tracebak macro       */



/*********************************************************************/
/*                                                                   */
/*   Server Communications Vehicle {TEIES}                           */
/*   I U C V   M a n a g e m e n t   F u n c t i o n s               */
/*                                                                   */
/*                                                                   */
/*********************************************************************/

extern sint scv_recv( string vmid, string *buf);
                /*
                 * IUCV receive function
                 *
                 *   string    vmid
                 *   string    buffer
                 */
extern sint scv_send( string vmid, string buf);
                /*
                 * IUCV send function
                 *
                 *   string    vmid
                 *   string    buffer
                 */
extern  sint  scv_on( sint *max_con);
                /*
                 * Initialize the SCV - IUCV interface - no server
                 * communication possible before this call. Call only
                 * once!
                 *
                 *   sint *maxcon;     max connection possible
                 */
extern   sint  scv_off( void);
                /*
                 * Shut down the SCV-IUCV interface - no server
                 * communication possible after this function call.
                 */
extern   sint  scv_open( string vmid, sint *pathid);
                /*
                 * Open a connection between this server and another
                 *
                 *   string   vmid;
                 *   sint     *pid;
                 */
extern   sint  scv_cls( string invmid);
                /*
                 * Close a connection between this server and another
                 *
                 *   string   vmid;
                 *
                 */
extern sint scv_qry(string vmid,sint flags,sint *intdata,
                    string *strdata);
                /*
                 * Poll all waiting IUCV receives for this server.
                 * Returns sint for total number of waiting transfers.
                 *
                 *   string   vmid;
                 *   sint     flags;
                 *   sint     *intdata;
                 *   string   *strdata;
                 *
                 */
                /* Following are used by above levels and by scv_qry   */
#define NEW_SOMETHING  0
#define NEW_MESSAGE    1
#define MESSAGE_LEN    2
#define PATH_ID        3
#define NEW_DISCONNECTION 4
#define NEW_CONNECTION 5
#define QRY_CONNECTION_TYPE 6


extern   sint  scv__off( void);
                /* Tweak routine. call only if system was previously
                 * brought down improperly.
                 */
extern sint    scv_dump( void);
                /* Dump SCV structures while running rexxrun
                */
/* $endextract  */

extern void os_dcall( void );          /* In os_ctrak.c */
extern byte *os_dcbuf;                 /* Used in os_ctrak.c and       */
                                       /* assigned value in os_mem.ppp */

#define OSK_ERR                     11000
#define OSK_ERR_FUNC_NOT_DEF          888
#define INVALID_LENGTH                777
#define NO_ERR                          0

#define SCV_ERR                     30000
#define SCV_NO_ERR                 NO_ERR
#define ERR_INTERNAL_OFFSET            60
#define ERR_ALREADY_CONN               61
#define ERR_CONN_BUT_SEVER             62
#define ERR_IUCV_RECV                  63
#define ERR_CONN_EXIST                 64
#define ERR_NEVER_INIT                 65
#define ERR_ALREADY_INIT               65
#define ERR_NO_PATH                    66
#define ERR_NO_MSG                     67
#define ERR_SELF_CONN                  68
#define ERR_NO_NEW_CONN                70
#define ERR_NO_NEW_DSC                 71
#define ERR_NEW_MSG_BUT_DISC           72
#define ERR_NEW_CONN_BUT_DISC          73
#define ERR_MSG_BUT_NO_CONN            74
#define ERR_NO_MATCHING_PID            79
#define ERR_FUNC_NOT_DEF               80
#define ERR_CON_TIME_OUT               81
