/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     OBJ_LIST H                                             |*/
/*| <AUTHOR>   Kevin J. Walsh                                         |*/
/*| <DATE>     December 30, 1988                                      |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <PURPOSE>  Declares functions and tokens required to use the      |*/
/*|            object level list functions.                           |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/

#ifndef OBJ_LIST_H_INCLUDED
#define OBJ_LIST_H_INCLUDED

/* $EXTRACT $MAKER-PROTO */

/*+-------------------------------------------------------------------+*/
/*| The following functions are all actually front ends to list_fun{} |*/
/*+-------------------------------------------------------------------+*/
extern sint ab_list  (OCB*,string,hword,byte*) ;
extern sint do_list  (OCB*,string,hword,byte*) ;
extern sint gt_list  (OCB*,string,hword,void*) ;
extern sint jn_list  (OCB*,string,hword,byte*) ;
extern sint sc_list  (OCB*,string,hword,byte*) ;
extern sint rc_list  (OCB*,string,hword,byte*) ;
extern sint vw_list  (OCB*,string,hword,byte*) ;

/*+-------------------------------------------------------------------+*/
/*| list_fun{} performs most of the actual list viewing functionality:|*/
/*+-------------------------------------------------------------------+*/
extern sint list_fun (hword,OCB*,string,hword,byte*) ;

/*+-------------------------------------------------------------------+*/
/*| Creates OCB**, if OCB* exists it is cleared.                      |*/
/*+-------------------------------------------------------------------+*/
extern sint cr_list  (OCB*,string,hword,OCB**) ;

/*+-------------------------------------------------------------------+*/
/*| Adds, deletes, or performs modifiy action on elements of list.    |*/
/*+-------------------------------------------------------------------+*/
extern sint md_list  (OCB*,string,hword,void*) ;

/*+-------------------------------------------------------------------+*/
/*| Reorders list according to sort criteria.                         |*/
/*+-------------------------------------------------------------------+*/
extern sint or_list  (OCB *, string, hword, void * );

/*+-------------------------------------------------------------------+*/
/*| Review list.                                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint rv_list  (OCB * , string, hword, void * );

/*+-------------------------------------------------------------------+*/
/*| Verifys if the indicated list exists.                             |*/
/*+-------------------------------------------------------------------+*/
extern sint ex_list  (OCB*,string,hword,byte*) ;

/*+-------------------------------------------------------------------+*/
/*| Finds a saved list according to certain search criteria.          |*/
/*+-------------------------------------------------------------------+*/
extern sint fn_list  (OCB *, string, hword, void * );

/*+-------------------------------------------------------------------+*/
/*| Link list to another object.                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint lk_list  (OCB * , string, hword, void * );

/*+-------------------------------------------------------------------+*/
/*| Remove a list.                                                    |*/
/*+-------------------------------------------------------------------+*/
extern sint rm_list  (OCB*,string,hword,OCB**) ;
/* $ENDEXTRACT */

/*+-------------------------------------------------------------------+*/
/*| LEVEL parameters for all OBJECT LIST functions.                   |*/
/*+-------------------------------------------------------------------+*/
#define  OBJ_LIST_INIT       0x01      /* Level to start or restart    */
#define  OBJ_LIST_START      OBJ_LIST_INIT

#define  OBJ_LIST_ADD        0x03      /* Level to ADD to existing list*/

#define  OBJ_LIST_END        0x04      /* Level to DELETE list         */

#define  OBJ_LIST_DEL        0x05      /* Level to DELETE list element */
#define  OBJ_LIST_DELETE     OBJ_LIST_DEL

#define  OBJ_LIST_ADD_ENTRY  0x06      /* These ADD or DEL an item with*/
#define  OBJ_LIST_DEL_ENTRY  0x07      /* an ENTRY field to a list.    */

#define  OBJ_LIST_ADD_NUM    0x08      /* These ADD or DEL an item with*/
#define  OBJ_LIST_DEL_NUM    0x09      /* an NUMBER field to a list.   */
#define  OBJ_LIST_ADD_NUMBER OBJ_LIST_ADD_NUM
#define  OBJ_LIST_DEL_NUMBER OBJ_LIST_DEL_NUM

#define OBJ_DELETE_LOCAL_LIST   0x0A   /* Levels for rm_list to delete */
#define OBJ_DELETE_REMOTE_LIST  0x0B   /* local or remote lists        */

#define OBJ_DELETE_REMOTE_CALL  0x0C   /* internal level flag used in  */
                                       /* rm_list. SHOULD NOT be used  */
                                       /* by calling routine.          */
#endif
