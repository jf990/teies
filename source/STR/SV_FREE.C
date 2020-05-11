/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    sv_free                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 10, 1991                                         |
  |  <Library> Str                                                    |
  |  <PARAM>   vector      SVECTOR *          Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate             |
  |  <PURPOSE> Free memory used by a string vector.                   |
  |  <Comments>Need to rewrite the malloc call to a standardized mem- |
  |            ory manager that will operate under a variety of en-   |
  |            vironments.                                            |
  |  <OUTPUT>  standard string level error code.                      |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/
#include <stdlib.h>
#include <malloc.h>
#include <datatype.h>
#include "str.h"



int   sv_free   (   SVECTOR * vector   )
{

   int     result       =  0 ,
           area         =  0 ;
   SVECTOR StringVector = NULL ;




   if ( vector == NULL  ||  * vector == NULL )
   {

      result = STRING_POINTER_NOT_PROVIDED ;

   }
   else
   {

      sv_clear ( * vector ) ;
      _ffree   ( * vector ) ;
      * vector = NULL       ;

   }
   return( result ) ;

}
