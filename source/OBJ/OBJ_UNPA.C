/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_unpack                                             |
  |  <AUTHOR>  Tanmay S Kumar                                         |
  |  <DATE>    June 7, 1988                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   ocb     struct OCB   INPUT    Pointer to the OCB that  |
  |                                          contains the pointer to  |
  |                                          the ODT                  |
  |  <PARAM>   data      string     INPUT                             |
  |                                                                   |
  |  <PURPOSE> To receive an object from a specified user id and to   |
  |            send a confirmation when the data is sucessfully un-   |
  |            packed.  See obj_pack() for a description of the packet|
  |            format.                                                |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"



sint obj_unpack(  ocb, data  )

OCB            ** ocb;
string            data;

{


 sint      rc          =      0,
           i           =      0;
 hword     type        =      0,
           counter     =      0,
           len         =      0;
 byte     *current_ptr =    NIL;
 string    curpointer  = S_NULL;
 OCB      *new_ocb     =    NIL;
 OID      *oid         =    NIL;

 struct    dms_cb *dmscb  = NIL;








 type    = *(hword *) ( s_buffer(data)      );
 oid     =    (OID *) ( s_buffer(data) +  2 );
 counter = *(hword *) ( s_buffer(data) + 10 );




new_ocb = obj_opn( oid, OBJ_UPDATE );

if ( ! OCB_POINTER_OK( new_ocb ))
{

    new_ocb = obj_all( type, LOCAL_OBJECT, oid );

}
else
{

   if( obj_test_mode( new_ocb, OBJ_CACHE_PENDING ) )
   {

      rc = ocb_segflush( new_ocb, OCB_CACHE_CLEAR );

   }
}

if( OCB_POINTER_OK ( new_ocb ) )
{

   current_ptr = (s_buffer(data) + 16) ;
   curpointer  = (string) current_ptr;

   while (  (counter > 0)   &&   (rc == 0)  )
   {

      len = (hword) ( s_len( curpointer ));
      if( len > 0 )
      {

         rc = dms_unpk( new_ocb->dmscb, s_buffer( curpointer ), len, NIL );
         if( rc == DMS_UNPACK_OK )
         {
            rc = 0;
         }
      }
      current_ptr += s_smp_al( curpointer ) + S_OVERHEAD;
      curpointer   = (string) current_ptr;
      counter     -= 1;

   }
   if ( rc == 0 )
   {

                             /*+--------------------------------------+
                               |Close-flush the object to assure the  |
                               |current view is committed in the data |
                               |base.  We must tweek the open count   |
                               |because the caller assumes obj_unpack |
                               |simulates an open and the close-flush |
                               |will act as a close and decrement the |
                               |open counter.                         |
                               +--------------------------------------+*/
      new_ocb->open_count ++ ;
      obj_set_mode( new_ocb, OBJ_FLUSH );
      rc = obj_cls( new_ocb );
      if( ocb != NIL )
      {

         *ocb = new_ocb;

      }
   }
   else
   {

       rc = DMS_UNPACK_FAILED;

   }
   s_drop( & ((*ocb)->lastdbp) );

}
else
{

   rc = CANNOT_CREATE_OBJECT;

}


   return(rc);
}
