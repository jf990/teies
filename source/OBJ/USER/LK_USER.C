/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_user                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   ocb         struct OCB * INPUT   Control block of the  |
  |                                             object to link.       |
  |                                                                   |
  |  <PARAM>   modifier    string               Packet if used.       |
  |  <PARAM>   level       hword                Dummy ptr, not used.  |
  |  <PARAM>   result_str  String *             Where to store result.|
  |                                                                   |
  |  <PURPOSE> Link the user object into the proper indexes. Check to |
  |            see 1) if the user OID is already indexed, if so, only |
  |            replace the changed values; 2) if a new value is in use|
  |            by another user OID, then we cannot index the new value|
  |            as is.                                                 |
  |                                                                   |
  |            Also, fill in all default information in the users     |
  |            object, initializing the stats index, user status, etc.|
  |                                                                   |
  |            If this is not the MS, then send the user object there |
  |            for linking.                                           |
  |                                                                   |
  |            All name parts in the SNI are stored in upper case and |
  |            are searched insensitive to case.  To get the actual   |
  |            case the user wants it in get the compontent out of the|
  |            user's object, not the SNI.  The SNI only locates a    |
  |            user object.                                           |
  |                                                                   |
  |  <OUTPUT>  sint                    result code                    |
  |  <CALLS>   obj_get(), obj_rep(), ocd_new(), t_sysndx(), oid_key(),|
  |  <ERRORS>  SNI_MISSING_OR_INVALID                                 |
  |  <LIMITS>  Owner not linked.                                      |
  |            Groups not linked.                                     |
  |            Initial conferences not linked.                        |
  |  <MODS>                                                           |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "obj_prim.h"




sint     lk_user( user, modifier, level, result_str )

OCB              *user;
string            modifier;
hword             level;
string           *result_str;



{



OCB            *sni      =   NIL;
OCD            *sni_ocd  =   NIL,
               *name_ocd =   NIL,
               *user_ocd =   NIL;
OID             dummy_oid;
sint            rc       =     0,
                state    =     0,
                max_len  =     0,
                errors   =     0;
string         *temp     = S_NUL,
                namepart = S_NUL,
                firstname= S_NUL,
                lastname = S_NUL,
                penname  = S_NUL,
                fullname = S_NUL,
                nickname = S_NUL,
                number   = S_NUL,
                systemid = S_NUL,
                separator= S_NUL,
                sni_key  = S_NUL,
                report   = S_NUL;
boolean         unindex  = FALSE;








if( TM_master->usrtype == MASTER_SERVER )
{


   sni = t_sysndx( SNI_OBJ );
   if( ! OCB_POINTER_OK( sni ) )
   {

      except( SNI_MISSING_OR_INVALID, CONTROL_ERROR, S_NULL,
                             S_NULL, S_NULL, S_NULL, S_NULL);
   return(SNI_MISSING_OR_INVALID);

   }

   s_init(  & separator, "; " );
   ocd_new( & sni_ocd, "SNI.OBJECT.SYSTEMID"   );
   oid_key(   sni_ocd, & (user->oid)           );

   rc = obj_get( sni, sni_ocd, & systemid      );

   if( rc == 0 )
   {                                           /************************/
                                               /* If the entry for this*/
                                               /* OID is in the Names  */
                                               /* Index, we must remove*/
                                               /* all references to it */
                                               /* before we put the new*/
                                               /************************/
      unindex = TRUE;
      s_init(  & (sni_ocd->name), "SNI.OBJECT.LASTNAME"   );
      obj_get(    sni, sni_ocd,        & lastname         );
      s_init(  & (sni_ocd->name), "SNI.OBJECT.FIRSTNAME"  );
      obj_get(    sni, sni_ocd,        & firstname        );
      s_init(  & (sni_ocd->name), "SNI.OBJECT.NICKNAME"   );
      obj_get(    sni, sni_ocd,        & nickname         );
      s_init(  & (sni_ocd->name), "SNI.OBJECT.PENNAME"    );
      obj_get(    sni, sni_ocd,        & penname          );
      s_init(  & (sni_ocd->name), "SNI.OBJECT.FULLNAME"   );
      obj_get(    sni, sni_ocd,        & fullname         );
      s_init(  & (sni_ocd->name), "SNI.OBJECT.NUMBER"     );
      obj_get(    sni, sni_ocd,        & number           );

   }
   ocd_new( & user_ocd, "SYSTEM.ID"         );
   oid_key(   user_ocd, & (user->oid)       );
   ocd_new( & name_ocd, "SNI.SYSTEM.OID"    );

   rc = obj_get( user, user_ocd, & namepart );

   /* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
    | For each indexable name part in the order of (1) SYSTEM ID, |
    | (2) TEIES USER NUMBER, (3) TEIES NICKNAME, (4) PEN-NAME,    |
    | (5) full name, lastname, firstname, determine if the part to|
    | be indexed is different than the part already there.  If    |
    | they are different then we need to look it up again in its  |
    | separate index to verify the name belongs to the same user  |
    | (OID).  Therefore, we only index the name part if it is     |
    | unique.  If it has been changed, we remove the old reference|
    | If it is a conflict (another OID already owns it), then we  |
    | hold it to report an error (unless it is the last case since|
    | we do not index unique first, last or full names).          |
   \* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

   if( rc == 0 )
   {

      s_upper( namepart, 0, 0 );

                               /* If the name part (in this case, System
                                  ID) does not match what was already in
                                  the index, then attempt to index it. */

      if( s_comp( namepart, systemid, 0, 0, 'M', NIL ) > 0 )
      {

                               /* Now look it up in the part index (in
                                  this case, System ID) to verify there
                                  is no conflict with another OID.     */

          s_init ( &  name_ocd->name, "SNI.SYSTEM.OID"      );
          s_copy(  & (name_ocd->key), namepart, 0, 0        );

          if( obj_get( sni, name_ocd, & dummy_oid ) != 0 )
          {
                               /* The part does not exist, so we can go
                                  ahead and put it in.                 */

             rc = obj_rep( sni, name_ocd, & (user->oid), SEG_UPDATE );

             rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE       );

             if( unindex )
             {

                s_init ( &  name_ocd->name, "SNI.SYSTEM.ID" );
                s_copy( & (name_ocd->key), systemid, 0, 0 );
                rc = obj_rmv( sni, name_ocd );

             }
          }
          else
          {

                               /* Now we have trouble, as the name part
                                  chosen is already indexed to another
                                  OID.  Save this error and then continue
                                  to the end to tally all errors.      */

             errors ++ ;
             s_cat( & report, report, namepart,  0, 0 );
             s_cat( & report, report, separator, 0, 0 );

          }
      }
   }


   s_init( & (user_ocd->name), "NAME.NUMBER" );
   rc = obj_get( user, user_ocd, & namepart  );

   if( rc == 0 )
   {


      if( s_comp( namepart, number,  0, 0, 'M', NIL ) )
      {

          s_init ( &  name_ocd->name, "SNI.NAME.OID"   );
          s_copy(  & (name_ocd->key), namepart, 0, 0   );

          if( obj_get( sni, name_ocd, & dummy_oid ) != 0 )
          {

             rc = obj_rep( sni, name_ocd, & (user->oid), SEG_UPDATE );

             s_init( & (sni_ocd->name), "SNI.OBJECT.NUMBER"   );
             rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE );

             if( unindex )
             {

                s_init ( &  name_ocd->name, "SNI.NAME.NUMBER" );
                s_copy( & (name_ocd->key), number, 0, 0 );
                rc = obj_rmv( sni, name_ocd );

             }
          }
          else
          {

             errors ++ ;
             s_cat( & report, report, namepart,  0, 0 );
             s_cat( & report, report, separator, 0, 0 );

          }
      }
   }
   s_copy( & number, namepart, 0, 0 );

   s_init( & (user_ocd->name), "NAME.NICKNAME" );
   rc = obj_get( user, user_ocd, & namepart    );

   if( rc == 0 )
   {

      s_upper( namepart, 0, 0 );

      if( s_comp( namepart, nickname,  0, 0, 'M', NIL ) > 0 )
      {

          s_init( & (name_ocd->name), "SNI.NIC.OID"  );
          s_copy( & (name_ocd->key ), namepart, 0, 0 );

          if( obj_get( sni, name_ocd, & dummy_oid ) != 0 )
          {

             rc = obj_rep( sni, name_ocd, &(user->oid), SEG_UPDATE );

             s_init( &(sni_ocd->name), "SNI.OBJECT.NICKNAME"    );
             rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE );

             if( unindex )
             {

                s_init( & (name_ocd->name), "SNI.NIC.NAME"  );
                s_copy( & (name_ocd->key), nickname, 0, 0 );
                rc = obj_rmv( sni, name_ocd );

             }
          }
          else
          {

             errors ++ ;
             s_cat( & report, report, namepart,  0, 0 );
             s_cat( & report, report, separator, 0, 0 );

          }
      }
   }


   s_init( & (user_ocd->name), "PRIVATE.PEN_NAME" );
   rc = obj_get( user, user_ocd, & namepart       );

   if( rc == 0 )
   {

      s_upper( namepart, 0, 0 );

      if( s_comp( namepart, penname, 0, 0, 'M', NIL ) > 0 )
      {

          s_init( & (name_ocd->name), "SNI.PEN.OID"  );
          s_copy( & (name_ocd->key ), namepart, 0, 0 );

          if( obj_get( sni, name_ocd, & dummy_oid ) != 0 )
          {

             rc = obj_rep( sni, name_ocd, & (user->oid), SEG_UPDATE );

             s_init( & (sni_ocd->name), "SNI.OBJECT.PENNAME"   );
             rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE  );

             if( unindex )
             {

                s_init( & (name_ocd->name), "SNI.PEN.NAME"  );
                s_copy( & (name_ocd->key), penname, 0, 0 );
                rc = obj_rmv( sni, name_ocd );

             }
          }
          else
          {

             errors ++ ;
             s_cat( & report, report, namepart,  0, 0 );
             s_cat( & report, report, separator, 0, 0 );

          }
      }
   }


   s_init( & (user_ocd->name), "NAME.LAST"  );
   rc = obj_get( user, user_ocd, & namepart );

   if( rc == 0 )
   {

      s_upper( namepart, 0, 0 );
      if( s_comp( namepart, lastname, 0, 0, 'M', NIL ) )
      {

          max_len = s_len( lastname );
          if( max_len > ( SNI_MAXIMUM_KEY_LENGTH - USER_MAXIMUM_ID_LENGTH ) )
          {

             max_len = SNI_MAXIMUM_KEY_LENGTH - USER_MAXIMUM_ID_LENGTH;

          }
          s_cat( & sni_key, lastname, number, 0, max_len );

          s_init( &(sni_ocd->name), "SNI.OBJECT.LASTNAME" );
          rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE);
          s_init( &(name_ocd->name), "SNI.LAST.OID"       );
          s_copy( &(name_ocd->key), sni_key,         0, 0 );
          rc = obj_rep( sni, name_ocd, &(user->oid), SEG_UPDATE);
          if( unindex )
          {

             s_init( &(sni_ocd->name), "SNI.LAST.NAME"    );
             s_copy( & (name_ocd->key), lastname, 0, 0 );
             rc = obj_rep( sni, sni_ocd, S_NULL, SEG_UPDATE );

          }
      }
   }
   s_init( & (user_ocd->name), "NAME.FIRST" );
   rc = obj_get( user, user_ocd, & namepart );
   if( rc == 0 )
   {

      s_upper( namepart, 0, 0 );
      if( s_comp( namepart, firstname, 0, 0, 'M', NIL ) )
      {

          max_len = s_len( firstname );
          if( max_len > ( SNI_MAXIMUM_KEY_LENGTH - USER_MAXIMUM_ID_LENGTH ) )
          {

             max_len = SNI_MAXIMUM_KEY_LENGTH - USER_MAXIMUM_ID_LENGTH;

          }
          s_cat( & sni_key, firstname, number, 0, max_len );

          s_init( & (sni_ocd->name), "SNI.OBJECT.FIRSTNAME" );
          rc = obj_rep( sni, sni_ocd, namepart, SEG_UPDATE  );
          s_init( & (name_ocd->name), "SNI.FIRST.OID"       );
          s_copy( & (name_ocd->key ), sni_key,         0, 0 );
          rc = obj_rep( sni, name_ocd, &(user->oid), SEG_UPDATE);
          if( unindex )
          {

             s_init( & (sni_ocd->name), "SNI.FIRST.NAME"    );
             s_copy( & (name_ocd->key), firstname,     0, 0 );
             rc = obj_rep( sni, sni_ocd, S_NULL, SEG_UPDATE );

          }
      }
   }

   obj_set_flag(   sni, OBJ_FLUSH );
   obj_cls(        sni            );
   obj_cls(        sni            );

   ocd_drp( & sni_ocd   );
   ocd_drp( & user_ocd  );
   ocd_drp( & name_ocd  );
   s_drop(  & namepart  );
   s_drop(  & firstname );
   s_drop(  & lastname  );
   s_drop(  & penname   );
   s_drop(  & fullname  );
   s_drop(  & nickname  );
   s_drop(  & number    );
   s_drop(  & systemid  );
   s_drop(  & separator );
   s_drop(  & sni_key   );

   if( s_len( report ) > 0 )
   {

      s_smp_ul( report ) = s_len( report ) - 2;
      rc = USER_ALREADY_EXISTS;
      if( result_str != NIL )
      {

         s_copy(  result_str, report, 0, 0       );

      }
      s_drop( & report );

   }
   else
   {

      if( result_str != NIL )
      {

         gt_name(  user, S_NULL, FULL_NAME, result_str );

      }
      rc = 0;

   }
}
else
{

      sint    command    = OCD_SEND,
              status     =        0;
      hword   flags      =        0;
      string  packet     =   S_NULL;
      boolean ok_to_link =    FALSE;


      packet = modifier;


      if ( level < 2  )
      {

         /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
          |  Fill in initial user variables to assure we have them to |
          |  a known starting value.                                  |
          |                                                           |
          * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          ocd_new( &user_ocd, "SYSTEM.TYPE" );
          status = 1;
          rc = obj_rep( user, user_ocd, &status, SEG_HOLD );

          s_init( &(user_ocd->name), "SYSTEM.UI_LEVEL" );
          status = CASUAL_USER;
          rc = obj_rep( user, user_ocd, &status, SEG_HOLD   );

          s_init( &(user_ocd->name), "SYSTEM.LOGTOTAL" );
          status = 0;
          rc = obj_rep( user, user_ocd, &status, SEG_HOLD   );

          s_init( &(user_ocd->name), "SYSTEM.STATUS" );
          status = FIRST_TIME_USER;
          rc = obj_rep( user, user_ocd, &status, SEG_UPDATE );

          s_init( &(user_ocd->name), "INDEXES.MAIL_COUNT" );
          status = 0;
          rc = obj_rep( user, user_ocd, &status, SEG_UPDATE );

       }

       flags |= OK_TO_LINK_OBJECT;
       ok_to_link = TRUE;

       if ( s_len ( packet ) > 1 )
       {


          rc = t_send( TM_master->MS, command, flags, state, packet );
          if( rc != 0 )
          {

             except( rc, CONTROL_ERROR, TM_master->MS, S_NULL, S_NULL,
                                                       S_NULL, S_NULL );
          }
          else
          {

             rc = t_waitc( TM_master->MS, NIL );

          }
       }
       else
       {

          state = (sint) level;
          rc = t_m_obj( TM_master->MS, user, result_str, ok_to_link, state );

       }

}



   return(rc);

}
