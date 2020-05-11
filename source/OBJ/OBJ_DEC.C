/********************************************************************
<DOC>
<NAME>             obj_dec.C
<LEVEL>            OBJ
<AUTHOR>           John L. Foster
<DATE>             December 30, 1987
<PARAM>            packet    string       INPUT Teies string containing
                                                buffer of OCD's

<PARAM>            ocb       struct OCB * INPUT Pointer to TEIES OCB to
                                                put components into. Used
                                                only if the OID specified
                                                in the packet does not
                                                exist or is NIL.

<PARAM>            obj_type  hword *     OUTPUT Type of object received.

<PARAM>            oid       OID **      OUTPUT Resulting oid.  This is a
                                                pointer to the OCB OCD so
                                                if you need it longer
                                                than it is open, copy it.

<PARAM>            ok_to_link boolean     INPUT TRUE if this object is to
                                                to run its link action.

<PARAM>            result                OUTPUT Where to put a result str
                                                if one is generated.

<OUTPUT>           rc        sint               Result code of operation.
                                                0 if normal completion.
<PURPOSE>          Given a string containing the OCD packet format
                   described in the documentation in T_objenc.c, update
                   the object indicated by the object control block.

                   If the OCB is NIL a new object will be allocated.
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"



#define OBJECT_NOT_FOUND         0
#define OCD_COUNT_FIELD          8
#define OBJECT_TYPE_FIELD       10
#define SEQUENCE_FIELD          12
#define FIRST_OCD_FIELD         14





sint       obj_dec(  packet, ocb, obj_type, oid, ok_to_link, result  )

string               packet;
OCB                         *ocb;
hword                            *obj_type;
OID                                       **oid;
boolean                                          ok_to_link;
string                                                      *result;




{


sint    rc             =      0,
        i              =      0,
        bias           =      0,
        data_len       =      0;
string  current_ptr    = S_NULL,
        contents       = S_NULL,
        modifier       = S_NULL;
hword   object_type    =      0,
        seq_cntr       =      0,
        data_type      =      0,
        number_of_ocds =      0,
        level          =      0;
OID    *packet_oid     =    NIL,
       *temp           =    NIL;
OCB    *packet_ocb     =    NIL;
OCD    *ocd            =    NIL;
ODT    *odt_entry      =    NIL;






if( packet != S_NULL && s_len( packet ) >= 12 )
{

   object_type = *( hword * )(s_buffer( packet ) + OBJECT_TYPE_FIELD);
   *obj_type = object_type;
   packet_oid = ( OID * ) s_buffer( packet );
   packet_ocb = obj_opn( packet_oid, OBJ_UPDATE );
   if( ! OCB_POINTER_OK( packet_ocb ) )
   {

       packet_ocb = ocb;

   }

   if( ! OCB_POINTER_OK( packet_ocb ) )
   {

      packet_ocb  = obj_all( object_type, REMOTE_OBJECT, packet_oid );

   }

   if( ! OCB_POINTER_OK( packet_ocb ) )
   {

      except( CANNOT_DETERMINE_OID,CONTROL_ERROR, packet, S_NULL, S_NULL,
                                                         S_NULL, S_NULL );
   return(CANNOT_DETERMINE_OID);

   }
   temp = *oid = &( packet_ocb->oid );
   seq_cntr       = *( hword * )( s_buffer( packet ) + SEQUENCE_FIELD  );
   number_of_ocds = *( hword * )( s_buffer( packet ) + OCD_COUNT_FIELD );
   current_ptr = (string) s_buffer( packet ) + FIRST_OCD_FIELD;
   for( i=0; i<number_of_ocds; i++ )
   {

      ocd = ocd_new ( &ocd, NIL );
      s_copy ( &(ocd->name), current_ptr, 0, 0 );

      /* HOLD IT!! If there is a index key in the OCD, we deal with it */
      /* Dummy routine right now, returns 0 always.                    */

      rc = ocd_dcd( ocd );
      contents = (string)s_buffer( current_ptr )+s_allen( current_ptr );
      odt_entry = odt_find( packet_ocb, ocd );
      if( ODT_POINTER_OK( odt_entry ) )
      {

         if( odt_type( odt_entry ) != OC_STR )
         {

            data_len = odt_len( odt_entry, packet_ocb->type );
            bias = 0;

         }
         else
         {

            data_len = s_allen( (string)contents );
            bias = 4;

         }
         rc = obj_rep( packet_ocb, ocd, contents, SEG_UPDATE );

         current_ptr = (string) ( (byte *)contents + data_len + bias );

      }
   }
   if( ok_to_link )
   {

      rc = t_jmpst( object_type, LINK_ACT, level, modifier, packet_ocb,
                                                      (byte *) result  );
   }
}
else
{

   rc = INVALID_OCD_PACKET;

}



   return(rc);

}
