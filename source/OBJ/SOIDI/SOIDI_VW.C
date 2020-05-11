/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    soidi_vw()                                             |
  |                                                                   |
  |  :AUTHOR.  Rana Ajaz-Ur-Rehman                                    |
  |                                                                   |
  |  :DATE.    April 19, 1989                                         |
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
  |  :PURPOSE. Display contents of OID, LFD, GOID, and OBJECT TYPE    |
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




sint soidi_vw(  oid_ptr,  lfd,  goid,  obj_type )

OID      *oid_ptr;
hword     lfd;
sint      goid;
hword     obj_type;

{


   string   oid_string     =  S_NULL,
            form_id        =  S_NULL,

            quit           =  S_NULL,
            abandon        =  S_NULL,
            process        =  S_NULL,

            OID_CONTENTS   =  S_NULL,
            object_id      =  S_NULL,
            LFD_CONTENTS   =  S_NULL,
            log_fl_desc    =  S_NULL,
            GOID_CONTENTS  =  S_NULL,
            group_obj_id   =  S_NULL,
            OBJECT_TYPE    =  S_NULL,
            type_of_obj    =  S_NULL;


   sint     rc          =  0,
            rc_form     =  0,
            R           =  0;



#D_begin( SOIDI_VW, DBM);


s_init( & form_id,       "38010"         );
s_init( & OID_CONTENTS,  "OID_CONTENTS"  );
s_init( & LFD_CONTENTS,  "LFD_CONTENTS"  );
s_init( & GOID_CONTENTS, "GOID_CONTENTS" );
s_init( & OBJECT_TYPE,   "OBJECT_TYPE"   );


s_init( &process,  "$ENTER"       );

isv_get( &quit,    ISV_QUIT,    0 );
isv_get( &abandon, ISV_ABANDON, 0 );




   s_htos ( & log_fl_desc,  lfd      );
   #D_str ( 'log_fl_desc', 'logical file descriptor' );

   s_sitos( & group_obj_id, goid     );
   #D_str ( 'group_obj_id', 'group object identifier' );

   s_htos ( & type_of_obj,  obj_type );
   #D_str ( 'type_of_obj', 'object type' );

   s_mtoxs( & object_id,  (byte*)oid_ptr, sizeof( OID ) );
   #D_str ( 'object_id', 'object identifier' );

   var_set( LFD_CONTENTS,  log_fl_desc    );
   var_set( GOID_CONTENTS, group_obj_id   );
   var_set( OBJECT_TYPE,   type_of_obj    );
   var_set( OID_CONTENTS,  object_id      );

   rc = t_form( form_id, S_NULL, NIL );


   f_close( form_id, NO_FLAGS );
   var_clr( OID_CONTENTS      );
   var_clr( LFD_CONTENTS      );
   var_clr( GOID_CONTENTS     );
   var_clr( OBJECT_TYPE       );

   s_drop( & form_id          );
   s_drop( & OID_CONTENTS     );
   s_drop( & LFD_CONTENTS     );
   s_drop( & GOID_CONTENTS    );
   s_drop( & OBJECT_TYPE      );


#D_leave( rc, 'Done with soidi_display' );

}

