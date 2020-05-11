/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_active()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   type        int      INPUT    Permitted types:         |*/
/*|                                          0 basic--name or level   |*/
/*|                                          1 aux  --name or level   |*/
/*|                                                                   |*/
/*|                                          2...6  --reserved        |*/
/*|                                                                   |*/
/*|                                          7 diag --name or level   |*/
/*|                                          8 error--name or level   |*/
/*|                                          9 spec --name or level   |*/
/*|                                                                   |*/
/*|                                          '0' basic for name       |*/
/*|                                          '1' aux   for name       |*/
/*|                                          '2' basic & aux for name |*/
/*|                                                                   |*/
/*|                                          '3' basic for level      |*/
/*|                                          '4' aux   for level      |*/
/*|                                          '5' basic & aux for level|*/
/*|                                                                   |*/
/*|                                          '6' (reserved)           |*/
/*|                                                                   |*/
/*|                                          '7' diag for name        |*/
/*|                                          '8' error for name       |*/
/*|                                          '9' special for name     |*/
/*|                                                                   |*/
/*|                                          '!' name & level exact   |*/
/*|                                          '*' name exactly         |*/
/*|                                          'T' get text, exact routn|*/
/*|                                          't' get text, exact routn|*/
/*|                                                                   |*/
/*| Note that in all these cases, "name", the second parameter, is    |*/
/*| intended to be but not necessarily a routine name; likewise,      |*/
/*| "level" denotes the third parameter.                              |*/
/*|                                                                   |*/
/*|  :PARAM.   name        char*    INPUT    name of calling routine  |*/
/*|  :PARAM.   level       char*    INPUT    level of calling routine |*/
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
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    14_Jun_87   K.D.Ng      Added calls to D_qry() to use  |*/
/*|                                    new hash table lookups.        |*/
/*|  :MODS.    15_Jun_87   K.J.Walsh   Removed old debugging lookups, |*/
/*|                                    renamed D_qry() to D_lookup()  |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <rexxdef.h>
#include <stdio.h>
#include "defs.h"
#include "debug.h"
#include "debug_i.h"
extern byte *D_gin();
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Make sure two debugging switches are OK.                          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#ifndef D_SHOWVARS
   #define  D_SHOWVARS  FALSE
#endif

#ifndef D_SHOWREST
   #define  D_SHOWREST  FALSE
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following is a D_RETURN() style construct--if internal debugging  |*/
/*| is on, report return values.                                      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_SHOWREST
   #define RETURN(var) \
  {(*D_pr_d)("<!!!:  RETURN(%d); D_a_str/0x%X={%s}, D_r_str/0x%X={%s}\n"\
      ,var,D_a_str,D_a_str,D_r_str,D_r_str); return(var); }
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
/*| Added "fudge" factor to account for old-stype PPPMAC's.           |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_SHOWREST
   #define EXAMINE \
     (*D_pr_d)("!!!!:  SET:  D_a_str/0x%X={%s}, D_r_str/0x%X={%s}\n",\
     D_a_str,D_a_str,D_r_str,D_r_str);
#else
   #define EXAMINE /* Just kidding */
#endif
#define SETUP(OLD)\
{\
   D_a_str(:0:) = ' ';\
   n_ptr = 1; \
   o_ptr = 0; \
   while ( OLD(:o_ptr:) != '\0' ) \
      D_a_str(:n_ptr++:) = OLD(:o_ptr++:); \
   D_a_str(:n_ptr:) = '\0'; \
   EXAMINE \
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
   D_a_str(:0:) = TYPE;\
}

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Actual start of routine follows:                                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
boolean D_active(type,name,level,ident)
   int   type      ;         /* The type of switch to check            */
   char  *name     ;         /* The calling routine name               */
   char  *level    ;         /* The calling routine's level            */
   char  *ident    ;         /* Identification string for output       */
{
   byte  D_a_str(:50:);      /* Old style checking string              */
   sint  n_ptr =  0;  /* Pointer into strings beging created (new)     */
   sint  o_ptr =  0;  /* Pointer into strings being copied (old)       */

   D_a_str(:0:) = '\0';
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Before we do the actual execution, do a D_BEGIN type action if    |*/
/*| the internal debugging switch is set.                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if D_SHOWREST
(*D_pr_d)("!!!>:D_active(?):  type=%d/'%c', name={%s}, level={%s}, id={%s}\
   \n!!!!:  D_a_str/0x%X={%s}, D_r_str/0x%X={%s}, D_byrexx=%d\n",\
   type,type,name,level,ident,D_a_str,D_a_str,D_r_str,D_r_str,D_byrexx);
#endif

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| There is a chance that we have already determined that the stuff  |*/
/*| we are debugging has not been called from REXX, so there will be  |*/
/*| no REXX variables to check!                                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   if ( D_INT_SW(D_ok_use) == FALSE )  /* Not loaded?                  */
      RETURN(FALSE);                   /* ...then return negatory.     */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Execution begins with checking of the given 'type'.  Following are|*/
/*| the "standard" types.                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
switch (type)
{
   case 0:
      /*+-------------------------------------------------------------+*/
      /*|                                                             |*/
      /*| Type 0 is standard basic debugging.  The following tests    |*/
      /*| are of:                                                     |*/
      /*|                                                             |*/
      /*|     "D0NAME",  "D2NAME"                                     |*/
      /*|     "D3LEVEL", "D5LEVEL"                                    |*/
      /*|                                                             |*/
      /*| Given the assumption that that string "name" is "NAME" and  |*/
      /*| string "level" is "LEVEL"                                   |*/
      /*|                                                             |*/
      /*+-------------------------------------------------------------+*/
      SETUP(name)                            /* Ready to test NAMEs    */

      INFIX('0')                             /* Ready to test D0NAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      INFIX('2')                             /* Ready to test D2NAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      SETUP(level)                           /* Ready to test LEVELs   */

      INFIX('3')                             /* Ready to test D3LEVEL  */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      INFIX('5')                             /* Ready to test D5LEVEL  */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      if ( D_lookup("D__ALL",ident)== TRUE ) /* One last shot!        */
         RETURN(TRUE);

      break;

   case 1:
      /*+-------------------------------------------------------------+*/
      /*|                                                             |*/
      /*| Type 1 is standard auxilary debugging.  The following tests |*/
      /*| are of:                                                     |*/
      /*|                                                             |*/
      /*|     "D1NAME",  "D2NAME"                                     |*/
      /*|     "D4LEVEL", "D5LEVEL"                                    |*/
      /*|                                                             |*/
      /*| Given the assumption that that string "name" is "NAME" and  |*/
      /*| string "level" is "LEVEL"                                   |*/
      /*|                                                             |*/
      /*+-------------------------------------------------------------+*/
      SETUP(name)                            /* Ready to test NAMEs    */

      INFIX('1')                             /* Ready to test D1NAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      INFIX('2')                             /* Ready to test D2NAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      SETUP(level)                           /* Ready to test LEVELs   */

      INFIX('4')                             /* Ready to test D5LEVEL  */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      INFIX('5')                             /* Ready to test D5LEVEL  */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      if ( D_lookup("D__ALL",ident)== TRUE ) /* One last shot!        */
         RETURN(TRUE);

      break;

   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '7':
   case '8':
   case '9':
      /*+-------------------------------------------------------------+*/
      /*|                                                             |*/
      /*| These types are explicit calls for a particular switch to be|*/
      /*| tested.  Only the first argument, the "name" is tested,     |*/
      /*| although a "level" or any special variable could be tested. |*/
      /*| Following, we set up the variable, given that name = "VAR", |*/
      /*| to include the standardized prefix:                         |*/
      /*|                                                             |*/
      /*| Given "VAR" and type '9', setup for and test "D9VAR".       |*/
      /*|                                                             |*/
      /*+-------------------------------------------------------------+*/
      SETUP(name)                            /* Ready to test NAMEs    */
      INFIX(type)                            /* Ready to test DxNAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      break;

   case 7:
   case 8:
   case 9:
      /*+-------------------------------------------------------------+*/
      /*|                                                             |*/
      /*| Test misc-type switches by both name and level:             |*/
      /*| tested.  Only the first argument, the "name" is tested,     |*/
      /*| although a "level" or any special variable could be tested. |*/
      /*| Following, we set up the variable, given that name = "VAR", |*/
      /*| to include the standardized prefix:                         |*/
      /*|                                                             |*/
      /*| Given "NAME" and "LEVEL", setup for and test "D9NAME" and   |*/
      /*| D9LEVEL.                                                    |*/
      /*|                                                             |*/
      /*+-------------------------------------------------------------+*/
      SETUP(name)                            /* Ready to test NAMEs    */

      INFIX(type+'0')                        /* Ready to test DxNAME   */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      SETUP(level)                           /* Ready to test LEVELs   */

      INFIX(type+'0')                        /* Ready to test DxLEVEL  */
      if ( D_lookup(D_a_str,ident) == TRUE )
         RETURN(TRUE);

      break;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following are tests of non-standard switches.  These types are not|*/
/*| intended for getting text out of the REXX environment.            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   case '!':                           /* check LEVEL and NAME ...     */
      if ( level(:0:) == 'D'           /*    If it's a standard style  */
         & level(:1:) >= '0'           /*    strip off the 'D'.        */
         & level(:1:) <= '9' )
      {
         if ( D_lookup(level+1,ident) == TRUE )
            RETURN(TRUE);
      }
      else
      {
         if ( D_lookup(level,ident) == TRUE ) /* check LEVEL var xactly*/
            RETURN(TRUE);
      }

      /* NO "break;" because '!' means "check name and level" */
      /* NO "break;" because '!' means "check name and level" */
      /* NO "break;" because '!' means "check name and level" */

   case '*':
      if ( name(:0:) == 'D'            /*    If it's a standard style  */
         & name(:1:) >= '0'            /*    strip off the 'D'.        */
         & name(:1:) <= '9' )
      {
         if ( D_lookup(name+1,ident) == TRUE )
            RETURN(TRUE);
      }
      else
      {
         if ( D_lookup(name,ident) == TRUE ) /* check NAME var xactly*/
            RETURN(TRUE);
      }

      break;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following is code to process a bad 'type'.                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   default:

      (*D_pr_d)("%s%s: D_active:  Cannot process given type, (%d), '%c'.\n",
         D_gin(-99,-99),ident,type,type);

      break;


} /* end of switch(type) */
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Following the structure of the case/break shows that we only exit |*/
/*| if a TRUE condition is found.  All other cases exit here at this  |*/
/*| final FALSE.                                                      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
RETURN(FALSE);
}
