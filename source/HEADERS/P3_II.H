/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     P3_II H                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    PPP                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Main internal header file for the Pre-Preprocessor.    |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     06_Aug_86   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Jan_89   K.J.Walsh   Added prototyping declarations.|*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef P3_II_H_INCLUDED
#define P3_II_H_INCLUDED

#define  MAXLEN      80          /* Max arg/param/symbol length        */
#define  MAXINPUT    800         /* Maximum input line length          */
#define  TABSIZE     40          /* Len of tables (max# of syms/params */
#define  TERMWIDTH   79          /* Expected with of screen            */
#define  FIRST       0           /* These 3 are used by p3g_line().    */
#define  REMAINING   1           /*   see the documentation in that    */
#define  ALL         2           /*       routine for all details.     */
#define  CMS         1           /* For features need on CMS only      */
#define  OPSYS       "CMS"       /* Incase we need to know             */

/* $EXTRACT $MAKER-PROTO */

int   p3badarg (  int,     char**,  int,     int               );
int   p3choose (  char*,   char*,   int,     int*,    char*    );
int   p3ckmac  (  char*,   char*,   int,     int               );
int   p3cludge (  char*,   char*                               );
int   p3cuser  (  byte*,   int,     byte                       );
int   p3disp   (  byte*,   ...                                 );
int   p3done   (  byte                                         );
int   p3dt     (  byte*,   int,     byte                       );
int   p3erinm  (  void                                         );
int   p3erins  (  void                                         );
int   p3execsf (  int,     char*,   char*                      );
int   p3fix    (  char[]                                     );
int   p3g_glob (  void                                         );
int   p3g_line (  char*,   FILE*,   int*,int                   );
int   p3g_mode (  char*                                        );
int   p3g_parms(  char[]                                     );
int   p3g_syms (  FILE*                                        );
int   p3h_pair (  char*,   char*,   char                       );
int   p3hilite (  int,     int,     char*,   char*             );
int   p3i_bsym (  char*,   char*                               );
int   p3insert (  char[],int,     FILE*                      );
int   p3iscpp  (  char*                                        );
int   p3iswhite(  char                                         );
int   p3itoa   (  int,     char[]                            );
int   p3lookup (  char[TABSIZE][MAXLEN], int,     char*    );
int   p3main   (  int,     char**                              );/*$MAIN*/
int   p3point  (  int,     char*,   char*                      );
int   p3proces (  char*,   char*,   char*                      );
int   p3qarg   (  char*,   char*,   int*                       );
int   p3rep    (  int,     int,     char[],char*             );
int   p3sarg   (  char*,   char*,   int*                       );
int   p3setlin (  char*,   int,     char[]                   );
int   p3scomp  (  char*                                        );
int   p3start  (  char[],int*,    char*                      );
int   p3symarg (  char*,   char*,   int*                       );
int   p3symfun (  char*,   char*,   int*,    char*             );
int   p3trim   (  char*                                        );
int   p3upper  (  char[]                                     );
int   p3wrapup (  int                                          );
int   p3xparam (  char[],int*,    char*                      );
int   p3yesno  (  char*                                        );

/* $ENDEXTRACT */

#define  FNAME_SIZE  9                 /* How long an file name is     */
#define  FTYPE_SIZE  9                 /* How long an file type is     */
#define  FMODE_SIZE  3                 /* How long an file mode is     */
#define  FDESC_SIZE  25                /* How long an file descriptor z*/

/* Global variables */

extern   byte  IN_FILE  [FDESC_SIZE];   /* Source file name          */
extern   byte  IN_NAME  [FNAME_SIZE];   /* Source name               */
extern   byte  MAC_FILE [FDESC_SIZE];   /* Macro being expanded      */
extern   byte  MAC_NAME [FNAME_SIZE];   /* Macro name                */

extern   int   CURLIN   ;                 /* Current line # in source  */
extern   int   ERRORS   ;                 /* Total # of errors so far  */
extern   int   GCOUNT   ;                 /* # of globals in table     */
extern   int   MACLIN   ;                 /* Current line # in macro   */
extern   int   MASTSYNC ;                 /* Do we override sync settin*/
extern   int   MASTWARN ;                 /* Do we override warn settin*/
extern   int   OUTLINES ;                 /* Counter of lines outputted*/
extern   int   PARCOUNT ;                 /* # of PARAMS in above      */
extern   int   SYMCOUNT ;                 /* # of SYMBOLS in above     */
extern   int   SYNC     ;                 /* Do we put in #lines?      */
extern   int   WARN     ;                 /* Supress warnings?         */

extern char  GLOBALS[TABSIZE][MAXLEN];/* Global symbolic variables */
extern char  GVALS[TABSIZE][MAXLEN];  /* Values assigned to globals*/
extern char  PARAMS[TABSIZE][MAXLEN]; /* Macro paramater table     */
extern char  SYMBOLS[TABSIZE][MAXLEN];/* Symbol names table        */

#ifdef CLK_TCK
   extern time_t DATETIME;             /* Used only with <time.h>      */
#endif

#endif
