/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    Perform.C                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 13, 1987                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  TM_master             INPUT   All object level tables  |
  |                                           are found off this ptr. |
  |                                                                   |
  |  :PARAM.   object      string    INPUT   Object type name string  |
  |  :PARAM.   action      string    INPUT   Action procedure name Str|
  |  :PARAM.   args        byte *    INOUT   Arguments for the Proc   |
  |                                                                   |
  |  :PURPOSE.                                                        |
  |            This routine is the immediate interface between the    |
  |            Object Level and the Transaction Manager. Here two     |
  |            strings are passed in, either coming from the User     |
  |            or a TM state, with contain an Object name and an      |
  |            Action name. The two are combined to locate the right  |
  |            routine to invoke to perform the Action.               |
  |                                                                   |
  |            First, locate the object to use by going through the   |
  |            ODT's and comparing the string with each ODT ISV.      |
  |            If found, go through the OAMT comparing the action str |
  |            with the ISV of the available actions.                 |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   except(),                                              |
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
#include "exc.h"
#include "isv.h"
#include "tm.h"


sint     perform(  object,   action,   args  )

string             object,
                             action;
byte                                  *args;


{

sint      i           =    0,
          r           =    1,
          object_isv  =    0,
          action_isv  =    0,
          rc          =    0;




    object_isv = isv_match( object, ISV_USER, ISV_TICKET );

    if( object_isv == 0 )
    {

         rc = UNKNOWN_OBJECT_TYPE;
         except( rc, INFORMATIONAL, S_NUL, S_NUL, S_NUL, S_NUL,S_NUL);

    }
    else
    {

       action_isv = isv_match( action, ISV_VIEW, ISV_LINK );

       if( action_isv == 0 )
       {

            rc = UNKNOWN_ACTION_TYPE;
            except( rc, INFORMATIONAL, S_NUL, S_NUL, S_NUL, S_NUL,S_NUL);

       }
                                         /* At this point we have the */
                                         /* ISV numbers for both the  */
                                         /* Object and the Action.    */
                                         /* Now get the pointers to   */
                                         /* their respective tables.  */

    }


   return(rc);
}
