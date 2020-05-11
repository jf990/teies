/* This file is program generated from file "DIC_I.H" */
/* using REXX routine CI2E */

#define DICCMDDIS 1   /* display structures */
#define DICCMDTST 2   /* perform tests on data structures */

#define bit(x,y) (((x) & (y)) || ((x) & (y)))

/* $ci2e-struct */
struct dic_stl {hword dummy; /* dummy definition to keep C happy */
};


extern struct dic_cb {hword dummy; /* dummy definition to keep C happy */
} diccb;


/* block count structure used by vir_vlc and virvlca */
struct vlc_str {hword dummy; /* dummy definition to keep C happy */
};

/* used by dicgc routines */
/* note that first element in the array is special, blknum contains
the number of elements currently in use, count is zero */

struct dic_gc {hword dummy; /* dummy definition to keep C happy */
};


/* used by diclc routines */

struct dic_lc {hword dummy; /* dummy definition to keep C happy */
};

struct dic_blc {hword dummy; /* dummy definition to keep C happy */
};

/* note: if goid is zero, then the count is really the block type
   that is in the id field of the block (first hword).  Block number
   is determined either by the position in the array, or the
   computed database path for the huge table */






extern struct dic_sys {hword dummy; /* dummy definition to keep C happy */
} dicsys;

/* subsystem naming conventions:
   _ : external
   f : free chain manipulation




*/

/* $ci2e-end-struct */
/* $extract $maker-proto */

hword              dic_beg(void); /* dic initialization */

sint               dic_cnb( hword lfd , hword blk_inq , tint option);
                   /*         "Count Number of Blocks" given type */
#define DIC_CNB_FREE VIRBTFRE /* blocks on free chain */
#define DIC_CNB_KEY  VIRBTTLK /* blocks that are part of stl key */
#define DIC_CNB_DATA VIRBTSGB /* data blocks of various types */
#define DIC_CNB_NB   VIRBTTLN /* number of nb blocks */
#define DIC_CNB_LU   VIRBTTLL /* number of lu blocks */
#define DIC_CNB_TM   VIRBTTLT /* number of tm blocks */


hword              dic_end(void);


hword              dic_free( hword lfd, hword option);
#define DIC_FREE_BTM 1 /* set values into 'free_array' */
#define DIC_FREE_RO  2 /* read only, don't clear if corrupt */

#define DIC_FREE_OK 0 /* all tests passed */
#define DIC_FREE_LOOP 1 /* possible loop detected in free chain */
#define DIC_FREE_BAD_BLOCK 2 /* a block that is not free found on free */
#define DIC_FREE_CHAIN_COUNT 3 /* mismatch between number of blocks on
                                  free chain and value stored */
#define DIC_FREE_SCAN_COUNT 4 /* mismatch between blocks on free chain
                                 and blocks found free in scan */


hword              dic_itbl( hword lfd); /* initialize btm tables
                   for current goid */




tint               dic_valc(hword lfd);
                   /* "Verify All Link Counts"
                   rc zero if ok, else number goids with mismatch */


hword              dic_vlc(hword lfd, tint goid, tint max_blks
                   , hword flags);
                   /* verify and possibly fix link counts */
#define DIC_VLC_FIX 1 /* fix the link counts */
#define DIC_VLC_LOG 2 /* log blocks into dicsys.current_goid_array */

/* return codes follow */
#define DIC_VLC_OK 0 /* everything went ok */
#define DIC_VLC_LOCK 1 /* goid is currently open, can't process */
#define DIC_VLC_ERROR 2 /* an error occurred in processing */
#define DIC_VLC_NOMATCH 3 /* a link count does not match calc value */
#define DIC_VLC_BUFFER 4 /* buffer overflow, internal error */
#define DIC_VLC_ALLOC  5 /* goid not allocated */
#define DIC_VLC_GOID   6 /* goid collision */
#define DIC_VLC_BT     7 /* ptr to wrong blk/seg detect/clear */


hword              dic_verify( hword lfd, hword flags);
                   /* verify the contents of an lfd */
#define DIC_VERIFY_FREE 1 /* verify the free list */
#define DIC_VERIFY_GOID 2 /* verify all goid's */
#define DIC_VERIFY_HDR  4 /* dms hdr blocks */
#define DIC_VERIFY_RFREE 8 /* rebuild free list */
#define DIC_VERIFY_APPEND_FREE 16 /* append "loose" free blocks */
#define DIC_VERIFY_FIX_LC  32 /* fix link counts */
#define DIC_VERIFY_COMMIT  64 /* commit updates */
#define DIC_VERIFY_ALL  0xff /* do everything */



