/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_des()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman, Tanmay S. Kumar   |
  |                                                                   |
  |  :DATE.    June 23, 1988                        August 15, 1989   |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *      INOUT  Pointer to Group         |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_GRUP()   |
  |                                                                   |
  |  :PARAM.   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  :PARAM.   level       hword      INPUT  not used.                |
  |                                                                   |
  |  :PARAM.   params      struct *   INOUT  not used                 |
  |                                                                   |
  |  :PURPOSE. Create A Group.         This routine handles the       |
  |            adding of DESCRIPTION component to the GROUP,          |
  |            It calls up the "s_pad", implements TEIES function     |
  |            keys.                                                  |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
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


sint grp_des( group_ocb, modifier, level, params )

OCB     *group_ocb;
string   modifier;
hword    level;
byte    *params;

{
string
         scratchpad     = S_NULL;

sint     rc             = NO_ERR,
         junk           = 0;

OCB     *sp_ocb         = NULL;

OCD     *group_ocd      = NULL;


   s_init( &scratchpad, "A * GRP_DESC $SP");

   rc = t_spad( scratchpad, (string *)params );
   if ( (rc == USER_REQUESTED_ENTER) || (USER_REQUESTED_PROCESS) )
   {
      rc = 0;
      sp_ocb = obj_opn( (OID *) params, OBJ_READ | OS_FILE );
      if( OCB_POINTER_OK( sp_ocb ) )
      {
         ocd_new(  &group_ocd, "GROUP.DESCRIPTION" );
         rc = ocd_bse( group_ocb, group_ocd, sp_ocb );
         obj_cls( sp_ocb );

      }  /* if pointer is good */

   }  /* if rc_form was not quit or abandon */

   s_drop( &scratchpad);

   ocd_drp( &group_ocd );


   return(rc);

}
