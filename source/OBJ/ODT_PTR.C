/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_ptr                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    June 15, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   obj_type    hword    INPUT    This is a #defined num-  |
  |                                          ber that indicates which |
  |                                          ODTs pointer to return   |
  |                                                                   |
  |  :PURPOSE. This routine returns the pointer to the ODT of type    |
  |            'obj_type' and if the ODT is not in memory, loads it.  |
  |                                                                   |
  |  :OUTPUT.  Returns a pointer to a ODT                             |
  |                    a -5 if ODT could not be loaded                |
  |                                                                   |
  |  :CALLS.   except (), odt_load ()                                 |
  |                                                                   |
  |  :ERRORS.  0 : Could not load ODT                                 |
  |           -1 : No more memory to load ODT or ODT head pointer     |
  |                  is not valid.                                    |
  |  :LIMITS.  None realized.                                         |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"






ODT       *odt_ptr (obj_type)

hword obj_type  ;                 /* Object type of ODT                */



{


sint           count   = 0 ,     /* Counter in loop                   */
               rc      = 0 ;     /*                                   */
ODT           *odtnew  = NIL;    /* Return code from other functions  */
ODTLIST       *odt_list= NIL;




                                  /* Go through the ODT table trying   */
                                  /* to find the ptr to the ODT        */
   if( ! (obj_type > 0 && obj_type <= MAX_OBJ ) )
   {

   return(NIL);

   }
   odt_list = TM_master->odt;
   while( odt_list != NIL && odt_list->odt_ptr != NIL )
   {

      if ( odt_list->obj_type == obj_type)
      {

   return(odt_list->odt_ptr);

      }
      odt_list++;

   }                              /* End if end or null reached        */
                                  /* Try to load the new ODT if room   */
   if (odt_list >= (TM_master->odt + (sizeof(ODTLIST)*NUMBER_OF_ODTs)) )
   {
      except (ODT_MEMORY_FULL, INFO,S_NULL,S_NULL,S_NULL,S_NULL,S_NULL) ;
      odtnew = NIL;
   }
   else
   {

      odtnew = odt_load (obj_type) ;

      if ( ODT_POINTER_OK( odtnew ) )
      {

         if( TM_master->odt == NIL )
         {

            TM_master->odt =
                  (ODTLIST *)os_mema(sizeof(ODTLIST)*NUMBER_OF_ODTs );
            os_memn( TM_master->odt, sizeof(ODTLIST)*NUMBER_OF_ODTs );
            odt_list = TM_master->odt;

         }
         while( odt_list->odt_ptr != NIL )
         {

            odt_list++;

         }
         odt_list->odt_ptr  = odtnew   ;
         odt_list->obj_type = obj_type ;

      }
      else
      {

         except( ODT_CANNOT_BE_LOADED, INFO,
                                    S_NULL,S_NULL,S_NULL,S_NULL,S_NULL) ;
         odtnew = NIL;

      }
   }


   return(odtnew);
}
