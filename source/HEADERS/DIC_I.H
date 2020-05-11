
#define DICCMDDIS 1   /* display structures */
#define DICCMDTST 2   /* perform tests on data structures */

#define bit(x,y) (((x) & (y)) || ((x) & (y)))

/* $ci2e-struct */
struct dic_stl {
   tint keyblk;    /* key block */
   tint tmblk;     /* tm block number */
   tint nbblk;     /* nb block number */
   tint lublk;     /* lu block number */
   tint numnb;     /* number nb elements */
   tint numlu;     /* number lu elements */
   struct dic_stl *next;
   };


extern struct dic_cb {
   tint stlnbsum;   /* number of blocks on nb chain */
   tint stllusum;   /* number of blocks on lu chain */
   tint stlnumnb;   /* number of nb blocks */
   tint stlnumlu;   /* number of lu blocks */
   tint stlkysum;   /* number of key blocks in stl chain */
   tint stlousum;   /* number of open unit pairs (nb lu tm pairs ) */
   struct dic_stl *stlchain;  /* in memory stl chain setup */
   } diccb;


/* block count structure used by vir_vlc and virvlca */
struct vlc_str {
   tint blknum;
   hword count;
   };

/* used by dicgc routines */
/* note that first element in the array is special, blknum contains
the number of elements currently in use, count is zero */

struct dic_gc {
   tint blknum;
   hword count;
   };


/* used by diclc routines */

struct dic_lc {
   tint goid;
   hword count;
   };

struct dic_blc {  /* big link count, used by big lc routines */
   tint goid;
   hword count;
   };

/* note: if goid is zero, then the count is really the block type
   that is in the id field of the block (first hword).  Block number
   is determined either by the position in the array, or the
   computed database path for the huge table */






extern struct dic_sys {
   hword scratch_lfd; /* the lfd to store the large lc table into */
   struct dic_lc *lc_table; /* starting address of in memory gc table */
   tint lc_max; /* number entries in the lc table */
   hword lfd_being_tracked; /* self explanitory I hope */
   boolean btm_active;
   struct btm_str *free_array;
   struct btm_str *total_goid_array;
   struct btm_str *current_goid_array;
   struct btm_str *stl_array;
   struct btm_str *hdr_array; /* goid "root" blocks from the dms */
   struct btm_str *goids_closed; /* set with stl_ca */
   struct btm_str *blocks_accounted_for;
   struct btm_str *goids_with_wrong_link_counts;
   struct btm_str *goid_collision;
   struct btm_str *blocks_unable2read;
   struct btm_str *loose_free_blocks;
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



/* $ci2e-internal */
hword              dicvlca( hword lfd , tint goid , tint blknum
                   , hword segnum , struct vlc_str *vlcstr
                   , tint * table_size , tint trace_blk
                   , hword options, tint depth);
                   /*         Recursive part of dic_vlc */


/* $ci2e-external */
/* $ci2e-internal */


/* $endextract */

hword              dic_ca(hword lfd, hword options);
                   /* close all with following options */
#define DIC_CA_OPEN 1 /* close all open items fast */
#define DIC_CA_CHECK 2 /* close and fix link counts on open items */
#define DIC_CA_VERIFY 3 /* close all opens, fix *ALL* counts */


hword              dic_flc(hword lfd, tint goid); /* Fix Link Counts */

hword              dic_fld(hword lfd); /* fix lfd, brute force */


hword              dic_gbt( hword lfd , tint blknum);
                   /*         "Get Block Type", returns identifier
                              that is placed at the start of every blk *

hword              diccsb( hword lfd , hword diccmd);
                   /*         "Count Stl Blocks", counts STL blocks
                              found in memory and compares with the
                              values stored in diccb.  Return values
                              are bit ored into result. */
#define DICCSBN 1 /* nb block count mismatch */
#define DICCSBL 2 /* lu block count mismatch */
#define DICCSBK 4 /* key block count mismatch */


hword              diccst( hword lfd);
                   /*         "Construct STL Tables" for stl checks */

hword              dicfree( hword lfd , hword flags);
                   /*         display/test free block chain */

/* goid count routines: these routines compute the link counts for
an individual goid.  Note that the goid must have all views closed
for the values to be computed properly */

struct dic_gc     *dicgcbeg (hword lfd,tint goid);
                  /* allocate some space, and build what the link
                  counts should be, returns address of table start */


tint               dicgccc (hword lfd, struct dic_gc *table);
                   /* checks all blocks in table to compare their
                   actual link counts.  If all match, will return
                   zero.  Otherwise returns number of blocks that
                   do not match */


tint               dicgccl (hword lfd, tint goid
                   , struct dic_gc *table2compare ,hword commands);

#define DICGCCL_COMPARE 1 /* compare with lc block counts */
#define DICGCCL_REPLACE 2 /* replace lc counts with gc counts */


hword              dicgcend (struct dic_gc *table);
                  /* deallocate the table built in dicgcbeg */

hword              dicgcbr(hword lfd, tint goid
                          , tint blknum, hword segnum
                          , struct dic_gc *table, tint table_max
                          , tint *table_size, tint trace_blk
                          , hword options);
                          /* recursively build link count table */

hword              dicgcqry(struct dic_gc *table,tint blknum);
                   /* given block number, return count or zero if none*/


/* lfd count routines */
hword              diclcab(hword lfd,tint goid,tint blknum
                   ,hword new_count,hword *old_type);
                   /* add a block to the big lc scan list */
#define DICLCAB_OK 0 /* no problems occurred */
#define DICLCAB_ERROR 1 /* some kind of internal error */
#define DICLCAB_LC_MISS 2 /* marked under another goid */

hword              diclcbeg(hword lfd2track, tint number_blocks);
                           /* initialize link count tables */

hword              diclcend(void); /* deallocate the junk */

hword              diclcdis(tint first_block, tint last_block
                   ,hword block_type);
                   /* display contents of following blocks */
                   /* block type zero means all blocks, otherwise
                      print only matches for type given */

hword              diclcmat(tint blknum, tint goid, hword link_count);
                   /* see if lc values match computed ones */

#define DICLCMAT_OK      0 /* they all match */
#define DICLCMAT_UNKNOWN 1 /* lc has no data on this block */
#define DICLCMAT_COUNT   2 /* count does not match */
#define DICLCMAT_GOID    3 /* goid does not match */
#define DICLCMAT_TYPE    4 /* block type is off */
#define DICLCMAT_ERROR   5 /* error occurred somehow */



struct dic_lc     *diclcqry(tint blknum);
                          /* return pointer to structure of the  */
                          /* block requested, if nil, then its not
                             there for some reason */


hword              diclcset(tint goid, tint blknum, hword link_count);
                          /* set link count of block requested */

hword              diclcrub(void);
                   /* resolve unknown blocks */
                   /* warning: do not call this routine if any
                      dms entries are open, it will trash the
                      new views. */


hword              dicroot( hword lfd , hword flags); /*  lfd,diccmd);
                              display/test the root block for validty */

hword              dicstart(void); /* no arguements */
                           /* Database Integrity Check startup routine *




hword              dictest ( void);

/* $ci2e-external */
