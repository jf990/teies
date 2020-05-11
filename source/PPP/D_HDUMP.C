/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_HDUMP C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htable   D_htentry* IN   The debugging hash table    |*/
/*|                                                                   |*/
/*| :PARAM.    id         *byte      IN   Identifies caller           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Dumps the entire hash table                            |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     17_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"
void D_hdump(id)
byte *id;
{

   struct    D_htentry   *temp1 = NULL;
   struct    D_htentry   *temp2 = NULL;
   int       count      =  0  ;
   int       count2     =  0  ;
   int       primaries  =  0  ;
   int       extensions =  0  ;
   int       empties    =  0  ;

   if ( D_htable == NULL)
   {
      D_pr("----:%s:D_hdump(A):  No hash data table allocated.\n",id);
   }
   else
   {
      D_pr("----:%s:D_hdump(B):  Used hash table buckets dump follows.\n",
         id);

      for ( count = 0; count < 256; count++ )
      {
         if ( D_htable(:count:).first != NULL )
         {
            primaries++;
            temp1 = D_htable(:count:).first ;
            count2 = 1;
   D_pr("    :D_hdump(C):  Bucket %03d:  val=%d, nxt=0x%06X, id={%s}.\n",
               count,temp1->value,temp1->next,temp1->name);
            temp1 = temp1->next;
            while (temp1 != NULL )
            {
               count2++;
               extensions++;
   D_pr("    :D_hdump(D):            :  val=%d, nxt=0x%06X, id={%s}.\n",
                  temp1->value,temp1->next,temp1->name);
               temp1 = temp1->next;
            }
         }
         else
         {
            empties++;
         }
      }

      D_pr("----:%s:D_hdump(E):  Used hash table buckets dump above.\n",
         id);
   }

   D_pr("    :%s:D_hdump(F):  Primary buckets    =%6d.\n",id,
                              primaries);

   D_pr("    :%s:D_hdump(G):  Extension buckets  =%6d.\n",id,
                              extensions);

   D_pr("    :%s:D_hdump(H):  Unused buckets     =%6d.  (Primary)\n",
                           id,empties);

   D_pr("    :%s:D_hdump(I):  D_STATS(D_s_qrys)  =%6d.  (queries)\n",
                           id,D_STATS(D_s_qrys));

   D_pr("    :%s:D_hdump(J):  D_STATS(D_s_nxts)  =%6d.  (nexts)\n",
                           id,D_STATS(D_s_nxts));

   D_pr("    :%s:D_hdump(K):  D_STATS(D_s_mnxt)  =%6d.  (maxnexts)\n",
                           id,D_STATS(D_s_mnxt));

   D_pr("    :%s:D_hdump(L):  D_SWITCH(D9PSTAT1) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D9PSTAT1)) );

   D_pr("    :%s:D_hdump(M):  D_SWITCH(D9PSTAT2) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D9PSTAT2)) );

   D_pr("    :%s:D_hdump(N):  D_SWITCH(D5DEBUG)  = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D5DEBUG)) );

   D_pr("    :%s:D_hdump(O):  D_SWITCH(D2END)    = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2END)) );

   D_pr("    :%s:D_hdump(P):  D_SWITCH(D2EXIST)  = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2EXIST)) );

   D_pr("    :%s:D_hdump(Q):  D_SWITCH(D2HASHIT) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2HASHIT)) );

   D_pr("    :%s:D_hdump(R):  D_SWITCH(D2HDUMP)  = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2HDUMP)) );

   D_pr("    :%s:D_hdump(S):  D_SWITCH(D2HSET)   = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2HSET)) );

   D_pr("    :%s:D_hdump(T):  D_SWITCH(D2INIT)   = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2INIT)) );

   D_pr("    :%s:D_hdump(U):  D_SWITCH(D2INIT_M) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2INIT_M)) );

   D_pr("    :%s:D_hdump(V):  D_SWITCH(D2LOAD)   = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2LOAD)) );

   D_pr("    :%s:D_hdump(W):  D_SWITCH(D2LOOKUP) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2LOOKUP)) );

   D_pr("    :%s:D_hdump(X):  D_SWITCH(D2OUTEND) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2OUTEND)) );

   D_pr("    :%s:D_hdump(Y):  D_SWITCH(D2OUTPUT) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2OUTPUT)) );

   D_pr("    :%s:D_hdump(Z):  D_SWITCH(D2QUERY)  = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2QUERY)) );

   D_pr("    :%s:D_hdump(AA): D_SWITCH(D2QUERIE) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2QUERIE)) );

   D_pr("    :%s:D_hdump(BB): D_SWITCH(D2SETVAR) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2SETVAR)) );

   D_pr("    :%s:D_hdump(CC): D_SWITCH(D2UNLOAD) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2UNLOAD)) );

   D_pr("    :%s:D_hdump(DD): D_SWITCH(D2UTIL)   = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D2UTIL)) );

   D_pr("    :%s:D_hdump(EE): D_SWITCH(D_ok_run) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D_ok_run)) );

   D_pr("    :%s:D_hdump(FF): D_SWITCH(D_ok_use) = %5s.\n",id,
                            D_BOOLSTR(D_SWITCH(D_ok_use)) );

}
