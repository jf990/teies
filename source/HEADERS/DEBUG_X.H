/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    debug_i.h                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Standardized Debugging header file for use in all      |*/
/*|            routines using SD; should only be included via         |*/
/*|            "debug.h".                                             |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    06_Oct_87   K.J.Walsh   Written                        |*/
/*|  :MODS.    16_Apr_87   K.J.Walsh   Documentation added            |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Moved #include of rexxdef.h to |*/
/*|                                    "debug.h"                      |*/
/*|  :MODS.    08_Dec_87   K.J.Walsh   Documented to 1.42 standards,  |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEBUG_X_H_INCLUDED
#define DEBUG_X_H_INCLUDED


/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  We hafta make sure                 "defs.h" is  #included to     |*/
/*|  avoid the generation of some misleading error messages:          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEFS_H_INCLUDED    /* Note:  "DEFS_H" is defined in "defs.h.l" */
#  error "DEFS.H" must be included before this header file.
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  GLOBAL DECLARATIONS (for use with REXX debugging options)        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#ifndef     D_OSIDEBUG
#  define   D_OSIDEBUG  0              /* Internal debugging           */
#endif

#ifndef     OSIDEBUG
#  define   OSIDEBUG 1                 /* Obsolete switch              */
#endif

#define     D_SLEN   200               /* Max len of string buffer     */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  SEMI-INTERNAL VARIABLES                                          |*/
/*|                                                                   |*/
/*|  These are used internally by standardized debugging.             |*/
/*|  Although changed by certain PPPMACs and interrogated by most,    |*/
/*|  altering them in not advised in most cases.                      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   sint     D_nst              ; /* Routine nest (0 = in main()) */
extern   sint   (*D_pr_d)()          ; /* Assume output error print fun*/

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  GENERAL USER VARIABLES                                           |*/
/*|                                                                   |*/
/*|  These are for use by macro writers for whatever is desired.      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   bool     D_bool             ; /* TEIES boolean                */
extern   byte     D_byte             ; /* TEIES byte (character)       */
extern   char     D_char             ; /* C-style character            */
extern   byte    *D_cstr             ; /* C-style string (len=D_SLEN)  */
extern   int      D_int              ; /* C-style integer              */
extern   sint     D_rc               ; /* (sint) return code           */
extern   sint     D_sint             ; /* TEIES signed integer         */
extern   tint     D_tint             ; /* TEIES unsigned integer       */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  SPECIAL USER VARIABLES                                           |*/
/*|                                                                   |*/
/*|  These are special purpose variables used by particular PPPMACs   |*/
/*|  {or programmers} do not alter any that are not yours.  You can   |*/
/*|  request to have entries added.                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   tint     D_has              ; /* For D_STR PPPMAC             */
extern   byte    *D_pointer          ; /* For D_STR PPPMAC             */
extern   tint     D_used             ; /* For D_STR PPPMAC             */

/* $EXTRACT $MAKER-PROTO */

/*+-------------------------------------------------------------------+*/
/*| End debugging -- deallocates tables used in dubugging.  ID is a   |*/
/*| null-terminated string indicating who called the function; it     |*/
/*| should have the format "routine{line}" inwhich "routine" is the   |*/
/*| routine calling and "line" is the line number on which the        |*/
/*| invocation occurs.  This is easly done in a macro by using the    |*/
/*| string "@_name{@curlin}", which is replaced with the correct      |*/
/*| values.                                                           |*/
/*+-------------------------------------------------------------------+*/
extern void D_end(byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Exist checks to see if the debugging variable indicated by the    |*/
/*| null-terminated string SWITCH exists.  If so, TRUE is returned,   |*/
/*| else FALSE.  See "D_end{}" for details regarding the ID variable. |*/
/*+-------------------------------------------------------------------+*/
extern bool D_exist(byte *SWITCH, byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Returns the hash-algorithm value of the null-terminated string    |*/
/*| SWITCH, which indicates it's location in the debugging table.     |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern hword D_hashit(byte *SWITCH, char *ID);

/*+-------------------------------------------------------------------+*/
/*| Displays the entire debugging table in use with some statistics.  |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_hdump(byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Adds and sets {or resets uf existing} the debugging switch        |*/
/*| indicated by both the null-terminated string SWITCH and its       |*/
/*| corresponding hashed representation HASH.  The boolean SETTING    |*/
/*| is set for the indicated varable.                                 |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_hset(byte *SWITCH, hword HASH, bool SETTING, char *ID);

/*+-------------------------------------------------------------------+*/
/*| Initialize debugging:  Initializes tables and file pointers for   |*/
/*| the debugging system; need be called only once.  The              |*/
/*| null-terminated strings INFILE and OUTFILE indicate the files     |*/
/*| from which settings are to be read and where output might be      |*/
/*| sent.                                                             |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern sint D_init(byte *INFILE, char *OUTFILE, char *ID);

/*+-------------------------------------------------------------------+*/
/*| Initialize debugging memory:  Allocates all debugging tables and  |*/
/*| is the resolution point for all debugging globals.  Does nothing  |*/
/*| after being called once unless after a D_unload{}.                |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_init_m(byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Load table:  Reads the debugging hashdata file named by the       |*/
/*| null-terminated string INFILE into memory.                        |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern sint D_load(byte *INFILE, char *ID);

/*+-------------------------------------------------------------------+*/
/*| Look up switch:  Returns TRUE if the debugging variable named by  |*/
/*| the null-terminated string SWITCH is set on; returns FALSE if the |*/
/*| variable is set off or if it does not exist.                      |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern bool D_lookup(byte *SWITCH, char *ID);

/*+-------------------------------------------------------------------+*/
/*| Memory dump:  Displays memory pointed to by FROM for length LENGTH|*/
/*| {with certain overriding limits}.  Each line of the dump is       |*/
/*| preeceded by the data given in the null-terminated string PREFIX. |*/
/*+-------------------------------------------------------------------+*/
extern sint D_mdump(byte *PREFIX, byte *FROM, tint LEN);

/*+-------------------------------------------------------------------+*/
/*| End output to file:  {DO NOT USE}  This routine will end output   |*/
/*| as started by D_output{} and redirect output as possible.         |*/
/*| This routine is partly implemented and the FLAG does nothing.     |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_outend(sint FLAG, byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Initiate output to file:  Internally sets up debugging for output |*/
/*| to the file indicated by the null-terminated string OUTFILE.      |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern sint D_output(byte *OUTFILE, byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Query switch:  Returns TRUE if the debugging switch               |*/
/*| indicated by both the null-terminated string SWITCH and its       |*/
/*| corresponding hashed representation HASH is set on.  If set off   |*/
/*| or does not exist, FALSE is returned.                             |*/
/*| This function will immediately return FALSE if an internal switch |*/
/*| has been set by the D_util{} function.                            |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern bool D_query(byte *SWITCH, hword HASH, byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Query switches:  Returns TRUE if one of the debugging switches    |*/
/*| indicated by both the null-terminated strings SW1, SW2, SW3, and  |*/
/*| SW4 and the corresponding hashed representations H1, H2, H3, and  |*/
/*| H4 is set on.  If none are set and/or exist, then "9ALL" is       |*/
/*| checked; if it is set TRUE is returned; if it is not set or does  |*/
/*| exist, then FALSE is returned.                                    |*/
/*| This function will immediately return FALSE if an internal switch |*/
/*| has been set by the D_util{} function.                            |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern bool D_queries(byte *SW1, hword H1, byte *SW2, hword H2,
                      byte *SW3, hword H3, byte *SW4, hword H4,
                      byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Adds and sets {or resets if existing} the debugging switch        |*/
/*| indicated by the null-terminated string SWITCH.  The boolean      |*/
/*| SETTING is used as the new setting.                               |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_setvar(byte *SWITCH, bool SETTING, byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Unload debugging tables:  Deallocates all debugging tables.       |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern void D_unload(byte *ID);

/*+-------------------------------------------------------------------+*/
/*| Debugging utility function:  Performs various special purpose     |*/
/*| functions for debugging as per the FLAG.  Depending on the        |*/
/*| function requested, the variables BOOLEAN, SINT, and CSTR might   |*/
/*| also be used {but should always be provided}.                     |*/
/*| See "D_end{}" for details regarding the ID variable.              |*/
/*+-------------------------------------------------------------------+*/
extern sint D_util(sint FLAG, bool BOOLEAN, sint SINT, char *CSTR,
                   byte *ID);

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Rather than have a routine that does if/else/etc tests to see how|*/
/*|  to print every time it is called, the #D_main PPPMAC sets a      |*/
/*|  dummy function {D_pr_d} to point the the real routine to be used |*/
/*|  {any of D_pr_s, _f, _sf }.  D_pr_d is initialized to call D_pr_e |*/
/*|  {which reports an error and sets to D_pr_s} if #D_main   is      |*/
/*|  missing.                                                         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern  sint   D_pr_e(byte *FMT,...);  /* Default D_pr--an error       */
extern  sint   D_pr_s(byte *FMT,...);  /* D_pr to screen only          */
extern  sint   D_pr_f(byte *FMT,...);  /* D_pr to file only            */
extern  sint   D_pr_sf(byte *FMT,...); /* D_pr to screen & file        */
extern  sint   D_pr_ss(byte *FMT,...); /* Force 2 screen, maybe 2 file */
extern  sint   D_pr_nil(byte *FMT,...);/* Discard output {goes nowhere}*/

/* $ENDEXTRACT */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Because most people and macros use "D_pr.c" in PPPMAC calls to   |*/
/*|  print stuff, a #define of (*D_pr_d) to D_pr is provided for      |*/
/*|  back convience and compatability.                                |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   sint   (*D_pr_d)  ()       ;  /* Dummy printing function      */
#define  D_pr     (*D_pr_d)

#define  D_PR     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  D_Pr     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  D_pR     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  D_PR     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  d_PR     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  d_Pr     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  d_pR     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */
#define  d_pr     WRONG_CASE_D_PR_CALL /* To catch "mispellings"       */

/*+-------------------------------------------------------------------+*/
/*| Following are parameters for D_util() function.                   |*/
/*+-------------------------------------------------------------------+*/
#define  D_UTIL_SET_MASTER          1  /* Used 2nd/bool to switch mast */
#define  D_UTIL_TRACEBACK           2  /* Used 3rd/sint for rcode      */
#define  D_UTIL_READY               3  /* Ignores params/sint rc=0 OK  */

/*+-------------------------------------------------------------------+*/
/*| This handy deal is used to display a boolean as TRUE or FALSE     |*/
/*| strings.  It takes a boolean expression as an argument.           |*/
/*+-------------------------------------------------------------------+*/
#define  D_BOOLSTR(exp)  (((exp) == TRUE) ? "TRUE" : "FALSE" )

#endif
