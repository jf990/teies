/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    defs.h                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   MAIN                                                   |*/
/*|                                                                   |*/
/*|  :GLOBAL.  DEFS_H      PPCvar   IN/OUT   Defined by this file and |*/
/*|                                          checked by this file so  |*/
/*|                                          that it can accidentally |*/
/*|                                          be #included more than   |*/
/*|                                          once without harm; also  |*/
/*|                                          used by other files      |*/
/*|                                          requiring defs.h.        |*/
/*|  :GLOBAL.  bool        PPCvar   OUTPUT   Synonym for "boolean"    |*/
/*|  :GLOBAL.  NOT         PPCvar   OUTPUT   Synonym for logical "not"|*/
/*|                                          operator "!"             |*/
/*|  :GLOBAL.  PROD_SYS    PPCvar   IN/OUT   Boolean switch indicating|*/
/*|                                          if a PRODUCTION system   |*/
/*|                                          is to be compiled.       |*/
/*|  :GLOBAL.  SYS_TYPE    PPCvar   IN       System OS system type to |*/
/*|                                          be compiled (for adjust- |*/
/*|                                          ments based on system    |*/
/*|                                          dependancies).  MUST BE  |*/
/*|                                          SPECIFIED ON COMPILER    |*/
/*|                                          INVOCATION--NOT DEFAULT. |*/
/*|  :GLOBAL.  IBMCMS      PPCvar   OUT      Indicates IBM/CMS system.|*/
/*|  :GLOBAL.  DOS         PPCvar   OUT      Indicates PC based DOS.  |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Contains all TYPEDEF's and DEFINE's that provide types |*/
/*|            and constants that are to be used to maintain          |*/
/*|            portability.                                           |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    06_Aug_86   N.R.Voronka Written                        |*/
/*|  :MODS.    07_Aug_86   K.D.Ng      Moved to L disk.               |*/
/*|  :MODS.    08_Aug_86   J.L.Foster  _                              |*/
/*|  :MODS.    30_Sep_86   J.L.Foster  _                              |*/
/*|  :MODS.    02_Oct_86   K.J.Walsh   Added DEFS_H #define           |*/
/*|  :MODS.    01_Feb_87   N.R.Voronka _                              |*/
/*|  :MODS.    01_Feb_87   K.J.Walsh   Changed "s_extern.h" to "str.h"|*/
/*|  :MODS.    16_Apr_87   K.J.Walsh   Added DEFS_H checking          |*/
/*|  :MODS.    07_Jul_87   K.J.Walsh   Added bool, PROD_SYS, and NOT  |*/
/*|  :MODS.    06_Jun_88   G.S.Johar   Added SYS_etc... info. also,   |*/
/*|                                    null and lint were defined.    |*/
/*|  :MODS.    03_Dec_88   G.S.Johar   Moved string definition from   |*/
/*|                                    STR_COMM H                     |*/
/*|  :MODS.    24_Jan_89   G.S.Johar   Moved datetime definition from |*/
/*|                                    OSK_X.H                        |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEFS_H_INCLUDED
#  define DEFS_H_INCLUDED

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| SYS_xxx are used to determine what type of system, OS and compiler|*/
/*| being used.  Overlapping system types should be defined           |*/
/*| seperately and the overlap reflected in source code.              |*/
/*|                                                                   |*/
/*| The various types of systems are also defined below.              |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
                                       /* IBM SP/CMS operating system  */
#define IBM4361   1                    /* Machine name                 */
#define ARCH360   10                   /* Architecture                 */
                                       /*      Operating Systems       */
#define CMS40     11                   /* CMS rls 4.0                  */
#define CMS50     12                   /* CMS rls 5.0                  */
                                       /*         Compilers            */
#define WATC13    101                  /* Waterloo C rls 1.3           */
#define WATC30    102                  /* Waterloo C rls 3.0           */

                                       /* IBM and compatibles          */
#define IBMPC     2                    /* Machine name                 */
#define ARCH8086  20                   /* Intel 8086 architecture      */
                                       /*      Operating Systems       */
#define MSDOS20   21                   /* MicroSoft rls 2.0            */
#define MSDOS30   22                   /* MicroSoft rls 3.0            */
#define MSDOS33   23                   /* MicroSoft rls 3.3            */
#define PCDOS20   31                   /* IBM rls 2.0                  */
#define PCDOS30   32                   /* IBM rls 3.0                  */
                                       /*         Compilers            */
#define TURBOC15  201                  /* Borland Turbo C rls 1.5      */
#define MSC40     211                  /* MicroSoft C rls 4.0          */
#define MSC51     212                  /* MicroSoft C rls 5.0          */
#define IBMC10    221                  /* IBM C rls 1.0                */
#define LATC203   231                  /* Lattice C 2.03               */

                                       /* System variables             */
#define SYS_NAME IBM4361               /* System hardware name         */
#define SYS_TYPE ARCH360               /* Hardware architecture type   */
#define SYS_OS   CMS40                 /* Operating system and version */
#define SYS_C    WATC30                /* Compiler and version         */

/*+-------------------------------------------------------------------+*/
/*| Datatype declarations follow:                                     |*/
/*+-------------------------------------------------------------------+*/
   typedef  unsigned char        byte   ; /*  8 bit   type             */
   typedef  unsigned short int   hword  ; /* 16 bit   type             */
   typedef  unsigned long  int   tint   ; /* 32 bit   type             */
   typedef                 int   sint   ; /* Default  integers         */
   typedef           long  int   lint   ; /* Signed 32 bit integer     */
   typedef           float       real   ; /* Real var type             */
   typedef           short int   boolean; /* True or False?            */
#if SYS_C == WATC13
   typedef           short int   void   ; /* No true void on IBM       */
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Basic string structure... str.h required for further use of       |*/
/*| STR functions.                                                    |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
struct stringmap
{
   hword    lused    ;                 /* 1st bit reserved; remaining  */
                                       /*    15 indicate length used   */
   hword    lalloc   ;                 /* Allocation size              */
   byte     begin    ;                 /* Start of data                */
} ;

typedef  struct   stringmap  *string;
typedef  string *             string_vector;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Basic date/time structure... osk_x.h required for furter use of   |*/
/*| D/T functions or D/T datatypes.                                   |*/
/*|                                                                   |*/
/*| The fraction of seconds defined here provides 2 digits of         |*/
/*| accuracy; we can have up to 6 digits by just changing a couple of |*/
/*| instructions in todclk routine.                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern struct tdttm
{
   byte     century  ;
   byte     year     ;
   byte     month    ;
   byte     day      ;
   byte     hour     ;
   byte     minute   ;
   byte     second   ;
   byte     fract    ;
};

typedef  struct   tdttm    datetime;

#endif

/*+-------------------------------------------------------------------+*/
/*| PPC variables for general use:                                    |*/
/*+-------------------------------------------------------------------+*/
#  define  bool     boolean               /* A synonym for "boolean"   */
#  define  NOT      !                     /* A synonym for logical "!" */

#if SYS_TYPE == ARCH8086
#  define  NIL   0L
#  define  NULL  0L
#else
#  define  NIL   0
#  define  NULL  0
#endif

#define MINUS_ONE_PTR ( 0xFFFFFFFF)

#  define  TRUE  1
#  define  FALSE 0

#if SYS_TYPE == ARCH360

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The PROD_SYS switch is used mostly by PPPMAC's to determine if   |*/
/*|  the system being compiled is to be used for PRODUCTION (that is, |*/
/*|  null PPPMAC's) or not production (DEVELOPMENT).                  |*/
/*|                                                                   |*/
/*|  This file is purposely written so that it has the lowest         |*/
/*|  precedance in deciding what the value of PROD_SYS will be so that|*/
/*|  one-shot compiles can be done to create either PRODUCTION or     |*/
/*|  DEVELOPMENT versions of a given routine without modification of  |*/
/*|  this file.                                                       |*/
/*|                                                                   |*/
/*|  The order is:                                                    |*/
/*|                                                                   |*/
/*|  (1) #define in source file.  This is the LEAST DESIRABLE because |*/
/*|      there is a possibility that the #define might accidentally   |*/
/*|      be left in the source code, over-riding the other methods.   |*/
/*|      The override occurs because source #defines take precedance  |*/
/*|      over CW EXEC command line PRM options.                       |*/
/*|                                                                   |*/
/*|  (2) PRM to CW EXEC via the CC EXEC.  This is the MOST DESIRABLE  |*/
/*|      because CC EXEC will not touch PROD_SYS if not used with the |*/
/*|      "$" (PRODUCTION) or "%" (DEVELOPMENT) options.               |*/
/*|                                                                   |*/
/*|  (3) The value set in this file.  Because modification of this    |*/
/*|      file will cause recompilation of all routines, this is only  |*/
/*|      pratical when doing a complete system compile to create a    |*/
/*|      new PRODUCTION or DEVELOPMENT version.                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  If PROD_SYS is not defined somewhere, #define to TRUE for        |*/
/*|  PRODUCTION version or FALSE for DEVELOPMENT version.             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#   ifndef PROD_SYS
#     define PROD_SYS FALSE            /* Default system is development*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Just to be on the safe side, check to make sure that the PROD_SYS|*/
/*|  value in use is truly boolean.                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#   else
#      if ( PROD_SYS != TRUE ) && ( PROD_SYS != FALSE )
         ACK   PROD_SYS_NOT_BOOLEAN;
#      endif
#   endif
/*+-------------------------------------------------------------------+*/
/*| End of production/development flag resolution                     |*/
/*+-------------------------------------------------------------------+*/
#else
#  define PROD_SYS FALSE
#endif
