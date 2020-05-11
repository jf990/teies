/********************************************************************
<DOC>
<NAME>             T_CFIRM.c
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             November 29, 1987

<PARAM>            server    string    INPUT    Server who sent us the
                                                confirmation.

<PARAM>            data      string    INPUT    Data packet containing
                                                confirmation information.

<PARAM>            flags     hword     INPUT    Standard tranaction flags
<PARAM>            dummy     byte **   INOUT    Store a result there.

<OUTPUT>           sint      return or completion code from remote server

<PURPOSE>          The server has performed a command for us and is now
                   giving a confirmation that the command is complete.
                   Not to be confused with t_waitc(), which waits for
                   a confirmation.

                   Returns the flags value of the Confirmation packet
                   when the confirmation data packet is empty.  Other-
                   wise the confirmation packet is formatted as follows:

                  +-------+---------+
                  |  rc   |   data  |
                  +-------+---------+

                  where rc is a TEIES 4-byte integer containing the
                  return code of the remote operation.  Data is anything
                  the remote operation decided was important to send
                  back as an indication as to what wne wrong (or right).
                  It is important that the routine that requested the
                  remote operation be in sync for the confirmation.

<LIMITS>         Should OS_MEMC the result and drop the buffer! Then the
                 caller is responsible to drop the result data.
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint     t_cfirm( server,  data,  flags, dummy  )

string            server,
                  data;
hword             flags;
byte            **dummy;



{


sint   rc      = 0,
       level   = 0,
       result_data_length = 0;
string request = S_NULL;




   if( s_len( data ) > 0 )
   {

      rc = *(( tint * )  s_buffer( data ) );

   }
   if( dummy != NIL && s_len( data ) > 4 )
   {

      result_data_length = s_len( data ) - 4;
      if( * dummy == NIL )
      {

         * dummy = os_mema( result_data_length );

      }
      os_memc( s_buffer( data ) + 4, * dummy, result_data_length );

   }
   if( rc == 0 )
   {

      rc = (sint) flags;

   }


   return(rc);

}
