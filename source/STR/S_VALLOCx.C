/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    s_valloc.C                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 09, 1988                                          |
  |  <LEVEL>   STR                                                    |
  |  <PARAM>   vector      SVECTOR *          Address of the variable |
  |                                           holding the string vectr|
  |                                           to allocate             |
  |  <PARAM>   size        int                Number of vector entries|
  |                                           to allocate             |
  |  <PURPOSE> Allocate and initialze a string vector.                |
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

      result = 1 ;

   }
   else if ( vector == NULL )
   {

      result = 2 ;

   }
   else
   {


      area     =   sizeof ( SVECTOR ) +
                 ( sizeof ( SSCALAR ) * size ) ;
      StringVector = ( SVECTOR ) malloc ( area ) ;

      StringVector->TotalAllocated = size ;
      StringVector->TotalUsed      =    0 ;

   }
   return( result );

}
