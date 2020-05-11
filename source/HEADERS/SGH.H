#ifndef SGH_I_H_INCLUDED  /* $ci2e-redefine */
#define SGH_I_H_INCLUDED  /* $ci2e-redefine */
/* Hierarchical segment structure routines */

/* structure of a hiearch element */
/* $ci2e-struct */
struct sgh_str {
   tint blknum;
   hword segnum;
   };

struct sgh_rcb {
   /* passed parameters */
   struct dms_cb *dmscb;          /* dms control block          */
   hword pos;                     /* pos of node to be replaced */
   tint rpl_blk;                  /* the new block number       */
   hword rpl_seg;                 /* the new segment number     */

   /* temp storage */
   /* "TBR": To Be Replaced. to prevent conflict with "old_seg" */
   tint tbr_blk;                  /* old block # in seg         */
   hword tbr_seg;                 /* old seg #,                 */

   /* old segment variables */
   struct virbh *old_hdr;         /* blk hdr with old seg       */
   byte *old_adr;                 /* addr from vir_gba          */
   struct sgbstr *old_seg;        /* found with sgb_fs          */
   struct sgh_str *old_node;      /* mem addr of actual start   */
   hword onode_size;              /* number of elements in node */

   /* new segment variables */
   struct virbh *new_hdr;
   byte *new_adr;
   struct sgbstr *new_seg;
   struct sgh_str *new_node;
   hword nnode_size;

   /* boolean flags */
   hword old_exist;          /* does old seg exist              */
   hword block_new;          /* is old seg on a new block       */
   hword new_fit;            /* does new seg fit into old seg ? */
   hword is_new;             /* do we create a new segment?     */
   };

/* flag notes:
1: if is_new true and block_new true then the old segment



   $ci2e-end-struct */


/* $extract $maker-proto */
hword             sgh_beg( void);

hword             sgh_qns(lfd, blknum, segnum);
                  /* return number of segments in element */

hword             sgh_rpl(struct dms_cb *dmscb, hword pos
                  , tint nblk, tint nseg);
                          /*  copy seg and replace element */

hword             sghde(hword lfd, tint blknum, hword segnum);
                             /* "Display Element" */


hword             sghfnd(struct dms_cb *dmscb, hword pos);
                         /*   get next lower segment */

tint              sghgb(hword lfd, tint blknum
                  , hword segnum, hword pos);
                        /*    get the block number */

hword             sghgpl(byte *addr);
                         /*   get path length */

hword             sghgs(hword lfd, tint blknum
                  , hword segnum, hword pos);
                        /*    get the segment number */

hword             sghmd(struct dms_cb *dmscb, hword index);
                        /*    "Move Down" the database path */

hword             sghmf(struct dms_cb *dmscb);
                        /*    "Move Forward" along database path */

#define SGH_Replace_Create_New_Segment sghrcns
hword             sghrcns(struct sgh_rcb *sghrcb);

#define SGH_Replace_Insert_New_Stuff sghrins
hword             sghrins(struct sgh_rcb *sghrcb);

#define SGH_Replace_Set_Flags sghrsf
hword             sghrsf(struct sgh_rcb *sghrcb);

#define SGH_Replace_Update_Link_Count sghrulc
hword             sghrulc(struct sgh_rcb *sghrcb);

#define SGH_remoVe_element sghve
hword             sghve(struct dms_cb *dmscb
                  ,struct dms_dla *dla, hword block_age);

#define SGH_remoVe_Level sghrl
hword             sghvl(struct dms_cb *dmscb);

hword             cfill( unsigned char * addr , long int len , unsigned
char insert);

#endif /* for first #ifndef */
