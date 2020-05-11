/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    odt_comp()                                             |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    December 20, 1988                                      |
  |                                                                   |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   component_entry   ODT *  INPUT   Component to compare. |
  |  <PARAM>   string_to_check   string INPUT   String to check to see|
  |                                             if in the ODT.        |
  |                                                                   |
  |  <PURPOSE> Return true if the string to check matches the ODT entry
  |            passed.                                                |
  |                                                                   |
  |  <OUTPUT>  boolean                                                |
  |                                                                   |
  |  <CALLS>   s_len(), s_comp()                                      |
  |                                                                   |
  |  <ERRORS>  None known                                             |
  |                                                                   |
  |  <LIMITS>  None known                                             |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   odt_comp( component_entry, string_to_check )

ODT             *component_entry;
string           string_to_check;

{

   sint        rc                =     0,
               len_to_compare    =     0;
   boolean     result_of_compare = FALSE;


      len_to_compare = s_len( odt_name( component_entry ));
      if(  len_to_compare > MAX_COMPONENT_NAME_LENGTH )
      {

         len_to_compare = s_len( string_to_check );

      }
      if (
           s_comp( string_to_check, odt_name( component_entry ), 0,
                                              len_to_compare, 'M', & rc )
           == 0 )
      {

         result_of_compare = TRUE;

      }



   return(result_of_compare);

}
