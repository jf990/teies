/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_ckkeys.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    September 19, 1988                                     |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   result_keys  string * INOUT   Result string if goes ok.|
  |  <PARAM>   keys         string * INPUT   Keys to check and format.|
  |                                                                   |
  |  <PURPOSE> Takes a string of keywords and checks to see if they   |
  |            are correct in context.  If free keywords are the con- |
  |            text then the keys are just upper cased and slashed.   |
  |            If not free keywords then the string is checked and    |
  |            any keys not present in the index are formatted into   |
  |            the result string with a return code indicating errors |
  |            are returned.                                          |
  |  <OUTPUT>  sint                    the result code.  If 0, result |
  |                                    string is the formatted input  |
  |                                    string.  If non-0, the result  |
  |                                    string are the formatted errors|
  |  <CALLS>   s_upper(), s_cat()                                     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



sint      t_ckkeys(  result_keys, keys  )

string              *result_keys, keys;


{

sint     rc          =      0;
string   work_keys   = S_NULL,
         slash       = S_NULL;







if( s_len( keys ) > 0 )
{

   s_copy(  & work_keys, keys, 0, 0 );
   s_upper(   work_keys,       0, 0 );
   s_init(  & slash,     "/"        );

   if(  s_char_s( work_keys, 0 )  !=  '/'  )
   {

      s_cat(  & work_keys, slash, work_keys, 0, 0 );

   }

   if( s_char_s( work_keys, s_len( work_keys ) - 1 )  !=  '/'  )
   {

      s_cat(  & work_keys, work_keys, slash, 0, 0 );

   }
   s_drop( & slash );

}

s_copy( result_keys, work_keys, 0, 0 );

s_drop( & work_keys );



   return(rc);

}
