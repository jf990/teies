/*+-------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>      hsh_del.c                                              |
 |                                                                    |
 | <LEVEL>     HASH                                                   |
 |                                                                    |
 | <AUTHOR>    Samir Chopra                                           |
 |                                                                    |
 | <DATE>      June 8th, 1989                                         |
 |                                                                    |
 | <PARAM>     NAME     string    INPUT                               |
 |                                                                    |
 | <GLOBALS>   None                                                   |
 |                                                                    |
 | <PURPOSE>   Routine deletes given string from hash table           |
 |                                                                    |
 |                                                                    |
 | <ERRORS>                                                           |
 |                                                                    |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "hash.h"
#include "isv.h"

sint    hsh_del ( name , tab)

string          name;
CELL            **tab;

{

CELL    *np      = NIL;
CELL    *old_np  = NIL;
sint     hashval =   0;
sint     r       =   0;
sint    rc       =   0;

#D_begin ('hsh_del', 'hash');

#D_str ( ' name ', 'input string variable');

rc = (sint)hsh_find(name, tab);

#D_show('rc=%d','rc');

if (rc == NIL)
{
   rc = HASH_CANNOT_DELETE_STRING;
}
else
{

   rc = 0;

   hashval = hsh_calc (name);

   #D_str ('(*(tab+hashval))->v_name','resident string');

   if (*(tab+hashval) != NULL)
   {
      np = *(tab+hashval);
      old_np = np;
      #D_str ('np->v_name','resident string');
      while (np->next != NULL)
      {
         if (s_comp( np->v_name, name, 0, 0, 'F', &r)== 0)
         {
            #D_say('found string');
            old_np->next = np->next;
            break;
         } else
         {
            old_np = np;
            np = np->next;
         }
      }
   }
   if (old_np =np)
   {
      *(tab+hashval) = np->next;
   }
   #D_str ('np->v_name','variable deleted');
   s_drop ( &(np->v_name) );
   #D_show ( 'old_np = 0x%x', 'old_np');
}

   #D_return (rc);

}
