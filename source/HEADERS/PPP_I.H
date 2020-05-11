/*
Author:       Kevin J. Walsh
Date:         6 August 1986
Name:         ppp (pre-preprocessor)
Globals:      None
Description:  Refer to PPP DOC for compete details of operaton.
              Basically, accepts a file descriptor as an argument;
              if the file type is missing, it is assumed to be "C";
              if the file mode is missing, it is assumed to be "*".
              Ordinarily, lines are just copied from the source into
              the file "filename PPP A", except for "PPP macros".
              The file is scanned for #xxx statements, where "xxx"
              is NOT a standard C pre-processor command (ie, not
              a "#if", etc.  The file "xxx PPPMAC *" is opened, and
              various operations are performed on the contents thereof
              and the results inserted into the output file.
*/

extern close()  ;
extern exit()   ;
extern fgets()  ;
extern fprintf();
extern fputs()  ;
extern open()   ;
extern puts()   ;
extern printf() ;
extern sprintf();

#define void      int         /* 'Cause IBM don't like void.        */
#define TRUE      1           /* "boolean" true                     */
#define FALSE     0           /* "boolean" false                    */
#define MAXLEN    80          /* Maximum  arg/param/symbol length   */
#define MAXINPUT  800         /* Maximum input line length          */
#define DEBUG     0           /* ZERO is off, ONE is on             */
#define TABSIZE   25          /* Len of tables (max# of syms/params */
#define TERMWIDTH 79          /* Expected with of screen            */
#define FIRST     0           /* These 3 are used by getline(). See */
#define REMAINING 1           /*       the documentation in that    */
#define ALL       2           /*       routine for all details.     */
#define CMS       1           /* For features need on CMS only      */
#define OPSYS     "CMS"       /* Incase we need to know             */

/* $extract */
int  built_in ();
int  getparams();
int  getsyms  ();
int  iscpp    ();
int  iswhite  ();
int  lookup   ();
int  no_more  ();
int  ppp2     ();     /* $main */
int  replace  ();
int  start    ();
int  symfun   ();
int  trim     ();
int  xparam   ();
int  yesno    ();
void badarg   ();
void checkmac ();
void display  ();
void errinm   ();
void errins   ();
void execsf   ();
void fix      ();
void getglobals();
void getline  ();
void getmode  ();
void hashpair ();
void hilite   ();
void insert   ();
void itoa     ();
void m_setlin ();
void n_cludge ();
void pointout ();
void process  ();
void qarg     ();
void sarg     ();
void symarg   ();
void upper    ();
void whichsym ();
void wrapup   ();
/* $endextract */

typedef char fname [9];     /* File name storage.                 */
typedef char ftype [9];     /* File type storage.                 */
typedef char fmode [3];     /* File mode storage.                 */
typedef char filed [25];    /* File descriptor storage.           */

/* Global variables */

extern char  SYMBOLS[TABSIZE][MAXLEN];/* Symbol names table        */
extern int   SYMCOUNT ;                   /* # of SYMBOLS in above     */
extern char  PARAMS[TABSIZE][MAXLEN]; /* Macro paramater table     */
extern int   PARCOUNT ;                   /* # of PARAMS in above      */
extern char  GLOBALS[TABSIZE][MAXLEN];/* Global symbolic variables */
extern char  GVALS[TABSIZE][MAXLEN];  /* Values assigned to globals*/
extern int   GCOUNT   ;                   /* # of globals in table     */
extern int   ERRORS   ;                   /* Total # of errors so far  */
extern int   OUTLINES ;                   /* Counter of lines outputted*/
extern int   CURLIN   ;                   /* Current line # in source  */
extern int   MACLIN   ;                   /* Current line # in macro   */
extern int   SYNC     ;                   /* Do we put in #lines?      */
extern int   MASTSYNC ;                   /* Do we override sync settin*/
extern filed IN_FILE;                     /* Source file name          */
extern filed MAC_FILE;                    /* Macro being expanded      */
extern fname IN_NAME;                     /* Source name               */
extern fname MAC_NAME;                    /* Macro name                */
