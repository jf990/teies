/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_HSET C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htable    D_htentry  IN/OUT  Hash table              |*/
/*| :GLOBAL.   D_htrans    sint(::)   IN      Hash translate table    |*/
/*| :GLOBAL.   D_DEBUGHASH #define    IN      Internal debugging flag |*/
/*|                                                                   |*/
/*| :PARAM.    variable    byte*      IN      Name of switch to set   |*/
/*| :PARAM.    hvalue      hword      IN      Hash value of variable  |*/
/*| :PARAM.    setting     bool       IN      Value to set switch to  |*/
/*| :PARAM.    id          byte*      IN      String identifying callr|*/
/*|                                                                   |*/
/*| :PURPOSE.  Given a switch name and setting, it places the data in |*/
/*|            the current hash table (either allocating or replacing |*/
/*|            as needed).                                            |*/
/*|                                                                   |*/
/*| :OUTPUT.   None                                                   |*/
/*|                                                                   |*/
/*| :CALLS.    D_hashit, strncmp, strncpy                             |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     16_Jun_88   K.J.Walsh   Implemented, based on D_set2(),|*/
/*|                                    D_setit (written by K.D.Ng) and|*/
/*|                                    D_set (written by A.K.Srinivis-|*/
/*|                                    ta).                           |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include <string.h>
#include "debug_i.h"

void D_hset(variable,hvalue,setting,id)
byte  *variable;
hword hvalue;
bool  setting;
byte  *id;

{
   struct D_htentry *h_ptr = NULL ;    /* Entry we might create        */
   struct D_htentry *cur   = NULL ;    /* An entry being examined      */
   struct D_htentry *old   = NULL ;    /* Origional version of an entry*/
   bool   found            = FALSE;    /* Does it already exist?       */
   sint   count            = 0    ;    /* A counter for some loops     */

   #if (NOT PROD_SYS)
      if (D_SWITCH(D2HSET) || D_SWITCH(D5DEBUG) )
         D_pr("!!!!:%s:D_hset(A): sw={%s}; hv=%d; set=%d.\n",
            id,variable,hvalue,setting);
   #endif
   /*+----------------------------------------------------------------+*/
   /*| Below we discover that the bucket is unallocated; this is fixed|*/
   /*| after this "if" and the "do...while loop" it contains.  We have|*/
   /*| to set up "old" to allocate a pointer later.                   |*/
   /*+----------------------------------------------------------------+*/
   if (D_htable(:hvalue:).first == NIL)
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2HSET) || D_SWITCH(D5DEBUG) )
               D_pr("!!!!:%s,D_hset(B):  No bucket--creating\n",id);
      #endif
      h_ptr = (struct D_htentry *) D_ALLOCATE(sizeof(struct D_htentry));
      D_htable(:hvalue:).first = h_ptr;

      strncpy(h_ptr->name,variable,D_HVARLEN);
      h_ptr->name(:D_HVARLEN+1:) = '\0';

      h_ptr->value = setting;
      h_ptr->next = NULL;
   }
   /*+----------------------------------------------------------------+*/
   /*| Here we discover that the bucket *is* allocated, so we search  |*/
   /*| it for the entry we want to set.                               |*/
   /*+----------------------------------------------------------------+*/
   else
   {
      cur = D_htable(:hvalue:).first;
      do
      {

         #if (NOT PROD_SYS)
            if (D_SWITCH(D2HSET) || D_SWITCH(D5DEBUG) )
            {
               D_pr("!!!!:%s:D_hset(C): Checking {%s}\n",id,cur->name);
            }
         #endif

         if (strncmp(variable,cur->name,D_HVARLEN) == 0)
         {
            #if (NOT PROD_SYS)
               if (D_SWITCH(D2HSET) || D_SWITCH(D5DEBUG) )
                  D_pr("!!!!:%s:D_hset(D):  Replacement.\n",id);
            #endif
            /*+-------------------------------------------------------+*/
            /*| The entry exists!  Just change the value.             |*/
            /*+-------------------------------------------------------+*/
            cur->value = setting;
            found = TRUE;
         }
         else
         {
            /*+-------------------------------------------------------+*/
            /*| Haven't found it yet, get next pointer (but keep old) |*/
            /*+-------------------------------------------------------+*/
            old = cur;
            cur = cur->next;
         }

      }
      while ((found == FALSE) && (cur != NIL));

      /*+-------------------------------------------------------------+*/
      /*| If "found" is set then we have already replaced the switch  |*/
      /*| in quesiton.  If it is not set, then either the bucket was  |*/
      /*| found but no element existed for the element.  We'll add it |*/
      /*| and fill it in here.                                        |*/
      /*+-------------------------------------------------------------+*/
      if (found == FALSE)
      {
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2HSET) || D_SWITCH(D5DEBUG) )
               D_pr("!!!!:%s:D_hset(E):  Adding new element.\n",id);
         #endif
         h_ptr = (struct D_htentry *) D_ALLOCATE(sizeof(struct \
            D_htentry));
         old->next = h_ptr;

         strncpy(h_ptr->name,variable,D_HVARLEN);
         h_ptr->name(:D_HVARLEN+1:) = '\0';

         h_ptr->value = setting;
         h_ptr->next = NULL;
      }
   }
}

