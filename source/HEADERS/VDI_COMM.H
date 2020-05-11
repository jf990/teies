/*********************************************************************

:NAME.        VDI_COMM.H
:LEVEL.       VDI
:DATE.        June 11, 1987
:AUTHOR.      Kenneth Liang


:PURPOSE.     Contains definitions used by both external and internal
              VDI functions. It is included by both vdi.h and vdi_i.h.
              vdi_comm.h checks "VDI_COMM" to make sure it is invoked
              only once.



:MODS.

:END.

*********************************************************************/




#ifndef       VDI_COMM
#define       VDI_COMM       TRUE







/*********************************************************************

    VDI function codes used by VDI dispatcher.

**********************************************************************/



#define       INIT_DEVICE    0
#define       CLEAR_DEVICE   1
#define       CLEAR_AREA     2
#define       FLUSH_DEVICE   3
#define       WRITE          4
#define       SET_TCOLOT     5
#define       BEG_RVIDEO     6
#define       END_RVIDEO     8
#define       BEG_HIGHL      9
#define       END_HIGHL     10
#define       REQUEST_LOC   11
#define       SAMPLE_LOC    12
#define       REQUEST_CHR   13
#define       SAMPLE_CHR    14
#define       REQUEST_STR   15
#define       REQUEST_MSTR  16
#define       SAMPLE_STR    17

#define       BEG_FIELD     18
#define       END_FIELD     19
#define       BEG_BLINK     20
#define       END_BLINK     21
#define       BELL          22
#define       SET_COLOR     51
#define       REMO_DEVICE   99


#define       V_DEV_LEN      8
#define       V_NAME_LEN    15






extern   struct    VDIattrib
    {
         sint theight;            /* Character height           */
         byte talign;             /* Character alignment        */
         sint trot;               /* Character rotation         */

         sint tcolor;             /* Text color index           */
         sint numtcolor;          /* Number of colors available */
         byte *tcolors;           /* List of text colors        */

         byte tintense;           /* Character highlighting     */
         byte font;               /* Character font             */
         sint bcolor;             /* Background color           */
         sint fcolor;             /* Foreground color           */
         byte fint;               /* Fill interior              */
         byte fstyle;             /* Fill style                 */
        hword highlight;          /* Highlighting               */
         byte edit[5];          /* Line edit characters       */
         byte lstyle;             /* Line style                 */
         byte plinet;             /* Polyline Type              */
         byte pmarkt;             /* Polymarker type            */
         sint baud;               /* Baud rate                  */
         byte parity;             /*                            */
         byte wlen;
         byte sbits;
         byte protocol;
         byte encrypt;
    };









extern   struct    dev_entry

                   {
                                         /*****************************/
                      byte   name[19];   /* "highlevel" name of device*/
                                         /*  Allocates 15 chars for   */
                                         /*  the human readable name &*/
                                         /*  4 bytes for TEIES string */
                                         /*  header.                  */
                                         /*                           */
                      sint   device;     /* VDI device number         */
                      byte  *scrbuf;     /* Screen buffer used as a   */
                                         /*   cache memory            */
                      hword *scrhigh;    /* Screen highlighting attrib*/
                      tint  *scrclr;     /* Screen color attribute    */
                      sint   length;     /* Length of screen buffer   */
                      tint   flags;      /* 32 bit flags              */
                      byte   type;       /* device type (see Specs)   */
                      byte   class;      /* device class (see Specs)  */
                      hword  height;     /* display surface rows      */
                      hword  width;      /* display surface columns   */
                      byte   os_name[12];/* Device name as OS knows   */
                                         /*  Store 4 bytes for TEIES  */
                                         /*  string simulation and 8  */
                                         /*  chars for OS device name */
                                         /*                           */
                      tint   driver;     /* pointer to code entry     */
         struct  VDIattrib  *attrib;     /*        attribute list     */
                      byte  *font;       /* ptr to current char table */
                      sint   status;     /* activity status           */
                      byte  *rxbuf;      /* ptr to the Input Buffer   */
                      sint   rxlen;      /* Length of  Input Buffer   */
                      byte  *devdep;     /* Pointer to device         */
                                         /*   dependent information   */
                      real   x;          /* Current X location        */
                      real   y;          /* Current Y location        */
         struct  dev_entry  *next;       /* pointer to next VDI device*/
                                         /*****************************/

                   };

typedef  struct  dev_entry    VDIdevice;

extern   VDIdevice           *_dev_tab; /* table head pointer          */

extern   sint                 nxt_dev;  /* next available VDI device   */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

      Exception Handler Error Codes

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define UNKNOWN_DEVICE                14001
#define DEVICE_NOT_IN_TABLE           14002
#define UNKNOWN_FUNCTION              14003
#define IO_ERROR                      14004

#define NO_DEVICE_ENTRY               14011
#define KEYBD_RD_ERROR                14012
#define BUFFER_EXCEEDED               14013


#endif

