/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    clr_cr( )                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |                                                                   |
  |  :DATE.    October 24, 1988                                       |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   conf_ocb    OCB *     INPUT   Conference Object Control|
  |                                          to be closed.            |
  |                                                                   |
  |            modifier    string    INPUT   Not used.                |
  |                                                                   |
  |            level       hword     Controls whether the new object  |
  |                                  is to be allocated or not        |
  |                                                                   |
  |                                  level = 0, => get a new object   |
  |                                                control block to   |
  |                                                be used from here  |
  |                                                on.                |
  |                                                                   |
  |                                  level= 1, =>  just clear out     |
  |                                                the VARS on the    |
  |                                                forms involved and |
  |                                                close the current  |
  |                                                object. AND DON'T  |
  |                                                ALLOCATE A NEW ONE |
  |                                                                   |
  |            new_conf_ocb OCB *    INPUT   Not used.                |
  |                                                                   |
  |  :PURPOSE. Clear out the vars on the forms involved 3140, 3141,   |
  |            3142 and 3142. ( Could be 3190 as well in future, not  |
  |            done yet ). Close the obect, and aloocate a new object |
  |            control block depending upon the LEVEL, as mentioned   |
  |            above.                                                 |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  returns  a pointer to newly allocated conference       |
  |            object control block.                                  |
  |                                                                   |
  |                                                                   |
  |  :CALLS.   obj_close(), f_close(), obj_all(), s_init(), s_drop()  |
  |            var_clr().                                             |
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


sint     clr_cr( conf_ocb, modifier, level, new_conf_ocb )

OCB      *conf_ocb;
string    modifier;
hword     level;
OCB     **new_conf_ocb;

{
   string   clear_variable =  S_NULL,
            form_id        =  S_NULL;

   sint     rc             =  0;


#D_begin( clr_cr,  OBJ_CR  );
#D_show ( 'conf_ocb_ptr=0x%X; level=%d', 'conf_ocb, level'   );

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the SATRTING_POINT_CONFERENCE_CREATE FORM "3140"   */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3140"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( & clear_variable, "ALL_INF"       );
   var_clr( clear_variable );

   s_init ( & clear_variable, "ALL_DONE"      );
   var_clr( clear_variable );

   s_init ( & clear_variable, "REQ_INF"       );
   var_clr( clear_variable );

   s_init ( & clear_variable, "REQ_DONE"      );
   var_clr( clear_variable );

   s_init ( & clear_variable, "PUB_INF"       );
   var_clr( clear_variable );

   s_init ( & clear_variable, "PUB_DONE"      );
   var_clr( clear_variable );

   s_init ( & clear_variable, "DESC_INF"      );
   var_clr( clear_variable );

   s_init ( & clear_variable, "DESC_DONE"     );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "ASSIGN_INF"    );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "ASSIGN_DONE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "REVIEW_INPUTS" );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "OK_TO_ESTAB"   );
   var_clr( clear_variable );

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the REQUIRED_INFORMATION FORM  "3141"              */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3141"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( & form_id, "314A"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "MEM_GR_ID"     );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_OWNER"    );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_ID"       );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_LABEL"    );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_TYPE"     );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "ACCESS_TYPE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CATEGORY"      );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "STATUS_CODE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "TITLE_PUBLIC"  );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "DESC_PUBLIC"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "MEMBER_PUBLIC" );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "DEFAULT_STRUCT");
   var_clr( clear_variable );

   s_init ( &clear_variable,  "DEFAULT_ACTVTY");
   var_clr( clear_variable );


   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the PUBLIC INFORMATION FORM  "3142"                */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3142"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "CONF_TITLE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_KEYS"    );
   var_clr( clear_variable );

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the PUBLIC INFORMATION FORM  "3143"                */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


   s_init ( & form_id, "3143"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "CREATE_ANOTHER");
   var_clr( clear_variable );



   s_drop( & form_id        );
   s_drop( & clear_variable );



#D_leave( rc, 'Done with cr_clr' );

}
