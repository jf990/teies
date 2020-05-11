/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                 T_S_OFF.C
<LEVEL>                TM
<AUTHOR>               John L. Foster
<DATE>                 June 12, 1987
<PARAMS>
<OUTPUT>               sint
<GLOBAL>               - none -
<PURPOSE>
       A user is leaving TEIES, close down all processes, inform
       the Local MS, close all objects, and clean up.
<CALLS>
<ERRORS>
<LIMITS>
<MODS>
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"
#include "obj_user.h"



sint     t_s_off( type_of_signoff )

sint              type_of_signoff;


{


   string    result     =  S_NULL,
             time_off   =  S_NULL,
             dt         =  S_NULL,
             cumulative =  S_NULL,
             created    =  S_NULL,
             modified   =  S_NULL,
             received   =  S_NULL,
             time_used  =  S_NULL,
             cumm_str   =  S_NULL,
             stat_entry =  S_NULL,
             state      =  S_NULL;
   sint      rc         =       0,
             state_id   =       0,
             request    =       0,
             cumm_time  =       0;
   hword     flags      =       0;
USER_STAT_ENTRY * stat_record = NIL;




   TM_master->flags |= IN_SIGNOFF;


   if( type_of_signoff != SIGNOFF_REQUESTED )
   {

      rc = t_send( TM_master->MS, SESSION_STAT_INFO, flags,
                                                      state_id, S_NULL );
      if( rc == 0 )
      {

         t_waitc( TM_master->MS, & stat_entry );

      }
      s_init( & state,      "SIGNOFF"  );
      s_init( & time_off,   "%TIMEOFF" );
      s_init( & cumulative, "%CUMMULA" );
      s_init( & created,    "CREATED"  );
      s_init( & modified,   "MODIFIED" );
      s_init( & received,   "RECEIVED" );

      t_gtdate( & dt,         NIL       );
      var_set(    time_off,   dt        );

      if( s_len( stat_entry ) > 0 )
      {

         stat_record = ( USER_STAT_ENTRY * ) s_buffer( stat_entry );

         s_sitos(  & cumm_str,   cumm_time                       );
         var_set(    cumulative, cumm_str                        );
         s_nulout( & cumm_str  );
         s_sitos(  & cumm_str,   stat_record->count_oid_created  );
         var_set(    created,    cumm_str                        );
         s_nulout( & cumm_str  );
         s_sitos(  & cumm_str,   stat_record->count_oid_modified );
         var_set(    modified,   cumm_str                        );
         s_nulout( & cumm_str  );
         s_sitos(  & cumm_str,   stat_record->count_oid_received );
         var_set(    received,   cumm_str                        );

      }
      request = t_form( state, NIL, NIL );

      if( request == USER_REQUESTED_PROCESS )
      {

         rc = SIGNOFF_ABANDON_REQUESTED;

      }
      else
      {

         rc = t_send( TM_master->MS, SIGNOFF, flags, state_id, S_NULL );
         if( rc == 0 )
         {

            t_waitc( TM_master->MS, NIL );

         }
      }
      f_close(  state, NO_FLAGS  );
      s_drop( & state     );
      s_drop( & time_off  );
      s_drop( & cumulative);
      s_drop( & created   );
      s_drop( & modified  );
      s_drop( & received  );
      s_drop( & time_used );
      s_drop( & dt        );
      s_drop( & cumm_str  );
      s_drop( & stat_entry);

   }

   TM_master->flags &= ~IN_SIGNOFF;


   return(rc);

}
