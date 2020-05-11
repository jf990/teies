/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_init()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  INPUT    pointer to output string |*/
/*|  :PARAM.   cstr        byte*    INPUT    pointer to C-type string |*/
/*|                                                                   |*/
/*|  :PURPOSE. To convert the C string to a TEIES string.             |*/
/*|                                                                   |*/
/*|  :OUTPUT.  sint zero                                              |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Cannot process complex strings                         |*/
/*|                                                                   |*/
/*|  :MODS.    18_Aug_87   N.Voronka   Authored.                      |*/
/*|  :MODS.    11_Mar_87   K.J.Walsh   Do not assume "tstr" is null.  |*/
/*|  :MODS.    15_May_87   K.J.Walsh   Doc and RC to 1.42 spec.       |*/
/*|                                    removed memory allocation chks.|*/
/*|                                                                   |*/
/*| :MODS.     15_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer checking         */

sint s_init (string *tstr, byte *cstr)
{
   sint       len    = 0   ;           /* Length of C-str              */
   sint       allen  = 0   ;           /* TEIES-str allocation len     */
   sint       count  = 0   ;           /* Copy counter                 */
   sint       rc     = 0   ;           /* Our Return Code              */


   while (cstr[len] != '\0')         /* Get length of C-str          */
      ++len ;

   /*+----------------------------------------------------------------+*/
   /*|  If C-string is too big then we cannot make a TEIES string.    |*/
   /*+----------------------------------------------------------------+*/
   if (len > S_MAXLEN)
   {

      rc = S_BAD_SIZE;

   }
   /*+----------------------------------------------------------------+*/
   /*|  If C-string is empty, then we do not make a TEIES string.     |*/
   /*+----------------------------------------------------------------+*/
   else if ( len == 0 )
   {
   }

   else
   {
      /*+-------------------------------------------------------------+*/
      /*|  If given string was not previously used, allocate.         |*/
      /*+-------------------------------------------------------------+*/
      if (*tstr == S_NUL)
      {
         allen = s_ovrall(len);        /* Overallocation size          */
         *tstr = (string) os_mema(s_true_al(allen));
         s_smp_al(*tstr) = allen  ;    /* Set allocation field         */

      }

      /*+-------------------------------------------------------------+*/
      /*|  String was allocated but is too short to use.              |*/
      /*+-------------------------------------------------------------+*/
      else if ( len > s_smp_al(*tstr) )
      {
         s_drop(tstr);                 /* Trash old one                */

         allen = s_ovrall(len);        /* Size up new one              */

         *tstr = (string) os_mema(s_true_al(allen));

         s_smp_al(*tstr) = allen;      /* Set allocation field         */
      }

      /*+-------------------------------------------------------------+*/
      /*|  String was allocated, see if we will reallocate a shorter  |*/
      /*|  one before doing re-initilization.                         |*/
      /*+-------------------------------------------------------------+*/
      else
      {
         allen = s_cutall(s_smp_al(*tstr),len);  /* Cut it?            */
         if ( allen != -1 )
         {
            s_drop(tstr);

            *tstr = (string) os_mema(s_true_al(allen));

            s_smp_al(*tstr) = allen;   /* Set allocation field         */
         }
      }

      /*+-------------------------------------------------------------+*/
      /*|  Now we have definately allocated at least the required     |*/
      /*|  size; do the actual copy.                                  |*/
      /*+-------------------------------------------------------------+*/
      s_smp_ul(*tstr) = len;           /* Set usage length             */

#if STR_LEVEL_OPTIMIZED

         rc = os_memc(cstr,s_buffer(*tstr),len);
         if ( rc != 0 )
         {
            rc = S_OSK_ERROR;
         }

#else

         for ( count = 0; count < len; count++)
            s_char_s(*tstr,count) = cstr[count];

#endif

   }

   return(rc);
}
