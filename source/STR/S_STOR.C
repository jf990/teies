/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stor()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   hw_out     *real    OUTPUT   Output of conversion.     |*/
/*|  :PARAM.   str_in      string   INPUT    Contains the number.     |*/
/*|  :PARAM.   beg         tint     INPUT    Start of where to find it|*/
/*|  :PARAM.   len         tint     INPUT    How long to look for it  |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts S1 to a type real  returned as hw_out         |*/
/*|            Does NOT check for corrupt strings.   All leading and  |*/
/*|            trailing non-numerics are considered white space.      |*/
/*|            Only "white space" delimited number is returned.       |*/
/*|            If a "-" is found immediately before the first digit   |*/
/*|            (no white space), then the number is negative.         |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Returns a standard STR error code or 0.                |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  This dummy version is provided to facilitate headerfile|*/
/*|            and txtlib generation.                                 |*/
/*|                                                                   |*/
/*|            sscanf() has a limited resolution of 8 characters in   |*/
/*|            the whole and fraction portions of the number being    |*/
/*|            converted.  No checking is done to check for this      |*/
/*|            and larger numbers will usually be incorrectly         |*/
/*|            converted without an error being detected.             |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Implemented.                   |*/
/*|  :MODS.    17_Jan_88   K.J.Walsh   Documented to 1.42 standards;  |*/
/*|                                    Added use of #s_calc;          |*/
/*|                                    Added use of OS_NUMERIC()      |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"


extern            sscanf   () ;

sint  s_stor(r_out, str_in, beg, len)

real     *r_out   ;                    /* Results - a real conversion  */
string   str_in   ;                    /* Input - a real-able string   */
sint     beg      ;                    /* Beginning position of real   */
sint     len      ;                    /* Length of real in str        */

{
   string   dummy    =  S_NUL ;        /* For C-string version of      */
   byte     *cstr    =  NIL;           /* Pointer into dummy           */
   sint     rc       =  0     ;        /* RCode from things            */

   *r_out = 0;

   s_alloc(&dummy,1);
   s_put(dummy,'\0',1);
   s_smp_ul(dummy) = 1;

   rc = s_pre(&dummy,dummy,str_in,beg,len);
   cstr = s_buffer(dummy);             /*                              */
   if ( rc == 0 )
   {
      /*+-------------------------------------------------------------+*/
      /*| Here we have to locate the first numeric for sscan.         |*/
      /*+-------------------------------------------------------------+*/
      while                            /*                              */
      (                                /*                              */
         NOT OS_ISNUM(*cstr)           /* While not a number or NUL    */
         && ( *cstr |= '\0' )          /* or not the end (\0) of cstr  */
      )                                /*                              */
         cstr++;                       /*    ...point to next          */
                                       /*                              */
      if ( cstr > s_buffer(dummy) )    /* If not at beginning          */
         if ( *(cstr-1) == '-' )       /*    ...if preeceded by minus  */
            cstr--;                    /*       ...then include minus  */
                                       /*                              */
      /*+-------------------------------------------------------------+*/
      /*| Now do it|                                                  |*/
      /*+-------------------------------------------------------------+*/

      rc = sscanf(cstr,"%f",r_out);
      if ( rc != 1 )
      {
         rc = S_BAD_NUM;
      }
      else                             /*                              */
      {
         rc = 0;                       /* Because it looks OK          */
      }
   }

   s_drop(&dummy);
   return(rc);
}
