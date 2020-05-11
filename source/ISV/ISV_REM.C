/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_rem()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV byte* IN/OUT   The ISV root pointer     |*/
/*|                                                                   |*/
/*|  :PARAM.   isv_id      string   INPUT    Name of ISV to be unloaded*/
/*|                                          in standard file format  |*/
/*|                                          (refer to os_qfd())      |*/
/*|                                                                   |*/
/*|  :PURPOSE. Removes "isv_id" (a teies string) from ememory and     |*/
/*|            marks the ISV search table entry as unloaded.  If the  |*/
/*|            given isv_id is S_NULL, then the 1st ISV in the search |*/
/*|            path is removed (unless it is the DEFAULT ISV)         |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or a standard ISV error RC                |*/
/*|                                                                   |*/
/*|  :CALLS.   os_memd(), os_parse()                                  |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  This version can only handle S_NULL isv_id.            |*/
/*|                                                                   |*/
/*|  :MODS.    31_Mar_87   K.J.Walsh   Written.                       |*/
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


sint isv_rem(isv_id)

string   isv_id   ;                    /* Name of ISV to be done in    */

{
   byte     *next    =  0        ;     /* points to start of the next 1*/
   sint     rcode    =  0        ;     /* needed for os_parse rc       */
   tint     out      =  0        ;     /* pointer for creating target  */
   tint     pointer  =  0        ;     /* pointer for string work      */
   string   target   =  S_NULL   ;     /* will contain target ISV name */
   boolean  wild_ft  =  FALSE    ;     /* wild card instead of f-type? */
   boolean  wild_fm  =  FALSE    ;     /* wild card instead of f-mode? */
   string   fname    =  S_NULL   ;     /* file name of ISV             */
   string   ftype    =  S_NULL   ;     /* file type of ISV             */
   string   fmode    =  S_NULL   ;     /* file mode of ISV             */
   sint     glen     =  0        ;     /* length of given name         */
   sint     ptr      =  0        ;     /* pointer req'd by osparse()   */


   if ( TM_master->ISV == NIL )        /* Do we got a leg to stand on? */
   {
   return(ISV_NIM);
   }

   next = *((byte**)((TM_master->ISV)+ISV_LINK_AT));
   if ( next == 0 )                    /* Is this last isv?            */
   {
   return(ISV_DIC);
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Removing the 1st ISV--a pop--is quite simple...                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   if ( s_len(isv_id) == 0 )           /* Null name -- do the pop!     */
   {

      os_memd(TM_master->ISV);         /* Chuck it                     */

      TM_master->ISV = next;           /* Move pointer to next in      */
   return(0);
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Given a name though, we have some searching to do...             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   else
   {

      s_alloc(&fname,8);
      s_alloc(&ftype,8);
      s_alloc(&fmode,2);
      s_alloc(&target,17);

      ptr = 0;                            /* start at the beginning    */
      glen = s_len(isv_id);

      rcode = os_parse(&fmode,isv_id,&ptr,1,glen);

      rcode += os_parse(&target,isv_id,&ptr,1,glen);

      rcode += os_parse(&fname,isv_id,&ptr,8,glen);

      rcode += os_parse(&ftype,isv_id,&ptr,8,glen);

      if ( rcode != 0 || s_smp_ul(fname) == 0 )
      {
         rcode = ISV_BNG;              /* then bad name given          */
         goto QUIT;                    /* We do the jump to drop str's */
      }

      if ( s_smp_ul(ftype) == 1 )      /* if type size is 1            */
      {
         if ( s_char_s(ftype,1) == '*')/* and it's a '*', then         */
            wild_ft = TRUE;            /* it's wild                    */
      }
      else if ( s_smp_ul(ftype) == 0 ) /* but if it's size is zero,    */
         s_init(&ftype,"ISV$OBJ");     /* then the default is ISV$OBJ  */

      if ( s_smp_ul(fmode) == 1 )      /* if mode size 1               */
      {
         if ( s_char_s(fmode,1) == '*')/* and it's a '*', then         */
            wild_fm = TRUE;            /* it's a wild one              */
      }
      else if ( s_smp_ul(fname) == 0 ) /* if mode size is zero         */
         wild_fm = TRUE;               /* well, that's pretty wild too!*/

      s_pad(&fname,fname,8,' ','R');
      s_pad(&ftype,ftype,8,' ','R');  /* Pad with blanks for format   */
      s_pad(&fmode,fmode,1,' ','R');

      s_cat(&target,fname,ftype,0,0);  /* built the                    */
      s_cat(&target,target,fmode,0,0); /* discriptor                   */


/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  At last it's apart!  Now to do what we came here for:            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
rcode = isv_rem(S_NULL);
/*+-------------------------------------------------------------------+*/
/*|CLUDGE-Don't have time to implement this portion yet.  Will -KLUDGE|*/
/*|CLUDGE-get back to it.  All calls to isv_rem check params   -KLUDGE|*/
/*|CLUDGE-and pop an ISV                                       -KLUDGE|*/
/*+-------------------------------------------------------------------+*/

QUIT:

   s_drop(&fname);
   s_drop(&ftype);
   s_drop(&fmode);
   s_drop(&target);

   return(rcode);
   }
}
