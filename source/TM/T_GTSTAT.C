/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_gtstat                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 23, 1990                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   user_object  OCB    *  INPUT  Who to get record of.    |
  |  <PARAM>   stat_entry   string *  INOUT  Where to return statistic|
  |                                           information structure.  |
  |  <PARAM>   flag         sint      INPUT  Type of data to return.  |
  |  <PURPOSE> Get the last statistic entry for the current user.  The|
  |            record is assumed to reside on the Master Server.      |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "obj_user.h"




sint   t_gtstat(  OCB * user_object, string * stat_entry, sint flag  )
{

   sint        rc          =      0;
   boolean     opened_here =  FALSE;
   OCB       * stat_ocb    =    NIL;
   OCD       * ocd         =    NIL;
   OID         stat_list           ;







if( ! OCB_POINTER_OK( user_object ) )
{

   user_object = t_author();
   opened_here = TRUE      ;

}
if( OCB_POINTER_OK( user_object )  &&  user_object->type == USER_OBJ )
{

   if( TM_master->usrtype != MASTER_SERVER )
   {

     rc = t_remote( TM_master->MS, 0, 0, TM_GET_LAST_USER_STAT_RECORD,
                                   user_object, S_NULL, stat_entry  );

   }
   else
   {

      if( stat_entry != NIL  &&  stat_entry != (string *) -1 )
      {

         rc = obj_find( user_object, "INDEXES.STATS", & stat_list );
         if( rc == 0  &&  ! OID_IS_NULL( & stat_list ) )
         {

            stat_ocb = obj_opn( & stat_list, OBJ_READ );
            if( OCB_POINTER_OK( stat_ocb ) )
            {

               ocd_new ( & ocd, "LIST.LIST.ENTRY" );
               ocd->bias = -1;
               ocd->key  =  (string) -1 ;
               rc = obj_get( stat_ocb, ocd, stat_entry );
               ocd_drp( & ocd      );
               obj_cls(   stat_ocb );

            }
         }
         else
         {

            rc = TM_CANNOT_STORE_RESULT;

         }
      }
      TM_master->current->except = rc;
      rc = TM_SEND_CONFIRMATION;

   }
}
else
{

   rc = IMPROPER_OBJECT_TYPE;

}

if( opened_here )
{

   obj_cls( user_object );
   user_object = NIL;

}


   return(rc);

}
