/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_sdeli()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 24, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB **     INPUT  description              |
  |  <PARAM>   object_type hword      INPUT  description              |
  |  <PARAM>   level       hword      INPUT  description              |
  |  <PARAM>   object_id   OID **     INPUT  description              |
  |  <PARAM>   date        datetime **INPUT  description              |
  |  <PARAM>   who         OID **     INPUT  description              |
  |                                                                   |
  |  <PURPOSE> Interface to the System Deletion Index of deleted      |
  |            objects.  See SDELI OBJECT definition.  Performs       |
  |            functions based on the level parameter as follows:     |
  |                                                                   |
  |            SDELI_FIND_BY_OBJECT_TYPE:                             |
  |               Given object_type returns an OCB if a deleted object|
  |               of that type is available in the index.  If not the |
  |               return code will indicate so.  If an OCB can be re- |
  |               turned then date and who will be filled in if and   |
  |               only if they are valid pointers.                    |
  |                                                                   |
  |            SDELI_FIND_OID_ONLY:                                   |
  |               Given object_type returns only an OID if an object  |
  |               matching is on the deletion queue.                  |
  |                                                                   |
  |            SDELI_FIND_BY_DELETOR:                                 |
  |               Given an OID to match the WHO field, return the     |
  |               first entry deleted by the specified OID.  This re- |
  |               turns an open object and the same parameter fill in |
  |               rules apply as above.                               |
  |                                                                   |
  |            SDELI_FIND_BY_DATE:                                    |
  |               Given a date to match the DATETIME field, return the|
  |               first entry deleted by the specified date. This re- |
  |               turns an open object and the same parameter fill in |
  |               rules apply as above.                               |
  |                                                                   |
  |            SDELI_ADD_TO_INDEX:                                    |
  |               Given an OID and Object Type, add the entry to the  |
  |               deletion queue.                                     |
  |                                                                   |
  |            SDELI_REMOVE_FROM_INDEX                                |
  |               Given an OID and Object Type, remove the entry from |
  |               the deletion queue.                                 |
  |                                                                   |
  |            SDELI_REMOVE_ALL_ENTRIES                               |
  |               Requires no parameters, empties the deletion queue  |
  |               and frees all objects marked for deletion.          |
  |                                                                   |
  |            SDELI_QUERY_QUEUE_SIZE                                 |
  |               Requires no parameters, returns the number of en-   |
  |               tries in the deletion queue.                        |
  |                                                                   |
  |            Deletion queues (indexes) are only by one server, so   |
  |            this routine implies no interserve communication.      |
  |                                                                   |
  |            When an open object is returned it is removed from the |
  |            deletion queue!  It must be removed again to place it  |
  |            back on the deletion queue.                            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>  None known                                             |
  |  <LIMITS>  None known                                             |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint obj_sdeli( object, object_type, level, object_id,
                                          date_of_deletion, who_deleted )

OCB           **object;
hword           object_type, level;
OID           **object_id, **who_deleted;
datetime      **date_of_deletion;

{

   sint        rc         =      0;
   hword       type_found =      0;
   OCB       * sdeli      =    NIL;
   OCD       * component  =    NIL;
   OID       * deletor    =    NIL;
   datetime    dt                 ;




sdeli    = t_sysndx( SDELI_OBJ );

if( OCB_POINTER_OK( sdeli ) )
{

   ocd_new( & component, NIL );
   s_alloc( & (component->key), 10 );
   os_memc( & object_type, s_buffer( component->key ),     2 );

   if( OID_POINTER_OK( * object_id ) )
   {

      os_memc( * object_id, s_buffer( component->key ) + 2, 8 );

   }
   else
   {

      os_memn( s_buffer( component->key ) + 2, 8 );

   }
   s_smp_ul( component->key ) = 10;

   switch( level )
   {

      case SDELI_FIND_BY_OBJECT_TYPE    :

         component->bias = 1;
         s_init( & (component->name), "SDELI.INDEX.DATETIME" );
         rc = obj_get( sdeli, component, & dt );
         if( rc == 0 )
         {

            type_found = * ( (hword *) s_buffer( component->key ) );
            if( object_type == type_found )
            {


            }
         }
         break;

      case SDELI_FIND_OID_ONLY          :

         break;

      case     SDELI_FIND_BY_DELETOR    :

         rc = FUNCTION_NOT_IMPLEMENTED;
         break;

      case     SDELI_FIND_BY_DATE       :

         rc = FUNCTION_NOT_IMPLEMENTED;
         break;

      case     SDELI_ADD_TO_INDEX       :

         os_dttm( & dt );
         deletor = & (TM_master->ucb->user_id);
         s_init( & (component->name), "SDELI.INDEX.DATETIME"    );
         obj_rep( sdeli, component,   & dt,          SEG_HOLD   );
         s_init( & (component->name), "SDELI.INDEX.WHO"         );
         rc = obj_rep( sdeli, component,    deletor, SEG_UPDATE );
         break;

      case     SDELI_REMOVE_FROM_INDEX  :

         s_init( & (component->name), "SDELI.INDEX" );
         rc = obj_rmv( sdeli, component );
         break;

      default                           :

         rc = OBJ_LEVEL_UNSUPPORTED;
         break;

   }
   ocd_drp( & component );

}
else
{

   /* rc = obj_del( object->oid, FORCE_DELETION ); */

}



   return(rc);

}
