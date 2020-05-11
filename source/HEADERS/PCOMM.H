/*
$extract */
   extern sint os_conn();
   extern hword pc_crc();
   extern sint pc_recv();
   extern sint _pc_recv();
   extern sint pc_prrcv();
   extern sint pc_decode();
   extern sint pc_send();
   extern sint _pc_send();
   extern sint pc_prsnd();
   extern sint pc_encode();
   extern sint pc_on();
   extern sint pc_off();
   extern sint pc_open();
   extern sint pc_close();

/*
extern sint pc_main();     $main
$endextract                      */

   extern sint CONINTR();

struct pktbuf {
   byte start;
   byte type;
   byte data;     /* here starts the real len & data  */
};

extern struct construc
{
   sint pktlen;
   byte pktdata;
}  *condata;

#define PC_ERR 31000
#define PC_ERR_CRC 66
#define PC_ERR_PROTOCOL 100
#define PC_ERR_FUNC_NOT_DEF 99
#define PC_ERR_OS 44
#define PC_ERR_UPL_REQ 21
#define PC_ERR_TOUT 33
#define PC_ERR_ABORT 22
#define PC_ERR_MAX_RETRY 77
#define PC_ERR_INV_PKT_RCVD 55
#define PC_ERR_ACK_RCVD (PC_ERR_PROTOCOL + PC_ACK)
#define PC_ERR_NAK_RCVD (PC_ERR_PROTOCOL + PC_NAK)
#define PC_ERR_CAN_RCVD (PC_ERR_PROTOCOL + PC_CAN)
#define PC_ERR_EOT_RCVD (PC_ERR_PROTOCOL + PC_EOT)
#define PC_ERR_SOT_RCVD (PC_ERR_PROTOCOL + PC_SOT)
#define PC_ERR_FIL_RCVD (PC_ERR_PROTOCOL + PC_FIL)
#define PC_ERR_DWN_RCVD (PC_ERR_PROTOCOL + PC_DWN)
#define PC_ERR_UPL_RCVD (PC_ERR_PROTOCOL + PC_UPL)
#define PC_ERR_DATA_RCVD (PC_ERR_PROTOCOL + PC_DATA)

#define PC_NULL 0x00         /* States which don't send or recv data   */
#define PC_SOH 0x01          /* Start of header marker                 */
#define PC_STX 0x02          /* Start of TeXt                          */
#define PC_ETX 0x03          /* End of TeXt                            */
#define PC_CR  0x0D          /* Carriage return                        */
#define PC_ACK 0x41          /* Ascii 'A' Acknowledge pkt rcvd okay    */
#define PC_NAK 0x4E          /* Ascii 'N' Acknowledge pkt not rcvd okay*/
#define PC_CAN 0x43          /* Ascii 'C' Abort communication          */
#define PC_EOT 0x45          /* Ascii 'E' End of data pkts             */
#define PC_SOT 0x53          /* Ascii 'S' Start of data pkts           */
#define PC_FIL 0x46          /* Ascii 'F' Filename data pkt            */
#define PC_DWN 0x78          /* Ascii 'W' Downloading data pkts        */
#define PC_UPL 0x55          /* Ascii 'U' Uploading data pkts          */
#define PC_DATA 0x44         /* Ascii 'D' Uploading data pkts          */

                     /* following are actually ascii char values  */
                     /* used to prefix the buffer                 */
#define PC_DEL_CHR   0x7F    /* delete char             */
#define PC_CTL_PFX   0x23    /* pound sign(#)           */
#define PC_HIBIT_PFX 0x26    /* ampersand(&)            */
#define PC_RPT_PFX   0x7E    /* tilde(~)                */

#define PC_x80   0x80        /* hi-bit on                              */
#define PC_x7F   0x7F        /* hi-bit off                             */
#define PC_MOD64 0x40        /* used to complement 7th bit             */

#define PC_CTL_CHRS 0x1F     /* ascii ctl chrs 0..31                   */

#define AID_CR       0xE8    /* 1st byte of inbound 3270DS if ENTER    */
                             /* was pressed. See GA23-0059-2  pg. 3-9  */

#define PC_MAX_RETRY 5        /* Maximum of retry before giving up     */
#define PC_PKTLEN 60          /* pkt len where we cutouff data         */
#define PC_MAX_PKTLEN 4095    /* Maximum packet length allowed         */
#define PC_MIN_PKTLEN 3       /* Min pkt len allowed. SOH+char+CR      */
#define PC_MAX_CODE_CHRS 3    /* Max one chr can expand with '&','#'   */
#define PC_PKTINFOLEN (1+3*2+1+3+1+3*2) /* 18 extra possible chrs      */
                              /* SOH+len(2)+STX+pktnum...+ETX+crc(2)   */

#define CRC_CALC 1            /* Calculate crc function                */
#define CRC_SETUP 2           /* set up crc_tab                        */
#define CRC_TAB_SIZE 256
                              /* Maximum # of times following errors   */
#define PC_MAX_READ_TRY 5     /* are allowed.                          */
#define PC_ERR_MAX_OS 5
#define PC_ERR_MAX_TOUT 5
#define PC_ERR_MAX_PROTO 5
#define PC_ERR_MAX_NAK 5

#define PC_OS_ERR_EXCEEDED (PC_ERR_PROTOCOL + 101)
#define PC_NAK_ERR_EXCEEDED (PC_ERR_PROTOCOL + 102)
#define PC_PROTO_ERR_EXCEEDED (PC_ERR_PROTOCOL + 103)
#define PC_TOUT_ERR_EXCEEDED (PC_ERR_PROTOCOL + 104)

#define PC_ST_NONE 0
#define PC_ST_SEND_UPL 1
#define PC_ST_RECV_UPL_ACK 2
#define PC_ST_PREP_FIL     3
#define PC_ST_SEND_FIL     4
#define PC_ST_RECV_FIL_ACK 5
#define PC_ST_SEND_SOT     6
#define PC_ST_RECV_SOT_ACK 7
#define PC_ST_PKTIZE_DATA  8
#define PC_ST_SEND_DATA    9
#define PC_ST_RECV_DATA_ACK 10
#define PC_ST_NXT_DATA_ACK 11
#define PC_ST_SEND_EOT     12
#define PC_ST_RECV_EOT_ACK 13
                  /* nxt state is exit */
                           /* receive states */
#define PC_ST_RECV_UPL     1
#define PC_ST_SEND_UPL_ACK 2
#define PC_ST_RECV_FIL     3
#define PC_ST_COPY_FIL     4
#define PC_ST_SEND_FIL_ACK 5
#define PC_ST_RECV_SOT     6
#define PC_ST_SEND_SOT_ACK 7
#define PC_ST_RECV_EOT     8
#define PC_ST_SEND_EOT_ACK 9
#define PC_ST_COPY_DATA    10
               /* nxt state is exit */
#define PC_ST_CHK_PKT 12
#define PC_ST_SEND_DATA_ACK 13
               /* nxt state is RECV_EOT */
                           /* exception states */
#define PC_ST_ANY 0
#define PC_ST_FILLER 0
#define PC_ST_SEND_CAN 16
               /* nxt state is exit */
#define PC_ST_SEND_NAK 18
               /* nxt state depends on who sent NAK */
#define PC_ST_RECV_REQ 90
#define PC_ST_REQ_NAKED 91
#define PC_ST_TOUT_ERR 95
#define PC_ST_OS_ERR  96
#define PC_ST_PROTO_ERR 97
#define PC_ST_PROTO_ERR_SEND_NAK 98
#define PC_ST_TRANSFER_ABORT 99
#define PC_ST_EXIT 100
