/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_trans()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE.                                                        |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.   s_copy(), s_nulout(), s_alloc()                        |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  An ASSEMBLER verion would be many times faster in VM   |*/
/*|                                                                   |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|  :MODS.    27_Nov_87   K.J.Walsh   Implemented                    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"


extern   sint  s_copy   () ;
extern   sint  s_alloc  () ;
extern   sint  s_nulout () ;

sint s_trans(out,in,beg,len,in_map,out_map,pad)

string  *out;                                /* Output/target string   */
string   in;                                 /* Input string           */
sint     beg;                                /* Start at BEG in input  */
sint     len;                                /* Go for LEN in input    */
string   in_map;                             /* Map these...           */
string   out_map;                            /* ...to these            */
byte     pad;                                /* Padding character      */
{

   sint     rc       =  0     ;              /* RC from things         */
   sint     lens     =  0     ;              /* Source used length     */
   tint     counter  =  0     ;              /* Counter for stuff      */
   hword    ot_m_len =  0     ;              /* Used length of out  map*/
   tint     ulen     =  0     ;              /* Used length of in str  */
   tint     cur_out  =  0     ;              /* Current output char    */
   boolean  new_tar  =  TRUE  ;              /* in & *out not the same?*/
   boolean  not_done =  TRUE  ;              /* Was a mapping done?    */




   if (len < 0)                   /* Check for negative length         */
   {
      s_nulout(out);
   return(S_BAD_LEN);
   }

   lens = s_len(in);
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

   if (len == 0)                  /* If beginning is past end          */
   {
      s_nulout(out);
   return(0);
   }

/*+-------------------------------------------------------------------+*/
/*| The out string must be different than the maps...                 |*/
/*+-------------------------------------------------------------------+*/
   if ( *out == in_map )
   return(S_BAD_PARM);

   if ( *out == out_map )
   return(S_BAD_PARM);

/*+-------------------------------------------------------------------+*/
/*| We know know the expected B/L; now see if in&out are the same     |*/
/*+-------------------------------------------------------------------+*/
   if ( *out == in )              /* Target & origional are same!      */
      new_tar = FALSE;
   else
   {
      rc = s_alloc (out, len) ;
   }

/*+-------------------------------------------------------------------+*/
/*| If maps are same then the user is a dolt be we can handle it...   |*/
/*+-------------------------------------------------------------------+*/
   if ( in_map == out_map )
   {
      if ( new_tar || beg != 0 )
      {                                /* Equivalent to a copy         */
         rc = s_copy(out,in,beg,len);
   return(rc);
      }
      else
      {
         s_smp_ul(*out) = len;
   return(0);
      }
   }

/*+-------------------------------------------------------------------+*/
/*| Do the acutal translation                                         |*/
/*+-------------------------------------------------------------------+*/

   s_smp_ul(*out) = len ;
   for ( cur_out = 0 ; cur_out < len  ; cur_out++ )
   {
      not_done = TRUE;
      for ( counter = 0 ; counter < s_smp_ul(in_map) && not_done ; counter++ )
      {
         if ( s_char_s(in_map,counter) == s_char_s(in,beg) )
         {
            /*+-------------------------------------------------------+*/
            /*| A match!  Either map or use pad if out_map too short  |*/
            /*+-------------------------------------------------------+*/
            if ( counter < s_smp_ul(out_map)  )
            {
               s_char_s(*out,cur_out) = s_char_s(out_map,counter);
            }
            else
            {
               s_char_s(*out,cur_out) = pad;
            }
            not_done = FALSE;
         }
      }

      /*+-------------------------------------------------------------+*/
      /*| Copy char if doing a new string and no match as made        |*/
      /*+-------------------------------------------------------------+*/
      if ( not_done && new_tar )
      {
         s_char_s(*out,cur_out) = s_char_s(in,beg);
      }
      beg++;                           /* Incr for next attempt        */
   }

/*+-------------------------------------------------------------------+*/
/*| Done!                                                             |*/
/*+-------------------------------------------------------------------+*/

   return(0);
}

