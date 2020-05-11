/*********************************************************************
 *                                                                   *
 *  File Name:  OSK_I H                                              *
 *                                                                   *
 *  Purpose:    To keep all the INTERNAL definitions required to     *
 *              use the OSK   functions.                             *
 *                                                                   *
 *  Author:     Sal Johar                                            *
 *                                                                   *
 *  Version:    1.0                                                  *
 *                                                                   *
 *  Date:       15 August 1986                                       *
 *                                                                   *
 *  Modification History:                                            *
 *       Date           Author              Change                   *
 *                                                                   *
 *      05/04/87        JF                  Added IUCV internal info.*
 *                                                                   *
 *      08/28/89        K.J.Walsh           Added OSMEMFIL.          *
 *                                                                   *
 *                                                                   *
 *********************************************************************/

/*   These are Assembly routines, we need to define them so the
     linker can resolve their addresses for the INTER routine        */

extern Inter()                 ;       /* C & ASSEMBLY INTERFACE     */
extern OSMEMCPY()              ;       /* Copy data using MVCL       */
extern OSMEMCMP()              ;       /* Compare   using CLCL       */
extern OSMEMFIL()              ;       /* Fill memory with char.     */
extern OSTODCLK()              ;       /* for time/day               */
extern OSCON ()                ;       /* for 3270 interface         */
extern OSREAD()                ;       /* to read a char from keyb.  */
extern OSOPCON()               ;       /* Open device CONSOLE Macro  */
extern OSCLCON()               ;       /* Close device CONSOLE Macro */
extern OSWRITE()               ;       /* Write to device            */
extern OSCLRCON()              ;       /* clear and write device     */
extern OSEWACON()              ;       /* write alternate mode device*/
extern OSQYCON()               ;       /* Query the device           */
extern TEIESCMN()              ;       /* Retuns addr of the TEIES   */
                                       /* common block Nucleus exten */
extern OSWAIT()                ;       /* Assemble wait called by    */
                                       /* os_wait()                  */
extern WAIT4INT()              ;       /* wait for inerrupt called   */
                                       /* from os_wint()             */
extern OSDFREE()               ;       /* Calls DMSFREE CMS macro    */
extern OSDFRET()               ;       /* Calls DMSFRET CMS macro    */

                     /* following functions are used in OS_CONN()    */
#define CONPATH "CONS"
#define CONPATHLEN 4
#define CONFLAGS 0
                                 /* called from os_conn.c   */
   extern CONOPEN();
   extern CONCLO();
   extern CONREAD();
   extern CONWRIT();
   extern CONCLEA();
   extern CONQRY();
   extern CONWAIT();
                                 /* following are called from os_tran.c*/
   extern OS2ASC();
   extern OS2EBC();
                  /*  IUCV interface with SCV level         */

extern SCVON();                         /* Init IUCV                  */
extern SCVOFF();                        /* Shut off IUCV              */
extern SCVOFFF();                       /* FORCE off IUCV             */
extern SCVOPEN();                       /* Establish connection       */
extern SCVCLS();                        /* Remove connection          */
extern SCVSEND();                       /* Send data                  */
extern SCVRECV();                       /* Receive data               */
extern SCVINTR();                       /* IUCV interrupt handler     */

/*
 *
 * C library functions needed by the OSK:
 *
 *
 */


extern byte     *malloc()      ;
extern tint      free()        ;
extern byte     *cuserid()     ;
extern sint      system()      ;
extern sint      strlen()      ;
extern sint      open()        ;
extern sint      close()       ;
extern sint      lseek()       ;
extern sint      read()        ;
extern sint      write()       ;
extern void      exit()        ;
extern sint      fprintf()     ;
extern byte      *strcpy()     ;
extern byte *fgets()     ;
extern tint fwrite();

/*===============================================================*
 |                                                               |
 |  The following is the Channel Program structure header, for   |
 |  use with OS_CON and CCW programming.                         |
 |                                                               |
 *===============================================================*/

extern struct ccw                      /*                            */
       {                               /*                            */
              byte  ccwcode;           /* ccw command code           */
              byte *dataddr;           /* virtual storage address to */
                                       /*    use                     */
              byte  flags;             /* ccw flags                  */
              byte  ctl;               /* controle byte              */
              hword count;             /* length of data             */
       };                              /*                            */
                                       /*- - - - - - - - - - - - - - */
typedef struct ccw CCW;                /*                            */

/*===============================================================*
 |                                                               |
 |  The following definitions are for use with the VM/SP IUCV    |
 |  facility.                                                    |
 |                                                               |
 *===============================================================*/




extern struct       scv_conq /* IUCV CONNECTIONS Q ELEMENT           */
{                           /*                                      */
    string          vmid;   /* connected to vm id                   */
    sint            pathid; /* path id between the vm's             */
    tint            flags;
    struct scv_msgq *msghead;  /* head of out-standing msg que  */
    struct scv_msgq *msgtail;  /* tail. pointing to the end of que */
    struct scv_conq *next;  /* next element in the connect queue*/
};                          /*                                      */


extern struct    scv_recq   /* IUCV RECEIVE Q ELEMENT               */
{                           /*                                      */
    sint         msgid;     /* message id on the above path         */
    sint         trgcls;    /* target calls                         */
    sint         msglen;    /* length of the message                */
    hword        vmpath;    /* vm path id                           */
    hword        flags;     /* flags                                */
    struct scv_recq *next;  /* next element in the receive queue    */
};                          /*                                      */

struct  scv_msgq            /* all items in this list have not been   */
{                           /* beed recieved by the others            */
                            /* Info here was allocated in SCVSEND so  */
                            /* other system can receive msg before we */
                            /* de-allocate here. Once the message has */
                            /* been rcvd by other user, memory is     */
                            /* freed in SCVINTR. Pointer to this info */
                            /* is sent in IUCV parameter list=IPMSGTAG*/

   sint  msgnum;            /* message number associated with this    */
   sint  dblwds;            /* # of double-words allocated by DMSFREE */
   struct scv_msgq *next;   /* point to the next element in the list  */

                            /* Memory area after next is where actual */
                            /* data buffer is copied                  */
};


#define SCV_CON_ACCEPT  0x00000001     /* Conn accepted by other sys */
#define SCV_CON_DISCONN 0x00000002     /* other system disc-onnected */
#define SCV_PEND_CONN   0x00000004     /* Pending connection         */
#define SCV_PEND_MSG    0x00000008     /* pending message            */
#define SCV_PEND_DISC   0x00000010     /* Pending sever              */
#define SCV_CON_CALLER  0x00000020     /* System extablished the conn*/
#define SCV_CON_SEEN    0x00000100     /* Above level already knows  */
                                       /* about this connection      */
#define SCV_DSC_SEEN    0x00000200     /* Application knows connection*/
                                       /* has been dis-connected      */
#define SCV_MSG_SEEN    0x80           /* Above level know about this */

                                       /* Following two should be    */
                                       /* same in SCVMCRO MACRO      */
#define SCVDATA_LEN 40                 /* Data buffer for IUCV MACRO */
#define IDLEN 8                        /* Size of a VM User ID       */

#define SCV_CON_TIME_OUT 5             /* # of secs after to give up */
                                       /* on a connection            */
extern struct   scv_xstr
{
   struct scv_conq *conhead;   /* connection link list poiters         */
   struct scv_conq *contail;
   struct scv_recq *rqhead;    /* message list waiting to be received  */
   struct scv_recq *rqtail;
   string sysid;               /* this system name                     */
   byte   *scvdata;            /* iucv data buffer SCVDATA_LEN long    */
   byte   *intrdata;           /* iucv data buf, used in SCVINTR only  */
}
   *SCVExtrn;                  /* external needed for scv level        */

/*********************************************************************/
/*                                                                   */
/*   Server Communications Vehicle {TEIES}                           */
/*   I U C V   M a n a g e m e n t   F u n c t i o n s               */
/*   ONLY to be called from scv_qry{}                                */
/*                                                                   */
/*********************************************************************/


/*
$maker-proto
$extract */

extern sint scv_qml ( string *vmid, sint *len, struct scv_recq **rqprev);
                /* query length of msg in receive queue */
                /*
                 *     string  *vmid;           message is from this vmid
                 *     sint    *len;            length of waiting message
                 *  return codes:
                 *                     0 : if no message waiting.
                 */
extern sint scv_qpth( string vmid, struct scv_conq **conprev);
                /* query to get path id number          */
                /*
                 *       string   vmid;  vmid to whome the path is made
                 *       scv_conq *ptr;   ptr to the entry in que table
                 *  return codes:
                 *                 pathid:  if succeedes
                 *                     -1:  if no path exists.
                 */
extern sint scv_qid( sint pid, string *vmid, struct scv_conq **conprev);
                /*    return vmid for given pid

                      sint    pid;
                      string *vmid;       id matching with pid
                  return code:
                      0 if successful in locating vmid,
                      else an error code

                */
extern sint scv_qcnw(string *vmid, sint *pid, struct scv_conq **conprev);
                /*
                     return any new connection accepted

                     string *vmid;        id connected to
                     sint   *pid;         pathid of the system

                     return code : 0 if new conn found else error code

                */
extern sint scv_qdnw(string *vmid, sint *pid, struct scv_conq **conprev);
                /*
                     return any new dis-connection accepted

                     string *vmid;        id connected to
                     sint   *pid;         pathid of the system

                     return code : 0 if new conn found else error code

                */
