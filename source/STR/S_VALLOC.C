/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    s_valloc.C                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 09, 1988                                          |
  |                                                                   |
  |  :LEVEL.   STR                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   vector      string **  INPUT   Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate.            |
  |  :PARAM.   size        hword      INPUT   Number of vector entries|
  |                                           to allocate.            |
  |                                                                   |
  |  :PURPOSE. Allocate and initialze a string vector.                |
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



sint   s_valloc( vector, size )

string         **vector;
hword                    size;

{

   sint    rc      =     0;
   sint    area    =     0;




   if ( 0 > (sint) size )
   {
      rc = S_BAD_PARM;
   }
   else if ( vector == (string *) NIL
      || vector == (string **) MINUS_ONE_PTR )
   {
      rc = S_BAD_STR;
   }
   else
   {

      area = sizeof( string * )  *  (size + 1);
      *vector = (string *)  os_mema( area );
      os_memn( *vector, area );
      s_smp_al( (string) *vector ) = size;

   }


   return(rc);

}
