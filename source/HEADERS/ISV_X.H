/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     ISV_X H                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    ISV                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  External header file for ISV level.  Should only be    |*/
/*|            #included'd by "isv.h".  Contains declarations for     |*/
/*|            using ISV funcitons (as opposed to "isv_key.h", which  |*/
/*|            contains only #define'atations of ISV indexes.)        |*/
/*|                                                                   |*/
/*|            This header file is also the MAKER source file for the |*/
/*|            ISV level.                                             |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     31_Mar_87   K.J.Walsh   Implemented.                   |*/
/*| :MODS.     06_Jul_88   K.J.Walsh   Updated documentation.         |*/
/*|                                                                   |*/
/*| :MODS.     11_Jul_88   K.J.Walsh   Added "externs" (which had     |*/
/*|                                    somehow been missing since     |*/
/*|                                    Mar 87!); also added synonym   |*/
/*|                                    for isv_matc().                |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef ISV_X_H_INCLUDED
#define ISV_X_H_INCLUDED

/* $EXTRACT $MAKER-PROTO */

/*+-------------------------------------------------------------------+*/
/*| Initialize:  Allocates ISV table, brings in default ISV, updates  |*/
/*| TM_master to locate table.                                        |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_init (void) ;

/*+-------------------------------------------------------------------+*/
/*| Add ISV table:  Allocates memory for and reads in a new ISV table,|*/
/*| which is searched before previous tables.  The string isv_id is   |*/
/*| standardized TEIES file descriptor and may be abbreviated to just |*/
/*| the file name.  If S_NULL is provided, the first file of type     |*/
/*| ISV$OBJ is used.                                                  |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_add  (string isv_id) ;

/*+-------------------------------------------------------------------+*/
/*| Remove ISV table:  Deallocates a table and updates links in       |*/
/*| existing tables.  The string isv_id is                            |*/
/*| standardized TEIES file descriptor and may be abbreviated to just |*/
/*| the file name.  If S_NULL is provided, then the last ISV added    |*/
/*| will be removed.                                                  |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_rem  (string isv_id) ;

/*+-------------------------------------------------------------------+*/
/*| Get ISV entry:  Returns the address of the ISV given by ISV_NUM.  |*/
/*| The ISV will be no larger than MAXSIZE, unless 0 is specified,    |*/
/*| in which case the largest available will be indicated.            |*/
/*| ISV_STRING CANNOT BE MODIFIED--COPY IT IF NEED BE.                |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_get (string *ISV_STRING, hword ISV_NUM, sint MAXSIZE);

/*+-------------------------------------------------------------------+*/
/*| Match string to ISV:  Returns the ISV id number {or error code if |*/
/*| not possible} of an ISV matching string FIND.  Search is conducted|*/
/*| between START_NUM and END_NUM inclusive.                          |*/
/*+-------------------------------------------------------------------+*/
extern isv isv_matc (string FIND, isv START_NUM, isv END_NUM) ;

/*+-------------------------------------------------------------------+*/
/*| End ISV tables:  Deallocates all ISV tables.                      |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_end  (void) ;

/* $ENDEXTRACT */

/*+-------------------------------------------------------------------+*/
/*|  This synonym is provided because of some confusion with the      |*/
/*|  routine name--debugging calls it "ISV_MATCH".                    |*/
/*+-------------------------------------------------------------------+*/
#define isv_match isv_matc

/*+-------------------------------------------------------------------+*/
/*|  These routines will not be available in Phase one.               |*/
/*+-------------------------------------------------------------------+*/
#if 0
/*+-------------------------------------------------------------------+*/
/*| Dumps out ISV table pointed to and all subseqent ones.  If pointer|*/
/*| is NIL then TM_master->ISV is used.                               |*/
/*+-------------------------------------------------------------------+*/
extern   sint  isv_dump (hword *TABLE);
extern   sint  isv_qry  () ;
extern   sint  isv_lkup () ;
extern   sint  isv_numb () ;
#endif

#endif
