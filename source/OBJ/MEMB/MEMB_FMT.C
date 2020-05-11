/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                  memb_fmt
<LEVEL>                 OBJ
<AUTHOR>                Sal Johar
<DATE>                  01/26/89
<PARAM>                 membership   OCB *   INPUT
<OUTPUT>                sint
<PURPOSE>               Format the Membership object entries for future
                        display.

                        If we are running on the user server side, just
                        fake the formatting for now so we can display
                        something.  The actual formatting does not happen
                        until the membership is fully linked on the
                        Master Server side.
</DOC>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "bse.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"



sint memb_fmt(  memb_ocb  )

     OCB      * memb_ocb;


{

   PRIVS    roles             ;
   sint     root      =      0,
            rc        =      0,
            bse_value =      0,
          * line_num  =      0;
   string   root_name  = S_NULL,
            roles_name = S_NULL,
            bse_name   = S_NULL,
            line_name  = S_NULL,
            orig_name  = S_NULL,
            line      = S_NULL,
            name_str  = S_NULL,
            paragraph = S_NULL,
            space     = S_NULL,
            tmpstr    = S_NULL;
   OCD    * content_ocd =  NIL,
          * memb_ocd  =    NIL;
   OCB    * user_ocb  =    NIL;
boolean     user_server = FALSE;




if( OCB_POINTER_OK( memb_ocb ) )
{

   if( TM_master->usrtype == USER_SERVER )
   {

      user_server = TRUE;

   }
   s_init ( & paragraph,  "<p>"               );
   s_init ( & space,      "<space 0>"         );
   s_init ( & bse_name ,  "TMP.CONTENTS.BSE"  );
   s_init ( & line_name,  "TMP.CONTENTS.TEXT" );
   s_init ( & orig_name,  "TMP.CONTENTS"      );
   ocd_new( & content_ocd, NIL                );
   content_ocd->name  =    orig_name           ;

   obj_rmv( memb_ocb, content_ocd );

   s_alloc( &       (content_ocd->key),   BSE_MAX_KEY_SIZE );
   os_memn( s_buffer(content_ocd->key),   BSE_MAX_KEY_SIZE );
   s_smp_ul(         content_ocd->key)  = BSE_MAX_KEY_SIZE  ;

   s_init (  & root_name,  "MEMBERSHIP.MEMBERS.VARIABLES.ROOT_RANGE" );
   ocd_new(  & memb_ocd,   NIL                                       );
   s_init(   & roles_name, "MEMBERSHIP.MEMBERS.ROLES"                );
   memb_ocd->name =        roles_name;
   memb_ocd->bias =        1;

   bse_value = B_RFIRST | B_PRINTABLE;
   line_num   = (sint *)        s_buffer( content_ocd->key );

   while( obj_get( memb_ocb, memb_ocd, & roles ) == NO_ERROR )
   {


      memb_ocd->name =  root_name;
      memb_ocd->bias =         0;

      obj_get( memb_ocb, memb_ocd, & root );

      memb_ocd->name = roles_name;
      memb_ocd->bias =         1;


      if( user_server )
      {

         s_init( & name_str, "(TEIES Phantom User)" );

      }
      else
      {

         user_ocb = obj_opn( (OID *) s_buffer( memb_ocd->key ),
                                                            OBJ_READ );
         if( OCB_POINTER_OK( user_ocb ) )
         {

           rc = gt_name( user_ocb, S_NULL, FULL_TEIES_NAME, & name_str );
           obj_cls( user_ocb );

         }
      }
      if(  rc == NO_ERROR  )
      {


         ( * line_num ) ++ ;
         content_ocd->name = line_name;

         rc = obj_rep( memb_ocb, content_ocd, name_str, SEG_UPDATE );

         if( rc != NO_ERROR )
         {

            break;

         }
         content_ocd->name = bse_name;

         rc = obj_rep( memb_ocb, content_ocd, & bse_value, SEG_UPDATE );
         bse_value = B_RCONT | B_PRINTABLE;

      }
   }                                                          /* while */
   s_drop(  & root_name  );
   s_drop(  & bse_name   );
   s_drop(  & line_name  );
   s_drop(  & line      );
   s_drop(  & name_str  );
   s_drop(  & tmpstr    );
   s_drop(  & roles_name );
   s_drop(  & orig_name  );
   s_drop(  & space     );
   s_drop(  & paragraph );

   content_ocd->name = S_NULL;
   memb_ocd->name = S_NULL;

   ocd_drp( & content_ocd  );
   ocd_drp( & memb_ocd  );

}
else
{

   rc = INVALID_OCB;

}

   return(rc);

}
