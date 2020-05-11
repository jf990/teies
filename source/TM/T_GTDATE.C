/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                 T_gtdate()
<LEVEL>                TM
<AUTHOR>               John L. Foster
<DATE>                 July 24, 1988
<PARAM>                result    string *     String to contain the date
                                              and time result.
<PARAM>                date_time datetime *   Pointer to a date time stru
                                              cture to format.  NIL or -1
                                              to ignore and use the date
                                              and time it currently is.
<GLOBAL>               TM_master->ucb
<OUTPUT>               sint                   Return completion code
<PURPOSE>              Return the current date and time as a string
                       formatted in the invoking user's specified format.

                       If the datetime pointer points to a date and time
                       structure use that instead of the current datetime
</DOC>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"



sint     t_gtdate(  result, specified_date  )

string          *result;
datetime        *specified_date;


{


sint    rc        =      0;
boolean allocated =  FALSE;




   if( specified_date == NIL || specified_date == (datetime *) -1 )
   {

      specified_date = ( datetime * ) os_mema( sizeof( datetime ) );
      allocated = TRUE;
      os_dttm( specified_date );

   }


   if( TM_master != NIL && TM_master->ucb != NIL  &&
       TM_master->ucb->dtform != S_NULL                 )
   {

      s_dttos( result, specified_date, TM_master->ucb->dtform );

   }
   else
   {

      s_mtoxs( result, (byte *) specified_date, sizeof( datetime ) );

   }

   if( allocated )
   {

      os_memd( specified_date );

   }



   return(rc);

}
