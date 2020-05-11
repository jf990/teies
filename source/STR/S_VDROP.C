/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    s_vdrop()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 20, 1988                                          |
  |                                                                   |
  |  :LEVEL.   STR                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   vector      string **  INPUT   Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate.            |
  |                                                                   |
  |  :PURPOSE. Deallocate the string vector.                          |
  |                                                                   |
  |  :OUTPUT.  sint -- standard string level error code.              |
  |                                                                   |
  |  :CALLS.   os_mema(), s_smp_al(), s_smp_ul()                      |
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
#include "str.h"



sint   s_vdrop( vector )

string         **vector;

{

   sint    rc      =     0,
           size    =     0,
           i       =     1;





   if( vector == (string**) NIL || vector == (string**) MINUS_ONE_PTR )
   {
      rc = S_BAD_STR;
   }
   else
   {
      size = s_smp_al(   (string)  *vector   );


      for( i=1; i<=size; i++ )
      {

            s_drop(  (*vector)+i  );

      }
      os_memd( *vector );
      *vector = NIL;

   }

   return(rc);

}
