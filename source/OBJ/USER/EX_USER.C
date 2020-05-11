/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ex_user()                                              |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    November 7, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   list_ocb    OCB *     IN/OUT  Pointer to list object   |
  |                                          where resultant OID's    |
  |                                          are stored or where to   |
  |                                          get OID's to get names   |
  |                                                                   |
  |  <PARAM>   user_name   string    INPUT   What to look up          |
  |  <PARAM>   search_type hword     INPUT   defined in tm.h  (tm_x)  |
  |  <PARAM>   params      string *  OUTPUT  Return user names        |
  |                                                                   |
  |  <PURPOSE> Check for existance of a user in the SNI.              |
  |  <OUTPUT>  sint rc  error code                                    |
  |  <CALLS>   t_sysndx(), except(), ocd_new(), obj_get(),            |
  |            s_copy(), t_getname()                                  |
  |  <ERRORS>          SNI_MISSING_OR_INVALID                         |
  |                    NO_DATA_TO_SEARCH_FOR                          |
  |                    INVALID_SEARCH_TYPE_REQUESTED                  |
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


sint     ex_user( object, usr_name, level, params )

OCB     * object;
string    usr_name;
hword     level;
string  * params;

{

   sint      rc             =      NO_ERR,
             rrc            =      NO_ERR,
             trc            =      NO_ERR,
             conv_type      =           0;
   boolean   ON_MS_ALREADY  =       FALSE;
   OCB     * list_ocb       =         NIL;
   string  * bad_names      =         NIL,
             bad_name_string=      S_NULL;
   boolean   do_not_save_bad_names = TRUE;




conv_type     = level & 0x00ff;        /* Get the lower byte           */
ON_MS_ALREADY = level & NO_NEED_TO_XMIT; /* If calling from MS,        */



if( ! OCB_POINTER_OK( object ) )
{


}
else
{

   bad_names = & bad_name_string;
   if( params == NIL || params == (string *) -1 )
   {

      do_not_save_bad_names = TRUE;
      s_alloc( bad_names, 1 );

   }
   else
   {

      s_nulout( params );
      do_not_save_bad_names = FALSE;
      bad_names = params;

   }
   list_ocb = object;

   if( list_ocb->type != LIST_OBJ )
   {

      rc = IMPROPER_OBJECT_TYPE;

   }
   else
   {

   if( TM_master->usrtype == MASTER_SERVER )
   {


      OCD      *data_ocd       =     NULL,
               *list_ocd       =     NULL;

      OCB      *user_ocb       =     NULL;

      OID       user_oid,
              **oid_arry       =     NULL;

      string    packet         =     S_NULL,
                null_str       =     S_NULL,
                errstr         =     S_NULL,
                key            =     S_NULL,
                name_str       =     S_NULL;

      sint      list_size      =     0,
                i              =     0,
                dummy_int      =     0,
                count          =     0;

      boolean   ok_to_link     =     FALSE;



      switch( conv_type )
      {

      case OID_TO_STRING :


         rc = obj_find( list_ocb, "LIST.SIZE", (byte *) &list_size );

         if( rc == NO_ERR && list_size != 0 )
         {

            ocd_new( &data_ocd, "LIST.LIST.OID");
            data_ocd->bias = 1;

                                  /* Allocate data for maximum usage by*/
                                  /* fullname and 2 bytes for len of   */
                                  /* each name and 4 bytes for total   */
                                  /* plus size of sint for number of   */
                                  /* strings in packet                 */

            s_alloc( &packet, MAX_FULLNAME_LEN * list_size +
                                 S_OVERHEAD * list_size + sizeof(sint) );

                                     /* Now copy size of the list         */
            os_memc( &list_size, s_buffer( packet ), sizeof( sint ) );
                                     /* And take account of its length    */
            s_smp_ul( packet ) = sizeof( sint );

            for( i = 0; i <= list_size; i++ )
            {

               if(obj_get(list_ocb, data_ocd,(byte *) &user_oid ) == NO_ERR)
               {


                  if(   (oid_cmp( (list_ocb->oid), user_oid ))
                     || OID_IS_NULL( &user_oid ) )
                  {

                     user_ocb = NULL;

                  }
                  else
                  {

                     user_ocb = obj_opn( &user_oid, OBJ_UPDATE );

                  }

                  rc = gt_name(user_ocb, S_NULL, FULL_TEIES_NAME,&name_str);

                  if( (user_ocb->type == USER_OBJ) && (rc == NO_ERR) )
                  {

                     s_catx( &packet, name_str, 0, 0 );

                  }
                  else
                  {
                                               /* Invalid user. Null name */
                     s_catb( &packet, (byte *)&dummy_int, sizeof( sint ) );

                  }

                  if( OCB_POINTER_OK(user_ocb) )
                  {                       /* Close only if ocb is valid  */
                                          /* or if it is NOT system oid  */
                      rc = obj_cls( user_ocb );

                  }

               }
               else
               {
                                 /* Get out of the loop. Should be an err */
                  break;         /* Since for loop will exit if list_size */
                                 /* should be equal to # of items in index*/
               }  /* if else obj_get()  */


            }  /* for */

            rc = NO_ERR;                  /* Reset RC just incase last    */
                                          /* is bad                       */


            if( ON_MS_ALREADY == FALSE )
            {

               rc = t_csend( t_invoker(), rc, packet);
                                       /* Send back the packet we created */
               s_drop( & packet   );

            }
            else
            {

               if( bad_names != NIL )
               {

                  *bad_names = packet;

               }
            }

            ocd_drp( & data_ocd );
            s_drop(  & name_str );

         }  /* If there was NO_ERR and there was something to process     */
            /* in the list                                                */
      break;

      case STRING_TO_OID :

         rrc = t_valnam( usr_name, &oid_arry, &count, &errstr, ANY_NAME);

         if( rrc == TM_BAD_CHAR_IN_NAME )
         {

            rrc = 0;

         }
         if( ((rrc / 100) * 100) == NAME_NOT_FOUND )
         {

            rrc = NAME_NOT_FOUND;

         }

         s_alloc( &key, sizeof( OID ) );  /* Alloc room for key. We're    */
         s_smp_ul( key ) = sizeof( OID ); /* using it to get rid of       */
                                          /* duplication in the list      */

         if( (rrc == NO_ERR) || ( rrc == NAME_NOT_FOUND) )
         {

            for( i = 1; i <= count; i++ )
            {

               if( *(oid_arry + i) == NULL )
               {

                  continue;              /* Why bother putting in bad OID */

               }

               os_memc( *(oid_arry + i), s_buffer( key ), sizeof( OID ) );
               rc = md_list( list_ocb, key, OBJ_LIST_ADD,
                                                (OID *) s_buffer( key ) );
               os_memd( *(oid_arry + i) );
               *(oid_arry + i) = NIL;
               if( rc != NO_ERR )
               {

                  break;               /* Get out! Problems with OCB      */

               }

            }  /* For */

         }  /* if NO_ERR while validating names */

         if( rrc == NO_ERR || ( rrc == NAME_NOT_FOUND) )
         {

            if( ON_MS_ALREADY == FALSE )
            {

               trc = t_m_obj( t_invoker(), list_ocb, &errstr, FALSE, 0 );

               if( trc == NO_ERR )
               {

                  rc = t_csend(t_invoker(), rrc, errstr );

               }

            }
            else
            {

               if( bad_names != NIL && bad_names != (string *) -1 )
               {

                  *bad_names = errstr;

               }
            }
         }
         rc = rrc;

         os_memd( oid_arry );
         s_drop(  & key     );

      break;   /* STRING_TO_OID */

      default :

         rc = INVALID_CHOICE;

      }  /* Switch */

   }  /* If this is a master server */
   else
   {
      sint     list_size      = 0,
               i              = 0;

      string   *str_vec       =    NIL;
      byte     *current_ptr   = S_NULL;



                                     /* Send object to Master server first*/
      if( (rc = t_m_obj(TM_master->MS,list_ocb,NULL,FALSE, 0 )) == NO_ERR )
      {

                                     /* Now execute ex_user() on MS       */
         rc = t_remote( TM_master->MS, USER_OBJ, EXIST_ACT, conv_type,
                                       list_ocb, usr_name,  NIL       );

         switch( conv_type )
         {

         case STRING_TO_OID :

            rc = t_waitc( TM_master->MS, bad_names );
            if( bad_names != NIL && bad_names != (string *) -1 )
            {


               if ( (rc == NO_ERR) && (s_len( *bad_names ) > 0) )
               {

                  rc = NAME_NOT_FOUND;

               }
            }

            break;

         case OID_TO_STRING :
            if ( (rc == NO_ERR) && (s_len( *bad_names ) > 0) )
            {

               current_ptr = (byte *) s_buffer( *bad_names );
               os_memc( current_ptr, &list_size, sizeof( sint ) );
               current_ptr += sizeof( sint );

               s_valloc( &str_vec, list_size );
               s_smp_ul( (string) str_vec) = (hword) list_size;


               for(i = 1; i <= list_size; i++ )
               {


                  if( s_len( (string) current_ptr ) > 0 )
                  {

                     s_copy( &str_vec[i], (string) current_ptr, 0, 0 );
                     current_ptr += s_len( (string) current_ptr )
                                 + S_OVERHEAD;

                  }
                  else
                  {

                     str_vec[i] = S_NULL;
                     current_ptr += S_OVERHEAD;

                  }

               }  /* for */


               if( bad_names != NIL && bad_names != (string *) -1 )
               {

                  *bad_names = (string) str_vec;

               }
            }  /* If no errors */

            break;

         default :

            rc = INVALID_CHOICE;

         }  /* switch */

      }

   }  /* if else this is not a Master Server */

   }  /* if else list OCB is what we expected */
}

if( do_not_save_bad_names  &&  bad_names != NIL )
{

  s_drop( bad_names );

}


   return(rc);

}
