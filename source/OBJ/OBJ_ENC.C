/********************************************************************
<DOC>
<NAME>             obj_enc.C
<LEVEL>            OBJ
<AUTHOR>           John L. Foster
<DATE>             December 30, 1987
<PARAM>            packet    string *     INOUT Pointer to TEIES String
                                                buffer to place contents
                                                of OCD's onto.

<PARAM>            ocb       struct OCB * INPUT Pointer to TEIES OCB to
                                                get components from.

<PARAM>            ocd       struct OCD * INPUT Pointer to TEIES OCD of
                                                component to place in
                                                packet.

<OUTPUT>           rc        sint               Result code of operation.
                                                0 if normal completion.

<PURPOSE>          Add an object component and contents of the component
                   into a packet.  The packet should be destined to
                   another server once completely filled in.

                   If the packet input string is empty or S_NULL, it will
                   be formatted with the proper header material. If there
                   already are components inside the packet that belong
                   to this same object, the new OCD's will be appended.If
                   the OID's conflict the packet will be cleared and the
                   new OCD and contents will be placed in the new packet.

                   If the packet is already full or if the addition of
                   the new component goes over the maximum packet size,an
                   error code will be returned and the component will not
                   be entered into the packet.  The maximum packet size
                   is defined by the compile time constant T_PACKET_SIZE.

                   The format of a packet is:

 +======+=====+====+===+====+========+====+========+=====+====+=========+
 ||     |     |    |   |    |        |    |        |     |    |        ||
 || OID | OCD |Obj |Seq|OCD1|Contents|OCD2|Contents| ... |OCDn|Contents||
 ||     |Count|Type|Cnt|    |  OCD1  |    |  OCD2  |     |    |  OCDn  ||
 +======+=====+====+===+====+========+====+========+=====+====+=========+

                    OID is the 8 byte TEIES Object ID format.

                    OCD count is a halfword integer count of the total
                    number of Object Components in the packet.

                    ObjType is the Object type code for the Object
                    associated to the OID.  If the OID exists, the type
                    value in the index (SOIDI) overrides this value.
                    This field is 2 bytes (half word).

                    SeqCnt (SequenceCount) is a two byte sequence counter
                    used to indicate a sequence of packets belonging to
                    the same operation.  If SeqCnt is 0 or -1 then this
                    is the first and last packet.  Any other value is the
                    sequence count for multiple packets.  Checksums and
                    transmission errors are not part of this level and
                    should be handled by ISCF.

                    OCD Contents are TEIES string structures
                    containing the Object Component Descriptor and
                    component contents, respectively.

                    The routine to decode (un-packet) these packets is
                    obj_dec().

                    A valid packet contains at least the OID, OCD count,
                    Object Type, and Sequence Count; there do not have to
                    be any OCD's in the packet (OCD count = 0).

<CALLS>  s_catx(), s_catb(), os_memc()

<ERRORS>            0 : Sucessful.
                48756 : OID incompatibility.
                48757 : Packet full, component not added.
                20348 : OCD not part of the object requested.
                20023 : Object Get returned no data.
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint             obj_enc( packet,  ocb,  ocd )

string                   *packet;
OCB                               *ocb;
OCD                                     *ocd;


{


sint    rc            = 0,
        packet_length = 0,
        data_len      = 0,
        data_type     = 0;
byte   *results       = NIL;
ODT    *odt_entry     = NIL;
hword   ocd_count     = 0,
       *ocd_cnt_ptr   = NIL,
        seq_id        = 0,
        obj_type      = 0;





if( OCB_POINTER_OK( ocb ) && OCD_POINTER_OK( ocd ) )
{
                                                 /**********************/
   results = os_mema( 10 );                      /*                    */
   os_memn( results,  10 );                      /*                    */
   odt_entry = odt_find( ocb, ocd );             /* First, see if there*/
   if( odt_entry != NIL )                        /*is any data to put  */
   {                                             /*in the packet.  If  */
                                                 /*not, or if the OCD/ */
      data_len = odt_len( odt_entry, ocb->type );/*OCB is wrong, we can*/
      data_type= odt_entry->type;                /*do nothing except   */
      rc = obj_get( ocb, ocd,  results );        /*return.             */
      if( rc == 0 )                              /*                    */
      {                                          /**********************/
                          /*+-----------------------------------------+
                            |Attempt to determine if the packet passed|
                            |in is already a valid packet.  If not,   |
                            |initialize it with a valid packet header |
                            +-----------------------------------------+*/
         packet_length = s_len( *packet );
         if( packet_length < 14 || packet_length > 4000 )
         {

            byte * pointer = NIL;


            packet_length = 0;
            s_alloc( packet, 128 );
            pointer = (byte *) s_buffer( *packet );
            os_memc( &(ocb->oid), pointer+0, sizeof( OID ) );
            *((hword *)(pointer+8 ))= ocd_count;
            *((hword *)(pointer+10))= ocb->type;
            *((hword *)(pointer+12))= seq_id;
            s_smp_ul( *packet )     = 14;

         }
                          /*+-----------------------------------------+
                            |Copy in the OCD as a TEIES string, and   |
                            |correct its allocated length.            |
                            +-----------------------------------------+*/

         s_catx( packet, ocd->name, 0, 0 );
         if( data_type != OC_STR )
         {

                          /*+-----------------------------------------+
                            |If the data is not type String, then copy|
                            |in the data for the length its type re-  |
                            |quires.                                  |
                            +-----------------------------------------+*/

            s_catb( packet, results, data_len );

         }
         else
         {

                          /*+-----------------------------------------+
                            |If the data is String type, we must copy |
                            |in the whole TEIES string and correct the|
                            |allocated length.                        |
                            +-----------------------------------------+*/

            s_catx( packet, *(string *)results, 0, 0 );

         }

                          /*+-----------------------------------------+
                            |Correct the used length of the packet to |
                            |reflect the new data.  Then increment the|
                            |total number of OCD's in the packet.     |
                            +-----------------------------------------+*/

         ocd_cnt_ptr = (hword *)(s_buffer(*packet)+8);
         ocd_count = *ocd_cnt_ptr;
         ocd_count++;
         *ocd_cnt_ptr = ocd_count;

         rc = 0;

      }
   }
   else
   {

      rc = OCD_NOT_IN_ODT;

   }
   os_memd( results );

}
   return(rc);

}
