/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_INIT_M C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   All DEBUG globals are defined in this routine.         |*/
/*|                                                                   |*/
/*| :PARAM.    id           *byte    IN     Identification of caller. |*/
/*|                                                                   |*/
/*| :PURPOSE.  All debugging variables are definded in this routine.  |*/
/*|                                                                   |*/
/*|            Upon being called the first time, it will allocate the |*/
/*|            various structures used by the Standardized Debugging, |*/
/*|            includeing those not used by the hashing debugging.    |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.    D_ALLOCATE                                             |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   The debugging hash table is                            |*/
/*|            allocated by D_load() and deallocated by D_unload().   |*/
/*|                                                                   |*/
/*| :MODS.     18_Jul_88   K.J.Walsh   Implemented                    |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include <string.h>
#include "debug_i.h"

#ifndef D_INIT_M_DEFAULT
   #define   D_INIT_M_DEFAULT FALSE    /* Should usually be FALSE      */
#endif

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
tint     D_tint              = 0     ; /* TEIES unsigned integer       */

/*+-------------------------------------------------------------------+*/
/*|  SPECIAL USER VARIABLES -- refer to DEBUG_X.H for more info.      |*/
/*+-------------------------------------------------------------------+*/
tint     D_has               = 0     ; /* For D_STR PPPMAC             */
byte    *D_pointer           = NULL  ; /* For D_STR PPPMAC             */
tint     D_used              = 0     ; /* For D_STR PPPMAC             */

/*+-------------------------------------------------------------------+*/
/*|  STRICTLY INTERNAL VARIABLES -- see DEBUG_I.H                     |*/
/*+-------------------------------------------------------------------+*/
tint     D_s_qrys            = 0     ; /* Count of queries done        */
tint     D_s_nxts            = 0     ; /* Count of next's thru buckets */
tint     D_s_mnxt            = 0     ; /* Max count of next's in a qry */
bool     D9PSTAT1            = FALSE ; /* Show level 1 stats (D_flush)?*/
bool     D9PSTAT2            = FALSE ; /* Show level 1 stats (D_query)?*/
bool     D5DEBUG             = FALSE ; /* Master (on) switch.          */
bool     D2END               = FALSE ;
bool     D2EXIST             = FALSE ;
bool     D2HASHIT            = FALSE ;
bool     D2HDUMP             = FALSE ; /* Not used                     */
bool     D2HSET              = FALSE ;
bool     D2INIT              = FALSE ;
bool     D2INIT_M            = FALSE ;
bool     D2LOAD              = FALSE ;
bool     D2LOOKUP            = FALSE ;
bool     D2OUTEND            = FALSE ;
bool     D2OUTPUT            = FALSE ;
bool     D2QUERY             = FALSE ;
bool     D2QUERIE            = FALSE ;
bool     D2SETVAR            = FALSE ;
bool     D2UNLOAD            = FALSE ;
bool     D2UTIL              = FALSE ;
bool     D_ok_run            = FALSE ;
bool     D_ok_use            = FALSE ;
byte    *D_buffer            = NULL  ; /* Internal buffer for stuff    */
struct   D_HTABLE  *D_htable = NULL  ; /* Debugging switch table struct*/
int     *D_htrans            = NULL  ; /* Hash translation table       */
FILE    *D_io                = NULL  ; /* Output file--default screen  */
#if 0                                  /* Discontinuted 7 Aug 88       */
struct   D_INFO     D_info           ; /* Flags and stats structure    */
#endif

/*+-------------------------------------------------------------------+*/
/*|  SEMI-INTERNAL VARIABLES -- see DEBUG_X.H                         |*/
/*+-------------------------------------------------------------------+*/
sint     D_nst               = -1    ; /* Routine nest (0 = in main()) */
sint   (*D_pr_d)()           = D_pr_e; /* Assume output error print fun*/

/*+-------------------------------------------------------------------+*/
/*|  End of definitions--routine follows.                             |*/
/*+-------------------------------------------------------------------+*/
void D_init_m(id)
byte *id;
{
   sint  count =  0  ;                 /* Counter for initilizing      */

   /*+----------------------------------------------------------------+*/
   /*| If apparently not called before, do allocations.               |*/
   /*+----------------------------------------------------------------+*/
   if ( D_SWITCH(D_ok_run) == FALSE )         /* Not set?               */
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2INIT_M) || D_SWITCH(D5DEBUG))
            D_pr("!!!!:%s:D_init_m(A):  Allocating tables and bufs.\n");
      #endif

      /*+-------------------------------------------------------------+*/
      /*| Allocate the misc segments.                                 |*/
      /*+-------------------------------------------------------------+*/
      D_cstr    = D_ALLOCATE(D_SLEN);
      D_buffer  = D_ALLOCATE(D_BUFFER_SIZE);

      /*+-------------------------------------------------------------+*/
      /*| Allocate and initialize hash value lookup table             |*/
      /*+-------------------------------------------------------------+*/
      D_htrans = (int *) D_ALLOCATE( sizeof( int ) * 258 );

      for (count = 0; count < 257; count++)  /* Clear to all zeros     */
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
      /*| Initialize statistics counters and switches                 |*/
      /*+-------------------------------------------------------------+*/


      D_STATS(D_s_qrys) = 0;
      D_STATS(D_s_nxts) = 0;
      D_STATS(D_s_mnxt) = 0;
      D_INT_SW(D9PSTAT1) = D_INIT_M_DEFAULT;
      D_INT_SW(D9PSTAT2) = D_INIT_M_DEFAULT;

      /*+-------------------------------------------------------------+*/
      /*| Initialize debugging-debugging switches                     |*/
      /*+-------------------------------------------------------------+*/
      D_INT_SW(D5DEBUG)  = D_INIT_M_DEFAULT; /* Master (on) switch     */
      D_INT_SW(D2END)    = D_INIT_M_DEFAULT;
      D_INT_SW(D2EXIST)  = D_INIT_M_DEFAULT;
      D_INT_SW(D2HASHIT) = D_INIT_M_DEFAULT;
      D_INT_SW(D2HDUMP)  = D_INIT_M_DEFAULT;
      D_INT_SW(D2HSET)   = D_INIT_M_DEFAULT;
      D_INT_SW(D2INIT)   = D_INIT_M_DEFAULT;
      D_INT_SW(D2INIT_M) = D_INIT_M_DEFAULT;
      D_INT_SW(D2LOAD)   = D_INIT_M_DEFAULT;
      D_INT_SW(D2LOOKUP) = D_INIT_M_DEFAULT;
      D_INT_SW(D2OUTEND) = D_INIT_M_DEFAULT;
      D_INT_SW(D2OUTPUT) = D_INIT_M_DEFAULT;
      D_INT_SW(D2QUERY)  = D_INIT_M_DEFAULT;
      D_INT_SW(D2QUERIE) = D_INIT_M_DEFAULT;
      D_INT_SW(D2SETVAR) = D_INIT_M_DEFAULT;
      D_INT_SW(D2UNLOAD) = D_INIT_M_DEFAULT;
      D_INT_SW(D2UTIL)   = D_INIT_M_DEFAULT;

      /*+-------------------------------------------------------------+*/
      /*| Last and hardly least turn on indicator that this has run.  |*/
      /*| and turn off the indicator that says something was used (for|*/
      /*| now).                                                       |*/
      /*+-------------------------------------------------------------+*/
      D_INT_SW(D_ok_run) = TRUE;
      D_INT_SW(D_ok_use) = FALSE;
   }
   else
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2INIT_M) || D_SWITCH(D5DEBUG))
           D_pr("!!!!:%s:D_init_m(B):  Already loaded--request ignored.\
            \n",id);
      #endif
   }

}
