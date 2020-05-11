#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "teies.h"



main()

{


   OCD     *ocd_ptr           =  NIL;

   string   component         =  S_NULL,
            source            =  S_NULL,
            ocd_name          =  S_NULL,
            ocd_str_key       =  S_NULL,
            ocd_hex_key       =  S_NULL,
            quotes            =  S_NULL,
            ocd_bias          =  S_NULL,
            bias_str          =  S_NULL,
            ocd_num_keys      =  S_NULL,
            num_str           =  S_NULL,
            key_str           =  S_NULL,

            form_id           =  S_NULL,
            rc_form           =  S_NULL,
            quit              =  S_NULL,
            abandon           =  S_NULL;


   sint     offset            =  0,
            R                 =  0,
            rc                =  0,
            num_keys          =  0;

   byte    *byte_ptr          =  0;

   boolean  DONE              =  FALSE;


#D_main(OCD_MAIN, OBJ);
t_init ( S_NULL, S_NULL, NIL );

s_init( & form_id,      "OCD1"         );
s_init( & component,    "COMPONENT"    );
s_init( & ocd_name,     "OCD_NAME"     );
s_init( & ocd_str_key,  "OCD_STR_KEY"  );
s_init( & ocd_hex_key,  "OCD_HEX_KEY"  );
s_init( & ocd_bias,     "OCD_BIAS"     );
s_init( & ocd_num_keys, "OCD_NUM_KEYS" );
s_init( & quotes,       "\""           );

isv_get( & quit,    ISV_QUIT,    0     );
isv_get( & abandon, ISV_ABANDON, 0     );



while ( ! DONE )
{

   rc_form = t_form( form_id, S_NULL, NIL );

   if( ! t_ifquit( rc_form ) )
   {

      source = var_get( component );
      #D_str( 'source', 'What is provided by user' );

      if( s_len( source ) != 0 )
      {

         #D_say( 'trying to se the field' );
         f_setfld( form_id, component, source );

         ocd_new ( & ocd_ptr, NIL );
         s_copy  ( &(ocd_ptr->name), source, 0, 0 );

         #D_say( 'Calling ocd_dcd' );

         rc = ocd_dcd( ocd_ptr );
         #D_show( 'rc of ocd_dcd = %d', 'rc' );
         if ( rc == 0 )
         {

            var_set( ocd_name, ocd_ptr->name           );
            f_setfld( form_id, ocd_name, ocd_ptr->name );

            rc = s_verify( source, quotes, 0, 0, S_DISALLOW );
            if( rc == 0 )
            {

               #D_str( 'ocd_ptr->key', 'key before conversion' );
               s_mtoxs( &key_str, s_buffer( ocd_ptr->key ),
                                     s_len( ocd_ptr->key ));
               #D_str( 'key_str', 'whats in key_str after conversion' );

               var_set( ocd_hex_key, key_str              );
               f_setfld( form_id, ocd_hex_key, key_str    );

               var_clr( ocd_str_key );
               f_rstfld( form_id, NO_FLAGS, ocd_str_key   );

            }
            else
            {

               var_set( ocd_str_key, ocd_ptr->key           );
               f_setfld( form_id, ocd_str_key, ocd_ptr->key );

               var_clr( ocd_hex_key );
               f_rstfld( form_id, NO_FLAGS, ocd_hex_key     );

            }

            #D_show( 'bias is = %d', 'ocd_ptr->bias'   );
            s_sitos( & bias_str, ocd_ptr->bias         );
            #D_str( 'bias_str', 'bias str is'          );
            var_set( ocd_bias, bias_str                );
            f_setfld( form_id, ocd_bias, bias_str      );

            s_sitos( & num_str, ocd_ptr->num_keys      );
            var_set(   ocd_num_keys, num_str           );
            f_setfld( form_id, ocd_num_keys, num_str   );

         }  /* if rc of ocd_dcd = 0 */
         else
         {

            except( rc, USER_INFORMATION, S_NULL, S_NULL, S_NULL,
                                                  S_NULL, S_NULL  );

            var_clr( ocd_name       );
            var_clr( ocd_str_key    );
            var_clr( ocd_hex_key    );
            var_clr( ocd_bias       );
            var_clr( ocd_num_keys   );

            f_rstfld( form_id, NO_FLAGS, ocd_name     );
            f_rstfld( form_id, NO_FLAGS, ocd_str_key  );
            f_rstfld( form_id, NO_FLAGS, ocd_hex_key  );
            f_rstfld( form_id, NO_FLAGS, ocd_bias     );
            f_rstfld( form_id, NO_FLAGS, ocd_num_keys );

         }


      } /* if s_len */
      else
      {

         var_clr( ocd_name       );
         var_clr( ocd_str_key    );
         var_clr( ocd_hex_key    );
         var_clr( ocd_bias       );
         var_clr( ocd_num_keys   );

         f_rstfld( form_id, NO_FLAGS, ocd_name     );
         f_rstfld( form_id, NO_FLAGS, ocd_str_key  );
         f_rstfld( form_id, NO_FLAGS, ocd_hex_key  );
         f_rstfld( form_id, NO_FLAGS, ocd_bias     );
         f_rstfld( form_id, NO_FLAGS, ocd_num_keys );

      }

   } /* ! t_ifquit  */
   else if( s_comp( rc_form, quit, 0, 0, 'F', &R ) == 0 )
   {

      DONE = TRUE;
      rc   = USER_REQUESTED_QUIT;

   }
   else if( s_comp( rc_form, abandon, 0, 0, 'F', &R ) == 0 )
   {

      DONE = TRUE;
      rc   = USER_REQUESTED_ABANDON;

   }

   var_clr( component );

} /* while not done */

f_close( form_id, NO_FLAGS );

s_drop( & form_id       );
s_drop( & component     );
s_drop( & ocd_name      );
s_drop( & ocd_str_key   );
s_drop( & ocd_hex_key   );
s_drop( & ocd_bias      );
s_drop( & ocd_num_keys  );
s_drop( & quotes        );


t_shutdn( );
#D_return( rc )

}

