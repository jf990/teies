/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_clr()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman, Tanmay s. Kumar   |
  |                                                                   |
  |  :DATE.    October 24, 1988                     August 23, 1989   |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *     INPUT   Group      Object Control|
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
  |  :PURPOSE. Clear out the vars on the forms involved 3120, 3121,   |
  |            3122 and 3123. ( Could be 3190 as well in future, not  |
  |            done yet ). Close the obect, and alocate  a new object |
  |            control block depending upon the LEVEL, as mentioned   |
  |            above.                                                 |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
  |                                                                   |
  |  :CALLS.   obj_cls(),   f_close(), obj_all(), s_init(), s_drop()  |
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
#include "obj_group.h"


sint   grp_clr ( group_ocb, modifier, level, new_group_ocb )

OCB      *group_ocb;
string    modifier;
hword     level;
OCB     **new_group_ocb;

{
   string   clear_variable =  S_NULL,
            form_id        =  S_NULL;

   sint     rc             =  0;



   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the STARTING_POINT_GROUP_CREATE FORM "3140"        */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3120"   );
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
   /*  Closing the REQUIRED_INFORMATION FORM  "3121"              */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3121"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( & form_id, "314A"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "GROUP_OWNER"    );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUP_ID"       );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUP_LABEL"    );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUP_TYPE"     );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "ACCESS_TYPE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "STATUS_CODE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "TITLE_PUBLIC"  );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "DESC_PUBLIC"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "MEMBER_PUBLIC" );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "CONF_LST_PUB");
   var_clr( clear_variable );


   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the PUBLIC INFORMATION FORM  "3122"                */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   s_init ( & form_id, "3122"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "GROUPID"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUPLABEL"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUP_TITLE"   );
   var_clr( clear_variable );

   s_init ( &clear_variable,  "GROUP_KEYS"    );
   var_clr( clear_variable );

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   /*                                                             */
   /*  Closing the Display creation   FORM  "3123"                */
   /*  and the clearing out the variables ( VARS ) on the form    */
   /*                                                             */
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


   s_init ( & form_id, "3123"   );
   f_close(   form_id, NO_FLAGS );

   s_init ( &clear_variable,  "CREATE_ANOTHER");
   var_clr( clear_variable );



   s_drop( & form_id        );
   s_drop( & clear_variable );



   return(rc);

}
