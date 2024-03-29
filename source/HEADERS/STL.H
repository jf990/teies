/* This file is program generated from file "STL_I.H" */
/* using REXX routine CI2E */
#ifndef STL_H_INCLUDED /*$ci2e-redefine */
#define STL_H_INCLUDED /*$ci2e-redefine */
/* this set of routines will handle the transaction log of objects,
object open requests (aka can we open an object), when we
can erase blocks, etc
$ci2e-struct
*/
struct stl_cb {hword dummy; /* dummy definition to keep C happy */
};
                    /*  following are used in open operation */
                    /*  and stored in stl_cb->open_mode      */
#define STL_OR 1    /* open read only    */
#define STL_OW 2    /* open writable     */

#define STL_OPEN_READ STL_OR  /* open read only */
#define STL_OPEN_WRITE STL_OW /* open normal write mode */
#define STL_OPEN_DELETE 4     /* open write able, but in delete mode */
#define STL_OPEN_NEW    8     /* bypass qnb checking */

struct stl_key {hword dummy; /* dummy definition to keep C happy */
};

#define STL_STAT_READ_OPEN    1 /* reads are open */
#define STL_STAT_DELETE_OPEN  2 /* delete is open */
#define STL_STAT_UPDATE_OPEN  4 /* update is open */
#define STL_STAT_ERROR        8 /* error encountered */
#define STL_STAT_UPDATE_IN_PLACE 16 /* updates in place ok */


/* note: update lockout also indicates that an update is open since
         only one open update is allowed at a time */

#define STL_COMMIT_NO 0  /* do not commit update on crash recovery */
#define STL_COMMIT_YES 1 /* do commit update on crash recovery */


struct stl_tm {hword dummy; /* dummy definition to keep C happy */
};

struct stl_lu {hword dummy; /* dummy definition to keep C happy */
};

struct stl_nb {hword dummy; /* dummy definition to keep C happy */
};

struct stl_hdr {hword dummy; /* dummy definition to keep C happy */
};

struct stl_sys {hword dummy; /* dummy definition to keep C happy */
};

extern struct stl_sys stlsys; /* really defined in stl_beg */


/* $ci2e-end-struct */
/* $extract $maker-proto */

hword              stl_beg(void); /* no arg at present */

hword              stl_ca( hword lfd, hword flags);

#define STL_CA_COMMIT 1 /* close them all commit (backout otherwise) */
#define STL_CA_VERIFY 2 /* run verify on all items in key afterwards */
#define STL_CA_DIC    4 /* mark goid bits into dicsys.goids_closed */

#define STL_CA_OK 0   /* everything went through ok */
#define STL_CA_OPEN 1 /* warning, things were found open */


hword              stl_ct( byte * time1 , byte * time2);
                   /* "Compare 2 Date/Times" */
#define STL_CT_FIRST 1 /* first is more recent */
#define STL_CT_SECOND 2 /* second is more recent */
#define STL_CT_EQUAL 3 /* the Date/times are equal */

hword              stl_dt( byte * input_time);  /* <*time);
                              "Display Time" */

hword              stl_end(void); /* shutdown */


lint               stl_qlc(hword lfd, tint blknum);
                   /* get link count on block */
/* return codes */
#define STL_QLC_FREE -1 /* block is on free chain */
#define STL_QLC_SGB_ERROR -2 /* error in sgb, is not sgb block? */
/* otherwise return lc entry in */


sint               stl_vkc(hword lfd);
                   /* validate the key chain, will rtn # key blks */
#define STL_VKC_LFD -1 /* lfd not proper or open */
#define STL_VKC_LOOP -2 /* loop detected */
#define STL_VKC_BREAK -3 /* break detected in chain */


hword              stlab(struct stl_cb *stlcb,tint blknum);
                             /*adds a block to nb list */

hword              stlas(struct stl_cb *stlcb,struct sgbstr *seg_adr);
                             /*add a seg to spill block given seg */

hword              stlass(struct stl_cb *stlcb, hword seg_size);
                            /* add a seg to spill block given size */

hword              stlcls( struct stl_cb * stlcb , hword mode);
                   /* close: */
/* following are used in close operations*/
#define STL_CC 1    /* close commit      */
#define STL_CB 2    /* close backout     */
#define STL_CR 3    /* close readonly    */

#define STL_CLOSE_COMMIT STL_CC
#define STL_CLOSE_BACKOUT STL_CB
#define STL_CLOSE_READONLY STL_CR

hword              stldcb( struct stl_cb * stlcb);
                   /* display all control block info */

hword              stldkb( struct stl_cb * stlcb);
                   /* display key block if d_detail on */

hword              stldks( hword lfd , tint goid , hword who);
                   /* "Display Key Structure" */
#define STLDKSN 1  /* display NB block if bit is set */
#define STLDKSL 2  /* display LU block if bit is set */
#define STLDKST 4  /* display TM block if bit is set */

hword              stldov( hword lfd , struct stl_key * key_adr);
                   /*          "Delete Old Views" */

hword              stlds( hword lfd , hword options);
                   /*          "Display Structures" */
#define STLDISG 1  /* display tint goid entries */
#define STLDISN 2  /* display new block entries */
#define STLDISL 4  /* display lu block entries */
#define STLDIST 8  /* display time block entries */

hword              stldsb( struct stl_cb * stlcb);
                   /* display spill block */

tint               stlgkb( hword lfd , tint goid);
                   /* get key block */

hword              stlgls( struct stl_cb * stlcb);
                   /* get last used segment */

hword              stlgnk( hword lfd , tint * keyblk , tint * goid);
                           /* gets key block, gets next key block, and
                              updates keyblk and goid. */

#define STLGNK_OK 0 /* got the next one */
#define STLGNK_END 1 /* reached end of the list */
#define STLGNK_ERR 2 /* improper args somehow */


tint               stlgpd( hword lfd , tint goid);
                   /* get pending root blk to delete
                               returns zero if none */

tint               stlgsb( struct stl_cb * stlcb);
                                /* get spill block   */

tint               stlikb( hword lfd , tint goid , tint prev
                   , tint next, tint top_blk);
                   /*          init a key block */

tint               stlitm( struct stl_key * key_adr , hword lfd);
                   /*  init TM block  returns TM block */

hword              stliul( struct stl_cb * stlcb , tint blk
                   , hword old , hword new);
                   /*  add element in LU block */

struct stl_cb     *stlopn( hword lfd , tint goid , tint root
                  , hword open_mode);
                  /*  open, check for user bugs */
                  /* see open open mode flags */


hword              stlqnb( struct stl_cb * stlcb , tint blknum);
                           /*  see if block is a new block */
/* return codes for stlqnb */
#define STLQNBE 2    /* error in code             */
#define STLQNBY 1    /* the block is a new block  */
#define STLQNBN 0    /* the block is an old block */

hword                stlrkb( hword lfd , tint goid);
                          /* "Remove Key Block" */
#define STL_REMOVE_KEY_BLOCK_OK 0
#define STL_REMOVE_KEY_BLOCK_NOT_ALLOCATED 1
#define STL_REMOVE_KEY_BLOCK_OPEN          2

hword               stlstat( hword lfd , tint goid);
                    /* see stl_stat flags for return codes */

hword               stlulc( struct stl_cb * stlcb , tint blknum
                    , sint  value);
                    /* how update LU table */



/* $endextract */

/* artist time: the relationship of the blocks and structures:
---------
| STL_CB|
---------
   |
   V
------------------------------
| STL_KEY: one per opened obj|
------------------------------
  |       |        |
  |       |        |
  V       V        V
------   ------   ------
STL_TM   STL_NB   STL_LU
------   ------   ------








*/
#endif /* for first ifndef */
