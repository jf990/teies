/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_dms.c                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 24, 1987                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   lfd         hword     INPUT   Logical file descriptor  |
  |  <PARAM>   goid        tint      INPUT   Group Object ID          |
  |                                                                   |
  |  <PURPOSE> Given the LFD and the GOID, determine if this object   |
  |            is already open.  If so, return a pointer to its DMScb,|
  |            otherwise return 0 (NIL).                              |
  |                                                                   |
  |  <OUTPUT>  (dmscb *)                                              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"

#define NOT_FOUND 0


struct dms_cb *obj_dms(   lfd, goid   )

hword                     lfd;
tint                           goid;



{

boolean              found  = FALSE;
OCB                 *curocb =   NIL;




   curocb = TM_master->OBJcb ;    /* Pointer to OCB table              */
   if (curocb == NULL)            /* If table doesn't exist            */
   {

   return(INFORMATIONAL);

   }
   while (  (curocb->goid  !=  goid)  && (curocb->lfd != lfd)   )
   {

      if( (curocb = curocb->nextocb) == NIL )
      {

   return(NOT_FOUND);

      }
   }

   return(curocb->dmscb);

}
