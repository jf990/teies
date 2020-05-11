/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    memb_gt                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    June 30 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PURPOSE> Receive Item and update membership                     |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_memb.h"




sint memb_gt( memb_ocb, modifier, level, itm_num, las_itm_num,
              seqnum_arry, arry_size )

OCB             *memb_ocb;
string           modifier;
hword            level;
sint             itm_num,
                 las_itm_num,
               **seqnum_arry,
                *arry_size;

{

sint        rc         = NO_ERR,
            i          = 0,
           *arry       = NULL,
            offset     = 0,
            las_itm_read = 0,
            winbeg     = 0;

string      window     = S_NULL,
            num_str    = S_NULL;
OCD       *memb_ocd    = NULL;


byte       *byt_offset = NULL,
            bit_offset = 0;

byte BIT_TAB[] = { 0x00, 0x80, 0x40, 0x20, 0x10,
                           0x08, 0x04, 0x02, 0x01  };



if( OCB_POINTER_OK(memb_ocb) )
{

ocd_new( &memb_ocd, "MEMBERSHIP.MEMBERS.WINDOW" );
oid_key( memb_ocd, &(TM_master->ucb->user_id)   );
if( arry_size != NIL )
{

   * arry_size = 0;
}
                                           /* Chk if user is in object */
if( (rc = obj_get( memb_ocb, memb_ocd, & window )) == NO_ERROR )
{

   if( window == S_NULL )
   {

      s_alloc( & window,           (MEMBERSHIP_BIT_WINDOW_SIZE / 8) );
      os_memn( s_buffer( window ), (MEMBERSHIP_BIT_WINDOW_SIZE / 8) );
      s_smp_ul( window ) =         (MEMBERSHIP_BIT_WINDOW_SIZE / 8  );

      obj_rep( memb_ocb, memb_ocd, window, SEG_UPDATE );

   }

   if( (seqnum_arry != NIL)  &&  (* seqnum_arry == NIL) )
   {

      * seqnum_arry = (sint *) os_mema( las_itm_num * sizeof(sint) );
      os_memn( * seqnum_arry, las_itm_num * sizeof(sint) );

   }
   arry = *seqnum_arry;


   s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.WINDOW_BEGIN" );
   obj_get(  memb_ocb, memb_ocd, & winbeg );

   if( level & MEMB_ALL_UNREAD_ITEM )
   {

      las_itm_read = winbeg;

   }
   else
   {

      s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.LAST_ITEM_READ");
      obj_get(  memb_ocb, memb_ocd, & las_itm_read );

   }

   if( las_itm_num > las_itm_read )
   {

      offset     = (las_itm_read - winbeg) / 8;
      bit_offset = (las_itm_read - winbeg) % 8;


      if( (bit_offset == 0) && (offset != 0) )
      {

         bit_offset = 8;
         offset--;

      }
      if( bit_offset == 0 )
      {

         bit_offset = 1;

      }

      byt_offset =  offset +  s_buffer(window) ;


      i = bit_offset;
      while( las_itm_read <  las_itm_num )
      {

         for(; (i <= 8) && (las_itm_read <  las_itm_num); i++)
         {


                     /*+-------------------------------------------+
                       | In the Bit Window, a bit set indicates    |
                       | the respective item has already been read,|
                       | 0 indicates it is still considered NEW    |
                       +-------------------------------------------+*/

            las_itm_read++;
            if( (* byt_offset & BIT_TAB[ i ]) == 0 )
            {

               *arry = las_itm_read;
               arry++;
               *arry_size = *arry_size + 1;

            }
         }  /* for */

         byt_offset++;
         i = 1;

      }  /* while */

   }  /* If item falls in window range */
}
else
{

   rc = MEMBER_DOES_NOT_EXIST;

}
s_drop( &window );

}
else
{

   rc = INVALID_OCB;

}

   return(rc);
}
