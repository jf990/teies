/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_OUTPUT C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   name       type       IN   comment                     |*/
/*|                                                                   |*/
/*| :PARAM.    id         *byte      IN   Identification of caller.   |*/
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

sint D_output(outfile,id)
   byte  *outfile;
   byte  *id;
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

   #if (NOT PROD_SYS)
      if ( D_SWITCH(D2OUTPUT) || D_SWITCH(D5DEBUG) )
      D_pr("!!!!:%s:D_output(A):  TOscreen=%s; TOfile=%s; inhibit=%s.\n",
            id,
            D_BOOLSTR(toscreen),
            D_BOOLSTR(tofile  ),
            D_BOOLSTR(inhibit ));
   #endif

   /*+----------------------------------------------------------------+*/
   /*| Open the file if output to file is desired.                    |*/
   /*+----------------------------------------------------------------+*/
   if (tofile == TRUE && inhibit == FALSE)
   {
      if (outfile == NULL )
      {
         strcpy(D_buffer,"DEBUG RROUT A");
      }
      else
      {
         strncpy(D_buffer,outfile,
            (D_BUFFER_SIZE-strlen(D_OPEN_INPUT_SUFFIX)-1));
      }
      strcat(D_buffer,D_OPEN_INPUT_SUFFIX);
      D_io = fopen(outfile,D_OPEN_OUTPUT_PARAM);

      if ( D_io == NULL )
      {
        D_pr_s(\
       "****:%s:D_output(B):  I/O ERROR ATTEMPTING FOPEN(%s), RC=0x%X.\n"
              ,id,outfile,D_io);
         toscreen = TRUE;
         tofile = FALSE;
      }
      else if ( D_io == 0 )
      {
         toscreen = FALSE;
         D_pr_s("----:%s:D_output(C):  fopen(%s) is stdout, RC=0.\n",
            id);
      }
    }

   /*+----------------------------------------------------------------+*/
   /*| Set D_pr() pointer and tell the user where the output is going.|*/
   /*+----------------------------------------------------------------+*/
   if ( inhibit == TRUE )
   {
      D_pr_d = D_pr_nil;               /* Assign to nowhere            */
      D_pr_s("----:%s:D_output(D):  DEBUG output inhibited.\n",
         id);
   }
   else if ( toscreen == TRUE && tofile == TRUE )
   {
      D_pr_d = D_pr_sf;                /* Assign to file & screen      */
      D_pr_sf("----:%s:D_output(E):  DEBUG output to STDOUT \
and \"%s\".\n",id,outfile);
   }
   else if ( toscreen == FALSE && tofile == TRUE )
   {
      D_pr_d = D_pr_f;                 /* Assign to just a file        */
      D_pr_sf("----:%s:D_output(F):  DEBUG output to \"%s\".\n",
         id,outfile);
   }
   else if ( toscreen == TRUE && tofile == FALSE )
   {
      D_pr_d = D_pr_s;                 /* Assign to screen             */
      #if (NOT PROD_SYS)
         if ( D_SWITCH(D2OUTPUT) || D_SWITCH(D5DEBUG) )
            D_pr("!!!!:%s:D_output(G):  DEBUG output to STDOUT.\n",id);
      #endif
   }
   else    /* toscreen == FALSE && tofile == FALSE */
   {
      D_pr_d = D_pr_s;                 /* Assume assign to screen      */
      #if (NOT PROD_SYS)
         if ( D_SWITCH(D2OUTPUT) || D_SWITCH(D5DEBUG) )
            D_pr("!!!!:%s:D_output(H):  DEBUG output assumed STDOUT.\n"
               ,id);
      #endif
   }
}
