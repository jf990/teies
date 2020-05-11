/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_gtlid                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 29, 1990                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   entry        sint      INPUT  LFD entry to get.        |
  |  <PARAM>   lfd_name     string *  OUTPUT LFD name for entry #.    |
  |  <PARAM>   lfd_access   string *  OUTPUT LFD access mode.         |
  |  <PARAM>   blk_size     sint   *  OUTPUT Size of each LFD block.  |
  |  <PARAM>   num_blocks   sint   *  OUTPUT Number of blocks in LFD. |
  |                                                                   |
  |  <PURPOSE>                                                        |
  |                                                                   |
  |        given an entry number, it will return the respective data  |
  |  in the system initialization structure for that lfd specificaion.|
  |  If entry is -1, rc will be the total number of entries in the    |
  |  init structure.  If entry access and empty slot, rc will reflect |
  |  that and none of the parameters will be touched.                 |
  |                                                                   |
  |  The strings that are returned are COPIES, and therefore must be  |
  |  deallocated by the caller.                                       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_gtlid(  entry, lfd_name, lfd_access, block_size, number_of_blocks )

sint             entry;
string                * lfd_name;
string                * lfd_access;
sint                            * block_size;
sint                                        * number_of_blocks;


{

       sint        rc         =      0;
struct _init_    * init_data  =    NIL;




if( TM_master != NIL )
{

   if( TM_master->sys_init != NIL )
   {

      init_data = TM_master->sys_init;

      if( entry == -1 )
      {

         if( init_data->_lfd_name != NIL )
         {

            rc = s_vlen( init_data->_lfd_name );

         }
      }
      else
      {

         if( entry < 1  ||  entry > s_vlen( init_data->_lfd_name ) )
         {

            rc = TM_SYS_INIT_REQUEST_OUT_OF_RANGE;

         }
         else
         {

            if( lfd_name != NIL )
            {

               s_copy( lfd_name, init_data->_lfd_name[ entry ], 0, 0 );

            }
            if( lfd_access != NIL )
            {

               s_copy( lfd_access,
                       init_data->_lfd_access_mode[ entry ],
                       0, 0 );

            }
            if( block_size != NIL )
            {

               * block_size = init_data->_lfd_block_size[ entry - 1 ];

            }
            if( number_of_blocks != NIL )
            {

               * number_of_blocks =
                         init_data->_lfd_number_of_blocks[ entry - 1 ];

            }
         }
      }
   }
   else
   {

      rc = TM_SYS_INIT_NOT_ACTIVE;

   }
}
else
{

   rc = MASTER_PROCESS_IS_NOT_ACTIVE;

}


   return(rc);

}
