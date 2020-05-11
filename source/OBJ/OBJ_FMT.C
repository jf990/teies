/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_fmt()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 14, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   lfd         hword      INPUT  LFD to format.           |
  |  <PARAM>   num_blocks  sint       INPUT  Size of the LFD in blocks|
  |                                                                   |
  |  <PURPOSE> Format the requested logical file.                     |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_fmt()                                              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "exc.h"



sint   obj_fmt( lfd, number_of_blocks )

hword           lfd;
sint            number_of_blocks;


{

sint   rc         =      0;
string index_name = S_NULL;







rc  =  dms_fmt( lfd, number_of_blocks );

if( rc != DMS_FMT_OK )
{



                       /* - - - - - - - - - - - - - - - - - - - - - - -*/
                       /* DO THE FOLLOWING ONLY IF CREATING THE FIRST  */
                       /* LOGICAL FILE FOR A TEIES SERVER.             */
                       /* - - - - - - - - - - - - - - - - - - - - - - -*/
}
else if( lfd == 1 )
{



   index_name = obj_name( SOIDI_OBJ );
   rc = psi_add( SOIDI_OBJ,  index_name  );


                                                 /* - - - - - - - - - */
}                                                /* End of LFD=1      */
                                                 /* dependent code.   */
                                                 /* - - - - - - - - - */

   return(rc);

}
