/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_gin()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   val         int*     I/O      current call level       |*/
/*|  :PARAM.   direction   int      INPUT    which way to change      |*/
/*|                                          in, out, special.        |*/
/*|                                                                   |*/
/*| -99 :  Alert/Error, do not alter "val",  return "****:"           |*/
/*|  -1 :  Un-nest,decrement "val" by one,   return "<n  :"           |*/
/*|   0 :  Just put standard prefix,         return "----:"           |*/
/*|   1 :  Nest, increment "val" by one,     return "  n>:"           |*/
/*|  40 :  Just put blank prefix,            return "    :"           |*/
/*|  32 :  Just put blank prefix,            return "    :"           |*/
/*| else:                                    return "????:"           |*/
/*|                                                                   |*/
/*|  :PURPOSE.                                                        |*/
/*|        D_gin is used to by the standard PPP macros to preceede all|*/
/*|     printed lines with an indication of changes in routine nesting|*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a pointer to a string containing the indicator |*/
/*|                                                                   |*/
/*|  :CALLS.   sprintf()                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  The variable "line" should be os_mema()ed from         |*/
/*|            outside to facilitate DCSS implememtation.             |*/
/*|                                                                   |*/
/*|  :MODS.    05_Feb_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Moved from debugi.h to         |*/
/*|                                    STR txtlib                     |*/
/*|  :MODS.    07_Jun_87   K.J.Walsh   Altered symbols used on 1 and  |*/
/*|                                    -1 direction outputs           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#ifndef D_OSIDEBUG
#define D_OSIDEBUG FALSE
#endif

extern   sprintf();

byte *D_gin(val,direction)        /* "Graphically Indicated Nesting" */
   int *val;                      /* Nest indicator                  */
   int direction;                 /* Which way to change             */
{

   static char *line = "!!!!:";        /* Initialize                   */

#if D_OSIDEBUG
   printf("####:D_gin(%d-->%d,%d)\n",val,*val,direction);
#endif
   switch(direction)
   {

        case 0:                        /* Show null operation       */
           sprintf(line,"----:");      /* instead of strcpy() is OK */
        break;

        case 1:                        /* Nest 1 level deeper        */
           (*val)++;
           if ( *val > 999 )
              sprintf(line,"***>:");
           else
              sprintf(line,"% 3d>:",*val);
        break;

        case -1:                       /* Un-nest 1 level            */
           if ( *val > 999 )
              sprintf(line,"<***:");
           else
              sprintf(line,"<%-3d:",*val);
           (*val)--;
        break;

        case -99:                      /* Alert of error!           */
           sprintf(line,"****:");      /* instead of strcpy() is OK */
        break;

        case 32:                       /* Show null operation       */
        case 40:                       /* Show null operation       */
           sprintf(line,"    :");      /* instead of strcpy() is OK */
        break;

        default:                       /* Not sure what to do!      */
           sprintf(line,"????:");
        break;
   }

   return(line);
}
