/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     DEBUG_II H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Internal DEBUGGING header file, used mostly for support|*/
/*|            of Hash debugging system.                              |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   Dependant on <stdio.h>                                 |*/
/*|                                                                   |*/
/*| :MODS.     16_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEBUG_II_H_INCLUDED
#define DEBUG_II_H_INCLUDED

/*+-------------------------------------------------------------------+*/
/*| D_DEBUGHASH controlls debugging messaging for just the hash       |*/
/*| controlled portion of the DEBUG level.                            |*/
/*+-------------------------------------------------------------------+*/
#define  D_DEBUGHASH TRUE

/*+-------------------------------------------------------------------+*/
/*| D_HVARLEN defines the maximum variable size considered by the     |*/
/*| Hashtable debugging.  Additional characters are ignored.          |*/
/*+-------------------------------------------------------------------+*/
#define  D_HVARLEN 16

/*+-------------------------------------------------------------------+*/
/*| The input file to D_LOAD must have fixed records and be of the    |*/
/*| following length.                                                 |*/
/*+-------------------------------------------------------------------+*/
#define D_INPUT_RECORD_SIZE 80

/*+-------------------------------------------------------------------+*/
/*| Internal-use-only debugging buffer length.                        |*/
/*+-------------------------------------------------------------------+*/
#define D_BUFFER_SIZE   100

/*+-------------------------------------------------------------------+*/
/*| Because of usage in D_load(), D_HVARLEN must be less than         |*/
/*| D_BUFFER_SIZE, and D_INPUT_RECORD_SIZE must be between the two.   |*/
/*| We'll do a quick check here to make sure there are no mysteries   |*/
/*| later on.                                                         |*/
/*+-------------------------------------------------------------------+*/
#if D_BUFFER_SIZE <= D_INPUT_RECORD_SIZE
   D_BUFFER_SIZE_must_be_greater_than_D_INPUT_RECORD_SIZE;
#endif
#if D_INPUT_RECORD_SIZE <= D_HVARLEN
   D_INPUT_RECORD_SIZE_must_be_greater_than_D_HVARLEN;
#endif

/*+-------------------------------------------------------------------+*/
/*| D_ALLHVAL is the hashed value given the string "D9ALL", for use   |*/
/*| in D_queries().  Note that if the hashing algorithm changes, then |*/
/*| this value must also be changed.                                  |*/
/*| This value is for "D9ALL" after the "D" has been stripped!        |*/
/*+-------------------------------------------------------------------+*/
#define  D_ALLHVAL 197

/*+-------------------------------------------------------------------+*/
/*| Within the DEBUG routines we use the same memory allocation       |*/
/*| routine whenever possible:                                        |*/
/*+-------------------------------------------------------------------+*/
#define  D_ALLOCATE(size)  malloc(size)
#define  D_DEALLOCATE(ptr) free(ptr)

/*+-------------------------------------------------------------------+*/
/*| These are things that we append to file descriptors to perform    |*/
/*| open on them.  Note the leading blank.                            |*/
/*+-------------------------------------------------------------------+*/
#define  D_OPEN_INPUT_SUFFIX   " ( bin"
#define  D_OPEN_OUTPUT_SUFFIX  " ( text lrecl 122 recfm v"
#define  D_OPEN_OUTPUT_PARAM   "w"

/*+-------------------------------------------------------------------+*/
/*| The following declares the hash table structure to be used.       |*/
/*| Note element "first", the anchor of the structure for searches.   |*/
/*| We define a table of 256+1 elements (+1 to be safe) in D_start(). |*/
/*+-------------------------------------------------------------------+*/
struct D_HTABLE
{
   struct   D_htentry   *first;
} ;

/*+-------------------------------------------------------------------+*/
/*| An element of the hash table consists of string of the name,      |*/
/*| as several names may resolve to the same hash value; a hword      |*/
/*| value; and a pointer to the next varable with the same has value. |*/
/*| We add 2 to D_HVARLEN for (1) a "\0", added in D_start() and (2)  |*/
/*| for safety.                                                       |*/
/*+-------------------------------------------------------------------+*/
struct D_htentry
{
   char      name[D_HVARLEN+2];
   bool      value;
   struct    D_htentry   *next;
};

#if 0                                  /* Discontinuted 7 Aug 88       */
/*+-------------------------------------------------------------------+*/
/*| The following structure is used to control debugging of the       |*/
/*| debugging, to calculate various statistics, and so on.            |*/
/*| In most cases, the code that uses them is #def'd out or turned    |*/
/*| off.                                                              |*/
/*+-------------------------------------------------------------------+*/
struct D_INFO
{
   /*+----------------------------------------------------------------+*/
   /*| These are used for gathering statistics                        |*/
   /*+----------------------------------------------------------------+*/
   tint D_s_qrys;                      /* Count of queries done        */
   tint D_s_nxts;                      /* Count of next's thru buckets */
   tint D_s_mnxt;                      /* Max count of next's in a qry */
   bool D_pstat1;                      /* Show level 1 stats (D_flush)?*/
   bool D_pstat2;                      /* Show level 1 stats (D_query)?*/

   /*+----------------------------------------------------------------+*/
   /*| These are used for debugging the debugging.                    |*/
   /*+----------------------------------------------------------------+*/
   bool allon;                         /* Master (on) switch.          */
   bool D_end;
   bool D_exist;
   bool D_hashit;
   bool D_hdump;                       /* Not used                     */
   bool D_hset;
   bool D_init;
   bool D_init_m;
   bool D_load;
   bool D_lookup;
   bool D_outend;
   bool D_output;
   bool D_query;
   bool D_querie;
   bool D_setvar;
   bool D_unload;
   bool D_util;

   /*+----------------------------------------------------------------+*/
   /*| These are always used to see if D_start() was called and to see|*/
   /*| if table looks are being done.                                 |*/
   /*+----------------------------------------------------------------+*/
   bool D_ok_run;
   bool D_ok_use;
};
#endif
/*+-------------------------------------------------------------------+*/
/*| Each of the debugging switches or stats variables are no longer in|*/
/*| a structure so that there are no problems when one is used but the|*/
/*| structure is unallocated.                                         |*/
/*+-------------------------------------------------------------------+*/
extern  tint D_s_qrys;                 /* Count of queries done        */
extern  tint D_s_nxts;                 /* Count of next's thru buckets */
extern  tint D_s_mnxt;                 /* Max count of next's in a qry */
extern  bool D9PSTAT1;                 /* Show level 1 stats (D_flush)?*/
extern  bool D9PSTAT2;                 /* Show level 1 stats (D_query)?*/

extern  bool D5DEBUG;                  /* Master (on) switch.          */
extern  bool D2END;
extern  bool D2EXIST;
extern  bool D2HASHIT;
extern  bool D2HDUMP;                  /* Not used                     */
extern  bool D2HSET;
extern  bool D2INIT;
extern  bool D2INIT_M;
extern  bool D2LOAD;
extern  bool D2LOOKUP;
extern  bool D2OUTEND;
extern  bool D2OUTPUT;
extern  bool D2QUERY;
extern  bool D2QUERIE;
extern  bool D2SETVAR;
extern  bool D2UNLOAD;
extern  bool D2UTIL;

extern  bool D_ok_run;
extern  bool D_ok_use;

/*+-------------------------------------------------------------------+*/
/*|  These macro are provided so that the means of accessing the      |*/
/*|  debugging switches and stats variables in the debugging routines |*/
/*|  in such a way that the means can be changed without altering all |*/
/*|  the routines again.                                              |*/
/*+-------------------------------------------------------------------+*/
#define D_INT_SW(var)  (var)                 /* An internal switch     */
#define D_STATS(var)   (var)                 /* An internal stats var  */
#define D_SWITCH(var)  (D_INT_SW(var) == TRUE)   /* Query of a switch  */

/*+-------------------------------------------------------------------+*/
/*|  STRICTLY INTERNAL VARIABLES                                      |*/
/*+-------------------------------------------------------------------+*/
extern   char    *D_buffer           ; /* Internal buffer for stuff    */
extern   struct   D_HTABLE  *D_htable; /* Debugging switch table struct*/
extern   int     *D_htrans           ; /* Hash translation table       */
extern   FILE    *D_io               ; /* Output file--default screen  */
#if 0
extern   struct   D_INFO     D_info  ; /* Flags and stats structure    */
#endif

#endif
