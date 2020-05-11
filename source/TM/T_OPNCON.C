/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_opncon.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 26, 1988                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   server_id   String     INPUT  Server to connect to.    |
  |  :PARAM.   server_type Sint       INPUT  Type of server.          |
  |                                                                   |
  |  :PURPOSE. To establish a connection between this server and      |
  |            another.                                               |
  |                                                                   |
  |  :OUTPUT.  sint.  0 if connection established.                    |
  |                   ISCFER if error in connection.                  |
  |                                                                   |
  |  :CALLS.   isf_open(), t_iscf_t().                                |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isf.h"
#include "exc.h"



sint   t_opncon(  server_id,  server_type  )

string            server_id;
sint              server_type;


{

sint   rc = 0;





   rc = isf_open( server_id, rc, &rc );
   if( rc == 0 )
   {

      t_iscf_t( server_id, server_type );

   }
   else
   {

      switch( rc % 100 )
      {

         case ERR_CON_TIME_OUT : rc = Connection_Timeout;
                                 break;

         case ERR_ALREADY_CONN :
         case ERR_CONN_EXIST   : rc = Connection_Already_In_Progress;
                                 break;

         default               : rc = Internal_ISCF_Error;
                                 break;


      }
   }


   return(rc);

}
