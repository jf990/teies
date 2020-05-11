/* $ci2e-struct */
struct sgr_str {
   tint nxt_blk;   /* next element in chain */
   hword nxt_seg;
   tint dat_blk;   /* ptr to data element at current position */
   hword dat_seg;
   };

struct sgr_cb {
   struct dms_cb *dmscb;
   struct stl_cb *stlcb;

   tint top_blk;        /* passed from external subsystem */
   hword top_seg;

   tint cur_blk;        /* set internally */
   hword cur_seg;

   tint dat_blk;        /* passed from external subsystem */
   hword dat_seg;

   hword user_pos, pos;
   hword lfd;

   tint new_blk;        /* set by sgrcns( ) */
   hword new_seg;

   tint nxt_blk;        /* answer from previous internal routine */
   hword nxt_seg;

   };

/* useage of 'pos' in sgr_cb if value is zero:
   caller  meaning
   sgr_fnd get priv
   sgr_rpl set priv
   sgr_ins insert at beginning
   sgr_del not valid
*/

/* $ci2e-end-struct */
/* $extract $maker-proto */
hword             sgr_beg ( void); /* */


hword             sgr_fnd  ( struct dms_cb * dmscb , hword pos); /*
 *dmscb, pos ); */

tint              sgr_gdb  ( hword lfd , tint blknum , hword segnum); /*
 lfd,blknum,segnum); */

hword             sgr_gds  ( hword lfd , tint blknum , hword segnum); /*
 lfd,blknum,segnum); */

tint              sgr_gnb  ( hword lfd , tint blknum , hword segnum); /*
 lfd,blknum,segnum); */

hword             sgr_gns  ( hword lfd , tint blknum , hword segnum); /*
 lfd,blknum,segnum); */


hword             sgr_rpl  ( struct dms_cb * dmscb , hword pos , tint
rpl_blk , hword rpl_seg); /*  *dmscb, pos, rpl_blk, rpl_seg); */

hword             sgr_rpm  ( struct dms_cb * dmscb);  /*  *dmscb ); */


hword             sgrdcb   ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                "Display Control Block" */

hword             sgrdstr  ( struct sgr_str * sgrstr);  /*  *sgrstr); */


hword             sgrfn    ( struct sgr_cb * sgrcb);  /*  *sgrcb); "Find
Node"
                                inc 'pos' elements down structure */

hword             sgrgpl   ( void);  /*   ); "Get Path Length */

hword             sgrmfsw  ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                "Make First Segment Writable"
                                if the first segment is an old
                                view, then copy it and return
                                indicator to new view in "top"
                                and "current" */

hword             sgrmnsw  ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                "Make Next Segment Writable"
                                If the next segment is an old
                                view, then make a new view of it.
                                New seg is returned in "next" */

hword             sgrqd    ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                 "Query Data"
                                 Get current node, return data
                                 pointer in "data" entry */

hword             sgrqpm   ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                 "Query Priv Mask" */


hword             sgrrfd   ( struct sgr_cb * sgrcb , tint rpl_blk
                  , hword rpl_seg);  /* "Replace First Data"       */

hword             sgrrfn   ( struct sgr_cb * sgrcb);  /*  *sgrcb);
                                 "Replace First Node"       */

hword             sgrrd    ( struct sgr_cb * sgrcb , tint rpl_blk ,
hword rpl_seg);  /*  *sgrcb, rpl_blk, rpl_seg */


hword             sgrrn    ( struct sgr_cb * sgrcb);  /*  *sgrcb);
Replace Node */

hword             sgrsd    ( struct sgr_cb * sgrcb , tint rpl_blk ,
hword rpl_seg);  /*  *sgrcb,rpl_blk,rpl_seg);
                                 "Set Data"
                                 If the current node is a new view,
                                 then set the data pointer to the
                                 new values */




hword             ccopy( byte * a , byte * b , hword len);







