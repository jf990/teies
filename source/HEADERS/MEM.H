/************************************************************************
  <DOC>

  <NAME>           MEM.H

  <LEVEL>          Memory Management.

  <AUTHOR>         Tanmay Kumar.

  <DATE>           March 20, 1988.

  <PARAMS>         None.

  <OUTPUT>         None.

  <GLOBALS>        struct mem_global - global memory structure which
                   keeps information about the master allocation and
                   master deallocation trees.
                   struct memtabs - structure for each process block
                   to maintain its allocation and deallocation trees.

  <PURPOSE>        Header file for memory management routines and tree
                   routines to keep all the External definitions required
                   to use the memory management functions.

  <OUTPUT>         None.

  <CALLS>          None.

  <ERRORS>         None.

  <LIMITS>         None.

  <MODS>
                   JF        1_Aug_88     Added MEMORY_H constant so the
                                          TM knows if MEM.H was already
                                          included.

                   TANMAY    15_FEB_89    Proto-typing done.


                   TANMAY    21_FEB_89    #define MEM_H_INCLUDED put in
                                          as per standards.

  </DOC>

*************************************************************************/
#ifndef MEM_H_INCLUDED

  #define MEM_H_INCLUDED

/*+===================================================================+*/
/*            Type definitions for Memory Management routines          */
/*+===================================================================+*/

extern struct memglobal           /*Global items for Memory Management */
       {
         struct memnode *tree1;   /* Master allocation tree            */
         struct memnode *tree2;   /* Master deallocation tree          */
         sint saddress;           /* starting address of master pool   */
         sint mcounter;           /* counts memory allocated from      */
                                  /*                  master pool      */
         sint reqsize;            /* size of master memory pool        */
       } mem_glob;                /* global variable- type memglobal   */

extern struct memtabs             /* structure which maintains the     */
                                  /* process block allocation and      */
                                  /* deallocation trees in the master  */
                                  /* allocation tree                   */
       {
         tint acounter;           /* counts memory allocated from the  */
                                  /* block                             */
         sint sbaddr;             /* starting address of block         */
         sint lbaddr;             /* ending address of block           */
         struct memtabs *nbptr;   /* pointer to connecting block       */
         struct memnode *balloc;  /* process block allocation tree     */
         struct memnode *bdealloc;/* process block dealloc tree        */
       };

/************************************************************************
*|                                                                     |*
*|  :NAME.         tm_tree.h                                           |*
*|                                                                     |*
*|  :LEVEL.        Transaction Management.                             |*
*|                                                                     |*
*|  :DATE.         December 21, 1987.                                  |*
*|                                                                     |*
*|  :AUTHOR.       Fen-Chung Kung.                                     |*
*|                                                                     |*
*|  :GLOBAL.       struct memnode      a key item.                     |*
*|  :GLOBAL.       struct memnode      a node.                         |*
*|  :GLOBAL.       MEM_MID             1/2 position of a node.         |*
*|  :GLOBAL.       MEM_MAX             max number of key items.        |*
*|  :GLOBAL.       MEM_LENG            length of key.                  |*
*|                                                                     |*
*|  :PARAM.        *mem_gitem          struct memitem for insertion    |*
*|                                                                     |*
*|  :OUTPUT.       None.                                               |*
*|                                                                     |*
*|  :PURPOSE.      To perform Delete, Insert, and Retrieve queries for |*
*|                 Transaction Management Level in the Memory.         |*
*|                                                                     |*
*|  :CALLS.        See below.                                          |*
*|                                                                     |*
*|  :ERRORS.       None.                                               |*
*|                                                                     |*
*|  :LIMITS.       None.                                               |*
*|                                                                     |*
*|  :MODS.         None.                                               |*
*|                                                                     |*
*|  :END.                                                              |*
*|                                                                     |*
************************************************************************/


/*+===================================================================+*/
/*            Type definitions for Tree Function routines              */
/*+===================================================================+*/

#define MEM_MID   3                    /* MINIMUN KEY FOR LOWER LEVEL  */
#define MEM_MAX   6                    /* MAXIMUN KEY NUMBER IN A NODE */
#define MEM_LENG                 16
#define MEM_MAXIMUM_KEY_LENGTH   16

extern struct memitem                  /*       Key item               */
       {                               /* contain : a key              */
          byte   key[MEM_LENG];      /*           a child pointer    */
          byte   *contents;            /*           contents           */
          struct memnode *p;           /*           a duplica. counter */
          hword  count   ;             /*                              */
       } *mem_gitem;                   /* u and g are global pointers  */

extern struct memnode                  /* each node                    */
       {                               /*                              */
          hword  member          ;     /* contain: counter for memner  */
          struct memnode *po     ;     /*          leftmost pointer    */

          struct memitem *e[MEM_MAX+1];
                                       /* key items                    */
       };
                                       /* root, q are global pointers  */
extern struct memstat
       {
          hword  pos ;                 /* forward search position      */
          hword  STOP;                 /* stops forward search         */
       } mem_gstat;
                                       /* status of dorward search     */

/*$EXTRACT $MAKER-PROTO*/

/*+===================================================================+*/
/*|               Memory Management Operation Functions                */
/*+===================================================================+*/

extern sint mem_init(sint resize);

/*   To allocate the master pool memory from the system
*    which will be used to allocate process memory blocks.             */

extern byte *mem_aloc(tint size);

/*  To assign the number of bytes requested by a process
*    out of its allocated block which in turn has been
*    allocated out of the master pool, and to update the
*    master allocation tree and process block allocation
*    tree to maintain a record of what has been allocated
*    and to whom.                                                      */

extern sint mem_dloc(byte *pntr);

/*  To deallocate memory previously allocated from the
*    block for that process when it is nolonger needed
*    and to update the process block deallocation tree
*    to maintain a record of what has been deallocated
*    and and of what size - or to free memory previously
*    allocated for the tree itself.                                    */

extern sint mem_kill(void);

/*  To deallocate all memory assigned to a process and to
*    remove the process from the master allocation tree
*    and place the freed memory in the master deallocation
*    tree.                                                             */

extern sint mem_end(void);

/* To return memory previously allocated back to the system            */


/*+===================================================================+*/
/*               Memory Management Internal Function                   */
/*+===================================================================+*/

extern sint mem_k2(struct memtabs *t_tab);

/*  To deallocate process block allocation
*    and deallocation trees and to add the
*    deallocated block to the master deallocation tree.                */


/*+===================================================================+*/
/*|                 TM_TREE  OPERATION  FUNCTIONS                     |*/
/*+-------------------------------------------------------------------+*/

extern struct memnode *mem_add(struct memitem *tmid,struct memnode *ptr);
                                       /* add key item into a node.    */
                                       /* parm. tmid and target tree   */
                                       /* it returns root of the tree. */

extern sint mem_fnd(struct memitem *tmid, struct memnode *tree,
                                                            hword pos);
                                       /* search key item from   root  */
                                       /* parm. tmid and target tree.  */

extern sint mem_del(struct memitem *tmid, struct memnode *ptr);
                                       /* delete a key item from a node*/
                                       /* parm. tmid and target tree.  */

extern sint mem_tend(struct memnode *tree, hword flag);
                                       /* free entired tree.           */
                                       /* parm. target tree.           */

extern sint mem_info(struct memnode *root, sint   length);
                                       /* print the status of tree     */
                                       /* parm. root of target tree.   */

/*+-------------------------------------------------------------------+*/
/*|                     END OF OPERATION FUNCTIONS                    |*/
/*+===================================================================+*/


/*---------------------------------------------------------------------*/
/*|                        INTERNAL FUNCTIONS                          */
/*---------------------------------------------------------------------*/

extern sint mem_ins(struct memitem *tmid, struct memnode *nodea,
                               boolean *flag, struct memitem *new_item);
                                       /* insertion routine            */

extern sint mem_ckp(hword  *left, hword *right, hword *k,
                               byte newkey[], struct memnode *header);
                                       /* check key item's position    */
                                       /*                              */

extern sint mem_dle(sint pos, struct memnode *nodea, struct memnode
                 *nodep, boolean *flag, boolean *flag1, boolean *tflag);
                                       /* deletion routine             */
                                       /*                              */

extern sint mem_dlt(struct memitem *tmid, struct memnode *nodea,
                         boolean *flag, boolean *flag1, boolean *tflag);
                                       /* deletion routine             */
                                       /*                              */

extern sint mem_find(struct memitem *tmid, struct memnode *pre_root,
                                                         boolean *flag);
                                       /* tree search.                 */

extern sint mem_bia(hword left, hword right, hword k, struct memnode *p,
                                   boolean *flag, struct memitem *tmid);

                                       /* bias  search.                */

extern sint mem_pos(sint p1, struct memnode *p, sint k,
                                                  struct memitem *tmid);

                                       /* forward search.              */

extern sint mem_tcle(struct memnode *tree, hword num, hword flag);

                                       /* clean all nodes from the tree*/

extern sint mem_und(struct memnode *nodec, struct memnode *nodea,
                                             sint  size, boolean *flag);

                                       /* handle the underflow routine */

extern struct memnode *mem_new(void);
                                       /* allocates a new node         */

extern struct memitem *mem_item(void);
                                       /* allocates a new item         */


/*$main  mem_test();                                                   */

/*$ENDEXTRACT                                                          */
#endif
