/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <LEVEL>   TM                                                     |
  |  <NAME>    t_fname                                                |
  |  <AUTHOR>  Rana Ajaz-Ur-Rehman                                    |
  |  <DATE>    July 24, 1989                                          |
  |  <PARAMS>  first_name     string   INPUT    first name of user to |
  |                                             be searched in SNI.   |
  |                                                                   |
  |            last_name      string   INPUT    last name of user to  |
  |                                             be searched in SNI.   |
  |                                                                   |
  |            list_object    OCB *    INOUT    Pointer to list OCB   |
  |                                             to return OID's of the|
  |                                             matches found.        |
  |                                             Calling routine should|
  |                                             create a list to be   |
  |                                             passed, if not this   |
  |                                             routine will create   |
  |                                             a list ( only if      |
  |                                             matches are found ).  |
  |  <OUTPUT>  None                                                   |
  |  <PURPOSE> This routine finds all the matching name parts of      |
  |            a user from SNI.                                       |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_prim.h"


sint t_fname( name_part, last_name, first_name, list_ocb )

string      name_part,
            last_name,
            first_name;

OCB       **list_ocb;

{

  sint     rc                =  0,
           len               =  0,
           R                 =  0,
           list_size         =  0;


  byte     letter            =  ' ',
           *component        =  NIL;

  string   cand_first_name   =  S_NULL,
           cand_last_name    =  S_NULL,
           cand_name_part    =  S_NULL,
           obj_first_name    =  S_NULL,
           obj_last_name     =  S_NULL,
           current_name      =  S_NULL,
           req_name          =  S_NULL,
           got_name          =  S_NULL,
           oid_key_str       =  S_NULL;


  hword    to_do_list        =  0,
           searches_to_do    =  0,
           things_to_do      =  0,
           done_list         =  0,
           not_first_and_last=  0,
           current_step      =  0,
           next_step         =  0;

  OCD      *sni_ocd          =  NIL,
           *obj_ocd          =  NIL;

  OCB      *sni_ocb          =  NIL;

  OID       oid_struct,
            *oid_ptr         =  NIL;

  boolean   DONE                    =  FALSE,
            MORE_TO_SEARCH          =  TRUE,
            OK_TO_ADD_IN_LIST       =  FALSE,
            LIST_HAS_BEEN_CREATED   =  FALSE;



s_upper( name_part,  0, 0 );
s_upper( first_name, 0, 0 );
s_upper( last_name,  0, 0 );


if( ( len = s_len( name_part ) ) != 0 )
{

   s_copy( &cand_name_part, name_part, 0, 0 );
   s_stob( & letter, name_part, len );
   letter--;
   s_put( cand_name_part, letter, len );

   not_first_and_last =  ( NICK_NAME | NUMBER_NAME | PEN_NAME );
   to_do_list |= not_first_and_last;

}

if( ( len = s_len( last_name ) ) != 0 )
{

   s_copy( &cand_last_name, last_name, 0, 0 );
   s_stob( & letter, last_name, len );
   letter--;
   s_put( cand_last_name, letter, len );

   to_do_list |=  LAST_NAME;

}

if( ( len = s_len( first_name )) != 0 )
{

   s_copy( &cand_first_name, first_name, 0, 0 );
   s_stob( & letter, first_name, len );
   letter--;
   s_put( cand_first_name, letter, len );

   to_do_list |=  FIRST_NAME;

}


things_to_do = searches_to_do = to_do_list;


if( to_do_list )
{


   sni_ocb = t_sysndx (       SNI_OBJ      );
   if( OCB_POINTER_OK       ( sni_ocb )    )
   {

      ocd_new( & sni_ocd, NIL );
      sni_ocd->bias = 1;

      oid_ptr = &oid_struct;

      while( ! DONE )
      {

         if( to_do_list & NICK_NAME )
         {

            current_step = not_first_and_last;
            next_step = NICK_NAME;
            component = "SNI.NIC.OID";
            s_copy( & req_name, name_part, 0, 0          );
            s_copy( & current_name, cand_name_part, 0, 0 );

         }

         else if( to_do_list & NUMBER_NAME )
         {

            current_step = not_first_and_last;
            next_step = NUMBER_NAME;
            component = "SNI.NAME.OID";
            s_copy( & req_name, name_part, 0, 0          );
            s_copy( & current_name, cand_name_part, 0, 0 );

         }

         else if( to_do_list & PEN_NAME )
         {

            current_step = not_first_and_last;
            next_step = PEN_NAME;
            component = "SNI.PEN.OID";
            s_copy( & req_name, name_part, 0, 0          );
            s_copy( & current_name, cand_name_part, 0, 0 );

         }

         else if( to_do_list & LAST_NAME )
         {

            current_step = LAST_NAME;
            next_step = LAST_NAME;
            component = "SNI.LAST.OID";
            s_copy( & req_name, last_name, 0, 0          );
            s_copy( & current_name, cand_last_name, 0, 0 );

         }

         else if( to_do_list & FIRST_NAME )
         {

            current_step = FIRST_NAME;
            next_step = FIRST_NAME;
            component = "SNI.FIRST.OID";
            s_copy( & req_name, first_name, 0, 0         );
            s_copy( & current_name, cand_first_name, 0, 0);

         }


         s_copy( &( sni_ocd->key  ), current_name, 0, 0 );
         s_init( &( sni_ocd->name ), component          );

         searches_to_do =   things_to_do;
         searches_to_do &= ~current_step;



         MORE_TO_SEARCH = TRUE;

         while( MORE_TO_SEARCH )
         {

            OK_TO_ADD_IN_LIST = FALSE;

            rc = obj_get( sni_ocb, sni_ocd, oid_ptr              );
            s_copy( & got_name, sni_ocd->key, 0, 0  );

            if( rc == 0 )
            {

              if( s_comp( req_name, got_name, 1, s_len( req_name ),
                                                     'F', &R ) == 0 )
              {



               if( searches_to_do )
               {


                  ocd_new( & obj_ocd, NIL   );
                  obj_ocd->bias = 0;
                  oid_key( obj_ocd, oid_ptr );

                  if( searches_to_do == ( FIRST_NAME | LAST_NAME ) )
                  {

                     s_init( &( obj_ocd->name ), "SNI.OBJECT.FIRSTNAME");
                     rc = obj_get( sni_ocb, obj_ocd, & obj_first_name );
                     if( rc == 0 )
                     {

                        s_init(&(obj_ocd->name), "SNI.OBJECT.LASTNAME" );
                        rc =obj_get( sni_ocb, obj_ocd, & obj_last_name );
                        if( rc == 0 )
                        {

                           if( (s_comp( first_name, obj_first_name, 1,
                                  s_len( first_name ), 'F', &R ) == 0)
                             &&(s_comp( last_name, obj_last_name, 1,
                                  s_len( last_name ),'F', &R ) == 0) )
                           {

                              OK_TO_ADD_IN_LIST = TRUE;
                              if( to_do_list )
                              {

                                 to_do_list &= ~LAST_NAME;
                                 to_do_list &= ~FIRST_NAME;
                                 to_do_list &= ~current_step;

                                 done_list  |= ( LAST_NAME | FIRST_NAME
                                                        | current_step );
                              }

                           }
                           else
                           {

                              OK_TO_ADD_IN_LIST = FALSE;

                           }

                        } /* rc = 0 for last name */
                        else
                        {

                           MORE_TO_SEARCH = FALSE;
                           DONE = TRUE;

                        }

                     } /* rc = 0 for obj_get first_name */
                     else
                     {

                        MORE_TO_SEARCH = FALSE;
                        DONE = TRUE;

                     }

                  } /* first ored with last */
                  else if( searches_to_do == ( searches_to_do &
                                                     LAST_NAME ) )
                  {

                     s_init( &(obj_ocd->name), "SNI.OBJECT.LASTNAME" );
                     rc = obj_get( sni_ocb, obj_ocd, & obj_last_name );
                     if( rc == 0 )
                     {

                        if( s_comp( last_name, obj_last_name, 1,
                                   s_len( last_name ), 'F', &R ) == 0 )
                        {

                           OK_TO_ADD_IN_LIST = TRUE;
                           if( to_do_list )
                           {

                              to_do_list &= ~LAST_NAME;
                              to_do_list &= ~current_step;

                              done_list |= ( LAST_NAME | current_step );

                           }

                        }
                        else
                        {

                           OK_TO_ADD_IN_LIST = FALSE;

                        }

                     } /* rc = 0 of get for obj_last_name */
                     else
                     {

                        MORE_TO_SEARCH = FALSE;
                        DONE = TRUE;

                     }

                  } /* else if last_name */
                  else if( searches_to_do == ( searches_to_do &
                                                     FIRST_NAME ) )
                  {

                     s_init( &(obj_ocd->name), "SNI.OBJECT.FIRSTNAME" );
                     rc = obj_get( sni_ocb, obj_ocd, & obj_first_name );
                     if( rc == 0 )
                     {

                        if( s_comp( first_name, obj_first_name, 1,
                                  s_len( first_name ), 'F', &R ) == 0 )
                        {

                           OK_TO_ADD_IN_LIST = TRUE;
                           if( to_do_list )
                           {

                              to_do_list &= ~FIRST_NAME;
                              to_do_list &= ~current_step;

                              done_list |= ( current_step | FIRST_NAME );

                           }

                        }
                        else
                        {

                           OK_TO_ADD_IN_LIST = FALSE;

                        }

                     } /* rc = 0 of get for obj_first_name */
                     else
                     {

                        MORE_TO_SEARCH = FALSE;
                        DONE = TRUE;

                     }

                  } /* else if first_name */

               } /* if searches to do */
               else
               {

                  OK_TO_ADD_IN_LIST = TRUE;

               }

              } /* s_comp failed */
              else
              {


              }
            /*  * * * * * * * * * * * * * * * * * * * * * * * * * */
            /*                                                    */
            /*    The follwoing form related piece of code is     */
            /*    temporary, and should be taken out, because     */
            /*    no form business is to be done on TEIES MS.     */
            /*                                                    */
            /*  * * * * * * * * * * * * * * * * * * * * * * * * * */
            /* if( OK_TO_ADD_IN_LIST )                            */
            /* {                                                  */
            /*                                                    */
            /*    string   found_name  =  S_NULL,                 */
            /*             form_id     =  S_NULL,                 */
            /*             rc_form     =  S_NULL;                 */
            /*                                                    */
            /*    s_init( & found_name, "NAME"   );               */
            /*    s_init( & form_id,    "FOUND"  );               */
            /*                                                    */
            /*    var_set(  found_name, got_name );               */
            /*                                                    */
            /*    rc_form = t_form( form_id, S_NULL, NIL );       */
            /*                                                    */
            /*    f_close( form_id, NO_FLAGS );                   */
            /*    s_drop ( & form_id );                           */
            /*    s_drop ( & found_name );                        */
            /*                                                    */
            /* }                                                  */
            /*                                                    */
            /* * * * * * * * * * * * * * * * * * * * * * * * * * **/

               if( OK_TO_ADD_IN_LIST )
               {

                  if( list_ocb != NIL )
                  {

                     if( ! OCB_POINTER_OK( *list_ocb ) )
                     {

                        rc = cr_list( *list_ocb, S_NULL, 0, list_ocb );
                        if( rc == 0 )
                        {

                           LIST_HAS_BEEN_CREATED = TRUE;

                        }
                        else
                        {

                           LIST_HAS_BEEN_CREATED = FALSE;
                           MORE_TO_SEARCH = FALSE;
                           DONE = TRUE;

                        }

                     } /* ocb ponter not ok */
                     else
                     {

                        LIST_HAS_BEEN_CREATED = TRUE;


                     }

                     if( LIST_HAS_BEEN_CREATED == TRUE )
                     {


                        /*+-----------------------------------------+*/
                        /*|Just to have a unique key make oid a str |
                        /*|and pass it to md_list as key (modifier) |
                        /*+-----------------------------------------+*/

                        s_mtoxs( &oid_key_str, (byte*) oid_ptr,
                                                       sizeof( OID ) );

                        rc = md_list( *list_ocb, oid_key_str,
                                            OBJ_LIST_ADD, oid_ptr );

                        if( rc == 0 )
                        {



                        }
                        else
                        {

                           MORE_TO_SEARCH = FALSE;
                           DONE = TRUE;

                        }

                     } /* if list has been created */

                  } /* list_ocb is not nil */
                  else
                  {

                     MORE_TO_SEARCH = FALSE;
                     DONE = TRUE;
                     rc = INVALID_DATA_POINTER;

                  }

               } /* if ok to add in list */

            } /* if rc = 0  */
            else
            {

               MORE_TO_SEARCH = FALSE;

            }


         } /* while more to search */

         if( to_do_list )
         {

            to_do_list &= ~next_step;
            done_list  |=  next_step;

         }


         if( done_list == things_to_do )
         {

            DONE = TRUE;

         }

      } /* while not done */

   } /* if ocb pointer ok */
   else
   {

      rc = SNI_MISSING_OR_INVALID;
      except( rc, CONTROL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                                         S_NULL );


   }

} /* if to do list */

   return(rc);

}

