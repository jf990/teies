/* This file is program generated from file "VOL_I.H" */
/* using REXX routine CI2E */
/* actual def in volstart( ) */
#define VOLSLEN 128 /* size of cms filename buffer */

/* $ci2e-struct */
struct vol_str {hword dummy; /* dummy definition to keep C happy */
};
/* flag values for flag in vol_str */
#define VOL_STR_READ_ONLY 1     /* if set, fd is read only */
#define VOL_STR_OPEN 2          /* lfd is currently open */
#define VOL_STR_JOURNAL 4       /* journal this lfd */

extern struct vol_sys {hword dummy; /* dummy definition to keep C happy */
} volsys;


/* routines $extract $ci2e-end-struct $maker-proto */

hword vol_beg(hword number_lfds, hword flags);
                          /* initialization and open routine.
                          If lfd is zero, then initialize data
                          structures, 'flags' then becomes maxinum
                          number of lfd's to be used */

hword vol_cbs( void);     /* check block size, debug routine */


hword vol_cls( hword lfd); /* the lfd to close access to */
#define VOL_CLS_OK 0
#define VOL_CLS_NOT_OPEN 1
#define VOL_CLS_INVALID  2


hword vol_cfd( void);     /* close remaining open lfd */

hword vol_end( void);     /* shut down vol level */

tint vol_gbs( void);      /* return block size */

int  vol_gfd( sint lfd);  /*  ret real fd of lfd */

tint vol_gnb( hword lfd); /* get number of blocks in lfd */

boolean vol_jnl( hword lfd, hword command); /* journal interface */
#define VOL_JNL_QUERY 1 /* is journaling on? */
#define VOL_JNL_ON    2 /* turn journaling on */
#define VOL_JNL_OFF   3 /* turn journaling off */


hword vol_opn( hword lfd, hword option); /* open an lfd */
#define VOL_OPN_READ_ONLY 1  /* open read only */
#define VOL_OPN_JOURNAL   2  /* turn journaling flag on */

#define VOL_OPN_OK 0      /* is ok */
#define VOL_OPN_ALREADY 1 /* is already open */
#define VOL_OPN_FAIL    2 /* access or open failed */

byte *vol_qla( hword lfd); /* "Query Lfd Assignment";
                           returns local file name address */

hword vol_qls( hword lfd); /* "Query Lfd Status" */
/* returns values listed in vol_str */


boolean vol_qlw( hword lfd); /* "Query Lfd Writeable"
                             True if lfd is open and writable */

hword volmsl( void);      /* return max seg length */

hword volqlal( void);     /* "Query Lfd Assignment Length";
                          return size of QLA buffer */

