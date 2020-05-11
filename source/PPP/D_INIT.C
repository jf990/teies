/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_INIT C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_info       D_INFO   IN     Internal debbuging switch.|*/
/*|                                                                   |*/
/*| :PARAM.    infile       *char    IN     Input data file string.   |*/
/*| :PARAM.    outfile      *char    IN     Outpuf data file string.  |*/
/*| :PARAM.    id           *char    IN     Identification of caller. |*/
/*|                                                                   |*/
/*| :PURPOSE.  Has two purposes:                                      |*/
/*|                                                                   |*/
/*|            This file contains all the definations of all the      |*/
/*|            debugging variables (formerly they were included in    |*/
/*|            the main() file by the #D_MAIN() PPPMAC and the now    |*/
/*|            obsolete debugi.h header file.                         |*/
/*|                                                                   |*/
/*|            Upon being called the first time, it will allocate the |*/
/*|            various structures used by the Standardized Debugging, |*/
/*|            includeing those not used by the hashing debugging.    |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.    D_init_m, D_load, D_ostart                             |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     18_Jul_88   K.J.Walsh   Implemented                    |*/
/*|                                                                   |*/
/*| :MODS.     20_Jul_88   K.J.Walsh   Cut out "D_init_m" so that it  |*/
/*|                                    could be called standalone by  |*/
/*|                                    PPP.                           |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"

sint D_init(infile,outfile,id)
byte *infile;
byte *outfile;
byte *id;
{
   sint  rc =  0  ;

   D_init_m(id);                       /* Allocate memory & init       */

   D_load(infile,id);                  /* Load up debugging table      */

   rc = D_output(outfile,id);          /* Attempt output open & ret(RC)*/

   /*+----------------------------------------------------------------+*/
   /*| If asked for, dump the hash table.  Note that if the           |*/
   /*| compiled-in debugging is on, further switches are checked to   |*/
   /*| see if the dump should be produced.                            |*/
   /*+----------------------------------------------------------------+*/
   #if (NOT PROD_SYS)
      if
      ( D_lookup("D9HASHDUMP",id) || D_SWITCH(D5DEBUG) == TRUE )
      {
         D_hdump(id);
      }
   #endif

   /*+----------------------------------------------------------------+*/
   /*| If the master switch has been set off in the initial control   |*/
   /*| data file, tell the user and flip it off now.                  |*/
   /*+----------------------------------------------------------------+*/
   if ( D_lookup("9MASTEROFF","D_init(A)") == TRUE )
   {
      D_pr("----:%s:D_init(B):  D_master set OFF by D9MASTEROFF switch.\
         \n",id);
      D_INT_SW(D_ok_use) = FALSE;
   }

   return(rc);
}
