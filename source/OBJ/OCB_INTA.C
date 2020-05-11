/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocb_inta                                               |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    July  2, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   *oid     OID         INPUT    Object ID Number of the  |
  |                                          OCB you want or a -1     |
  |                                          for the last one         |
  |                                                                   |
  |  <PARAM>    mode    hword       INPUT    Mode of object to check. |
  |                                                                   |
  |  <PURPOSE> This routine returns a pointer to the desired OCB      |
  |            given an OID of an open object.                        |
  |                                                                   |
  |  <OUTPUT>  OCB *                                                  |
  |  <CALLS>   oid_cmp()                                              |
  |  <ERRORS>  Returns:                                               |
  |                     NIL    if no OCB's in the system yet.         |
  |                    -1      if entry not found in current OCB list.|
  |  <MODS>    Jan_17_89   JF  Added OID_CMP() macro to test OID's.   |
  |  <MODS>    Mar_28_89   JF  Added MODE parameter to search by mode.|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"





OCB        *ocb_inta ( oid, mode )

OID        *oid    ;
hword       mode   ;


{

   OCB       * current_entry  =  NIL;
   hword       search_mode    =    0;
   boolean     still_checking = TRUE;






   current_entry = TM_master->OBJcb ;    /* Pointer to OCB table      */
   if (current_entry != NIL )            /* If table doesn't exist    */
   {

      if ( oid == (OID *) -1 )
      {

         while (current_entry->nextocb != NIL )
         {

            current_entry = current_entry->nextocb ;

         }
      }
      else                        /* Find OCB with passed OID          */
      {

         while ( still_checking )
         {


            if (  (mode != 0)  )
            {

                if( mode & OBJ_UPDATE )
                {

                   search_mode = OBJ_UPDATE;

                }
                else if( mode & OBJ_READ )
                {

                   search_mode = OBJ_READ;

                }
                else
                {

                   search_mode = OBJ_UPDATE;

                }
                if (  (current_entry->mode & search_mode)     &&
                      (oid_cmp( (*oid), (current_entry->oid) ))   )
                {

                   still_checking = FALSE;
                   break;

                }
            }
            else
            {

                if (  oid_cmp( (*oid), (current_entry->oid) )  )
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
