/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_tini                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 26, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  AOM      struct _act_obj[]    Action Object table.     |
  |  <PARAM>   * none *                                               |
  |                                                                   |
  |  <PURPOSE> Any one-time object level structure initialization.    |
  |                                                                   |
  |            If the number of objects or actions change, BOTH obj.h |
  |            and obj_act.h must be updated.                         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "isv.h"
#include "obj_table.h"
#include "tm.h"





       struct _OBJ_REF objtable[ ] = {

/*  -----------+-----------+-------------+--------------------------  */
/*    Object's | ISV       |    Object   |                            */
/*    Name     | Reference |    Type     |                            */
/*  -----------+-----------+-------------+--------------------------  */
       S_NULL,           0,      ANY_OBJ,   0,
       S_NULL,    ISV_USER,     USER_OBJ,   0,
       S_NULL,   ISV_GROUP,    GROUP_OBJ,   0,
       S_NULL,    ISV_NTDR,  NETWORK_OBJ,   0,
       S_NULL,    ISV_CONF,   CONFER_OBJ,   0,
       S_NULL,      ISV_CC,  CONFCOM_OBJ,   0,
       S_NULL,    ISV_MMSG,     MAIL_OBJ,   0,
       S_NULL,   ISV_NOTIF,    NOTIF_OBJ,   0,
       S_NULL,ISV_ACTIVITY, ACTIVITY_OBJ,   0,
       S_NULL,    ISV_MEMB,     MEMB_OBJ,   0,
       S_NULL,    ISV_LIST,     LIST_OBJ,   0,
       S_NULL,  ISV_TICKET,   TICKET_OBJ,   0,
       S_NULL,           0, CONTENTS_OBJ,   0,
       S_NULL,           0,      SYS_OBJ,   0,
       S_NULL,           0,      SGI_OBJ,   0,
       S_NULL,           0,      PSI_OBJ,   0,
       S_NULL,           0,    SOIDI_OBJ,   0,
       S_NULL,           0,      SKI_OBJ,   0,
       S_NULL,           0,      SAI_OBJ,   0,
       S_NULL,           0,     SDTI_OBJ,   0,
       S_NULL,           0,      CRI_OBJ,   0,
       S_NULL,           0,    SODTI_OBJ,   0,
       S_NULL,           0,      ARI_OBJ,   0,
       S_NULL,           0,      SHI_OBJ,   0,
       S_NULL,           0,      SNI_OBJ,   0,
       S_NULL,           0,    SERRI_OBJ,   0,
       S_NULL,           0,      SFI_OBJ,   0,
       S_NULL,           0,    SDELI_OBJ,   0,
       S_NULL,           0,      SMI_OBJ,   0,
       S_NULL,           0,            0,   0,
       S_NULL,           0,            0,   0,
       S_NULL,           0,            0,   0
};


sint obj_tini()


{

   sint position = 1;






   s_init( & ( objtable[ position++ ].object_name ),       "USER" );
   s_init( & ( objtable[ position++ ].object_name ),      "GROUP" );
   s_init( & ( objtable[ position++ ].object_name ),    "NETWORK" );
   s_init( & ( objtable[ position++ ].object_name ), "CONFERENCE" );
   s_init( & ( objtable[ position++ ].object_name ),    "COMMENT" );
   s_init( & ( objtable[ position++ ].object_name ),       "MAIL" );
   s_init( & ( objtable[ position++ ].object_name ),"NOTIFICATION");
   s_init( & ( objtable[ position++ ].object_name ),   "ACTIVITY" );
   s_init( & ( objtable[ position++ ].object_name ), "MEMBERSHIP" );
   s_init( & ( objtable[ position++ ].object_name ),       "LIST" );
   s_init( & ( objtable[ position++ ].object_name ),     "TICKET" );
   s_init( & ( objtable[ position++ ].object_name ),   "CONTENTS" );
   s_init( & ( objtable[ position++ ].object_name ),   "SYSTEM_P" );
   s_init( & ( objtable[ position++ ].object_name ),        "SGI" );
   s_init( & ( objtable[ position++ ].object_name ),        "PSI" );
   s_init( & ( objtable[ position++ ].object_name ),      "SOIDI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SKI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SAI" );
   s_init( & ( objtable[ position++ ].object_name ),       "SDTI" );
   s_init( & ( objtable[ position++ ].object_name ),        "CRI" );
   s_init( & ( objtable[ position++ ].object_name ),      "SODTI" );
   s_init( & ( objtable[ position++ ].object_name ),        "ARI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SHI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SNI" );
   s_init( & ( objtable[ position++ ].object_name ),      "SERRI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SFI" );
   s_init( & ( objtable[ position++ ].object_name ),      "SDELI" );
   s_init( & ( objtable[ position++ ].object_name ),        "SMI" );

   TM_master->obj_t_size = position;



   return(0);

}
