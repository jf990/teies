/*+--------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>    VARS.H                                                   |
 |                                                                    |
 | <LEVEL>   TM                                                       |
 |                                                                    |
 | <AUTHOR>  John L. Foster                                           |
 |                                                                    |
 | <DATE>    November 20, 1987                                        |
 |                                                                    |
 | <PURPOSE> This header file defines the routines for manipulating   |
 |           user interface variables.  User Interface procedures     |
 |           and Object Actions use these variables for data storage, |
 |           data conversion, user input and Forms interfacing.       |
 |                                                                    |
 | <MODS>  23_Aug_1989    Samir Chopra  Rewrote to include definitions|
 |                                      of routines to handle stemmed |
 |                                      variables. Hash tables are    |
 |                                      used as storage mechanism. For|
 |                                      definition of structures used |
 |                                      refer to Hash.h . All data    |
 |                                      types for interface variables |
 |                                      are now supported.            |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/

        /* $EXTRACT     */
        /* $MAKER-PROTO */

#ifndef VAR_X_H_INCLUDED
#define VAR_X_H_INCLUDED 1


/*+-------------------------------------------------------------------+
 | var_beg: Allocate the variables structure header. Returns a pointer|
 | to the head of the hash table. Calls hsh_init to intialize hash    |
 | table.                      .                                      |
 +--------------------------------------------------------------------+*/

extern sint    var_beg ( void ) ;

/*+-------------------------------------------------------------------+
 | var_set: Set a variable to a specific value.  Calls var_assign to  |
 | carry out the actual assignment of values { and to preserve        |
 | original parameters of var_set }                                   |
 +--------------------------------------------------------------------+*/

extern sint    var_set ( string name , void * contents) ;

/*+-------------------------------------------------------------------+
 | var_assign:  Assigns to a variable , a value specified by calling  |
 | routine.  Checks if variable exists, else adds it to variables     |
 | hash table.                                                        |
 +--------------------------------------------------------------------+*/

extern sint    var_assign ( string name , void * contents,hword type) ;

/*+-------------------------------------------------------------------+
 | var_init: Initialize a TM variable to a C string value and given   |
 |     contents.                                                      |
 +--------------------------------------------------------------------+*/

extern sint var_init(string *name,byte *s_name,void *contents,sint type);

/*+-------------------------------------------------------------------+
 | var_get : Returns a void pointer to the contents of the variable   |
 | requested. Does not copy contents.  Alterations of strings is the  |
 | responsibility of the caller.                                      |
 +--------------------------------------------------------------------+*/

extern void *var_get ( string name ) ;

/*+-------------------------------------------------------------------+
 | var_fnd : Returns a pointer to the location of a variable in a hash|
 | table.  If variables of form Name.Name. exist, returns a pointer to|
 | the last but one variable.                                         |
 +--------------------------------------------------------------------+*/

extern CELL   *var_fnd ( string name ) ;

/*+-------------------------------------------------------------------+
 | var_lfnd: Returns a pointer to the last variable part of a variable|
 | the form Name.Name.                                                |
 +--------------------------------------------------------------------+*/

extern CELL   *var_lfnd ( string name ) ;

/*+-------------------------------------------------------------------+
 | var_add: Adds a variable to a hash table, assigns contents and data|
 | type, only called when a new entry is to be made.                  |
 +--------------------------------------------------------------------+*/

extern CELL   *var_add ( string name, void* contents,hword type) ;

/*+-------------------------------------------------------------------+
 | var_drp: Drop the contents of a varible and delete variable name   |
 | from hash table, releasing all associated memory.                  |
 +--------------------------------------------------------------------+*/

extern sint    var_drp ( string name) ;

/*+-------------------------------------------------------------------+
 | var_clr : Clear out the contents of a variable. Variable name is   |
 | preserved in hash table.                                           |
 +--------------------------------------------------------------------+*/

extern sint    var_clr ( string name) ;

/*+-------------------------------------------------------------------+
 | var_end : Take head pointer to variables hash table and shut down  |
 | VARS level, deallocating all memory.                               |
 +--------------------------------------------------------------------+*/

extern sint    var_end ( CELL **root_ptr) ;

/*+-------------------------------------------------------------------+
 | var_ext: Adds the extensions of a variable of the form Name.Name.  |
 | to a hash table.  Called when stem already exists.                 |
 +--------------------------------------------------------------------+*/

extern CELL   *var_ext(string name,void *contents,hword type, CELL *ptr);


/*+-------------------------------------------------------------------+
 | var_parse:  Tears apart a variable into it individual stems into a |
 |   string vector.  If the vector is not allocated on entry it is    |
 |   in the routine.                                                  |
 +--------------------------------------------------------------------+*/

extern sint    var_parse( string var_name,  string_vector * vector );


/*+-------------------------------------------------------------------+
 | var_query:  Determine type of variable data or length of data given|
 | variable name and query request.  returns sint.                    |
 +--------------------------------------------------------------------+*/

extern sint    var_query( string name, sint query_request );

#define  VARIABLE_QUERY_LENGTH     1
#define  VARIABLE_QUERY_TYPE       2
#define  VARIABLE_QUERY_EXISTANCE  3



#endif
/* $ENDEXTRACT */



