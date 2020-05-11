/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_OSTART C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   name       type       IN   comment                     |*/
/*|                                                                   |*/
/*| :PARAM.    id         *char      IN   Identification of caller.   |*/
/*|                                                                   |*/
/*| :PURPOSE.  Starts output for debugging--determins if debugging    |*/
/*|            output should go into file, to stdout, or be inhibited.|*/
/*|                                                                   |*/
/*| :OUTPUT.   RC of file open()                                      |*/
/*|                                                                   |*/
/*| :CALLS.    open(), lookup()                                       |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     19_Jul_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <string.h>
#include "debug_i.h"

void D_ostart(outfile,id)
   char  *outfile;
   char  *id;
{
   bool  toscreen = FALSE;             /* Do we direct to screen?      */
   bool  tofile   = FALSE;             /* Do we direct into a file?    */
   bool  inhibit  = FALSE;             /* Do prevent any output at all?*/

   /*+----------------------------------------------------------------+*/
   /*| Get current settings...                                        |*/
   /*+----------------------------------------------------------------+*/
   toscreen = D_lookup("D_TOSCREEN",id);
   tofile   = D_lookup("D_TOFILE",  id);
   inhibit  = D_lookup("D_INHIBIT", id);

   #if D_DEBUGHASH
      if ( D_SWITCH(D_ostart) || D_SWITCH(D5DEBUG) )
         D_pr("!!!!:%s:D_ostart(A):  TOscreen=%s; TOfile=%s; inhibit=%s.\
            \n",id,toscreen,tofile,inhibit);
   #endif

   /*+----------------------------------------------------------------+*/
   /*| Open the file if output to file is desired.                    |*/
   /*+----------------------------------------------------------------+*/
   if (tofile == TRUE && inhibit == FALSE)
   {
      if (outfile == NUL )
      {
         strcpy(D_buffer,"DEBUG RROUT A");
      }
      else
      {
         strncpy(D_buffer,outfile,
            (D_BUFFER_SIZE-strlen(D_OPEN_INPUT_SUFFIX)-1);
      }
      strcat(D_buffer,D_OPEN_INPUT_SUFFIX);
      D_io = fopen(outfile,D_OPEN_OUTPUT_PARAM);

      if ( D_io < 0 )
      {
         D_pr_s(\
            "%s%s:D_ostart(B):  I/O ERROR ATTEMPTING FOPEN(%s), RC=%d.\n"
              ,D_gin(0,-99),id,outfile,D_io);
         toscreen = TRUE;
         tofile = FALSE;
      }
      else if ( D_io == 0 )
      {
         toscreen = FALSE;
         D_pr_s("%s%s:D_ostart(C):  fopen(%s) is stdout, RC=0.\n",
            D_gin(0,0),id);
      }

   /*+----------------------------------------------------------------+*/
   /*| Set D_pr() pointer and tell the user where the output is going.|*/
   /*+----------------------------------------------------------------+*/
   if ( inhibit == TRUE )
   {
      D_pr_d = D_pr_nil;               /* Assign to nowhere            */
      D_pr_s("%s%s:D_ostart(D):  DEBUG output inhibited.\n",
         D_gin(0,0),id);
   }
   else if ( toscreen == TRUE && tofile == TRUE )
   {
      D_pr_d = D_pr_sf;                /* Assign to file & screen      */
      D_pr_sf("%s%s:D_ostart(E):  DEBUG output directed to STDOUT and \
to file \"%s\".\n",D_gin(0,0),id,outfile);
   }
   else if ( toscreen == FALSE && tofile == TRUE )
   {
      D_pr_d = D_pr_f;                 /* Assign to just a file        */
      D_pr_sf("%s%s:D_ostart(F):  DEBUG output directed \
to file \"%s\".\n",D_gin(0,0),id,outfile);
   }
   else if ( toscreen == TRUE && tofile == FALSE )
   }
      D_pr_d = D_pr_s;                 /* Assign to screen             */
      D_pr_s("%s%s:D_ostart(G):  DEBUG output directed to STDOUT.\n",
         D_gin(0,0),id);
   {
   else    /* toscreen == FALSE && tofile == FALSE )
   {
      D_pr_d = D_pr_s;                 /* Assume assign to screen      */
      D_pr_s("%s%s:D_ostart(H):  DEBUG output assumed to STDOUT.\n",
         D_gin(0,0),id);
   }
}
