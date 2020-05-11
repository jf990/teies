/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_START C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htrans(::) D_HTRANS IN/OUT Hash translation table.   |*/
/*| :GLOBAL.   D_htable(::) D_HTABLE IN/OUT Switch table.             |*/
/*| :GLOBAL.   D_ifno.      D_HINFO  IN/OUT Information switchs/stats.|*/
/*|                                                                   |*/
/*| :PARAM.    datafile     *char    IN     Switch data file.         |*/
/*| :PARAM.    id           *char    IN     Identification of caller. |*/
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
/*|            open(), read()                                         |*/
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
#include <file.h>
#include <stdlib.h>
#include <string.h>
#include "debug_i.h"

/*+-------------------------------------------------------------------+*/
/*|  GENERAL USER VARIABLES -- refer to DEBUG_X.H for more info.      |*/
/*+-------------------------------------------------------------------+*/
bool     D_bool              = 0     ; /* TEIES boolean                */
byte     D_byte              = 0     ; /* TEIES byte (character)       */
char     D_char              = 0     ; /* C-style character            */
byte    *D_cstr              = NULL  ; /* C-style string               */
int      D_int               = 0     ; /* C-style integer              */
sint     D_rc                = 0     ; /* (sint) return code           */
sint     D_sint              = 0     ; /* TEIES signed integer         */
byte    *D_str               = S_NULL; /* TEIES string (OBSOLETE)      */
tint     D_tint              = 0     ; /* TEIES unsigned integer       */

/*+-------------------------------------------------------------------+*/
/*|  SPECIAL USER VARIABLES -- refer to DEBUG_X.H for more info.      |*/
/*+-------------------------------------------------------------------+*/
tint     D_has               = 0     ; /* For D_STR PPPMAC             */
hword    D_lvl               = 0     ; /* DMS level (used by Ken only  */
byte    *D_pointer           = NULL  ; /* For D_STR PPPMAC             */
tint     D_used              = 0     ; /* For D_STR PPPMAC             */

/*+-------------------------------------------------------------------+*/
/*|  INTERNAL VARIABLES -- refer to DEBUG_X.H for more documentation. |*/
/*+-------------------------------------------------------------------+*/
byte    *D_a_str             = NULL  ; /* OBSOLETE                     */
sint     D_byrexx            = -1    ; /* OBSOLETE                     */
struct   D_HTABLE  *D_htable = NULL  ; /* Debugging switch table struct*/
int     *D_htrans            = NULL  ; /* Hash translation table       */
struct   D_INFO     D_info           ; /* Flags and stats structure    */
FILE    *D_io                = stdout; /* Output file--default screen  */
sint     D_nst               = -1    ; /* Routine nest (0 = in main()) */
byte    *D_outfile           = NULL  ; /* OBSOLETE                     */
byte    *D_outmode           = NULL  ; /* OBSOLETE                     */
sint   (*D_pr_d)()           = D_pr_e; /* Assume output error print fun*/
byte    *D_r_str             = NULL  ; /* OBSOLETE                     */
bool     D_tofile            = FALSE ; /* OBSOLETE                     */
bool     D_toscreen          = TRUE  ; /* OBSOLETE                     */

/*+-------------------------------------------------------------------+*/
/*|  Some PPC MACROs for use in this file.                            |*/
/*+-------------------------------------------------------------------+*/
#define BUFFERSIZE   80                /* File read buffer size        */
#define MALLOC(size) malloc(size)      /* Memory allocation routine    */

sint D_start(datafile,id)
char *datafile;                        /* Must have "(bin" in it!      */
char *id;
{
   int      fcb_index         = -1;    /* C file ctrl blk ndx from open*/
   int      rc                =  0;    /* RC from read from file to buf*/
   int      count             =  0;    /* Counter for use in loops     */
   char     *buffer           =  NULL; /* Fread input buffer           */
   bool     setting           =  FALSE;/* Extracted switch value       */

   /*+----------------------------------------------------------------+*/
   /*| If apparently not called before, do allocations.               |*/
   /*+----------------------------------------------------------------+*/
   if ( D_info.started == FALSE )            /* Not set?               */
   {
      #if D_DEBUGHASH
         if (D_info.D_start == TRUE || D_info.allon == TRUE)
            D_pr("!!!!:%s:D_start(A):  Allocating tables.\n");
      #endif

      /*+-------------------------------------------------------------+*/
      /*| Allocate the misc segments.                                 |*/
      /*+-------------------------------------------------------------+*/
      D_a_str   = MALLOC(D_SLEN);
      D_cstr    = MALLOC(D_SLEN);
      D_outfile = MALLOC(D_SLEN);
      D_outmode = MALLOC(D_SLEN);
      D_r_str   = MALLOC(D_SLEN);
      D_str     = MALLOC(D_SLEN);
      /*+-------------------------------------------------------------+*/
      /*| Allocate and initialize hash value lookup table             |*/
      /*+-------------------------------------------------------------+*/
      D_htrans = (int *) MALLOC( sizeof( int ) * 257 );

      for (count = 0; count < 256; count++)  /* Clear to all zeros     */
      {
         D_htrans(: count :) = 0;
      }

      D_htrans(:'0':) =  1;                  /* Set character values   */
      D_htrans(:'1':) =  2;
      D_htrans(:'2':) =  3;
      D_htrans(:'3':) =  4;
      D_htrans(:'4':) =  5;
      D_htrans(:'5':) =  6;
      D_htrans(:'6':) =  7;
      D_htrans(:'7':) =  8;
      D_htrans(:'8':) =  9;
      D_htrans(:'9':) = 10;
      D_htrans(:'_':) = 11;
      D_htrans(:'A':) = 12; D_htrans(:'a':) = 12;
      D_htrans(:'B':) = 13; D_htrans(:'b':) = 13;
      D_htrans(:'C':) = 14; D_htrans(:'c':) = 14;
      D_htrans(:'D':) = 15; D_htrans(:'d':) = 15;
      D_htrans(:'E':) = 16; D_htrans(:'e':) = 16;
      D_htrans(:'F':) = 17; D_htrans(:'f':) = 17;
      D_htrans(:'G':) = 18; D_htrans(:'g':) = 18;
      D_htrans(:'H':) = 19; D_htrans(:'h':) = 19;
      D_htrans(:'I':) = 20; D_htrans(:'i':) = 20;
      D_htrans(:'J':) = 21; D_htrans(:'j':) = 21;
      D_htrans(:'K':) = 22; D_htrans(:'k':) = 22;
      D_htrans(:'L':) = 23; D_htrans(:'l':) = 23;
      D_htrans(:'M':) = 24; D_htrans(:'m':) = 24;
      D_htrans(:'N':) = 25; D_htrans(:'n':) = 25;
      D_htrans(:'O':) = 26; D_htrans(:'o':) = 26;
      D_htrans(:'P':) = 27; D_htrans(:'p':) = 27;
      D_htrans(:'Q':) = 28; D_htrans(:'q':) = 28;
      D_htrans(:'R':) = 29; D_htrans(:'r':) = 29;
      D_htrans(:'S':) = 30; D_htrans(:'s':) = 30;
      D_htrans(:'T':) = 31; D_htrans(:'t':) = 31;
      D_htrans(:'U':) = 32; D_htrans(:'u':) = 32;
      D_htrans(:'V':) = 33; D_htrans(:'v':) = 33;
      D_htrans(:'W':) = 34; D_htrans(:'w':) = 34;
      D_htrans(:'X':) = 35; D_htrans(:'x':) = 35;
      D_htrans(:'Y':) = 36; D_htrans(:'y':) = 36;
      D_htrans(:'Z':) = 37; D_htrans(:'z':) = 37;

      /*+-------------------------------------------------------------+*/
      /*| Allocate and initialize hash switch table.                  |*/
      /*+-------------------------------------------------------------+*/
      D_htable = (int *) MALLOC( sizeof( struct D_HTABLE ) * 257);

      for (count = 0; count < 256; count++)   /*init the hash table    */
         D_htable(: count :).first = NULL;

      /*+-------------------------------------------------------------+*/
      /*| Initialize statistics counters and switches                 |*/
      /*+-------------------------------------------------------------+*/
      D_info.num_qry = 0;
      D_info.num_next = 0;
      D_info.max_chain = 0;
      D_info.D_pstat1 = FALSE;
      D_info.D_pstat2 = FALSE;

      /*+-------------------------------------------------------------+*/
      /*| Initialize debugging-debugging switches                     |*/
      /*+-------------------------------------------------------------+*/
      D_info.allon    = FALSE;               /* Master (on) switch     */
      D_info.D_flush  = FALSE;
      D_info.D_hashit = FALSE;
      D_info.D_hdump  = FALSE;
      D_info.D_hset   = FALSE;
      D_info.D_lookup = FALSE;
      D_info.D_query  = FALSE;
      D_info.D_querie = FALSE;
      D_info.D_start  = FALSE;         /* Really can't be turned on    */

      /*+-------------------------------------------------------------+*/
      /*| Last and hardly least turn on indicator that this has run.  |*/
      /*| and turn off the indicator that says something was used (for|*/
      /*| now).                                                       |*/
      /*+-------------------------------------------------------------+*/
      D_info.started  = TRUE;
      D_info.in_use    = FALSE;
   }

   /*+----------------------------------------------------------------+*/
   /*| If not given a datafile, notify and set RC.                    |*/
   /*+----------------------------------------------------------------+*/
   if (datafile == NIL)
   {
      D_pr("    :%s:D_start(B): No file given--bypassing open().\n",
         id);
      rc = 0;
   }
   else
   {
      /*+-------------------------------------------------------------+*/
      /*| Open datafile and read contents.                            |*/
      /*+-------------------------------------------------------------+*/
      fcb_index = open (datafile, O_RDONLY);
      if ( fcb_index != -1 )
      {
         #if D_DEBUGHASH
            if (D_info.D_start == TRUE || D_info.allon == TRUE)
               D_pr("!!!!:%s:D_start(C):  Successfully opened {%s}.\n",
                  id,datafile);
         #endif

         buffer = MALLOC(BUFFERSIZE+1);      /* Our input buffer       */
         while ( (rc = read( fcb_index, buffer, BUFFERSIZE)) > 0)
         {
            buffer(:D_HVARLEN:) = '\0';/* As 0=1st, it goes after last */

            #if D_DEBUGHASH
               if (D_info.D_start == TRUE || D_info.allon == TRUE)
                  D_pr("!!!!:%s:D_start(D):  Read {%s}.\n",id,buffer);
            #endif
            /*+-------------------------------------------------------+*/
            /*| If the setting in the datafile is the *character* zero|*/
            /*| or one, use the *numeric* value 0 or 1 to set it to.  |*/
            /*| If neither, complain and use 0.                       |*/
            /*+-------------------------------------------------------+*/
            if (buffer(:0:) == '1')
            {
               setting = TRUE;
               D_info.in_use = TRUE;
            }
            else if (buffer(:0:) == '0')
               setting = FALSE;
            else
            {
               D_pr("****:%s:D_start(E):  Bad value '%c'(0x%02X) for \
{%s} in file {%s}; assuming 0.\n",id,buffer(:0:),buffer(:0:),
                  (buffer+1),datafile);
               setting = FALSE;
            }
            D_setvar ( buffer+1 , setting, id );
         }
         close( fcb_index );
         free(buffer);
      }

      else
      {
         rc = -1;                      /* The open() failed            */
         #if D_DEBUGHASH
            D_pr("!!!!:%s:D_start(F):  Could not open {%d}; rc=%d.\n",
               id,datafile,fcb_index);
         #endif
      }

   }

   /*+----------------------------------------------------------------+*/
   /*| If we supposed to be debugging, see if we're supposed to be    |*/
   /*| debugging.                                                     |*/
   /*+----------------------------------------------------------------+*/
   #if D_DEBUGHASH

      D_pr("!!!!:%s:D_start(G):  DEBUG-debugging option compiled in.\n",
         id);

      D_info.allon    = D_lookup("D_ALLON", "D_start(G)");
      D_info.D_flush  = D_lookup("D_FLUSH" ,"D_start(H)");
      D_info.D_hashit = D_lookup("D_HASHIT","D_start(I)");
      D_info.D_hdump  = D_lookup("D_HDUMP" ,"D_start(J)");
      D_info.D_hset   = D_lookup("D_HSET"  ,"D_start(K)");
      D_info.D_lookup = D_lookup("D_LOOKUP","D_start(L)");
      D_info.D_query  = D_lookup("D_QUERY" ,"D_start(M)");
      D_info.D_start  = D_lookup("D_START" ,"D_start(N)");
      D_info.D_querie = ( D_lookup("D_QUERIE","D_start(O)")
                        || D_lookup("D_QUERIES","D_start(P)"));

   #endif

   /*+----------------------------------------------------------------+*/
   /*| Likewise, check to see if STATS should be taken.               |*/
   /*+----------------------------------------------------------------+*/
   #if D_DEBUGHASH

      D_pr("!!!!:%s:D_start(Q):  DEBUG-statistics option compiled in.\n",
         id);

      D_info.D_pstat1 = D_lookup("D_PSTAT1","D_start(R)");
      D_info.D_pstat2 = D_lookup("D_PSTAT2","D_start(S)");

   #endif

   return (rc);
}
