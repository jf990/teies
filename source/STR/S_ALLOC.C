/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_alloc()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  S_MAXLEN    none     INPUT                             |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  I/O      Pointer to string        |*/
/*|  :PARAM.   size        tint     INPUT    How much to allocate     |*/
/*|                                                                   |*/
/*|  :PURPOSE. To allocate 'size' bytes at 'pointer', or if 'pointer' |*/
/*|            not null, to deallocate to 'size' bytes, preserving    |*/
/*|            data.  (Use s_reallo() if data preserve is not wanted.)|*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint, either zero or rc from called routines   |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    02_Mar_87   K.J.Walsh   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*| :MODS.     15_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For checking pointers        */

sint s_alloc (string *tstr, tint size)
{
   sint     asize    =  0    ;    /* Amount of space to allocate       */
   sint     has      =  0    ;    /* Allocation of existig string      */
   sint     used     =  0    ;    /* Usage of existing string          */
   sint     count    =  0    ;    /* Used for loops                    */
   sint     rc       =  0    ;    /* Return code from stuff            */
   string   oldtstr  =  S_NUL;


   /*+----------------------------------------------------------------+*/
   /*| Null or too long cases:                                        |*/
   /*+----------------------------------------------------------------+*/
   if ( size == 0 )
   {
      rc = 0;
   }
   else if ( size > S_MAXLEN )
   {
      rc = S_BAD_SIZE;
   }

   /*+----------------------------------------------------------------+*/
   /*| If creating a new string, just allocate & set allocation field |*/
   /*+----------------------------------------------------------------+*/
   else if (*tstr == S_NULL)
   {
      asize = s_ovrall (size)    ;     /* Overallocat how much?        */

      *tstr = (string) os_mema(s_true_al(asize));

      s_smp_ul(*tstr) = 0;             /* Set usage length             */
      s_smp_al(*tstr) = asize;         /* Set usage length             */
   }

   /*+----------------------------------------------------------------+*/
   /*| Already exists--increase, decrease, or leave allocation?       |*/
   /*+----------------------------------------------------------------+*/
   else
   {

      has = s_smp_al(*tstr);           /* Find current allocation      */
      used = s_smp_ul(*tstr);          /* Find current usage           */

      if ( has == size )
      {
         rc = 0;
      }

      /*+-------------------------------------------------------------+*/
      /*| In the following case the existing string is longer than    |*/
      /*| the requested allocation so it might have to be cut down.   |*/
      /*| Even if not, the usage length is shortened to be the        |*/
      /*| requested length if it if greater than the requested.       |*/
      /*+-------------------------------------------------------------+*/
      else if ( has > size)
      {

         if ( size < used )
         {
            s_smp_ul(*tstr) = size;    /* Cut usage to allocation      */
         }
         else
         {
            s_smp_ul(*tstr) = used;    /* Use older (shorter) usage len*/
         }

         asize = s_cutall(has,size);   /* Cut down to what?            */
         if (asize != -1 )             /* (A -1 meant no cut)          */
         {
            oldtstr = *tstr;           /* Save origional               */

            *tstr = (string) os_mema(s_true_al(asize));

            s_smp_al(*tstr) = asize;   /* Set allocation size          */

#if STR_LEVEL_OPTIMIZED

               rc = os_memc(s_buffer(oldtstr),s_buffer(*tstr),
                  s_smp_ul(oldtstr));
               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }

#else

               for ( count = 0 ; count < s_smp_ul(oldtstr); count++ )
                  s_char_s(*tstr,count) = s_char_s(oldtstr,count);

#endif

            rc = s_drop(&oldtstr );    /* Discard old version          */
         }
      }

      /*+-------------------------------------------------------------+*/
      /*| In the following case the existing string is shorter, so    |*/
      /*| we will allocate a bigger one (if needed).  The usage length|*/
      /*| of the existing string is retained.                         |*/
      /*+-------------------------------------------------------------+*/
      else
      {
         oldtstr = *tstr;              /* Save origional               */

         asize  = s_ovrall(size)    ;  /* Compute overallocation       */

         *tstr = (string) os_mema(s_true_al(asize));

         s_smp_ul(*tstr)   =  used  ;  /* Set length used              */
         s_smp_al(*tstr)   = asize  ;  /* Set length allocated         */

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(oldtstr),s_buffer(*tstr),used);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < used ; count++ )
               s_char_s(*tstr,count) = s_char_s(oldtstr,count);


#endif

         rc = s_drop(&oldtstr );       /* Get rid of old one          */

      }
   }
   return(rc);
}
