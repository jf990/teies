/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    memb_rc                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    June 30 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   memb_ocb    OCB *      INPUT                           |
  |  <PARAM>   modifier    string     INPUT                           |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   itm_num     sint       INPUT                           |
  |  <PARAM>   user_oid    OID *      INPUT                           |
  |  <PURPOSE> Receive Item and update membership                     |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_memb.h"




sint    memb_rc( memb_ocb, modifier, level, itm_num, user_oid )

OCB             *memb_ocb;
string           modifier;
hword            level;
sint             itm_num;
OID             *user_oid;

{

sint        rc         = NO_ERR,
            offset     = 0,
            winbeg     = 0;

string      window     = S_NULL;

byte       *byt_offset = NULL,
            bit_offset = 0;

OCD   *memb_ocd   = NULL;

byte BIT_TAB[] = { 0x00, 0x80, 0x40, 0x20, 0x10,
                           0x08, 0x04, 0x02, 0x01  };

#define WINDOW_SIZE ( 64 * 8 )



if( OCB_POINTER_OK(memb_ocb) )
{

ocd_new( &memb_ocd, "MEMBERSHIP.MEMBERS.WINDOW" );
oid_key(  memb_ocd, user_oid   );

if( (rc = obj_get( memb_ocb, memb_ocd, & window )) == NO_ERROR )
{

   if( window == S_NULL )
   {

      s_alloc(         & window,    (WINDOW_SIZE/8) );
      os_memn( s_buffer( window ),  (WINDOW_SIZE/8) );
      s_smp_ul(          window ) = (WINDOW_SIZE/8)  ;

   }

   s_init( & (memb_ocd->name), "MEMBERSHIP.MEMBERS.WINDOW_BEGIN" );
   obj_get(   memb_ocb, memb_ocd, & winbeg );


   if( itm_num > winbeg )
   {

      if( itm_num > (winbeg + WINDOW_SIZE) )
      {

         /* we need to do something here to slide window so */
         /* item falls into the window range. */

      }

      offset     = (itm_num - winbeg) / 8;
      bit_offset = (itm_num - winbeg) % 8;

      if( (bit_offset == 0) && (offset != 0) )
      {

         bit_offset = 8;
         offset--;

      }

      byt_offset = offset + s_buffer(window);


      if( (*byt_offset & BIT_TAB[bit_offset]) )
      {

         /* what will happen if last item was read? how can we update */
         /* last_itm_read component?? */
         rc = ITEM_ALREADY_RECEIVED;

      }
      else
      {

         *byt_offset |= BIT_TAB[bit_offset];


         s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.WINDOW" );
         obj_rep( memb_ocb, memb_ocd, window, SEG_UPDATE );

         if( level & MEMB_SCANNING_ITEMS )
         {

            s_init(&(memb_ocd->name),"MEMBERSHIP.MEMBERS.LAST_ITEM_READ");
            obj_rep( memb_ocb, memb_ocd, &itm_num, SEG_UPDATE );

         }

      }  /* if itm not rcvd yet */

   }  /* If item falls in window range */

}
else
{

   rc = MEMBER_DOES_NOT_EXIST;

}

ocd_drp( &memb_ocd );

}
else
{

   rc = INVALID_OCB;       /* or INVALID_OCD  */

}



   return(rc);

}
