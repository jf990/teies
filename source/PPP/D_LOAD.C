/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_LOAD C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htrans(::) D_HTRANS IN/OUT Hash translation table.   |*/
/*| :GLOBAL.   D_htable(::) D_HTABLE IN/OUT Switch table.             |*/
/*| :GLOBAL.   D_ifno.      D_HINFO  IN/OUT Information switchs/stats.|*/
/*|                                                                   |*/
/*| :PARAM.    datafile     *byte    IN     Switch data file.         |*/
/*| :PARAM.    id           *byte    IN     Identification of caller. |*/
/*|                                                                   |*/
/*| :PURPOSE.  Has three purposes:                                    |*/
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
/*|            If called with a valid filedescriptor it will attempt  |*/
/*|            add the switches listed in it to the current table. If |*/
/*|            the table has not yet been allocated, or has been      |*/
/*|            dropped with d_flush() it will allocate the table. If  |*/
/*|            the table already exists values will be add or replaced|*/
/*|            if they already appear in the table.                   |*/
/*|                                                                   |*/
/*| :OUTPUT.   output                                                 |*/
/*|                                                                   |*/
/*| :CALLS.    close(), D_lookup(), D_setvar(), free(), malloc(),     |*/
/*|            open(), printf(), read()                               |*/
/*|                                                                   |*/
/*| :ERRORS.   Returns file open error (-1) if file cannot be opened. |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     02_May_88   A.K.Srivastava Implemented                 |*/
/*|                                                                   |*/
/*| :MODS.     16_Jun_88   K.J.Walsh      Minor modifications.        |*/
/*|                                                                   |*/
/*| :MODS.     18_Jul_88   K.J.Walsh      Moved global definations in |*/
/*|                                       from main() routine; added  |*/
/*|                                       checking for main startup;  |*/
/*|                                       added D_info structure;     |*/
/*|                                       added switch reload/add     |*/
/*|                                       feature.                    |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include <string.h>
#include "debug_i.h"

sint D_load(datafile,id)
byte *datafile;                        /* Must have "(bin" in it!      */
byte *id;
{
   int      fcb_index         = -1;    /* C file ctrl blk ndx from open*/
   int      rc                =  0;    /* RC from read from file to buf*/
   int      count             =  0;    /* Counter for use in loops     */
   byte     *buffer           =  NULL; /* Fread input buffer           */
   bool     setting           =  FALSE;/* Extracted switch value       */
   bool     old_setting       =  FALSE;/* Previous value of switch     */

   #if (NOT PROD_SYS)
      printf("!!!!:%s:D_load(A):  DEBUG development version 2.1.\n",id);
   #endif

   /*+----------------------------------------------------------------+*/
   /*| If apparently not called before, do allocations.               |*/
   /*+----------------------------------------------------------------+*/
   if ( D_htable == NULL )                   /* Not set?               */
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2LOAD) || D_SWITCH(D5DEBUG) )
            printf("!!!!:%s:D_load(B):  Allocating table.\n");
      #endif

      D_htable = (struct D_HTABLE *) \
         D_ALLOCATE(sizeof(struct D_HTABLE) * 260);

      for (count = 0; count < 258; count++)   /*init the hash table    */
         D_htable(: count :).first = NULL;

   }

   /*+-------------------------------------------------------------+*/
   /*| Open datafile and read contents.                            |*/
   /*+-------------------------------------------------------------+*/
   if ( datafile == NULL )
   {
      strcpy(D_buffer,"DEBUG HASHDATA *");
   }
   else
   {
      strncpy(D_buffer,datafile,
         (D_BUFFER_SIZE-strlen(D_OPEN_INPUT_SUFFIX)-1));
   }
   strcat(D_buffer,D_OPEN_INPUT_SUFFIX);
   fcb_index = open (D_buffer, O_RDONLY);
   if ( fcb_index != -1 )
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2LOAD) || D_SWITCH(D5DEBUG) )
            printf("!!!!:%s:D_load(C):  Successful open(%s) done.\n",
               id,D_buffer);
      #endif

      while( (rc = read(fcb_index,D_buffer,D_INPUT_RECORD_SIZE) ) > 0 )
      {
         D_buffer(:D_HVARLEN:) = '\0';/* As 0=1st, it goes aft last */

         #if (NOT PROD_SYS)
            if (D_SWITCH(D2LOAD) || D_SWITCH(D5DEBUG) )
              printf("!!!!:%s:D_load(D):  Read {%s}.\n",id,D_buffer);
         #endif
         /*+-------------------------------------------------------+*/
         /*| If the setting in the datafile is the *character* zero|*/
         /*| or one, use the *numeric* value 0 or 1 to set it to.  |*/
         /*| If neither, complain and use 0.                       |*/
         /*+-------------------------------------------------------+*/
         if (D_buffer(:0:) == '1')
         {
            setting = TRUE;
            D_INT_SW(D_ok_use) = TRUE;
         }
         else if (D_buffer(:0:) == '0')
            setting = FALSE;
         else
         {
            printf("****:%s:D_load(E):  Bad value '%c'(0x%02X) for \
{%s} in file {%s}; assuming 0.\n",id,D_buffer(:0:),D_buffer(:0:),
               (D_buffer+1),datafile);
            setting = FALSE;
         }

         /*+-------------------------------------------------------+*/
         /*| If switch is previous assigned, warn the user if new  |*/
         /*| value is different.                                   |*/
         /*+-------------------------------------------------------+*/
         if ( D_exist( D_buffer+1,id) == TRUE )
         {
            old_setting = D_lookup(D_buffer+1,id);
            if ( old_setting != setting )
            {
               printf("****:%s:D_load(F):  Switch \"%s\" redefined \
from %s to %s.\n",id,D_buffer+1,
                  (old_setting ? "ON" : "OFF"),
                  (setting ? "ON" : "OFF"));
            }
         }

         D_setvar ( D_buffer+1 , setting, id );
      }
      close( fcb_index );
   }

   else
   {
      rc = -1;                      /* The open() failed            */
      #if (NOT PROD_SYS)
         printf("????:%s:D_load(G):  Could not open {%s}; rc=%d.\n",
            id,datafile,fcb_index);
      #endif
   }

   /*+----------------------------------------------------------------+*/
   /*| If we supposed to be debugging, see if we're supposed to be    |*/
   /*| debugging.                                                     |*/
   /*+----------------------------------------------------------------+*/
   #if (NOT PROD_SYS)

      D_INT_SW(D5DEBUG)  = D_lookup("5DEBUG"    ,"D_load(H)");
      D_INT_SW(D2END)    = D_lookup("2D_END"    ,"D_load(I)");
      D_INT_SW(D2EXIST)  = D_lookup("2D_EXIST"  ,"D_load(J)");
      D_INT_SW(D2HASHIT) = D_lookup("2D_HASHIT" ,"D_load(K)");
      D_INT_SW(D2HDUMP)  = D_lookup("2D_HDUMP"  ,"D_load(L)");
      D_INT_SW(D2HSET)   = D_lookup("2D_HSET"   ,"D_load(M)");
      D_INT_SW(D2INIT)   = D_lookup("2D_INIT"   ,"D_load(N)");
      D_INT_SW(D2LOAD)   = D_lookup("2D_LOAD"   ,"D_load(O)");
      D_INT_SW(D2LOOKUP) = D_lookup("2D_LOOKUP" ,"D_load(P)");
      D_INT_SW(D2OUTPUT) = D_lookup("2D_QUTPUT" ,"D_load(Q)");
      D_INT_SW(D2QUERY)  = D_lookup("2D_QUERY"  ,"D_load(R)");
      D_INT_SW(D2QUERIE) =(D_lookup("2D_QUERIE" ,"D_load(S)")
                        || D_lookup("2D_QUERIES","D_load(T)"));
      D_INT_SW(D2SETVAR) = D_lookup("2D_SETVAR" ,"D_load(U)");
      D_INT_SW(D2UNLOAD) = D_lookup("2D_UNLOAD" ,"D_load(V)");
      D_INT_SW(D2UTIL)   = D_lookup("2D_UTIL"   ,"D_load(W)");

      D_INT_SW(D9PSTAT1) = D_lookup("9PSTAT1"   ,"D_load(X)");
      D_INT_SW(D9PSTAT2) = D_lookup("9PSTAT2"   ,"D_load(Y)");

   #endif

   return (rc);
}
