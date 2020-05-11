/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_dump()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   first       hword*   INPUT    Table to start at or zero|*/
/*|                                          to implicate TM pointer  |*/
/*|                                          to tables.               |*/
/*|                                                                   |*/
/*|  :PURPOSE. Dumps the indicated table and all attached ones.       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  zero or a standard ISV error code                      |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.  ISV_FNF, ISV_IFC                                       |*/
/*|                                                                   |*/
/*|  :LIMITS.  Phase 1 version -- fetchs ISV's from CMS files         |*/
/*|                                                                   |*/
/*|  :MODS.    05_Apr_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42;             |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

sint isv_dump(hword *first)
{
   byte     *cstr       =  0      ; /*  C style version of the string  */
   sint     count       =  0      ; /* A counter that we use all over  */
   char     ch          =  0      ; /* Also used by SKIP()             */
   byte     *boot       =  0      ; /* To pull in ISV part cont'n len  */
   byte     *addr       =  0      ; /* Address of memory for ISV       */
   byte     *ptr        =  0      ; /* Pointer we move around it it    */
   sint     rc          =  0      ; /* RC from stuff                   */
   tint     *size       =  0      ; /* size of ISV extraced from ISV   */
   tint     realsize    =  0      ; /* *size is in D-words, hafta calc */
   string   blank       =  S_NULL ; /* A single space, need in file gen*/
   string   info        =  S_NULL ; /* mem man info from isv           */
   string   names_at    =  S_NULL ; /* location of name - list         */
   string   name_count  =  S_NULL ; /* total number of sets in isv     */
   string   defalt      =  S_NULL ; /* default file name.              */
   tint     *kludge     =  0      ; /* see descriptive paragraph below */



   size = (tint*)(first + ISV_LENGTH);
   realsize = *size * 8;            /* we need BYTES but it's in Dwords*/


   if ( *size < 1 )
   {
      os_memd(cstr);
      #D_return(ISV_INVALID_FORMAT)
   }


/*+-------------------------------------------------------------------+*/
/*|  Set ISV seek path pointers                                       |*/
/*+-------------------------------------------------------------------+*/

   ptr = ISV_OBJECT + addr;         /* Locate isv object FD field      */

   for ( count = 0 ; count < ISV_OBJECT_S ; count++)
   {
      *ptr = cstr[count:);
      ptr++;
   }

   if (  TM_master->ISV == 0 )      /* Is this the 1st ISV loaded?     */
   {
                                    /* yup                             */
      TM_master->ISV = addr;
   }
   else
   {                               /* nope -- push ISV's              */
/*+-------------------------------------------------------------------+*/
/*|CLUDGE - It seems that there is no portable way that I can - KLUDGE|*/
/*|CLUDGE - stick in the address of the preceding ISV, with   - KLUDGE|*/
/*|CLUDGE - the possible exception of using a structure that  - KLUDGE|*/
/*|CLUDGE - would also be unstable because C likes to align   - KLUDGE|*/
/*|CLUDGE - things in structures (and the data for the struct - KLUDGE|*/
/*|CLUDGE - is created by a by TSTR MACRO, which does no such - KLUDGE|*/
/*|CLUDGE - thing.                                            - KLUDGE|*/
/*+-------------------------------------------------------------------+*/
      /* point to link info in NEW one   */
      kludge = (tint*) (addr + ISV_LINK_AT);
      /* put pointer of OLD one into NEW */
      *kludge =  (tint) TM_master->ISV;


      TM_master->ISV = addr;       /* set TM pointer to the NEW one   */
   }


   #D_leave(0,'Looks OK');

}
