/*+-------------------------------------------------------------------+
  | <DOC>                                                             |
  |                                                                   |
  | <NAME>     hsh_calc.c                                             |
  |                                                                   |
  | <LEVEL>    HASH                                                   |
  |                                                                   |
  | <AUTHOR>   Samir Chopra                                           |
  |                                                                   |
  | <DATE>     June 8th, 1989                                         |
  |                                                                   |
  | <PARAM>    string     NAME       INPUT                            |
  |                                                                   |
  | <GLOBALS>                                                         |
  |                                                                   |
  | <PURPOSE>  Routine carries out hashing function on given string   |
  |                                                                   |
  | <OUTPUT>                                                          |
  |                                                                   |
  | <ERRORS>                                                          |
  |                                                                   |
  | </DOC>                                                            |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "hash.h"

  sint hsh_calc (name)

  string     name;

  {

     sint      hashval    = 0;
     sint      i          = 0;
     sint      return_val = 0;
     char      name_copy;


     for (i= 1 ; i < s_len(name); i++)

     {
        s_stoc (name_copy, name, i);

        hashval =  hashval + (sint)name_copy;

     }
     return_val = (hashval%HASH_TABLE_SIZE) ;
   return(return_val);

}


