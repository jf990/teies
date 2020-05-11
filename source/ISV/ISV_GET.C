/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_get()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  OUTPUT   pointer to set to ISV    |*/
/*|  :PARAM.   id          isv      INPUT    ID number of desired ISV |*/
/*|  :PARAM.   size        sint     INPUT    max allowable size of ISV|*/
/*|                                                                   |*/
/*|  :PURPOSE.      Modifies "tstr" (TEIES string) to point to the    |*/
/*|                 version of ISV "id" (a sint) of size equal to     |*/
/*|                 (or greatest entry less than) the given "size"    |*/
/*|                 (also a sint).                                    |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Either   A standard ISV error code (which is negative),|*/
/*|            or number indicating how deep the ISV was located, zero|*/
/*|            meaning the first encountered.                         |*/
/*|                                                                   |*/
/*|  :CALLS. (nothing)                                                |*/
/*|                                                                   |*/
/*|  :ERRORS.  ISV_NL, ISV_IOB, ISV_NF                                |*/
/*|                                                                   |*/
/*|  :LIMITS.  Should s_drop() "*tstr" before setting to ISV pointer  |*/
/*|            (after appropriate validation) because if it points to |*/
/*|            a string, then that memory will be permanently lost to |*/
/*|            a given instance of execution.                         |*/
/*|                                                                   |*/
/*|  :MODS.    31_Mar_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    18_Jun_87   K.J.Walsh   Removed s_drop().  See LIMITS. |*/
/*|  :MODS.    28_Jan_88   K.J.Walsh   Added #s_pcheck() checking.    |*/
/*|  :MODS.    28_Jan_88   K.J.Walsh   Added "desired size = 0 to     |*/
/*|                                    indicated largest wanted"      |*/
/*|                                    feature.                       |*/
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

#s_c_xtrn();

sint isv_get(string *tstr, isv id, sint size)
{
   byte     *start   =  0  ;  /* points to start of given ISV          */
   tint     *max_id  =  0  ;  /* points to max-id # (aka "N")          */
   tint     *tprt    =  0  ;  /* points to location of address of table*/
   isv      *pointer =  0  ;  /* used to move about ISV's after select */
   tint     isv_num  =  0  ;  /* how deep the ISV inwhich ID was found */
   byte     *vary    =  0  ;  /* Numbal of variations of this isv      */
   string   *phrase  =  0  ;  /* a given ISV entry                     */
   sint     p_size   =  0  ;  /* Size arg casted to be hword           */
   tint     sample   =  0  ;  /* Counter in phrase version loop        */

   #D_start('isv_get','isv','ptr=0x%X; id=%d; size=%d','tstr,id,size');
   #s_pcheck('tstr');
   #D_str(*tstr,'target string');

   if ( TM_master->ISV <= (byte*) NIL )
   {
      #D_leave(ISV_NL,'No ISV loaded');
   }

   start = (byte*) TM_master->ISV;
   #D_show('pointer to (first) ISV table from TM_master = 0x%X','start');

   for ( isv_num = 0 ; start > (byte*) NIL ; isv_num++)
   {
      max_id = (tint*) (start + ISV_N);
      #D_show('max_id(0x%X) = %d / 0x%X','max_id,*max_id,*max_id');


      if ( id > (isv) (*max_id) )
      {
         #D_show('max_id=%d less than wanted id=%d, skipping this one',\
           'max_id,id');
      }
      else
      {
         tprt = (tint*)(ISV_START + (byte*) start);/*Table is after ptr*/
      #D_show('1st ISV pointer @ 0x%06X shows an ISV 0x%06X from start',\
         'tprt,*tprt');

      tprt += (tint) (id - 1);
      #D_show('#%d ISV pointer @ 0x%06X shows an ISV 0x%06X from start',\
         'id,tprt,*tprt');

      if ( *tprt != 0 )                /* If the pointer is not zero   */
      {

         vary = *tprt + (byte*) start; /* This points to # variations  */
         #D_show('Total of 0x%06X variations indicated at 0x%06X',\
            '*vary,vary');

         phrase = (string*) (1 + vary);
         for ( sample = *vary ; sample > 0 ; sample-- )
         {
            #D_str('(string)phrase','checking');
            p_size =  (sint) s_smp_ul( (string) phrase);
            #D_show('p_size=%d','p_size');
            /*+-------------------------------------------------------+*/
            /*| We want one smaller or equal to the requested size,   |*/
            /*| unless the requested size is zero, in which case the  |*/
            /*| largest (and first) one will do.                      |*/
            /*+-------------------------------------------------------+*/
            if ( p_size <=  size | size == 0 )
            {
               #D_str(*tstr,'dropping');
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  KLUDGE -- Should s_drop(*tstr) after making sure it   -- CLUDGE  |*/
/*|  KLUDGE -- points to a string (or at least not into an -- CLUDGE  |*/
/*|  KLUDGE -- ISV in memory!)                             -- CLUDGE  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
               *tstr = (string) phrase;      /* Fork it over           */
               #D_str(*tstr,'outgoing');
               #D_leave(isv_num,'found the phrase');
            }
#D_show('p_size = 0x%X / %d ; phrase = 0x%X / %d','p_size,p_size,phrase,\
phrase');
            p_size = p_size + 4;       /* Hop  over alloc/usage info   */
#D_show('p_size = 0x%X / %d ; phrase = 0x%X / %d','p_size,p_size,phrase,\
phrase');
            phrase = (string *) (p_size + (byte*) phrase);
#D_show('p_size = 0x%X / %d ; phrase = 0x%X / %d','p_size,p_size,phrase,\
phrase');
         } /* Check another phrase in this ISV loop */
      } /* If a set of phrases was defined in this table */
      }

      /* Change "start" to point to the next ISV table as indicted in  */
      /* in the current ISV table.                                     */
      start = *((byte**)(start+ISV_LINK_AT));
      #D_show('pointer to next ISV = 0x%X','start');

   } /* Check another ISV table */
   #D_leave(ISV_NF,'Not found.');
}
