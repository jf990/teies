/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_user                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   None                                                   |
  |  <PURPOSE> Create a new TEIES user. All input and output          |
  |            is done through the Forms system.                      |
  |                                                                   |
  |            Procedure: Call up the forms to do the user interaction|
  |                       and get all necessary values for this new   |
  |                       user. Then, allocate a new object and fill  |
  |                       it in with this data.                       |
  |                                                                   |
  |                       Once the Object is allocated we need to link|
  |                       the various indexes to this object. 1) the  |
  |                       SOIDI; 2) Group memberships; 3) Name index; |
  |                       4) Add a canned message into the mailbox for|
  |                       first signon, 5) System Directory.          |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_form(), var_set(), s_init()                          |
  |  <ERRORS>  22035                                                  |
  |  <FORMS>   3110, 3111, 3112                                       |
  |  <LIMITS>  All characters in the name and address strings must    |
  |            be printable.                                          |
  |                                                                   |
  |            When we build the OCD packet to send to the MS we      |
  |            cannot drop it as lk_user() expands it the drops it.   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "obj_prim.h"
#include "exc.h"


sint     cr_user( dummy_ocb, modif, level, params )


OCB             * dummy_ocb;
string            modif;
hword             level;
OCB            ** params;


{

OCB                 *new_ocb  =    NIL, /* OCB of the new user object  */
                    *ocb      =    NIL;
string     form_id            = S_NULL,
           lastname           = S_NULL,
           firstname          = S_NULL,
           middle             = S_NULL,
           nickname           = S_NULL,
           userid             = S_NULL,
           code               = S_NULL,
           number             = S_NULL,
           penname            = S_NULL,
           NAME               = S_NULL,
           DATE               = S_NULL,
           modifier           = S_NULL,
           packet             = S_NULL,
           name_holder        = S_NULL,
           result_str         = S_NULL,
           field              = S_NULL,
           access             = S_NULL,
           system_role        = S_NULL,
           sysid              = S_NULL;
OID                 *psi_oid  =    NIL,
                    *sdi_oid  =    NIL;
sint                 mode     =      0,
           end_function       =      0,
                     rc       =      0;
OCD                 *ocd      =    NIL;
byte                *i        =    NIL;
boolean    linked             =  FALSE;
real                 x        =    0.0,
                     y        =    0.0;





ocb     = obj_all( USER_OBJ, LOCAL_OBJECT, NIL );

if( OCB_POINTER_OK( ocb ) )
{

   s_init( & lastname,   "LASTNAME"    );
   s_init( & firstname,  "FIRSTNAME"   );
   s_init( & middle,     "MIDDLE"      );
   s_init( & nickname,   "NICKNAME"    );
   s_init( & number,     "NUMBER"      );
   s_init( & penname,    "PENNAME"     );
   s_init( & code,       "ACCESS_CODE" );
   s_init( & sysid,      "%SYSTEMID"   );
   s_init( & access,     "ACCESS"      );

   if( params != NIL )
   {

      *params = ocb;

   }
   s_init( & form_id, "3110"         );


   while ( ! linked )
   {

      end_function = t_form( form_id, modifier, ocb );

      if( end_function == USER_REQUESTED_QUIT
       || end_function == USER_REQUESTED_ABANDON
       || end_function == TM_FORM_NOT_FORMED        )
      {

         break;

      }

      if( s_len( (string) var_get( number ) ) )
      {

         string  justified_num = S_NULL;


         ocd_new( &ocd, "NAME.NUMBER");
         s_pad( & justified_num, var_get( number ), 4, '0', 'L' );
         obj_rep( ocb, ocd, justified_num,  SEG_HOLD );
         s_drop( & justified_num );

      }
      else
      {

         except( USER_NUMBER_NOT_PROVIDED, USER_ERROR, S_NULL, S_NULL,
                                           S_NULL, S_NULL, S_NULL     );
         continue;

      }
      if( s_len( (string) var_get( lastname ) )   ||
          s_len( (string) var_get( firstname) )      )
      {

         s_init( &(ocd->name), "NAME.LAST" );
         obj_rep( ocb, ocd, var_get(lastname),  SEG_HOLD   );

         s_init( &(ocd->name), "NAME.FIRST"   );
         obj_rep( ocb, ocd, var_get(firstname), SEG_HOLD   );

      }
      else
      {

         except( INCOMPLETE_NAME_SPECIFIED, USER_ERROR, S_NULL, S_NULL,
                                                S_NULL, S_NULL, S_NULL );
         continue;

      }
      if( s_len( (system_role = (string) var_get( access )) ) )
      {
         /* System.Role */

         if( FALSE )
         {

            except( UNDEFINED_ROLE, USER_ERROR, system_role, S_NULL,
                                             S_NULL, S_NULL, S_NULL );
            continue;

         }
      }
      else
      {


      }

      s_init( &(ocd->name), "NAME.MIDDLE"  );
      obj_rep( ocb, ocd, var_get(middle),    SEG_HOLD   );

      s_init( &(ocd->name), "NAME.NICKNAME");
      obj_rep( ocb, ocd, var_get(nickname),  SEG_UPDATE );

      s_init( &(ocd->name), "SYSTEM.ID"         );
      obj_rep( ocb, ocd, var_get(sysid),     SEG_UPDATE );

      s_init( &(ocd->name), "PRIVATE.PEN_NAME"   );
      obj_rep( ocb, ocd, var_get(penname),   SEG_UPDATE );

      s_init( &(ocd->name), "PRIVATE.ACCESS_CODE" );
      obj_rep( ocb, ocd, var_get(code),  SEG_UPDATE );



      rc = lk_user( ocb, packet, level, & result_str );

      switch (  rc  )
      {

         case 0   :

                     linked = TRUE;
                     break;

         case USER_ALREADY_EXISTS :

                     linked = FALSE;
                     break;

         default  :

                     break;

      }
      if( rc > 0 )
      {

         except( rc, USER_ERROR, result_str, S_NULL, S_NULL, S_NULL, S_NULL );
         level = 99;

      }
      if( s_len( field ) > 0 )
      {

         f_curpos( form_id, NO_FLAGS, field, x, y );

      }
   }

   f_close( form_id, NO_FLAGS );

   if( linked && (rc == 0) )
   {

      string date_str = S_NULL;


      s_init( & form_id, "3111"      );
      s_init( & NAME,    "NAME"      );
      s_init( & DATE,    "DATE"      );
      var_set(  NAME,    result_str  );
      t_gtdate( & date_str, NIL      );
      var_set(  DATE,    date_str    );

      end_function = t_form(   form_id, S_NULL, ocb );

      f_close(  form_id, NO_FLAGS    );
      var_clr(  NAME     );
      var_clr(  DATE     );
      s_drop( & NAME     );
      s_drop( & DATE     );
      s_drop( & date_str );

   }


   var_clr( lastname   );
   var_clr( firstname  );
   var_clr( middle     );
   var_clr( nickname   );
   var_clr( number     );
   var_clr( penname    );
   var_clr( code       );
   var_clr( sysid      );

   ocd_drp( &ocd        );
   s_drop( &form_id     );
   s_drop( &lastname    );
   s_drop( &firstname   );
   s_drop( &middle      );
   s_drop( &nickname    );
   s_drop( &number      );
   s_drop( &penname     );
   s_drop( &code        );
   s_drop( &sysid       );

}
else
{

   rc = CANNOT_CREATE_OBJECT;

}

   return(rc);

}
