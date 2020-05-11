/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    obj_sysndx                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 13, 1987                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. purpose                                                |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
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



sint sysndx( system_index_type, object_component, data, data_length,
             flag,              result                               )

hword           system_index_type;
OCD            *object_component;
byte           *data;
sint            data_length;
hword           flag;
byte          **result;



{

   sint    rc                 =        0,
           state              =        0;
   hword   command            =        0,
           flags              =        0;
   string  packet             =   S_NULL,
           holder             =   S_NULL;






if(  system_index_type < START_SYSTEM_INDEX_RANGE   ||
     system_index_type > END_SYSTEM_INDEX_RANGE    )
{

   rc = NOT_A_SYSTEM_INDEX;

}
else
{

   if( OCD_POINTER_OK( object_component ) )
   {

      if( DATA_POINTER_OK( data ) )
      {

         if( data_length > 0 )
         {

            if( flag == UPDATE_SYSTEM_INDEX )
            {

               command = UPDATE_SYSTEM_INDEX;
               flags   = 0;
               ocd_pack( & packet, object_component );
               s_alloc(  & holder, data_length      );
               os_memc(  s_buffer( holder ), data_length, s_len(holder));
               s_smp_ul( holder ) = data_length;
               s_cat(    & packet, packet, holder, 0, 0 );
               s_drop(   & holder );

            }
            else if( flag == LOCATE_IN_SYSTEM_INDEX )
            {

               command = LOCATE_IN_SYSTEM_INDEX;
               flags   = 0;
               ocd_pack( & packet, object_component );

            }

            if( command != 0 )
            {

               if( ( rc =
                        t_send( TM_master->MS, command, flags, state, packet ))
                    == 0 )
               {

                  rc = t_waitc( TM_master->MS, result );

               }
               s_drop( & packet );

            }
            else
            {

               rc = FUNCTION_NOT_SUPPORTED;

            }
         }
         else
         {

            rc = NO_DATA_PROVIDED;

         }
      }
      else
      {

         rc = INVALID_DATA_POINTER;

      }
   }
   else
   {

      rc = OCD_POINTER_INVALID;

   }
}




   return(rc);

}
