
#ifndef VIR_I_H_INCLUDED /* $ci2e-redefine */

#define VIR_I_H_INCLUDED /* $ci2e-redefine */
/* actual definition in virstart( ); */
/* internal virtual level data structures */
/* $ci2e-struct */
struct virbh {
   struct virbh *next, *prev;
   byte *block;               /* addr of block itself */
   struct virbh *chain;       /* to resolve block collisions */
   tint blknum;
   hword lfd;
   hword segflags; /* flags for seg level */
   hword process;/* process it came from, for scheduler
                  * both for "kill" and "wakeup" */
   hword wrflag; /* 0=clean, 1=dirty */
/* following used for wrflag in virbh */
#define VIRBH_CLEAN 0 /* no modifications have been made */
#define VIRBH_DIRTY 1 /* modification has been made */

   hword bmm;    /* Buffer Memory Management status */
   hword stl;    /* hdr old/new status for SGH, SGR and SGI */
   hword list;   /* which list an item is on */
/* list values for blkhdr */
#define VIRBH_LIST_LOCK 1
#define VIRBH_LIST_UNLK 2
#define VIRBH_LIST_FREE 3

   hword lock_cnt; /* lock count */
   sint ident;   /* so we know which block this is, for debugging */
   byte xorsum;  /* xor of all bytes in block */
   };


extern struct vir_sys {      /* where all the vir stuff is */
   struct virbh *blkhdr;    /* start of block headers */
   unsigned char *block;    /* where the blocks start */
   struct virbh **hash;     /* where the hash table starts */
   struct virbh *locked, *free, *unlocked; /* various syst lists */
   long int hashsize, numblks, blksize;  /* set by setup routine */
   long int num_lock;
   tint gbh_count;  /* number times vir_gbh called */
   tint gbh_max;    /* max search length */
   tint gbh_sum;    /* sum of all chains in searching */
   tint gbh_hit;    /* number of times block found in queue */
   tint max_lock;   /* max number buffers locked */
   boolean flush_cache; /* if true, always flush/erase cache */
   boolean check_xor; /* if true, run xor checking on blocks */
   tint times_sync_called;
   tint number_disk_writes;
   tint number_disk_reads;
   void *out_of_space_vector;
   tint number_block_zero_writes;
   struct vir_lfd *virlfd;
   hword max_lfd;
   tint block2monitor; /* if non zero, will trace alloc and free */
   } virsys;        /* actually declared in virstart.c */


struct vir_lfd {  /* one of these for every lfd */
   lint *vir_ab_lookaside;
   hword vir_ab_pos; /* the next free block in vir_ab */
   void *free_array;       /* btm array if free chain detected bad */
   lint real_number_free_blocks; /* this also accounts for the
                                 free blocks in the cache */
   };



struct vir_fc {
   hword ident;  /* should be -1 for free blocks */
   hword filler; /* no longer used, for backwards compatibility */
   tint next;  /* next blk in chain */
   };


struct vir_root {            /* struct of blk zero in database */
   byte wipeout[16];       /* make sure SGB routines fail    */
   byte name[16];
   tint blksize;
   tint numblks;
   tint free_chain;          /* start of free chain */
   tint item_ndx;            /* note: this might not be used anymore */
   tint tl_chain;
   tint in_ndx;              /* note: this might not be used anymore */
   tint gt_root;             /* root of goid table */
   tint gt_low;              /* lowest free entry */
   tint obj_root;            /* used by object level to determine
                                the initial goid of system index */
   tint gt_high;             /* highest goid ever allocated */
   tint num_free;            /* number of free blocks */
   boolean shutdown_ok;      /* if true, was shut down ok */
   byte when_close[ 8 ];   /* when database was shut down */
   byte when_created[ 8 ]; /* when database was created */
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
/* $ci2e-internal */
hword         vir_dio(hword cmd,struct virbh *blkhdr);
                            /* do actual disk i/o */
/* disk read and write commands */
#define VIR_DIO_READ 1
#define VIR_DIO_WRITE 2
/* $ci2e-external */

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

/* $ci2e-internal */

struct vir_lfd *virglfd( hword lfd); /* get vir_lfd entry for lfd */



struct virbh *virngbh(hword lfd, tint blknum);
                         /* The real get block header function */
struct virbh *virngfb(void);
                         /* The real get free block function */

hword         virau(struct virbh *blkhdr);
hword         virdu(struct virbh *blkhdr);
hword         viraf(struct virbh *blkhdr);
hword         virdf(struct virbh *blkhdr);
hword         viral(struct virbh *blkhdr);
hword         virdl(struct virbh *blkhdr);
/* $ci2e-external */
/* $endextract*/

#endif /* to match initial #ifndef */

