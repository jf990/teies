/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    sv_alloc                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 09, 1988                                          |
  |  <LEVEL>   STR                                                    |
  |  <PARAM>   vector      SVECTOR *          Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate             |
  |  <PARAM>   size        int                Number of vector entries|
  |                                           to allocate             |
  |  <PURPOSE> Allocate and initialze a string vector.                |
  |  <Comments>Need to rewrite the malloc call to a standardized mem- |
  |            ory manager that will operate under a variety of en-   |
  |            vironments.                                            |
  |  <OUTPUT>  standard string level error code.                      |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/
#include <stdlib.h>
#include <datatype.h>
#include "str.h"



int   sv_alloc  (   SVECTOR * vector ,
                    int       size   )
{

   int     result       =  0 ,
           area         =  0 ;
   SVECTOR StringVector = NULL ;




   if ( size < 0 )
   {

      result = STRING_VECTOR_SIZE_NOT_ALLOWED ;

   }
   else if ( vector == NULL )
   {

      result = STRING_POINTER_NOT_PROVIDED ;

   }
   else
   {

      area     =   sizeof ( SVECTOR ) +
                 ( sizeof ( SSCALAR ) * size ) ;
      StringVector = ( SVECTOR ) malloc ( area ) ;

      StringVector->TotalAllocated = size ;
      StringVector->TotalUsed      =    0 ;

      sv_clear ( StringVector ) ;

      * vector = StringVector ;

   }
   return( result ) ;

}
