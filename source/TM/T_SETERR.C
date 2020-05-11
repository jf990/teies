/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_seterr(sint error_code)                              |
  |                                                                   |
  |  :AUTHOR.  Tanmay S. Kumar                                        |
  |                                                                   |
  |  :DATE.    August 09, 1988                                        |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   error_code   Sint      INPUT   Error code to look up.  |
  |                                                                   |
  |  :PURPOSE. Sets the TM interface variable %EXC with an error      |
  |            message that is retrieved from the System Error Code   |
  |            Index (SERRI) based on the passed error code.          |
  |                                                                   |
  |  :OUTPUT.  sint          0.                                       |
  |                                                                   |
  |  :CALLS.   t_sysndx(), obj_get(), var_set(), s_sitos()            |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  We can be faked that the Object level is active and    |
  |            it really is not and access to object level structures |
  |            will bomb.                                             |
  |                                                                   |
  |  :MODS     27 Jan 89    Steve Muccione   Rewrote routine to get   |
  |                                          error messages from file |
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

#define   ERROR_CODE_LENGTH          5
#define   START_OF_ERROR_MESSAGE     7



sint  t_seterr(error_code, error_message)

sint    error_code;
string *error_message;

{

 OCB    *ocb       = NIL;
 OCD    *ocd       = NIL;
 sint    rc        = 0;
 sint    scrc      = 0;
 string  inp_str   = S_NULL;
 string  num_str   = S_NULL;
 string  f_name    = S_NULL;




 if( error_message == NIL || error_message == (string *) 0xFFFFFFFF )
 {

    rc = S_BAD_STR ;

 }
 else
 {



    s_init ( &f_name, "* * TEIES SYSERR");

    ocb = obj_opn ( (OID *) f_name, OBJ_READ | OS_FILE );

    if ( ! OCB_POINTER_OK ( ocb ) )
    {
       rc = SERRI_DOES_NOT_EXIST;
    }
    else
    {

       s_sitos ( & num_str , error_code );
       s_pad   ( & num_str , num_str, ERROR_CODE_LENGTH, '0', 'L');

       ocd = ocd_new ( &ocd, NIL );
       rc = ERROR_CODE_NOT_IN_SERRI;

       while (  obj_grec ( ocb, ocd, & inp_str )  )
       {
          if ( s_comp ( num_str, inp_str, 0, ERROR_CODE_LENGTH, 'M', &scrc )
                 == 0 )
          {
             s_copy ( error_message, inp_str, START_OF_ERROR_MESSAGE, 0 );
             rc = 0;
             break;
          }
       }

       obj_cls(   ocb );
       ocd_drp( & ocd );

    }
    s_drop ( & inp_str );
    s_drop ( & num_str );
    s_drop ( & f_name  );

 }


   return(rc);

}
