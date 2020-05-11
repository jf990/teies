/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ex_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   an_object   OCB *      INPUT                           |
  |  <PARAM>   item_ids    string     INPUT                           |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   result      void *    OUTPUT                           |
  |  <PURPOSE> Check for existance of mail.  Will return mail OID's   |
  |            under the following (bitwise) conditions:              |
  |                                                                   |
  |            level = EX_MAIL_ITEM_IDS_PROVIDED - or -               |
  |            level = 0 && ( s_len( item_ids ) > 0 )                 |
  |                                                                   |
  |               the second parameter (string) contains a list of    |
  |               mail item ids (entered by a user or process gener-  |
  |               ated) to be converted to an OID or list of OIDs.    |
  |                                                                   |
  |            level = EX_MAIL_RETURN_ONE_OID                         |
  |                                                                   |
  |               the result pointer is considered an OID ** and one  |
  |               OID will be returned.  If the pointer points to some|
  |               thing it will be overwritten, if it points to NIL an|
  |               OID will be allocated and stored there.             |
  |                                                                   |
  |            level = EX_MAIL_RETURN_MANY_OIDS                       |
  |                                                                   |
  |               the result pointer is considered an OCB * and a list|
  |               object will be stored.  If the pointer already points
  |               to a list object it will be filled in (not cleared).|
  |               If it points to NIL a new list object will be created
  |               and stored there.                                   |
  |                                                                   |
  |            level = EX_MAIL_INTERACTIVE       - or -               |
  |            level = 0 && ( s_len( item_ids ) < 1 ) && USER_SERVER  |
  |                                                                   |
  |               a form will be presented asking for a list of mail  |
  |               item ids.                                           |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |            result will be an OID * if a single OID was requested  |
  |               and one was found.  It will be a string of unmatched|
  |               ID's if modifier provided a string of ID's and one  |
  |               or more do not match, and it will be a string of OIDs
  |               if one or more OIDs are provided through modifier.  |
  |               Finally, it will be an OCB * if no list object was  |
  |               provided and many OIDs are to be returned.  Under all
  |               circumstances, error conditions take precedence and |
  |               if an error condition exists the routine output will|
  |               reflect that.                                       |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"


sint     ex_mail(      OCB    * an_object,
                       string   item_ids,
                       hword    level,
                       void   * result                               )

{

sint        rc                      =   0,
            item_counter            =   0;
OCD       * item_ocd                = NIL;
OID       * item_oid                = NIL;
string    * mail_item_ids           = NIL,
            separator               = S_NIL;




s_init( & separator, "," );



if( 0 == s_first( item_ids, separator, 0, 0 ) )
{

   ocd_new(& item_ocd,       "SMI.MESSAGE.OID"  );
   s_copy( & item_ocd->key,  item_ids,     0, 0 );
   rc = t_fndsys( SMI_OBJ,   item_ocd, item_oid );


   if( rc != 0 )
   {

      rc = NO_DATA_FOUND;

   }
   ocd_drp( & item_ocd );
   os_memd(   item_oid );

}

   return(rc);

}
