/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    soidi_de                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 03, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *oid        OID      INPUT    Object Identifier        |
  |  <PURPOSE> This routine will remove an SOIDI entry given the OID. |
  |            We will close the SOIDI in flush mode to commit the    |
  |            change to the data base.  We also need to close it to  |
  |            match the call to t_sysndx() which performs an open on |
  |            the SOIDI.                                             |
  |  <OUTPUT>  sint                  result code.                     |
  |  <CALLS>   dms_rmv(), oid_key(), ocd_new(), t_sysndx()            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "exc.h"





sint   soidi_de(   oid   )

OID               *oid         ;



{


   OCB        *soidi =  NIL ;
   OCD        *ocd   =  NIL ;
   sint        rc    =    0 ;





   soidi = t_sysndx( SOIDI_OBJ );


   if( OCB_POINTER_OK( soidi ) )
   {

      ocd_new( & ocd,   "SOIDI.INDEX" );
      oid_key(   ocd,   oid           );
      rc = obj_rmv(      soidi,  ocd  );
      ocd_drp( & ocd );
      obj_set_mode( soidi, OBJ_SYNC_PENDING );
      obj_set_mode( soidi, OBJ_FLUSH        );
      obj_cls     ( soidi                   );
      if( soidi->open_count > 1 )
      {

         soidi->open_count --;

      }

   }



   return(rc);

}
