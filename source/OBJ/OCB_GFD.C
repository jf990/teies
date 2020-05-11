/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocb_gfd()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 28, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   gfd_name    String     INPUT  GFD to locate in the open|
  |                                          object table.            |
  |                                                                   |
  |  <PARAM>   mode        hword      INPUT  Mode to look for.  If 0  |
  |                                          then ignore mode.        |
  |                                                                   |
  |  <PURPOSE> Locate an object control block matching a GFD name.    |
  |            Mimicks ocb_inta() for OID's.  If the GFD is found the |
  |            mode must also match (eg. READ_ONLY, UPDATE, etc) to   |
  |            return the entry unless mode is specified as 0 to mean |
  |            "ignore mode".                                         |
  |                                                                   |
  |  <OUTPUT>  OCB *                                                  |
  |  <CALLS>   s_comp()                                               |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




OCB  * ocb_gfd(  gfd_name, mode  )

string           gfd_name;
hword            mode;


{

   sint        rc             =    0,
               x              =    0;
   OCB       * current_entry  =  NIL;
   boolean     still_checking = TRUE;





   current_entry = TM_master->OBJcb ;

   if (current_entry != NIL )
   {

      if ( gfd_name == (string) -1 )
      {


         while (current_entry->nextocb != NIL )
         {

            current_entry = current_entry->nextocb ;

         }
      }
      else
      {


         while ( still_checking )
         {

            if (  (mode != 0)  )
            {

                if( (current_entry->mode | mode)  &&
                    s_comp( current_entry->gfd, gfd_name, 0, 0, 'F', & x)
                            == 0 )
                {

                   still_checking = FALSE;
                   break;

                }
            }
            else
            {

                if( s_comp( current_entry->gfd, gfd_name, 0, 0, 'F', & x)
                            == 0 )
                {

                   still_checking = FALSE;
                   break;

                }
            }
            current_entry = current_entry->nextocb ;
            if ( current_entry == NIL  )
            {

               current_entry = OCB_NOT_FOUND;
               break;

            }
         }
      }
   }

   return(current_entry);

}
