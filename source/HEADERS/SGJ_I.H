/* functions to interface database subsystem with the
   index level subsystem */

#define SGJ_REAL 1 /* if '1', will compile actual calls in */

#define M_KEYLEN 32    /* maximum key length of index */

/* $ci2e-struct */
struct sgj_cb {
   hword lfd;                /* logical file descripter            */
   struct stl_cb *stlcb;     /* STL control block                  */
   sint offset;              /* *SIGNED* offset for index          */
   byte key[M_KEYLEN];     /* where the key is stored, left just */

   /* current top of index */
   tint topblk;
   hword topseg;

   /* updated by 'replace', 'add', 'delete' */
   tint newblk;
   hword newseg;

   /* data for 'add' and 'replace' operations */
   /* result for 'find' operation */
   tint datablk;
   hword dataseg;
   boolean found_it;
   };

struct sgj_str {
   /* disk address key */
   tint keyblk;
   hword keyseg;

   /* disk address data */
   tint datblk;
   hword datseg;
   };



/* $ci2e-end-struct */
   /* $extract $maker-proto */
hword             sgj_beg ( void);

tint              sgj_cni ( struct dms_cb * dmscb , byte * pos);
                  /* "Count Number of Indexes" */

hword             sgj_end ( void);

hword             sgj_find ( struct dms_cb * dmscb , byte * pos);
                  /* find in the index */

hword             sgj_gpl  ( byte * path);

hword             sgj_iss  ( struct dms_cb * dmscb);
                  /* "Initialize String Space" */

hword             sgj_rpl  ( struct dms_cb * dmscb , byte * pos
                  , tint rpl_blk , hword rpl_seg);

hword             sgjdcb   ( struct sgj_cb * sgjcb);
                  /* "Display Control Block" */

hword             sgjdstr  ( struct sgj_str * sgjstr);
                  /* "Display sgj Structure" */

hword             sgjmd    ( struct dms_cb * dmscb , hword index);
                  /* "Move Down" */

hword             sgjmf    ( struct dms_cb * dmscb , hword index);
                  /* "Move Forward" */

hword             sgjscb   ( struct sgj_cb * sgjcb , byte * pos);

#define SGJ_remoVe_Element sgjve
hword             sgjve( struct dms_cb * dmscb , struct dms_dla * dla);

#define SGJ_remoVe_Level sgjvl
hword             sgjvl    ( struct dms_cb * dmscb);

hword             sgjdndx  ( struct node * sgistr);



   /* $endextract */

