/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    PSI_DEL                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 22, 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   Sys_ID      string   INPUT    PSI Identifier           |
  |                                                                   |
  |  <PURPOSE> This routine will remove a PSI entry given the ID.     |
  |                                                                   |
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





sint   psi_del(   system_id   )

string            system_id    ;



{


   OCB        * psi_object =  NIL ;
   OCD        * component  =  NIL ;
   sint         rc         =    0 ;





   psi_object = t_sysndx( PSI_OBJ );

   if( OCB_POINTER_OK( psi_object ) )
   {

      ocd_new( & component,       "PSI.INDEX"      );
      s_copy(  & (component->key), system_id, 0, 0 );


      rc = obj_rmv( psi_object,  component  );
      ocd_drp( & component );

   }



   return(rc);

}
