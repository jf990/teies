/* This file is program generated from file "VIR_I.H" */
/* using REXX routine CI2E */

#ifndef VIR_H_INCLUDED /* $ci2e-redefine */

#define VIR_H_INCLUDED /* $ci2e-redefine */
/* actual definition in virstart( ); */
/* internal virtual level data structures */
/* $ci2e-struct */
struct virbh {hword dummy; /* dummy definition to keep C happy */
};


extern struct vir_sys {hword dummy; /* dummy definition to keep C happy */
} virsys;        /* actually declared in virstart.c */


struct vir_lfd {hword dummy; /* dummy definition to keep C happy */
};



struct vir_fc {hword dummy; /* dummy definition to keep C happy */
};


struct vir_root {hword dummy; /* dummy definition to keep C happy */
};

/* $ci2e-end-struct */
/* BMM stuff, to be phased out */
#define VIRBERR 2   /* coding error occured */
#define VIRBSET 1   /* command to set BMM flag */
#define VIRBRES 2   /* command to reset BMM flag */
#define VIRBQRY 3   /* command to query BMM flag */
#define VIRBYES 1   /* it is a BMM buffer */
#define VIRBNO  0   /* it is NOT a BMM buffer */

/* following used to be in dic_i.h, are being moved here */

/* constants to identify different types of database blocks */
#define VIRBT_UNKNOWN 0 /* unknown */
#define VIRBTFRE 0xffff /* free block */
#define VIRBT_MAY_BE_FREE 0xfffe /* used by dic */
#define VIRBTSGB 1    /* block is a sgb format */
#define VIRBTTLK 2    /* stl key block */
#define VIRBTTLN 3    /* stl nb block */
#define VIRBTTLL 4    /* stl lu block */
#define VIRBTTLT 5    /* stl tm block */
#define VIRBTHDR 6    /* goid header format */


/* routines $extract $maker-proto */
tint          vir_ab (hword lfd, tint respect); /*
                            allocate block, returns # */
hword         virbmm( struct virbh * blkhdr , hword command);
                             /*  used by BMM to set/query status */
hword         virbmd( void);  /* HARDCODED to return "4"
                            return lfd used for BMM */

hword         vir_beg( void ); /* startup routine */

hword         vir_cbv(byte *buffer,tint length); /*
                            "Calc Buffer Value" */

hword         vir_cdb(hword lfd, byte *name, tint numblks, tint blksize);
                            /* create the database of your choice */
hword         vir_chv(hword lfd, tint blknum);
                            /* calc hash value, used in hash tables */

hword         vir_cls(hword lfd, hword options);
              /* close an lfd, no options yet */
#define VIR_CLS_OK 0
#define VIR_CLS_NOT_OPEN 1 /* lfd was not open */


hword         vir_dbh(struct virbh *blkhdr);
                            /* display data in blk hdr */
hword         vir_dlc(void);
                            /* display list chains */
hword         vir_dhc(void);
                            /* display hash chains */
hword         vir_dht(void);
                            /* display hash table */

hword         vir_end(void); /* shutdown routine */

hword         vir_fb(struct virbh *blkhdr);
                         /* put block on free chain */
                         /* note: does not unlock block */

byte         *vir_gba(struct virbh *blkhdr);
                            /* get block address from header */
struct virbh *vir_gbh(hword lfd, tint blknum);
                            /* get block hdr, put blk on lcked list */
tint          vir_gbn(struct virbh *blkhdr);
                            /* get block number from blkhdr */
hword         vir_gbl(struct virbh *blkhdr);
                            /* get the list the block is on */
tint          vir_gbs(void); /* returns block size */

hword         vir_gbt(hword lfd, tint blknum);
                         /*   "Get Block Type" */

struct virbh *vir_gfb(void);
                         /* gets a free or unlocked buffer*/

hword         vir_gnlb(void);
                         /* "Get Number Locked Blocks" */

tint          vir_gtl(hword lfd);
                         /* get transaction log start */

hword         vir_opn(hword lfd, hword options);
              /* bring an lfd online, marking it open if r/w */
#define VIR_OPN_READ_ONLY 1  /* just mark it open read only */
#define VIR_OPN_JOURNAL   2  /* use journaling */
#define VIR_OPN_FORCE     4  /* force opn even if detect crash */

#define VIR_OPN_OK 0
#define VIR_OPN_NOT_EXIST  1 /* file to open does not exist */
#define VIR_OPN_NOT_CLOSED 2 /* warning: close ok flag not set */


hword         vir_pbh(struct virbh *blkhdr);
                         /* writes a block out to disk */

hword         vir_rhc(struct virbh *blkhdr);
                         /* "Remove from Hash Chain" */

hword         vir_rlb(hword lfd,hword mode);
                           /* remove buffers of given lfd
                            from unlocked list, also scans lock list */

hword         vir_ptl(hword lfd,tint blknum);
                            /* set blknum for transaction log */

struct virbh *vir_gbz(hword lfd);
                          /*"Get Block Zero" */

tint          vir_gndb(hword lfd);
                         /* "Get Number of Database Blocks"
                           returns number of blks on an lfd */

hword         vir_uac(struct virbh **blkhdr);
              /* "Unlock And Clear pointer" to vir blk hdr */


tint          virggtl(hword lfd);
                         /* get the goid table least free */
tint          virggtr(hword lfd);
                         /* get the goid table root */

tint          virgnfb(hword lfd);
                         /* Get Number of Free Blocks */


hword         virlock(struct virbh *blkhdr);
                         /* inc lock count for block header */

hword         virpgtl(hword lfd,tint blknum);
                         /* set the goid table least free */
hword         virpgtr(hword lfd,tint blknum);
                         /*  set the goid table root block */

hword         virrel(struct virbh *blkhdr);
                         /* put the block on free list */

hword         virsync(void); /* wait for pending i/o to finish */

hword         virunlk(struct virbh *blkhdr);
                         /* dec link count */

/* $endextract*/

#endif /* to match initial #ifndef */

