/*+-------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>     hash h                                                  |
 |                                                                    |
 | <LEVEL>    HASH                                                    |
 |                                                                    |
 | <AUTHOR>   Samir Chopra                                            |
 |                                                                    |
 | <DATE>     June 8th, 1989                                          |
 |                                                                    |
 | <PURPOSE>  Header file for hashing functions.                      |
 |                                                                    |
 | <MODS>     June_15_1989 :  Samir Chopra :   Completed prototyping  |
 |                                                                    |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#define  HASH_TABLE_SIZE 25


struct hasht
               {                                 /* o o o o o o o o  */
                                                 /*                  */
                 string   v_name;                /*                  */
                 void     *contents;             /*       Define     */
                 struct   hasht *next;           /*          a       */
                 hword    type;                  /*    Hash Table    */
                                                 /*     Structure    */
                                                 /*                  */
               }  ;                              /*                  */
                                                 /*                  */
typedef    struct hasht   CELL;                  /* o o o o o o o o  */




/*$EXTRACT $MAKER-PROTO */


/*+-------------------------------------------------------------------+
 | Definition of HASH level routines follow                           |
 +--------------------------------------------------------------------+*/

/*+-------------------------------------------------------------------+
 | Hsh_del: Takes as input a string and a pointer to a CELL structure,|
 | and deletes the string from the hash table.                        |
 +--------------------------------------------------------------------+*/

extern sint    hsh_del ( string name, CELL **hash_ptr);

/*+-------------------------------------------------------------------+
 | Hsh_calc: Takes as input a string and calculates its hash value by |
 | adding up the character values in the string and returning the     |
 | the modulo value as the array size.                                |
 +--------------------------------------------------------------------+*/

extern sint    hsh_calc( string name);

/*+-------------------------------------------------------------------+
 | Hsh_find:Takes as input a string and a pointer to a CELL structure,|
 | and looks for the string in the hash table. Returns a pointer to   |
 | the string location if found.                                      |
 +--------------------------------------------------------------------+*/

extern CELL   *hsh_find( string name, CELL **hash_ptr);

/*+-------------------------------------------------------------------+
 | Hsh_ins :Takes as input a string and a pointer to a CELL structure,|
 | and inserts the string in the hash table. Calls hsh_ins to check if|
 | the string already exists.                                         |
 +--------------------------------------------------------------------+*/

extern CELL   *hsh_ins ( string name, CELL **hash_ptr);

/*+-------------------------------------------------------------------+
 | Hsh_init : Intializes array of pointers to a CELL structure by     |
 | setting all to NULL, and returns head pointer of array.            |
 +--------------------------------------------------------------------+*/

extern CELL   **hsh_init( void );

/*+-------------------------------------------------------------------+
 | Hsh_new  : Allocates a new array of pointers to CELL structures and|
 | sets all to NULL. {obviously!}                                     |
 +--------------------------------------------------------------------+*/

extern CELL    **hsh_new ( void );

/*+-------------------------------------------------------------------+
 | Hsh_drop : Deallocates all memory allocated by hashing routines for|
 | a hash table structure. Routine is recursive in case multiple hash |
 | tables exist.                                                      |
 +--------------------------------------------------------------------+*/

extern sint    hsh_drop( CELL **tab_ptr );

/*+-------------------------------------------------------------------+
 | Hsh_copy : Makes a copy of an existing CELL structure and returns  |
 | location pointer.                                                  |
 +--------------------------------------------------------------------+*/

extern CELL   *hsh_copy(CELL *hash_ptr);

/*+-------------------------------------------------------------------+
 | Hsh_dump : Dumps out the contents of a hash table, calling itself  |
 | recursively in case multiple hash tables exist.                    |
 +--------------------------------------------------------------------+*/

extern void    hsh_dump(CELL **pointer,char *caller);


/*$ENDEXTRACT*/

/*+--------------------------------------------------------------------+
 | Definition of HASH level error codes follow.                        |
 | Exception level is set as 24000                                     |
 +--------------------------------------------------------------------+*/


 #define HASH_INPUT_STRING_NOT_FOUND                   24001
 #define HASH_INPUT_STRING_ALREADY_EXISTS              24002
 #define HASH_BAD_POINTER_TO_CELL                      24003
 #define HASH_LEVEL_NOT_INITIALIZED                    24004
 #define HASH_CANNOT_DELETE_STRING                     24005

#endif
