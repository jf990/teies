/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_sys.c                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 9, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master                                              |
  |  <PURPOSE> Create the system group (group 0) and system user (user|
  |            0) so a TEIES system can be initialized.  The system   |
  |            group is the public group all users belong to and the  |
  |            system user is a super user who has access to just     |
  |            about anything.  The local OS id that creates the      |
  |            system user is the only ID allowed to invoke TEIES with|
  |            this user unless the OS_ID field in the user object is |
  |            changed.                                               |
  |                                                                   |
  |            The user is created for all servers.  If a server runs |
  |            in "single user" mode, then this user is the identity  |
  |            of the server.  Group 0 is only created on a Master Ser|
  |            ver.                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_opn(), obj_cls(), obj_all(), lk_user()             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "obj_prim.h"


sint   cr_sys(  )


{

sint                 temp     =       0,
                     rc       =       0;
boolean              master_s =   FALSE;
string              *str_vec  =  S_NULL,
                    *str_ptr  =  S_NULL,
                     contents =   S_NUL;
OID                 *oid      =     NIL;
OCD                 *ocd      =     NIL;
OCB                 *grp_obj  =     NIL,
                    *usr_obj  =     NIL;






if( TM_master->usrtype == MASTER_SERVER )
{

   master_s = TRUE;

   grp_obj   =   obj_all( GROUP_OBJ, LOCAL_OBJECT, NIL );


   ocd_new( & ocd,       "GROUP.LABEL"                 );

   s_init( &contents   , "TEIES System Group"          );
   obj_rep( grp_obj, ocd, contents         , SEG_HOLD  );

   s_init( &(ocd->name), "GROUP.TITLE"                 );
   s_init( &contents   , "TEIES Users"                 );
   obj_rep( grp_obj, ocd, contents         , SEG_HOLD  );

   s_init( &(ocd->name), "GROUP.ID"                    );
   s_init( & contents,   "0000"                        );
   obj_rep( grp_obj, ocd, contents         , SEG_UPDATE);

   s_init( &(ocd->name), "STATUS.TYPE"                 );
   temp = PUBLIC_GROUP;
   obj_rep( grp_obj, ocd, &(temp)          , SEG_UPDATE);
   ocd_drp( & ocd );


}


usr_obj    =   obj_all( USER_OBJ, LOCAL_OBJECT, NIL );


ocd_new( & ocd,       "NAME.LAST"                   );
s_init(  & contents,  "System Administrator"        );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "NAME.NUMBER"                 );
s_init( &contents,    "0000"                        );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "NAME.NICKNAME"               );
s_init( &contents,    "TEIES"                       );
obj_rep( usr_obj, ocd, contents         , SEG_UPDATE);

s_init( &(ocd->name), "BUS_ADDRESS.ORGANIZATION"    );
s_init( &contents,    "CCCC at NJIT"                );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "BUS_ADDRESS.CITY"            );
s_init( &contents,    "Newark"                      );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "BUS_ADDRESS.STATE"           );
s_init( &contents,    "NJ"                          );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "BUS_ADDRESS.ZIP_CODE"        );
s_init( &contents,    "07102"                       );
obj_rep( usr_obj, ocd, contents          , SEG_HOLD );

s_init( &(ocd->name), "BUS_ADDRESS.COUNTRY"         );
s_init( &contents  ,  "USA"                         );
obj_rep( usr_obj, ocd, contents          , SEG_HOLD );

s_init( &(ocd->name), "BUS_ADDRESS.TELEPHONE"       );
s_init( &contents  ,  "(201) 596-EIES"              );
obj_rep( usr_obj, ocd, contents         , SEG_UPDATE);

s_init( &(ocd->name), "PRIVATE.PEN_NAME"            );
s_init( &contents  ,  "TEIES MASTER"                );
obj_rep( usr_obj, ocd, contents         , SEG_HOLD  );

s_init( &(ocd->name), "PRIVATE.ACCESS_CODE"         );
s_init( &contents  ,  "JACK DANIELS"                );
obj_rep( usr_obj, ocd, contents         , SEG_UPDATE);

s_init( &(ocd->name), "SYSTEM.ID"                   );
s_init( &contents  ,  "TEIESMS"                     );
obj_rep( usr_obj, ocd, contents         , SEG_UPDATE);



if( master_s )
{

   rc = lk_user( usr_obj, S_NULL, (hword) 0, NIL );
   obj_cls(  grp_obj  );

}

ocd_drp( & ocd      );
s_drop(  & contents );
obj_cls(   usr_obj  );

   return(rc);

}



/*  The oid_key() call is a tweek that should be removed.  The proper
    method to set the key is to call obj_rep() with the key component
    GROUP.MEMBERS.OID and the ocd_dbp() routine is supposed to figure
    out that is the key, fill it in, and update the data base path.

    JF  11/09/87
 */
