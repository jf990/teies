/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    vw_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 23, 1988    ( from JAU Danmark )                |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   some_ocb    OCB *      INPUT  OCB to view              |
  |  <PARAM>   modifier    string     INPUT                           |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   params      byte *     INPUT                           |
  |                                                                   |
  |  <PURPOSE> View Mail given the following possibilities:           |
  |                                                                   |
  |            Level is ISV_NEW then any mail item or list will be    |
  |            considered New Mail with respect to this user.         |
  |                                                                   |
  |            Level is ISV_OLD then any mail or list is considered   |
  |            Old or already received.                               |
  |                                                                   |
  |            Level is ISV_MARKED then the mail item or list of items|
  |            is considered to be Marked.                            |
  |                                                                   |
  |            If Level is 0 New Mail is assumed.                     |
  |                                                                   |
  |            If Modifier is not S_NULL then it is a string from the |
  |            user that must be parsed containing a list of mail     |
  |            item identifiers to be verified into a list for pro-   |
  |            cessing.                                               |
  |                                                                   |
  |            The input object can be 3 possible types:              |
  |                                                                   |
  |                Mail - then show the single piece of mail.         |
  |                List - the list contains mail items.  Items other  |
  |                       than mail will not be processed.            |
  |                User - Process mail associated to this user by the |
  |                       level assignments above.                    |
  |                                                                   |
  |            NOTE: View calls Scan, Abstract or Get based on the    |
  |            user's set preferance.  The default is Get to show the |
  |            whole mail item one at a time.  Scan and abstract show |
  |            a list format where the user can choose which items to |
  |            perform actions on.                                    |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <VARS>    TOTAL_ITEM         Total number of mail items in the   |
  |                               list of mail items to be viewed.    |
  |            NUMB_ITEM          List entry number of the current    |
  |  <CALLS>                                                          |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"
#include "form.h"
#include "obj_prim.h"




sint     vw_mail( some_ocb, modifier, level, params  )

OCB              *some_ocb;
string                      modifier;
hword                                 level;
byte                                        *params;


{

string state_var = S_NULL,
       holder    = S_NULL,
       cmd_line  = S_NULL,
       parser    = S_NULL,
       extra     = S_NULL,
       mail_key  = S_NULL,
      *strvec    =    NIL,
       MAIL_ITEMS= S_NULL,
       NUMB_ITEM = S_NULL,
       TOTAL_ITEM= S_NULL;
byte  *component =    NIL;
sint   i         =      0,
       rc        =      0,
       key_len   =      0,
       count     =      0,
       amount    =      0;
OCB   *mail_ocb  =    NIL,
      *list_ocb  =    NIL;
OCD   *ocd       =    NIL,
      *smi_ocd   =    NIL;
OID    oid               ,
       mail_oid          ;
hword  mail_type =      0,
       key_press =      0;



#D_begin(  vw_mail,  OBJ  )
#D_ocb(   'some_ocb'      );
#D_str(   'modifier', 'passed modifier to view mail.'          );
#D_show(  'Level is %d; Param Pointer is x%x.', 'level,params' );


s_init( & NUMB_ITEM,  "NUMB_MAIL"  );
s_init( & TOTAL_ITEM, "TOTAL_MAIL" );


if(   level    == VW_MAIL_INTERACTIVE ||
    ((some_ocb == NIL) && (level == 0)) )
{

   if( s_len( modifier ) > 0 )
   {

      some_ocb = TM_master->ucb->directory;
      #D_ocb('some_ocb');

      #D_say( 'Assuming VIEW MAIL from command line' )
      ocd_new(  & smi_ocd, "SMI.MESSAGE.OID" );
      s_init(   & parser, "@~.. @n ,."       );
      s_valloc( & strvec, 40                 );

      s_parse(  & modifier, parser, S_NULL, strvec, &extra   );
      #D_strvec( 'strvec', 'Mail id to view' );
      s_sitos( & holder,   s_len( (string) strvec) );
      var_set( TOTAL_ITEM, holder );

      s_alloc( & mail_key,     sizeof( datetime ) + 2 );
      s_smp_ul(  mail_key  ) = sizeof( datetime ) + 2  ;
      os_memc( & mail_type,      s_buffer( mail_key ),  2 );

      for( i=1; i<= s_len( (string)strvec ); i++ )
      {

         s_sitos( & holder, i );
         var_set( NUMB_ITEM,  holder );

         s_copy( &(smi_ocd->key), *(strvec + i), 0, 0);
         #D_ocd(  'smi_ocd' );
         rc = t_fndsys( SMI_OBJ, smi_ocd, & mail_oid );
         if( rc == 0 )
         {

            mail_ocb = obj_opn( & mail_oid, OBJ_UPDATE | OBJ_REMOTE );
            rc       = gt_mail(   mail_ocb, modifier, level,    NIL );
                       obj_cls(   mail_ocb    );
            if( rc  == USER_REQUESTED_ABANDON )
            {

               break;

            }
            else if ( rc == USER_REQUESTED_QUIT )
            {

               rc = 0;
               break;

            }
         }
         else
         {

            #D_say( 'Requested mail does not exist.' );
            except( MAIL_DOES_NOT_EXIST, USER_ERROR, *(strvec+i), S_NULL,
                                                  S_NULL, S_NULL, S_NULL );

         }
      }
      smi_ocd->key = S_NULL;
      ocd_drp( & smi_ocd );
      s_drop(  & parser  );
      s_vdrop( & strvec  );

   }
   else
   {

      #D_say( 'Nothing to view so present screen.' );
      s_init( & state_var,  "1261"       );
      s_init( & MAIL_ITEMS, "MAIL_ITEMS" );
      key_press = USER_REQUESTED_ENTER;

      while(  ! t_ifquit( key_press ) )
      {

         key_press = t_form( state_var, S_NULL, NIL );

         if( key_press == USER_REQUESTED_PROCESS
         ||  key_press == USER_REQUESTED_ENTER    )
         {

            cmd_line = var_get( MAIL_ITEMS );
            if( s_len( cmd_line ) > 0 )
            {

               rc = vw_mail( NIL, cmd_line, VW_MAIL_INTERACTIVE, NIL );
               key_press = (hword) rc;

            }
            else
            {

               #D_say( 'May want to say something like ENTER some items dummy');

            }
         }
         /* If rc is a user level error put except() here */

      }
      f_close(  state_var, STATE_FORM );
      var_clr(  MAIL_ITEMS );
      s_drop( & state_var  );
      s_drop( & MAIL_ITEMS );

   }
}
else
{

   if( ! OCB_POINTER_OK( some_ocb ) )
   {

      #D_say( 'Assuming VIEW NEW MAIL' );
      some_ocb = TM_master->ucb->directory;

   }


   if( OCB_POINTER_OK( some_ocb ) )
   {


      switch( some_ocb->type )
      {


         case  MAIL_OBJ :
               {

                  rc = gt_mail( some_ocb, modifier, level, params );
                  break;

               }

         case  LIST_OBJ :
               {

                  rc = gt_list( some_ocb, modifier, level, params );
                  break;

               }

         case  USER_OBJ :
               {

                  #D_say( 'From User: get associated mailbox.' );
                  if( level == ISV_NEW )
                  {

                     component = "INDEXES.MAILBOX";

                  }
                  else if( level == ISV_OLD || level == 0 )
                  {

                     component = "INDEXES.OLD_MAIL";

                  }
                  else
                  {

                     #D_say( 'Cannot handle given level.' );
                     break;

                  }
                  ocd_new( & ocd, component );
                  obj_get( some_ocb, ocd, & oid );
                  list_ocb = obj_opn( & oid, OBJ_UPDATE | OBJ_REMOTE );
                  if( OCB_POINTER_OK( list_ocb ) )
                  {

                     rc = gt_list( list_ocb, modifier, level, params );

                  }
                  else
                  {

                     rc = MAIL_DOES_NOT_EXIST;

                  }
                  ocd_drp( & ocd );
                  obj_cls( list_ocb );

               }

               break;


         case  NOTIF_OBJ :
               {

                  #D_say( 'Cannot handle notifications yet.' );
                  break;

               }


         default        :
               {

                  #D_say( 'Invalid object type.' );
                  rc = IMPROPER_OBJECT_TYPE;

               }

      }
   }
   else
   {

      rc = INVALID_OCB;

   }
}
if( holder != S_NULL ) { s_drop( & holder ); }

#D_return( 'rc' )

}
