/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    gt_name C                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 25, 1988                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb  OCB *             INPUT   OCB of the user to get  |
  |                                           the full name of.       |
  |  <PARAM>   modifier string        INPUT   not used.               |
  |  <PARAM>   level hword            INPUT   Used to determine how to|
  |                                           format the name.        |
  |  <PARAM>   params   string *      INPUT   String pointer where the|
  |                                           result will be stored.  |
  |                                           If S_NULL on entry will |
  |                                           be allocated.           |
  |  <PURPOSE> This routine will format an object's name if given     |
  |            the object control block of a non-system object.       |
  |            The variable level is used to determine how to format  |
  |            the name.  For example, in the case of a user type     |
  |            object, level indicates the following:                 |
  |                                                                   |
  |             0, 1 : Format full name "First M. Last (Nick, Numb)"  |
  |             2    : Full name only   "First M. Last"               |
  |             3    : Nick and Number  "(Nick, Numb)"                |
  |             4    : Nickname only, and if the nickname is NIL then |
  |                     return the number.                            |
  |             5    : Concatenate first name and number (to use as   |
  |                     SNI first name index key)                     |
  |             6    : Concatenate last name and number (to use as    |
  |                     SNI last name index key)                      |
  |                                                                   |
  |             The result is stored where the pointer params points  |
  |             to.  If params points to S_NULL a new string is al-   |
  |             located.                                              |
  |  <OUTPUT>  sint - pass back any error status from obj_* routines. |
  |  <CALLS>   obj_get(), s_cat(), s_put(), os_memc(), s_alloc()      |
  |  <LIMITS>  If the string must be allocated, the calling routine is|
  |            expected to drop it when done.                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "exc.h"
#include "obj_prim.h"



sint   gt_name( ocb, modifier, level, params )

OCB            *ocb;
string          modifier;
hword           level;
string         *params;


{

OCD          *ocd =    NIL;
sint          rc  =      0,
           keysize=     32,
           max_len=      0;
hword         len =      0;
string     first  = S_NULL,
           middle = S_NULL,
           last   = S_NULL,
           nick   = S_NULL,
           numb   = S_NULL;


#D_begin(  gt_name, OBJ  );
#D_show( 'OCB=x%x, level=%d, params=x%x', 'ocb, level, params' );


if( OCB_POINTER_OK( ocb ) )
{

   if( params == NIL )
   {

      #D_say( 'Result pointer (params) is NIL !' );
      except( INVALID_DATA_POINTER, DATA_ERROR, S_NULL,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
      #D_leave( INVALID_DATA_POINTER,\
                        'Cannot continue operation due to data error.' );

   }
   #D_str( '*params', 'Destination string for the name' );
   switch( ocb->type )
   {

      case USER_OBJ :

         switch( level )
         {


            case 0     :
            case FULL_TEIES_NAME :

                       ocd_new( & ocd, "NAME.FIRST" );
                       rc = obj_get( ocb, ocd, & first );
                       if( rc == 0 || rc == NO_DATA_FOUND )
                       {

                          s_init( &(ocd->name), "NAME.LAST" );
                          obj_get( ocb, ocd, & last   );
                          s_init( &(ocd->name), "NAME.NUMBER" );
                          obj_get( ocb, ocd, & numb   );
                          s_init( &(ocd->name), "NAME.MIDDLE" );
                          obj_get( ocb, ocd, & middle );
                          s_init( &(ocd->name), "NAME.NICKNAME" );
                          obj_get( ocb, ocd, & nick   );

                              /****************************************
                               *                                      *
                               * Allocate the maximum length of the   *
                               * name even though there may not be    *
                               * a middle initial.  Later we will fix *
                               * the result string length based on    *
                               * what we put in it.                   *
                               *                                      *
                               ****************************************/

                          s_alloc( params, 4+8+s_len( first )+\
                             s_len( last )+s_len( nick )       );

                          os_memc( s_buffer( first ), s_buffer(*params ),
                             s_len( first ) );
                          len = s_len( first )+1;
                          s_put( *params, ' ', len );
                          len++;
                          if( s_len( middle ) > 0 )
                          {

                             s_put( *params, *s_buffer( middle ), len );
                             ++len;
                             s_put( *params, '.', len );
                             ++len;
                             s_put( *params, ' ', len );
                             ++len;

                          }
                          os_memc( s_buffer( last ),
                             s_buffer( *params )+len-1, s_len( last ));
                          len += s_len( last );
                          s_put( *params, ' ', len );
                          ++len;
                          s_put( *params, '(', len );
                          ++len;
                          os_memc( s_buffer( nick ),
                             s_buffer( *params )+len-1, s_len( nick ) );
                          len += s_len( nick );
                          s_put( *params, ',', len );
                          os_memc( s_buffer( numb ),
                             s_buffer( *params )+len, 4 );
                          len += 5;
                          s_put(    *params, ')', len );
                          s_smp_ul( *params   ) = len  ;
                          s_put( *params, '\0', len+1 );
                          s_drop(  & first   );
                          s_drop(  & middle  );
                          s_drop(  & last    );
                          s_drop(  & nick    );
                          s_drop(  & numb    );
                          rc = 0;

                       }
                       break;

            case FULL_NAME_ONLY :

                       #D_say( 'Full name not yet implemented.' );
                       break;

            case NICKNAME_AND_NUMBER :

                          ocd_new(     &ocd, "NAME.NICKNAME"  );
                          rc = obj_get( ocb, ocd, &nick       );
                          s_init( &(ocd->name), "NAME.NUMBER" );
                          rc = obj_get( ocb, ocd, &numb );
                          if( rc != 0  ||  s_len( nick ) < 1  )
                          {

                             #D_say( 'Nickname cannot be determined.');

                          }
                          else
                          {

                             len = 1;
                             s_alloc( params, s_len( nick ) +
                                              s_len( numb ) + 3 );
                             s_put( *params, '(', len );
                             ++len;
                             os_memc( s_buffer( nick ),
                               s_buffer( *params )+len-1, s_len( nick ));
                             len += s_len( nick );
                             s_put( *params, ',', len );
                             os_memc( s_buffer( numb ),
                             s_buffer( *params )+len, 4 );
                             len += 5;
                             s_put( *params, ')', len );
                             s_smp_ul( *params ) = len;
                             s_put( *params, '\0', len+1 );
                             s_drop(  & nick    );
                             s_drop(  & numb    );

                          }
                          break;

            case NICKNAME_ONLY :

                          ocd_new(     &ocd, "NAME.NICKNAME"  );
                          rc = obj_get( ocb, ocd, &nick       );
                          if( rc != 0  ||  s_len( nick ) < 1  )
                          {

                             s_init( &(ocd->name), "NAME.NUMBER" );
                             rc = obj_get( ocb, ocd, &nick );

                          }
                          if( rc != 0  ||  s_len( nick ) < 1  )
                          {

                             #D_say( 'Nickname cannot be determined.');

                          }
                          else
                          {

                             s_copy( params, nick, 0, 0 );

                          }
                          s_drop(  & nick    );
                          break;

            case SNI_KEY_FIRST_NAME :

               ocd_new(     & ocd, "NAME.FIRST" );

            case SNI_KEY_LAST_NAME :

               if( level == SNI_KEY_LAST_NAME )
               {

                  ocd_new(     & ocd, "NAME.LAST" );

               }
               rc = obj_get(  ocb, ocd, & first    );
               s_init( & ( ocd->name ), "NAME.NUMBER" );
               rc = obj_get( ocb, ocd, & numb );
               max_len = s_len( first );
               if( max_len > (keysize - 4) )
               {

                  max_len = keysize - 4;

               }
               s_cat( params, first, numb, 0, max_len );
               s_drop( & first );
               s_drop( & numb  );
               break;

            default :

               #D_e_show( 'Option %d not implemented.','level' );
               break;

         }
         break;

      default      :

         #D_say( 'Object name get not implemented.' );

   }
   #D_str( '*params', 'Resulting string (if rc == 0).' );

   if( ocd != NIL )
   {

      ocd_drp( & ocd );

   }
}
else
{

   #D_say( 'Bad object control block - operation not done.' );
   rc = INVALID_OCB;

}

#D_leave( rc, '<<<==If != 0 then function error.' );

}
