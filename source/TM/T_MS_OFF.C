/********************************************************************
<DOC>
<NAME>                 T_MS_OFF.C
<LEVEL>                TM
<AUTHOR>               John L. Foster
<DATE>                 November 29, 1987

<PARAM>            server    string    INPUT    Server to signoff this
                                                  server.
<PARAM>            type_of_signoff sint INPUT   What to do.

<OUTPUT>           sint
<PURPOSE>          The server wants to close the connection it thinks
                   it has with this server.  First, check to make sure
                   there really is a connections with this server.  If
                   so close the connection and update the tables.

<CALLS>            t_cls_t(), obj_get(), obj_rep()
<MODS>             4-14-89  JF    Added User Statistic tracking.
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"
#include "obj_user.h"
#include "obj_list.h"






sint    t_ms_off( server, type_of_signoff )

string            server;
sint              type_of_signoff;


{


sint              rc          =      0,
                  user_status =      0,
                  stat_count  =      0,
                  level       =      0;
string            request     = S_NULL,
                  stat_entry  = S_NULL;
ISCF_TAB        * tabentry    =    NIL;
OID               stat_list           ;
OCD             * component   =    NIL;
OCB             * user_ocb    =    NIL,
                * stat_ocb    =    NIL;
USER_STAT_ENTRY * stat_record =    NIL;
datetime          time_off            ;









   tabentry = t_conn( server );
   if( ISF_TAB_POINTER_OK( tabentry ) )
   {

      user_ocb = obj_opn( & (tabentry->objectID), OBJ_UPDATE );
      if( OCB_POINTER_OK( user_ocb ) )
      {

         os_dttm(  & time_off  );
         ocd_new ( &   component,      "INDEXES.STATS" );
         rc = obj_get( user_ocb, component, & stat_list );
         if( rc == NO_DATA_FOUND  ||  OID_IS_NULL( & stat_list ) )
         {

            rc = cr_list( NIL, S_NULL, 0, & stat_ocb );
            if( rc == 0  &&  OCB_POINTER_OK( stat_ocb )  )
            {

               rc = obj_rep( user_ocb, component,
                                   & ( stat_ocb->oid ), SEG_UPDATE );

            }
            else
            {

               except( rc, CONTROL_ERROR, S_NUL, S_NUL,
                                          S_NUL, S_NUL, S_NUL );

            }
         }
         else
         {

            stat_ocb = obj_opn( & stat_list, OBJ_UPDATE );

         }
         if( OCB_POINTER_OK( stat_ocb ) )
         {

            s_alloc( & stat_entry,     sizeof( USER_STAT_ENTRY ) );
            s_smp_ul(  stat_entry ) =  sizeof( USER_STAT_ENTRY )  ;
            stat_record = (USER_STAT_ENTRY *) s_buffer( stat_entry );
            stat_record->count_oid_created  = tabentry->created ;
            stat_record->count_oid_modified = tabentry->modified;
            stat_record->count_oid_received = tabentry->received;
            stat_record->access_type        = 0                 ;
            os_memc( & (tabentry->on),
                     & (stat_record->time_on),
                        sizeof( datetime )  );

         }

         if( type_of_signoff != SESSION_STAT_INFO )
         {

            ocd_new(  & component, "SYSTEM.LAST_ON"                  );
            obj_rep(    user_ocb,  component, & time_off, SEG_UPDATE );
            s_init(   &(component->name), "SYSTEM.STATUS"            );
            obj_get(    user_ocb,   component, & user_status         );
            if( user_status == 0 || user_status == FIRST_TIME_USER )
            {

               user_status = CASUAL_USER ;
               obj_rep( user_ocb, component, & user_status, SEG_UPDATE );

            }

            s_init(   & ( component->name),   "LIST.LIST.ENTRY" );
            s_alloc(  & ( component->key ),  sizeof( datetime ) );
            os_memc(  & time_off, s_buffer( component->key ),
                                             sizeof( datetime ) );
            s_smp_ul(     component->key ) = sizeof( datetime )  ;

            rc = obj_rep( stat_ocb, component, stat_entry, SEG_UPDATE );

            rc = t_stop(  tabentry->tm_pid        );
            rc = t_cls_t( server, KEEP_CONNECTION );
            t_csend(      server, rc, S_NULL      );

         }
         else
         {

            t_csend( server, rc, stat_entry );

         }
         obj_cls(      user_ocb   );
         obj_cls(      stat_ocb   );
         ocd_drp(  &   component  );
         s_drop(   &   stat_entry );

      }
   }



   return(rc);

}
