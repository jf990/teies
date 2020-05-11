/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_ckpswd.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 17, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   oid         OID *     INPUT   User object to verify.   |
  |                                                                   |
  |  <PARAM>   password    string    INPUT   Requested  password.     |
  |                                                                   |
  |  <PURPOSE> Check to see if the password given is the proper       |
  |            password for the user in question.  The OID of the user|
  |            is provided, so it must be opened and the private      |
  |            password segment is looked at.                         |
  |                                                                   |
  |  <OUTPUT>  boolean                                                |
  |  <CALLS>   obj_opn(), obj_get(), s_comp()                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



boolean   t_ckpswd( oid, password )

OID                *oid;
string                   password;


{

OCB     *user_ocb    =   NIL;
string   user_pswd   = S_NUL;
boolean  verify      = FALSE;
sint     rc          =     0;




user_ocb = obj_opn( oid, OBJ_READ );
if( OCB_POINTER_OK( user_ocb ) )
{

   rc  = obj_find( user_ocb, "PRIVATE.ACCESS_CODE", & user_pswd );
   s_upper( user_pswd, 0, 0 );
   if( s_comp( user_pswd, password, 0, 0, 'M', &rc ) == 0 )
   {

      verify = TRUE;

   }
   s_drop( & user_pswd );
   obj_cls(  user_ocb  );

}



   return(verify);

}
