/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    a_addsel()                                              |
  |                                                                   |
  |  :AUTHOR.  Tanmay Kumar                                           |
  |                                                                   |
  |  :DATE.    July 26, 1989                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Receive Item and update membership                     |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "activity.h"




sint    a_addsel( ocb, modifier, level, itm_num, ocd )

OCB             *ocb;
string           modifier;
hword            level;
sint             itm_num;
OCD             *ocd;

{

sint        rc         = 0,
            offset     = 0,
            item       = 0,
            size       = 0,
            winbeg     = 0;

string      window     = S_NULL;

byte       *byt_offset = NULL,
            bit_offset = 0;

byte BIT_TAB[] = { 0x00, 0x80, 0x40, 0x20, 0x10,
                           0x08, 0x04, 0x02, 0x01  };


#define CANNOT_GET_WINDOW 6430



if ( OCB_POINTER_OK(ocb) && OCD_POINTER_OK(ocd) )
{


   if ( (rc = obj_get( ocb, ocd, &window )) == 0)
   {



      if ( level == ACT_LINK_SELECTION)
      {
         s_stosi(&item, modifier, 0, 0);
      }
      else
      {
         item = itm_num;
      }

      offset     = (item) / 8;
      bit_offset = (item) % 8;

      if( (bit_offset == 0) && (offset != 0) )
      {

         bit_offset = 8;
         offset--;

      }

      if (s_len(window) == 0)
      {
         size = ocd_len( ocb, ocd );
         s_alloc(&window, 64);
         s_smp_ul(window) = 64;
         os_memn(s_buffer(window), 64);
      }
      byt_offset = offset + s_buffer(window);

      if ( (*byt_offset) & (BIT_TAB[bit_offset]) )
      {
         rc = SELECTION_NOT_AVAILABLE;
      }
      else
      {
         *byt_offset |= BIT_TAB[bit_offset];
      }


      obj_rep( ocb, ocd, window, SEG_UPDATE );

   }

}
else
{

   rc = INVALID_OCB;       /* or INVALID_OCD  */

}



   return(rc);

}
