/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    sv_clear                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 10, 1991                                         |
  |  <Library> STR                                                    |
  |  <PARAM>   vector      SVECTOR            Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate             |
  |  <PURPOSE> Clear all strings and pointers in a vector.            |
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



int   sv_clear  (   SVECTOR   vector )
{

   int       result        =    0 ;
   SSCALAR * VectorElement = NULL ,
           * VectorEnd     = NULL ,
           * LastOneToFree = NULL ;



   if ( vector == NULL )
   {

      result = STRING_POINTER_NOT_PROVIDED ;

   }
   else
   {

      VectorEnd     = & vector->FirstElement + vector->TotalAllocated ;
      LastOneToFree = & vector->FirstElement + vector->TotalUsed ;

      for ( VectorElement  = & vector->FirstElement ;
            VectorElement <=   VectorEnd            ;
            VectorElement ++                        )
      {

         if ( (   VectorElement  < LastOneToFree )
           && ( * VectorElement != NULL          ) )
         {

            _ffree ( * VectorElement ) ;

         }
         * VectorElement = NULL ;

      }
   }
   return( result ) ;

}
