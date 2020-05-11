/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    soidi_next()                                           |
  |                                                                   |
  |  :AUTHOR.  Rana Ajaz-Ur-Rehman                                    |
  |                                                                   |
  |  :DATE.    April 7, 1989                                          |
  |                                                                   |
  |  :LEVEL.   DBM                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   soidi_oid   OID                                        |
  |  :PARAM.   lfd         hword                                      |
  |  :PARAM.   goid        sint                                       |
  |  :PARAM.   obj_type    hword                                      |
  |                                                                   |
  |  :PURPOSE. Get the next SOIDI.                                    |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.                                                         |
  |                                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
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
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_prim.h"




sint soidi_ne(  oid_ptr,  lfd,  goid,  obj_type )

OID     **oid_ptr;
hword    *lfd;
sint     *goid;
hword    *obj_type;

{

   OID   *temp_oid_ptr  = NIL;


   OCB   *soidi_ocb     =  NIL;

   OCD   *psi_ocd       =  NIL,
         *soidi_ocd     =  NIL;

   sint  rc             =  0;

   boolean DONE         =  FALSE;


#D_begin( SOIDI_NE, DBM);

if( oid_ptr != NIL )
{


   soidi_ocb = t_sysndx( SOIDI_OBJ );
   #D_show( 'soidi_ocb_ptr=0x%X', 'soidi_ocb' );

   if( OCB_POINTER_OK ( soidi_ocb ) )
   {
      ocd_new(  &soidi_ocd, "SOIDI.INDEX.OID" );
      soidi_ocd->bias = 1;

      if( OID_POINTER_OK( *oid_ptr ) )
      {

         #D_say( 'Using provided OID' );
         temp_oid_ptr = *oid_ptr;

      }
      else
      {

         #D_say( 'Allocating OID' );
         temp_oid_ptr = ( OID * )os_mema( sizeof( OID ) );
         os_memn( temp_oid_ptr, sizeof( OID ) );
         *oid_ptr = temp_oid_ptr;

      }

      #D_oid( '(temp_oid_ptr)', 'oid to be overwritten by next' );

      oid_key( soidi_ocd, temp_oid_ptr );
      rc = obj_get( soidi_ocb, soidi_ocd, temp_oid_ptr );

      #D_oid( '(temp_oid_ptr)', 'next oid' );
      #D_show( 'rc of obj_get = %d', 'rc' );

      if( rc == 0 )
      {

         soidi_ocd->bias = 0;

         s_init ( &(soidi_ocd->name), "SOIDI.INDEX.LFD"  );
         obj_get( soidi_ocb, soidi_ocd, lfd  );

         s_init ( &(soidi_ocd->name), "SOIDI.INDEX.GOID" );
         obj_get( soidi_ocb, soidi_ocd, goid );

         s_init ( &(soidi_ocd->name), "SOIDI.INDEX.TYPE" );
         obj_get( soidi_ocb, soidi_ocd, obj_type );


      }  /* rc == 0 */
      else
      {

         rc = NO_DATA_FOUND;

      }

   }  /* ocb_ponter ok soidi_ocb */
   else
   {

      rc = INVALID_OCB;

   }


} /* pointer passed is nil */
else
{

   rc = INVALID_OID_POINTER;

}

#D_leave( rc, 'Done with soidi_next' );

}

