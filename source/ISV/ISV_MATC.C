/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_match()                                            |*/
/*|                                                                   |*/
/*|  :AUTHOR.  John L. Foster                                         |*/
/*|  :DATE.    May 19, 1988                                           |*/
/*|  :LEVEL.   ISV                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   phrase_in   string   INPUT    Word or phase to check   |*/
/*|                                           if in the ISV table.    |*/
/*|  :PARAM.   start_isv   isv      INPUT    ISV to start search      |*/
/*|  :PARAM.   end_isv     isv      INPUT    ISV to stop search       |*/
/*|                                                                   |*/
/*|  :PURPOSE.      Checks to see if the input string is in the ISV   |*/
/*|                 table.  The checking starts at the ISV indicated  |*/
/*|                 by start_isv and ends at the ISV denoted by       |*/
/*|                 end_isv.  If end_isv is 0 then only start_isv is  |*/
/*|                 checked.  The result returns 0 if no match (input |*/
/*|                 is not an ISV) or the ISV key number of the       |*/
/*|                 matching ISV.                                     |*/
/*|                                                                   |*/
/*|                 Example usage: assuming you wanted to check user  |*/
/*|                 input from a known list of ISVs and printout the  |*/
/*|                 full ISV word the user selected:                  |*/
/*|                                                                   |*/
/*|                 isv_get( &output,isv_match( input, ISV1, ISV3),0);|*/
/*|                 s_copy( &prt_str, output, 0, 0 );                 |*/
/*|                                                                   |*/
/*|                 This will check the input string against ISVs     |*/
/*|                 ISV1 ... ISV3 and retrun the ISV number of the    |*/
/*|                 matching one.  This result is passed to ISV_GET   |*/
/*|                 to gain access to the full valid ISV string.  If  |*/
/*|                 isv_match() returned 0 then the result will be a  |*/
/*|                 null string (S_NULL).                             |*/
/*|                                                                   |*/
/*|  :OUTPUT.  isv  0 if no match.                                    |*/
/*|            isv  ISV_KEY if a matching ISV is found.               |*/
/*|            isv  < 0 if error.                                     |*/
/*|                                                                   |*/
/*|  :CALLS. isv_get()                                                |*/
/*|                                                                   |*/
/*|  :ERRORS.  ISV_NL, ISV_IOB, ISV_NF                                |*/
/*|                                                                   |*/
/*|  :LIMITS.  Should s_drop() "*tstr" before setting to ISV pointer  |*/
/*|            (after appropriate validation) because if it points to |*/
/*|            a string, then that memory will be permanently lost to |*/
/*|            a given instance of execution.                         |*/
/*|                                                                   |*/
/*|            Could be speeded up if it examined the ISV tables      |*/
/*|            itself instead of calling isv_get().                   |*/
/*|                                                                   |*/
/*|  :MODS.                                                           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "isv.h"
#include "osk.h"
#include "debug.h"




isv isv_match( string  phrase_in, isv start_isv, isv end_isv  )
{

   sint     isv_key       =     0;
   sint     holder        =     0;
   sint     dummy         =     0;
   isv      our_rc        =     0;
   string   isv_string    = S_NUL;



   #D_start('isv_match','ISV','ptr=0x%X; start=%d; end=%d','phrase_in,\
                               start_isv, end_isv' );
   #D_str( phrase_in, 'Word or phrase to match' );



   if( end_isv == 0 )
   {

      end_isv = start_isv;

   }
   else if( end_isv < start_isv )
   {

      holder    = start_isv;
      start_isv = end_isv;
      end_isv   = holder;

   }
   for( ; start_isv <= end_isv; start_isv++ )
   {

      our_rc = isv_get( &isv_string, start_isv, 0 );
      if ( our_rc < 0 )
      {
         if ( our_rc == ISV_NOT_FOUND )
         {
            our_rc = 0;
         }
         else
         {
            #D_e_say('Bad RC from isv_get()');
            break;
         }
       }
       else if ( s_comp(isv_string, phrase_in, 0, 0, 'F', &dummy) == 0 )
       {

          our_rc = start_isv;
          break;

       }
   }


   #D_leave(our_rc,'positive number = ISV id // zero=notfound');
}
