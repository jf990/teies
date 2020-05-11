/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_ISVDMP                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV                                         |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Dumps all ISV's in memory.                             |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     06_Jul_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug_i.h"


void D_isvdmp()


{

   tint  *start   =  NIL   ;           /* Pointer to an ISV table      */
   tint  *max_id  =  NIL   ;           /* Pointer to max id # (aka "N")*/
   tint  counter  =  0     ;           /* Counter for loops.           */
   byte  ptr      =  NIL   ;           /* Pointer for stuff.           */

   #D_begin('D_isvdmp','debug');

   if ( TM_master->ISV <= 0 )
   {
      D_pr("----:D_isvdmp(A):  No ISV loaded.\n");
   }
   else
   {
      D_pr("----:D_isvdmp(B):  Dump of all loaded ISV's follows:\n");
      start = TM_master->ISV;        /* Copy the first pointer       */
      D_pr("    :TM_master->ISV (first ISV table) at 0x%X.\n",start);
      do
      {

         /*+----------------------------------------------------------+*/
         /*| Dump source file descriptor                              |*/
         /*+----------------------------------------------------------+*/
         ptr = start + ISV_SOURCE;
         D_pr("    :source FD (D=%d,S=%d) at 0x%X value = \"",
            ISV_SOURCE,ISV_SOURCE,ptr);

         for ( counter = 0 ; counter < ISV_SOURCE_S ; counter++)
         {
            D_pr("%c",*ptr);
            ptr++;
         }

         D_pr("\".\n");

         /*+----------------------------------------------------------+*/
         /*| Dump object file descriptor                              |*/
         /*+----------------------------------------------------------+*/
         ptr = start + ISV_OBJECT;
         D_pr("    :object FD (D=%d,S=%d) at 0x%X value = \"",
            ISV_OBJECT,ISV_OBJECT,ptr);

         for ( counter = 0 ; counter < ISV_OBJECT_S ; counter++)
         {
            D_pr("%c",*ptr);
            ptr++;
         }

         D_pr("\".\n");

         /*+----------------------------------------------------------+*/
         /*| Dump creation date                                       |*/
         /*+----------------------------------------------------------+*/
         ptr = start + ISV_DATE;
         D_pr("    :create date (D=%d,S=%d) at 0x%X value = \"",
            ISV_DATE,ISV_DATE,ptr);

         for ( counter = 0 ; counter < ISV_DATE_S ; counter++)
         {
            D_pr("%c",*ptr);
            ptr++;
         }

         D_pr("\".\n");

         /*+----------------------------------------------------------+*/
         /*| Dump creation time                                       |*/
         /*+----------------------------------------------------------+*/
         ptr = start + ISV_TIME;
         D_pr("    :create time (D=%d,S=%d) at 0x%X value = \"",
            ISV_TIME,ISV_TIME,ptr);

         for ( counter = 0 ; counter < ISV_TIME_S ; counter++)
         {
            D_pr("%c",*ptr);
            ptr++;
         }

         D_pr("\".\n");

         /*+----------------------------------------------------------+*/
         /*| Dump isv max id:                                         |*/
         /*+----------------------------------------------------------+*/
         max_id = start + ISV_N ;
         D_pr("    :max_id(0x%X) = %d"\n,max_id,*max_id);


   for ( isv_num = 0 ; start > 0 ; isv_num++) /* Loop around ISV's   */
   {

      tprt = ISV_START + (byte*) start; /* Table is after that ptr*/
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

         phrase = 1 + (byte*) vary;
         for ( sample = *vary ; sample > 0 ; sample-- )
         {
            #D_str(phrase,'checking');
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
               *tstr = phrase;         /* Fork it over                 */
               #D_str(*tstr,'outgoing');
               #D_leave(isv_num,'found the phrase');
            }
            p_size = p_size + 4;       /* Hop  over alloc/usage info   */
            phrase = p_size + (byte) phrase;
         } /* Check another phrase in this ISV loop */
      } /* If a set of phrases was defined in this table */
      start = *start;                  /* Pointer to next at beginning */

   } /* Check another ISV table */
   #D_leave(ISV_NF,'Not found.');
}
