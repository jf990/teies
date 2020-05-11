/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_active()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   type        int      INPUT    Permitted types:         |*/
/*|                                          0,1,'0','1','2','3','4', |*/
/*|                                          '5','6','7','8','9','*', |*/
/*|                                          and '!'.  Note that the  |*/
/*|                                          numbers 0 and 1 have     |*/
/*|                                          different meanings than  |*/
/*|                                          the characters '0' and   |*/
/*|                                          '1'                      |*/
/*|  :PARAM.   name        char*    INPUT    name of calling routine  |*/
/*|                                          MUST BE PREFIXED WITH    |*/
/*|                                          TWO BLANKS               |*/
/*|  :PARAM.   level       char*    INPUT    level of calling routine |*/
/*|                                          MUST BE PREFIXED WITH    |*/
/*|                                          TWO BLANKS               |*/
/*|  :PARAM.   ident       char*    INPUT    identifier for errors    |*/
/*|                                                                   |*/
/*|  :PURPOSE. Centralizes processing of REXX fetch operations used   |*/
/*|            to determine if a debugging switch is on or off.       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  return TRUE or FALSE if a particular switch or set     |*/
/*|            on or off; FALSE if error detected during fetch.       |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Strings designating switches for type 0 and type 1     |*/
/*|            tests must be preeceded by two blanks.                 |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "rexxdef.h"
#include "defs.h"
#include "debug.h"
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Make sure two debugging switches are OK.                          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#ifndef D_OSIDEBUG
   #define  D_OSIDEBUG  FALSE
#endif

#ifndef D_ACTDEBUG
   #define  D_ACTDEBUG  FALSE
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following is the definition of the checking macro.  D_OSIDEBUG is |*/
/*| is intended for debugging PPPMACs, so when it is set, only the    |*/
/*| values of the NAME/LEVEL being tested is reported.                |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_OSIDEBUG
   #define SEE(var) {(*D_pr_d)("!!!!:D_active(?):  SEE:  {%s}\n",var); \
      (*D_pr_d)("!!!!:  D_a_str/0x%X{%s}; D_str/0x%X{%s}; D_SLEN=%d\n",\
      D_a_str,D_a_str,D_str,D_str,D_SLEN);\
      D_MACSEE(var) }
#else
   #define SEE(var) D_MACSEE(var)
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following is a D_RETURN() style construct--if internal debugging  |*/
/*| is on, report return values.                                      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_ACTDEBUG
   #define RETURN(var) \
      {(*D_pr_d)("<!!!:D_active(?):  RETURNing(%d); D_str/0x%X={%s}\n"\
      ,var,D_str,D_str); return(var); }
#else
   #define RETURN(var) return(var);
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Because this routine might be passed a name or level without the  |*/
/*| identifying "D#" prefix, we must copy it into a string allocated  |*/
/*| by "D_main()", D_a_str, and do the prefixing.   Note that the     |*/
/*| the origin string is usually "name" or "level", and we use regular|*/
/*| C-style strings (for the sake of safety).  SETUP is not used when |*/
/*| the caller explicitly identifies the var--it is only used with    |*/
/*| types 0,1,'0','1','2',...,'9'                                     |*/
/*|                                                                   |*/
/*| So, given "NAME", SETUP copies to D_a_str as "D NAME"             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define SETUP(OLD)\
{\
   D_a_str(:0:) = 'D';\
   D_a_str(:1:) = ' ';\
   for ( count = 2 ; OLD(:count-2:) != '\0'; count++ ) \
      D_a_str(:count:) = OLD(:count-2:);\
   D_a_str(:count:) = '\0';\
}

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| The INFIX macro sticks in the 0,1,2,...,9 designator into D_a_str |*/
/*| which has already be readied for such treatment by the SETUP macro|*/
/*| described and #defined above.  As above, it is only used in the   |*/
/*| standard debugging switch types.                                  |*/
/*|                                                                   |*/
/*| So, given "D NAME" or "D5NAME", INFIX called with '3' changes the |*/
/*| string to "D3NAME".                                               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define INFIX(TYPE)\
{\
   D_a_str(:0:) = 'D';\
   D_a_str(:1:) = TYPE;\
}

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Actual start of routine follows:                                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
boolean D_active(type,name,level,ident)
   int   type      ;  /* The type of switch to check            */
   char  *name     ;  /* The calling routine name               */
   char  *level    ;  /* The calling routine's level            */
   char  *ident    ;  /* Identification string for output       */
{
   sint  count =  0;  /* Counter for string copies & etc.       */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Before we do the actual execution, do a D_BEGIN type action if    |*/
/*| the internal debugging switch is set.                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_ACTDEBUG
   (*D_pr_d)("!!!>:D_active(?):  type=%d, name={%s}, level={%s}, id={%s}\n"\
      ,type,name,level,ident);
   (*D_pr_d)("!!!!:D_active(?):  type='%c', D_byrx=%d, D_a_str/0x%X={%s}\
, D_str/0x%X={%s}\n",type,D_byrexx,D_a_str,D_a_str,D_str,D_str);
#endif
if ( type == 't' || type == 'T' )
   RETURN(FALSE);
RETURN(TRUE);
}
