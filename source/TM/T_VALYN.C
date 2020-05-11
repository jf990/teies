/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             VAL_YN.C
:LEVEL.                            ACTIVITY
:AUTHOR.                           Heidi E. Harting
:DATE.                             Mar 24, 1988

:INPUT.             fld_val  :  a string containing the value of a
                                form-field to be validated.

:OUTPUT.            fld_err  :  FALSE if input was NO, '*' or NULL;
                                TRUE  if input was YES;
                                -1    if the input was neither YES or NO.


:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
          This routine will varify if the string value pasted, the value
          input in a form field, is a Y or N.

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"




sint t_valyn( fld_val )

string  fld_val;


{


   string  yes      = S_NULL,  no      = S_NULL,
           yes_copy = S_NULL,  no_copy = S_NULL;
   sint    R=0,         fld_err=0;



   s_upper(  fld_val, 0, 0 );




   isv_get( &yes, ISV_YES, 0 );
   isv_get( &no,  ISV_NO,  0 );

   s_copy( & yes_copy, yes, 0, 0 );
   s_copy( &  no_copy,  no, 0, 0 );

   s_upper( yes_copy, 0, 0 );
   s_upper(  no_copy, 0, 0 );

   if ( s_comp( fld_val, yes_copy, 0, s_len(fld_val), 'M', &R ) != 0 )
   {

      if ( s_comp( fld_val, no_copy, 0, s_len(fld_val), 'M', &R ) == 0
         || s_len( fld_val ) == 0
         || (s_len( fld_val ) == 1 && s_char_s( fld_val, 0 ) == '*') )
      {

         fld_err = FALSE;                  /* String is "NO"           */

      }
      else
      {                                    /* String is neither Y or N */

         fld_err = UNEXPECTED_RESPONSE;

      }
   }
   else
   {                                       /* String is "YES"          */

      fld_err = TRUE;

   }

   s_drop( & yes_copy );
   s_drop( &  no_copy );



   return(fld_err);
}
