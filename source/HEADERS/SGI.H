#define MAX 32                /* maximun number bytes of a key         */
#define MID 2
#define T_KEY 4               /* total number of keys in a node        */

#define SGI_KEY_LEN 32
#define SGI_NUM_ITEMS 4
#define SGI_HALF_ITEMS 2

/* $ci2e-struct */
extern struct item            /* item has single key and a pointer     */
       {
       byte   del  ;          /* deletion flag.                        */
#define SGI_DELFLAG_EXIST '0'
#define SGI_DELFLAG_DELETED '1'

       hword  count;         /* to count duplicated key if it existed */
       hword  d_seg;         /* data pointers                         */
       tint   d_blk;         /* data pointer which point to data field*/
       byte   key[MAX];    /* key can store up to MAXimum characters*/
       hword  diskseg;       /* next segment address on disk          */
       tint   diskblk;       /* next block address on disk            */
       struct node *p;       /* key's pointer which can point to next */
       }
              *sgiitem;        /* return back to the node               */

extern struct node {      /* node in the tree                      */
   sint   member;         /* number of items in the node           */
   hword  c_seg,          /* current segment address on the disk   */
          segment;        /* next segment address on the disk      */
   tint   c_blk,          /* current block address on the disk     */
          block;          /* next block address on the disk        */
   struct node *p_1;      /* first way pointer in the tree         */
   struct item e[T_KEY+1]; /*total keys can be in the node       */
   tint number_items; /* added by ken 24 Feb 1989 */
   }
           *root1,         /* old root pointer                      */
           *root,          /* new root pointer                      */
           *previou,       /* previous pointer for update using     */
           *current,       /* current pointer for update using      */
           *q;             /* global using pointer                  */

/* sgi add control block */
struct sgi_acb {
   /* where I am now, set by caller */
   tint curblk;
   hword curseg;

   /* left side to be passed back up, set by called */
   tint leftblk;
   hword leftseg;

   /* parent passed up, set by called */
   /* parentblk and parentseg also set by called for return on copy */
   tint parentblk;
   hword parentseg;
   struct item parentitem;

   };

/* indicates whether or not sgi definitions included yet */
#define SGI_I_DEF 1


/* size of structure to allocate in database */
/* eventually will be sizeof (struct node) */
#define NODE_SIZE sizeof(struct node)


/* $ci2e-end-struct */

/* if we have not included the function defintions, do so now */

/*$extract $maker-proto */       /* starting of MAKER */

extern sint sgi_beg( void);  /* startup routine */

tint          sgi_cns(hword lfd, tint curblk, hword curseg);
              /* count number of segments */
              /* rc is number of segments found */

tint          sgi_pos(hword lfd, tint curblk, hword curseg
              , byte *targetkey, hword *command);
#define SGI_POS_SEARCH 1
#define SGI_POS_DONE 2

hword         sgi_vi( hword lfd , struct stl_cb * stlcb
              , tint blknum , hword segnum);
              /* Remove */


hword                sgiae(); /* add element */
#define SGIAE_INIT          37
#define SGIAE_SEARCH        38
#define SGIAE_ADD           39
#define SGIAE_COPY          40
#define SGIAE_CLEANUP       41
#define SGIAE_DUPLICATE     42 /* duplicate existing key found */
#define SGIAE_FOUND_DELETED 43 /* special case, found existing del key*/
#define SGIAE_ADD_FIRST     44
#define SGIAE_SETUP_PARENT  45


                     /* display item */
hword                sgidi(struct item *curitem);


                     /* display recursive */
hword                sgidr(hword prefix,hword lfd
                     , tint curblk, hword curseg);

hword                sgife(struct sgj_cb *sgjcb);
                     /* find exact */

                     /* find relative */
hword                sgifr(struct sgj_cb *sgjcb,hword *command
                     , tint curblk, hword curseg, sint *how_many);
#define SGIFR_FIRST_TIME 31
#define SGIFR_INIT 32
#define SGIFR_SEARCH 33
#define SGIFR_CLEANUP 34

                     /* find relative reverse */
hword                sgifrr(struct sgj_cb *sgjcb, hword *command
                     , tint curblk, hword curseg, sint *how_many);
#define SGIFRR_FIRST_TIME 35
#define SGIFRR_INIT 36
#define SGIFRR_SEARCH 37
#define SGIFRR_CLEANUP 38


hword               sgire(struct sgj_cb *sgjcb, hword *command
                    , tint curblk, hword curseg);
                    /* replace element */
#define SGIRE_INIT 50
#define SGIRE_REPLACE 51
#define SGIRE_CLEANUP 52
#define SGIRE_COPYUP 53

                    /* remoVe Element */
hword               sgive(struct sgj_cb *sgjcb, struct dms_cb *dmscb
                    , hword *command, tint curblk, hword curseg);
#define SGIVE_INIT 35
#define SGIVE_REPLACE 36
#define SGIVE_CLEANUP 37


 /* $endextract */

/************************************************************************
*|                                                                     |*
*|  :NAME.         sgi_i.h                                             |*
*|                                                                     |*
*|  :LEVEL.        SeGment Index.                                      |*
*|                                                                     |*
*|  :DATE.         10-28-86.                                           |*
*|                                                                     |*
*|  :AUTHOR.       Fen-Chung Kung.                                     |*
*|                                                                     |*
*|  :GLOBAL.       *root    struct node the root pointer points to the |*
*|                                      root node of index tree.       |*
*|  :GLOBAL.       *root1   struct node new root pointer.              |*
*|  :GLOBAL.       *q       struct node it's pointing to new top of the|*
*|                                      tree when node is splited.     |*
*|  :GLOBAL.       *current struct node indicate current node in the   |*
*|                                      index tree.                    |*
*|  :GLOBAL.       *previou struct node indicate path of 3rd dimension.|*
*|  :GLOBAL.       *sgiitem  struct item updated key item.              |*
*|  :GLOBAL.       *sgilink struct values this is using to indicate the|*
*|                                      all path of 3rd dimension.     |*
*|                                                                     |*
*|  :PARAM.        none.                                               |*
*|                                                                     |*
*|  :OUTPUT.       none.                                               |*
*|                                                                     |*
*|  :PURPOSE.      This is the internal header file of the SeGment     |*
*|                 Index level, all necessary global declaration and   |*
*|                 definition are defined on this header file.         |*
*|                                                                     |*
*|  :CALLS.        none.                                               |*
*|                                                                     |*
*|  :ERRORS.       RC = 20001     duplicated key is found.             |*
*|  :ERRORS.       RC = 29997     no more keys can be forward searched |*
*|  :ERRORS.       RC = 29998     no more keys can be backward searched|*
*|  :ERRORS.       RC = 29999     key is not existing.                 |*
*|                                                                     |*
*|  :LIMITS.       none.                                               |*
*|                                                                     |*
*|  :MODS.         4-12-87   Fen-Chung Kung  add struct copies for the |*
*|                                           deletion operation using. |*
*|                                                                     |*
*|  :END.                                                              |*
*|                                                                     |*
************************************************************************/

