/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_valnam()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 6, 1988                                           |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   names       string     INPUT  String of names, each    |
  |                                          separated by a comma.    |
  |  <PARAM>   oid_array   OID **     INOUT  Pointer to a pointer to  |
  |                                          an array of OID pointers |
  |                                          where the resulting OID's|
  |                                          are to be stored.        |
  |  <PARAM>   array_size  sint   *   OUTPUT Size of oid_array.       |
  |  <PARAM>   bad_names   string *   OUTPUT String to store all the  |
  |                                          names that cannot be val-|
  |                                          idated.                  |
  |  <PARAM>   search_type hword      INPUT  Type of t_getnam() search|
  |                                                                   |
  |  <PURPOSE> Validate the list of names in the input string to see  |
  |            if they are valid TEIES users.  All names must be sep- |
  |            arated by commas.  The search method used can be asked |
  |            for by the flags parameter.  The default (flags=0) is  |
  |            ANY_NAME.  See the documentation on t_getnam() for more|
  |            details on types of search allowed.                    |
  |                                                                   |
  |            The caller must provide a pointer to where the result  |
  |            array of OIDs can be placed.  If this pointer points to|
  |            NIL then t_valnam() will allocated enough spaced to    |
  |            hold the array.  Else we assume the pointer points to a|
  |            pointer that has enough space to hold all the resulting|
  |            array of OID's.                                        |
  |                                                                   |
  |            If this user is not the master server then the request |
  |            is sent to the MS.  We then wait for a response back.  |
  |                                                                   |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <LIMITS>  Name validation is insensitive to case, all name strings
  |            are converted to upper case for searching.             |
  |                                                                   |
  |            Names cannot contain leading or trailing blanks.       |
  |                                                                   |
  |  <MODS>                                                           |
  |            Jan 14, 1989    JF     Added case insensitivity and    |
  |                                   trimming of each name string.   |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_prim.h"
#include "exc.h"
#include "tm.h"




sint     t_valnam( names, oid_array, array_size, bad_names, flags )

string             names;
OID             ***oid_array;
sint              *array_size;
string            *bad_names;
hword              flags;

{

   sint
            rc           =      0,
            i            =      0,
            errors       =      0;
   hword
            num_of_users =      0;
   string
            target       = S_NULL,
            extra        = S_NULL,
            next_name    = S_NULL,
           *vector       =    NIL,
            parser       = S_NULL,
            separator    = S_NULL;
   OID
           *name_oid     =    NIL;







rc = t_ckname( names, T_CKNAME_NONPRINTABLE_ONLY, & errors );
if( rc != TM_NONPRINTABLE_CHAR_IN_NAME )
{

   errors = 0;
   s_upper( names, 0, 0 );

   if( (oid_array != NIL)  &&  (names != S_NULL) )
   {

      s_init(   & parser,    " @~..@n,. "    );
      s_init(   & separator, "; "            );
      s_valloc( & vector,    40              );

      s_parse( & names, parser, S_NULL, vector, & extra   );

      num_of_users = s_smp_ul(  (string) vector );

      if( *oid_array == NIL )
      {

         *oid_array = (OID **) os_mema( num_of_users * sizeof( OID * ) );

      }
      *array_size = num_of_users;

      if( flags == (hword)0 )
      {

         flags = ANY_NAME;

      }

      for( i=1; i<=num_of_users; i++ )
      {

         s_copy(  & next_name, *(vector+i), 0, 0 );
         s_trim(  & next_name, 'B' );
         s_upper(   next_name, 0,0 );
         *( (*oid_array) + i ) = t_getnam( next_name, flags );

         if( *( (*oid_array) + i ) ==  NIL  )
         {

            errors++;
            if( s_len( *bad_names ) > 0 )
            {

               s_cat( bad_names, *bad_names, separator, 0, 0 );

            }
            s_cat( bad_names, *bad_names, next_name, 0, 0 );

         }
      }

      if( errors > 0 )
      {

         rc = NAME_NOT_FOUND + errors;

      }

      s_vdrop( & vector    );
      s_drop(  & parser    );
      s_drop(  & separator );
      s_drop(  & extra     );
      s_drop(  & next_name );

   }
   else
   {

      rc = INVALID_DATA_POINTER;

   }
}


   return(rc);

}
