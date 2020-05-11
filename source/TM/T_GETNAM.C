/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_getname()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 16, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   name        string    INPUT   Name to find user of.    |
  |                                                                   |
  |  <PARAM>   flag        sint      INPUT   Type of name part given. |
  |                                                                   |
  |  <PURPOSE> Find the user object given a name string.  Opens the   |
  |            system name index (SNI) to locate the OID of the name  |
  |            string given.  The flag indicates which index to search|
  |            unless it is 0 (ANY) and then all the indexes are      |
  |            searched until a match is found.                       |
  |                                                                   |
  |            ANY search order has to be 1) User's alias index,      |
  |            2) Nick-Name index, 3) Number index, 4) Full Name,     |
  |            5) Pen Name, 6) Last Name, 7) First Name, 8) System.   |
  |                                                                   |
  |  <OUTPUT>  OID *                                                  |
  |  <CALLS>   t_sysndx(), obj_opn()                                  |
  |  <LIMITS>  ANY search order not working yet.                      |
  |  <MODS>    Jan 31 1989     JF      Added trim of leading '0''s    |
  |                                    for NUMBER searches.           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



OID      *t_getnam( name, flag )

string              name;
sint                      flag;


{

OCB     *sni_ocb     =   NIL;
OID     *oid         =   NIL;
byte    *component   =   NIL;
sint     rc          =     0;
string   name_part   = S_NUL;




s_copy( & name_part, name, 0, 0 );

if( s_len( name_part ) > 0 )
{

   sni_ocb = t_sysndx( SNI_OBJ );

   switch( flag )
   {

      case ALIAS_NAME :

           component = "SNI.SYSTEM.OID";  break;

      case SYSTEM_NAME :

           component = "SNI.SYSTEM.OID";  break;

      case NICK_NAME :

           component = "SNI.NIC.OID";  break;

      case NUMBER_NAME :

           component = "SNI.NAME.OID";
           s_pad( & name_part, name_part, 4, '0', 'L' );
           break;

      case FULL_NAME :

           component = "SNI.FULL.OID";  break;

      case FIRST_NAME :

           component = "SNI.SYSTEM.OID";  break;

      case LAST_NAME :

           component = "SNI.LAST.OID";  break;

      case PEN_NAME :

           component = "SNI.PEN.OID";  break;

      case ANY_NAME :

           rc = 1;  break;

      default       :

           rc = SEARCH_METHOD_NOT_DEFINED;
           break;

   }
   if( rc == 1 )
   {


      while( TRUE )
      {

         oid = t_getnam( name_part, NICK_NAME );
         if( oid != NIL )
         {

            break;

         }
         oid = t_getnam( name_part, NUMBER_NAME );
         if( oid != NIL )
         {

            break;

         }
         oid = t_getnam( name_part, FULL_NAME );
         if( oid != NIL )
         {

            break;

         }
         oid = t_getnam( name_part, SYSTEM_NAME );
         if( oid != NIL )
         {

            break;

         }
         oid = t_getnam( name_part, LAST_NAME );
         if( oid != NIL )
         {

            break;

         }
         break;

      }
   }
   else if( component != NIL )
   {

      OCD    *ocd   =  NIL;


      ocd_new( & ocd, component );
      s_copy(  &( ocd->key ), name_part, 0, 0 );
      oid = (OID *) os_mema( sizeof( OID ));
      rc  = obj_get( sni_ocb, ocd, oid );

      if( rc != 0 )
      {

         os_memd( oid );
         oid = NIL;

      }
      ocd_drp( & ocd );

   }
}
else
{


}
s_drop( & name_part );



   return(oid);

}
