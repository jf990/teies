/*+-------------------------------------------------------------------+*/
/*|   <Doc>                                                           |*/
/*|   <NAME>      SKI.H                                               |*/
/*|   <AUTHOR>    Pamela Cham                                         |*/
/*|   <DATE>      September 22, 1989                                  |*/
/*|   <LEVEL>     SKI                                                 |*/
/*|   <PURPOSE>   System Keyword Index management structures and      |*/
/*|               definitions.                                        |*/
/*|   </Doc>                                                          |*/
/*+-------------------------------------------------------------------+*/

#ifndef SKI_H_INCLUDED
#define SKI_H_INCLUDED

/* $EXTRACT $MAKER-PROTO */

/*+------------------------------------------------------------------+*/
/*| Actual definition of the SKI structure may be found in           |*/
/*| OBJ_COMM.h.                                                      |*/
/*+------------------------------------------------------------------+*/
/*| Declarations of SKI level routines.                              |*/
/*+------------------------------------------------------------------+*/


/*+-------------------------------------------------------------------+*/
/*| SKI_FMT :  Formats the given key string into a valid keyword      |*/
/*| string result.  A slash is added at the start and end of the      |*/
/*| key string and each delimiting space in the given key string      |*/
/*| is replaced by a slash to separate each word.  Excess spaces      |*/
/*| are deleted.                                                      |*/
/*+-------------------------------------------------------------------+*/

extern sint ski_fmt (string *result, string key_string);

/*+-------------------------------------------------------------------+*/
/*| SKI_PRSE :  Parses the given string target to locate the primary  |*/
/*| key and secondary key strings which appear in front of target.    |*/
/*| Target is returned with the remainder of the input string.        |*/
/*| The front of target may be in any of the following forms:         |*/
/*|    /word/ or /word./ specifies a single keyword which is assigned |*/
/*|                      to primary key.                              |*/
/*|    /.word/           specifies only a secondary key.              |*/
/*|    /word.word/       specifies both the primary and secondary key.|*/
/*+-------------------------------------------------------------------+*/

extern sint ski_prse (string *target, string *primary_key,
                                      string *secondary_key);

/*+-------------------------------------------------------------------+*/
/*| SKI_ADD :  Adds an entry in the System Keyword Index, given the   |*/
/*| primary key, secondary key, oid, and status.                      |*/
/*+-------------------------------------------------------------------+*/

extern sint ski_add (string primary_key, string secondary_key,
                     OID *oid, sint status);

/*+-------------------------------------------------------------------+*/
/*| SKI_FIND :  Looks for the oid and status of an entry in the       |*/
/*| System Keyword Index based on the given primary and secondary     |*/
/*| keys.  The secondary key may be null, which means that only a     |*/
/*| single key is used.                                               |*/
/*+-------------------------------------------------------------------+*/

extern sint ski_find (string primary_key, string secondary_key,
                      OID **oid, sint *status);

/*+-------------------------------------------------------------------+*/
/*| SKI_DEL :  Deletes an entry corresponding to the given key string |*/
/*| from the System Keyword Index.                                    |*/
/*+-------------------------------------------------------------------+*/

extern sint ski_del (string primary_key, string secondary_key);

/* $ENDEXTRACT */

#endif
