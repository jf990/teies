/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_copy()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 03, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   target_ocb       OCB * INPUT  The object that will get |
  |                                          the results of the copy. |
  |  <PARAM>   target_ocd       OCD * INPUT  Where in the object to   |
  |                                          put the results.         |
  |  <PARAM>   source_ocb       OCB * INPUT  Object containing the    |
  |                                          data to be copied.       |
  |  <PARAM>   source_ocd       OCD * INPUT  Location in the object of|
  |                                          data to copy.            |
  |                                                                   |
  |  <PURPOSE> Copy object components from the source object into the |
  |            destination object.  The resulting hierarchy of object |
  |            segment structure must match.  There are a few tricks: |
  |                                                                   |
  |            . If both OCD's are NIL then copy the entire source    |
  |              object to the destination object.  The object types  |
  |              of both objects must be the same.                    |
  |                                                                   |
  |            . If either of the objects are an operating system file|
  |              then its respective OCD is ignored.  If the source   |
  |              is an OS File then the destination will get each     |
  |              record copied and formatted in the standard BSE fmt. |
  |              See ocd_bse().  If the destination object is an OS   |
  |              File then this routine will convert each segment into|
  |              record format for the external file.                 |
  |                                                                   |
  |            . Only components matching the invoking users privilege|
  |              are actually copied.                                 |
  |                                                                   |
  |            . Destination components are overwritten if they exist.|
  |                                                                   |
  |            Logic table based on input arguments:                  |
  |                                                                   |
  |       OCB       |      OCD         |        Function              |
  | target | source | target | source  |                              |
  |-------------------------------------------------------------------|
  | OS_FILE|OS_FILE |  NIL   |  NIL    |    COPY_FILE_TO_FILE  os_fcpy|
  | OS_FILE|OS_FILE |  NIL   | comp't  |          X                   |
  | OS_FILE|OS_FILE | comp't |  NIL    |          X                   |
  | OS_FILE|OS_FILE | comp't | comp't  |          X                   |
  | OS_FILE| object |  NIL   |  NIL    |          X                   |
  | OS_FILE| object |  NIL   | comp't  |    COPY_OBJECT_TO_FILE ocd_gfd
  | OS_FILE| object | comp't |  NIL    |          X                   |
  | OS_FILE| object | comp't | comp't  |          X                   |
  |  object|OS_FILE |  NIL   |  NIL    |          X                   |
  |  object|OS_FILE |  NIL   | comp't  |          X                   |
  |  object|OS_FILE | comp't |  NIL    |    COPY_FILE_TO_OBJECT ocd_bse
  |  object|OS_FILE | comp't | comp't  |          X                   |
  |  object| object |  NIL   |  NIL    |    COPY_OBJECT_TO_OBJECT obj_pac
  |  object| object |  NIL   | comp't  |          X                   |
  |  object| object | comp't |  NIL    |          X                   |
  |  object| object | comp't | comp't  |    COPY_COMPONENT_TO_COMPONENT
  |-------------------------------------------------------------------|
  |                                                                   |
  |  <OUTPUT>  sint    result code.                                   |
  |  <CALLS>   obj_pack(), obj_unpack(), ocd_new(), ocd_drp(), s_drop()
  |            os_fcpy(), ocd_bse(), ocd_gfd(), dms_copy()            |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



#define         COPY_FILE_TO_OBJECT              5
#define         COPY_FILE_TO_FILE                4
#define         COPY_OBJECT_TO_FILE              3
#define         COPY_OBJECT_TO_OBJECT            2
#define         COPY_COMPONENT_TO_COMPONENT      1



sint   obj_copy(  target_ocb, target_ocd,
                  source_ocb, source_ocd   )

OCB             * target_ocb,
                * source_ocb;
OCD             * target_ocd,
                * source_ocd;


{

   sint     rc                   =   0,
            index                =   0,
            function_matrix[ ] = { COPY_FILE_TO_FILE,   0, 0, 0,
                                     0, COPY_OBJECT_TO_FILE, 0, 0,
                                     0, 0, COPY_FILE_TO_OBJECT, 0,
                                     COPY_OBJECT_TO_OBJECT,  0, 0,
                                     COPY_COMPONENT_TO_COMPONENT       };

   string   packet               = S_NULL;
   OCD    * pack_ocd             =    NIL;
   hword    dms_copy_flags       =      0;






if( OCB_POINTER_OK( source_ocb )  &&  OCB_POINTER_OK( target_ocb ) )
{

   if( target_ocb->type != 0 )
   {

      index += 8 ;

   }
   else
   {

      target_ocd = NIL ;

   }

   if( source_ocb->type != 0 )
   {

      index += 4 ;

   }
   else
   {

      source_ocd = NIL ;

   }

   if( target_ocd != NIL )
   {

      index += 2 ;

   }
   if( source_ocd != NIL )
   {

      index += 1 ;

   }


   switch( function_matrix[ index ] )
   {

      case COPY_FILE_TO_FILE           :

           rc = os_fcpy( source_ocb->gfd, target_ocb->gfd );
           break;

      case COPY_OBJECT_TO_OBJECT       :


           ocd_new(  & pack_ocd, "%LINKS" );
           obj_pack( & packet, source_ocb, pack_ocd );
           os_memc(  & (target_ocb->oid), s_buffer( packet ) + 2,
                        sizeof( OID ) );
           rc = obj_unpack( & (target_ocb), packet );
           s_drop(  & packet   );
           ocd_drp( & pack_ocd );
           break;

      case COPY_COMPONENT_TO_COMPONENT :


           rc = dms_copy( source_ocb->dmscb, target_ocb->dmscb,
                          dms_copy_flags                        );
           if( rc == DMS_COPY_OK )
           {
              rc = 0;
           }
           break;

      case COPY_OBJECT_TO_FILE         :


           rc = ocd_gfd( source_ocb, source_ocd, target_ocb );

           break;

      case COPY_FILE_TO_OBJECT         :


           rc = ocd_bse( target_ocb, target_ocd, source_ocb );

           break;

      default                    :

           rc = FUNCTION_NOT_SUPPORTED ;
           break;

   }
}
else
{

   rc = INVALID_OCB ;

}


   return(rc);

}
