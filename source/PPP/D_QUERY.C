/*+--------------------------------------------------------------------+
  |                                                                    |
  |      :NAME.         D_query.c                                      |
  |                                                                    |
  |      :AUTHOR.       Anoop K. Srivastava                            |
  |                                                                    |
  |      :DATE.         June 3, 1988.                                  |
  |                                                                    |
  |      :LEVEL.        Debug.                                         |
  |                                                                    |
  |      :GLOBAL.       D_htable(::)   : Hash table;                   |
  |                     D_htrans(::)   : Translation table.            |
  |                     D_HASHSTAT     : Internal stastics switch      |
  |                     D_hstat.       : Internal stastics structure   |
  |                                                                    |
  |      :PARAM.        hvalue   INPUT : Hash value of switch to be    |
  |                                         looked up.                 |
  |                     variable INPUT : Character-string version of   |
  |                                         switch to be looked up.    |
                        id       INPUT : String identifying caller
  |                                                                    |
  |      :PURPOSE.      Interrogates the hash table.  If entries are   |
  |                     found for the given hvalue, the associated     |
  |                     switches are compared with the given switch    |
  |                     (variable).                                    |
  |                                                                    |
  |      :OUTPUT.       TRUE Variable was found and set on.            |
  |                     FALSE Variable was not found or was set off.   |
  |                                                                    |
  |      :CALLS.        none.                                          |
  |                                                                    |
  |      :ERRORS.       none.                                          |
  |                                                                    |
  |      :LIMITS.       It is assumed that the "variable" is in all    |
  |                     upper case.                                    |
  |                                                                    |
  |      :MODS.         16_Jun_88  K.J.Walsh    Converted D_qry2() into|
  |                                             D_query().  Most of the|
  |                                             origional code is      |
  |                                             unaltered.             |
  |                                                                    |
  |      :END.                                                         |
  |                                                                    |
  +--------------------------------------------------------------------*/
#include "debug_i.h"

extern strncmp();
extern strlen();

boolean D_query(variable,hvalue,id)
byte     *variable;                    /* Character string version     */
hword    hvalue;                       /* Already hashed switch        */
byte     *id;
{
   bool  rcode =  FALSE;               /* Our eventual results         */
   bool  found =  FALSE;               /* Did we find it in D_htable?  */
   sint  length=  0    ;               /* Length of *variable          */
   struct D_htentry *temp_ptr = NULL;  /*Temporary search pointer     */

   #if (NOT PROD_SYS)
      byte *ch;
      tint num_tries = 0;           /* # times looping in bucket    */

      D_SWITCH(D_s_qrys++);             /* If stats, chalkup anodder qry*/
   #endif

   if ( D_INT_SW(D_ok_use) == FALSE )
   {
      rcode = FALSE;
   }
   else
   {

      length = strlen(variable);       /* Get the length of incoming   */
      if ( length > D_HVARLEN )        /* Is it too long?              */
         length = D_HVARLEN;           /* Then pretend it is smaller.  */

      /*+-------------------------------------------------------------+*/
      /*| Find hash bucket, search it's entries for the one we want.  |*/
      /*+-------------------------------------------------------------+*/

      temp_ptr = D_htable(:hvalue:).first;

      #if (NOT PROD_SYS)
         if ( D_SWITCH(D2QUERY) || D_SWITCH(D5DEBUG) )
         {
            D_pr("!!!!:%s:D_query(A): hvalue=%d, trying to find {%s}\n",
               id,hvalue,variable);
            if (temp_ptr == NULL)
               D_pr("!!!!:%s:D_query(B):  Empty bucket\n",id);
         }
      #endif

      while ( temp_ptr != NULL && found == FALSE )
      {

         #if (NOT PROD_SYS)
            num_tries++;
            if ( D_SWITCH(D2QUERY) || D_SWITCH(D5DEBUG) )
            {
               D_pr("!!!!:%s:D_query(C): Checking against {%s} (0x%X)\n",
                  id,temp_ptr->name,temp_ptr->value);
            }
         #endif

         if (strncmp(variable,temp_ptr->name,length) == 0)
         {
            rcode = temp_ptr->value;
            found = TRUE;
            #if (NOT PROD_SYS)
               if ( D_SWITCH(D2QUERY) || D_SWITCH(D5DEBUG) )
                D_pr("!!!!:%s:D_query(D): FOUND MATCH with value 0x%X\n",
                  id,rcode);
            #endif
         }
         else
         {
            temp_ptr = temp_ptr->next;
         }
      }
   }

   #if (NOT PROD_SYS)
      D_STATS(D_s_nxts) = D_STATS(D_s_nxts) + num_tries;
      if (num_tries > D_STATS(D_s_mnxt))
         D_STATS(D_s_mnxt) = num_tries;
      if ( D_SWITCH(D9PSTAT2) == TRUE || D_SWITCH(D2QUERY) \
         || D_SWITCH(D5DEBUG) )
      {
D_pr("!!!!:D_QUERY: RC:%d\
 Nc:%d Nt:%d Nm:%d Qt:%d\
 %s->%03d:%s\
.\n"
       ,rcode                          /* RC                           */
       ,num_tries                      /* Nc, Next's that occurred here*/
       ,D_STATS(D_s_nxts)              /* Nt, Total number of Next's   */
       ,D_STATS(D_s_mnxt)              /* Nm, Max Nexts for single qry */
       ,D_STATS(D_s_qrys)              /* Qt, Query Total              */
       ,id                             /* Caller identification        */
       ,hvalue                         /* Hashvalue of variable name   */
       ,variable                       /* String of varable name       */
        );
      }
   #endif

   return(rcode);
}
