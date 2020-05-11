/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_type                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   str         string   INPUT    the string to be tested  |*/
/*|  :PARAM.   type        byte     INPUT    expected string type     |*/
/*|                                                                   |*/
/*|  :PURPOSE. Test the given string to see if it is of a given type. |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero if string is of asked type; returns       |*/
/*|            position at which string was not of given type, or     |*/
/*|            a standard STR error RC.                               |*/
/*|                                                                   |*/
/*|  :CALLS.   s_getlow(), s_up()                                     |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Not properly tested.                                   |*/
/*|                                                                   |*/
/*|  :MODS.    30_Aug_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|                                    tation up to 1.42;             |*/
/*|  :MODS.    21_Jan_88   K.J.Walsh   Revised to use OSK CSET functs.|*/
/*|  :MODS.    16_Mar_88   K.J.Walsh   Added "U" type (for s_parse).  |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "osk.h"
#include "str_i.h"
#include "debug.h"


#define BLANKS

sint s_type (str, beg, len, type)
string  str     ;                 /* String to tested                  */
sint    beg     ;                 /* Where to begin...                 */
sint    len     ;                 /* How much to do...                 */
byte    type    ;                 /* Expected type of string           */
{
   sint lens    = 0 ;             /* Used length of str                */
   sint pos     = 0 ;             /* Position in string                */
   sint end     = 0 ;             /* BEG+LEN=END, absolute position    */
   byte chr     = 0 ;             /* Single character for testing      */
   bool period  = 0 ;             /* Did we find a period in real?     */
   byte attr    = 0 ;             /* os_char[] attributes            */

   type = OS_TOUPPER(type);       /* Convert 'type' to uppercase       */
/*+-------------------------------------------------------------------+*/
/*| Standard length-checking...                                       |*/
/*+-------------------------------------------------------------------+*/
   if ( len < 0 )
   {
   return(S_BAD_LEN);
   }
   lens = s_len (str) ;           /* Get used length of string         */
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > lens )       /* also OK if LEN is too long   */
      {
         len = lens;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = lens;
      }
      if ( beg > lens )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > lens )        /* range bleeds right           */
      {
         len = lens - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = lens + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = lens;
         }
         if ( beg > lens )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > lens )     /* range bleeds right           */
         {
            len = lens - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = lens;
      }
      else if ( beg + len < 0 )          /* Completely out of range      */
      {
         len = 0;
      }
      else
      {                                /* Range bleeds left            */
         len = len + beg;                 /* NB -- adding a negative      */
         beg = 0;
      }
   }
}
   if ( len == 0 )
   {
   return(S_NUL_STR);
   }
   end = beg + len;

/*+-------------------------------------------------------------------+*/
/*| Following checks type integer...                                  |*/
/*+-------------------------------------------------------------------+*/
   if      (type == 'I')          /*   I N T E G E R                   */
   {
      pos = beg;
      /*+-------------------------------------------------------------+*/
      /*| Skip over leading white space...                            |*/
      /*+-------------------------------------------------------------+*/
      while ( s_char_s(str,pos) == ' ' )
      {
         pos++;
         if ( pos > end )
         {
   return(1);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After white space there can be a sign character:            |*/
      /*+-------------------------------------------------------------+*/
      chr = s_char_s(str,pos);
      if ( chr == '+' || chr == '-' )
      {
         if ( ++pos > end )            /* No next char to check!       */
         {
   return(1);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After the (optional) sign there must be just numerics       |*/
      /*+-------------------------------------------------------------+*/
      while ( OS_ISNUMERIC(s_char_s(str,pos) ) )
      {
         pos++;
         if ( pos > end )
         {
   return(0);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After numerics it is OK to have trailing white space...     |*/
      /*+-------------------------------------------------------------+*/
      while ( s_char_s(str,pos) == ' ' )
      {
         pos++;
         if ( pos >= end )
         {
   return(0);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| If we get here it is because a non-numeric or non-white was |*/
      /*| encountered in one of the two loops above (so no exit done.)|*/
      /*+-------------------------------------------------------------+*/
      pos++;
   return(pos);
   }
/*+-------------------------------------------------------------------+*/
/*| Following checks type real...                                     |*/
/*+-------------------------------------------------------------------+*/
   else if  (type == 'R')         /*   R E A L                         */
   {
      pos = beg;
      /*+-------------------------------------------------------------+*/
      /*| Skip over leading white space...                            |*/
      /*+-------------------------------------------------------------+*/
      while ( s_char_s(str,pos) == ' ' )
      {
         pos++;
         if ( pos > end )
         {
   return(1);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After white space there can be a sign character:            |*/
      /*+-------------------------------------------------------------+*/
      chr = s_char_s(str,pos);
      if ( chr == '+' || chr == '-' )
      {
         if ( ++pos > end )            /* No next char to check!       */
         {
   return(1);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After the (optional) sign there must be just numerics or "."|*/
      /*+-------------------------------------------------------------+*/
      period = FALSE;
      while(OS_ISNUMERIC(s_char_s(str,pos))||s_char_s(str,pos) == '.')
      {
         if ( s_char_s(str,pos) == '.' )
         {
            if ( period == TRUE )
            {
               pos++;
   return(pos);
            }
            else
            {
               period = TRUE;
            }
         }
         pos++;
         if ( pos > end )
         {
   return(0);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| After numerics it is OK to have trailing white space...     |*/
      /*+-------------------------------------------------------------+*/
      while ( s_char_s(str,pos) == ' ' )
      {
         pos++;
         if ( pos >= end )
         {
   return(0);
         }
      }
      /*+-------------------------------------------------------------+*/
      /*| If we get here it is because a non-numeric or non-white was |*/
      /*| encountered in one of the two loops above (so no exit done.)|*/
      /*+-------------------------------------------------------------+*/
      pos++;
   return(pos);
   }
/*+-------------------------------------------------------------------+*/
/*| Check alpha-numeric...                                            |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'A')
   {
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( NOT OS_ISALPHANUM( s_char_s(str,pos)) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Checks letters:                                                   |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'L')
   {
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( NOT OS_ISLETTER(s_char_s(str,pos)) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Checks Numbers:                                                   |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'N')
   {
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( NOT ( OS_ISNUMERIC(s_char_s(str,pos)) ) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Checks Printables:                                                |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'P')
   {
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( NOT ( OS_CANPRINT(s_char_s(str,pos)) ) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Checks specials:                                                  |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'S')
   {
      attr = ( OS_LOWER | OS_UPPER | OS_NUMERIC );
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( ( OS_CANPRINT(s_char_s(str,pos)) )
            &&( os_char[(sint) s_char_s(str,pos) ] & attr ) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Checks un-numeric printables:                                     |*/
/*+-------------------------------------------------------------------+*/
   else if (type == 'U')               /* Un-numeric printables       */
   {
      for ( pos = beg ; pos < end ; pos++ )
      {
         if ( OS_ISNUMERIC(s_char_s(str,pos)) |
              NOT ( OS_CANPRINT(s_char_s(str,pos)) ) )
         {
            pos++;
   return(pos);
         }
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Huh?                                                              |*/
/*+-------------------------------------------------------------------+*/
   else
   {
   return(S_BAD_PARM);
   }
   return(0);
}
