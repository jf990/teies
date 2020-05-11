/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_EXIST C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                         IN   Main table                  |*/
/*|                                                                   |*/
/*| :PARAM.    name       c-string   IN   Name of varable to check;   |*/
/*|                                                                   |*/
/*| :PARAM.    id         c-string   IN   Who asked;                  |*/
/*|                                                                   |*/
/*| :PURPOSE.  Checks if a switch exists (D_query returns false if it |*/
/*|            does not exist).                                       |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns FALSE if switch does not exist.  Returns TRUE  |*/
/*|            if it does exist regardless of it's setting.           |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     04_Jan_89   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"

extern strncmp();
extern strlen();

boolean D_exist(name,id)
byte     *name;                        /* Character string version     */
byte     *id;
{
   bool  found =  FALSE;               /* Did we find it in D_htable?  */
   bool  setting= FALSE;               /* The value it has if exists.  */
   sint  length=  0    ;               /* Length of *name              */
   struct D_htentry *temp_ptr = NULL;  /* Temporary search pointer     */
   hword hvalue = 0    ;               /* Has-value of variable        */

   #if (NOT PROD_SYS)
      byte *ch;
      tint num_tries = 0;           /* # times looping in bucket    */

      D_SWITCH(D_s_qrys++);             /* If stats, chalkup anodder qry*/
   #endif

   length = strlen(name);           /* Get the length of incoming   */
   if ( length > D_HVARLEN )        /* Is it too long?              */
      length = D_HVARLEN;           /* Then pretend it is smaller.  */

   hvalue = D_hashit(name,id);

   /*+-------------------------------------------------------------+*/
   /*| Find hash bucket, search it's entries for the one we want.  |*/
   /*+-------------------------------------------------------------+*/

   temp_ptr = D_htable(:hvalue:).first;

   #if (NOT PROD_SYS)
      if ( D_SWITCH(D2EXIST) || D_SWITCH(D5DEBUG) )
      {
         D_pr("!!!!:%s:D_exist(A): hvalue=%d, trying to find {%s}\n",
            id,hvalue,name);
         if (temp_ptr == NULL)
            D_pr("!!!!:%s:D_exist(B):  Empty bucket\n",id);
      }
   #endif

   while ( temp_ptr != NULL && found == FALSE )
   {

      #if (NOT PROD_SYS)
         num_tries++;
         if ( D_SWITCH(D2EXIST) || D_SWITCH(D5DEBUG) )
         {
            D_pr("!!!!:%s:D_exist(C): Checking against {%s} (0x%X)\n",
               id,temp_ptr->name,temp_ptr->value);
         }
      #endif

      if (strncmp(name,temp_ptr->name,length) == 0)
      {
         setting = temp_ptr->value;
         found = TRUE;
         #if (NOT PROD_SYS)
            if ( D_SWITCH(D2EXIST) || D_SWITCH(D5DEBUG) )
             D_pr("!!!!:%s:D_exist(D): FOUND MATCH with value 0x%X\n",
               id,setting);
         #endif
      }
      else
      {
         temp_ptr = temp_ptr->next;
      }
   }

   #if (NOT PROD_SYS)
      D_STATS(D_s_nxts) = D_STATS(D_s_nxts) + num_tries;
      if (num_tries > D_STATS(D_s_mnxt))
         D_STATS(D_s_mnxt) = num_tries;
      if ( D_SWITCH(D9PSTAT2) == TRUE || D_SWITCH(D2EXIST) \
         || D_SWITCH(D5DEBUG) )
      {
D_pr("!!!!:D_EXIST: RC:%d\
 Nc:%d Nt:%d Nm:%d Qt:%d\
 %s->%03d:%s\
.\n"
       ,setting                        /* RC                           */
       ,num_tries                      /* Nc, Next's that occurred here*/
       ,D_STATS(D_s_nxts)              /* Nt, Total number of Next's   */
       ,D_STATS(D_s_mnxt)              /* Nm, Max Nexts for single qry */
       ,D_STATS(D_s_qrys)              /* Qt, Query Total              */
       ,id                             /* Caller identification        */
       ,hvalue                         /* Hashvalue of varible name    */
       ,name                           /* String of varable name       */
        );
      }
   #endif

   return(found);
}
