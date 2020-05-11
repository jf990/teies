/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocb_drp                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 26, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master  struct TMCB *                               |
  |  <PARAM>   *ocb      struct OCB   INPUT  The OCB of the object to |
  |                                          drop.                    |
  |  <PURPOSE> Remove an OCB from the OCB Table and release any       |
  |            memory allocations it points to.                       |
  |  <OUTPUT>  Always return 0.                                       |
  |  <CALLS>   s_drop(),   os_memd ()                                 |
  |  <ERRORS>  20002 : OCB to close is invalid.                       |
  |            20003 : OCB to close was not open.                     |
  |  <LIMITS>  Always assumes the pointer is valid unless -1 or 0.    |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"


sint       ocb_drp( ocb )

OCB               * ocb     ;


{


   sint        dmsmode ,          /* DMS's mode for closing of the OBJ */
               rc = 0  ;          /* return code status                */






if( ! OCB_POINTER_OK( ocb ) )
{

   rc = INVALID_OCB;
   except( rc, INFO, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL) ;

}
else
{                                       /* Remove SNI cache reference. */

   if( ocb->type == SNI_OBJ && TM_master->sni_ocb != NIL )
   {

       TM_master->sni_ocb = NIL;

   }
                                  /*-----------------------------------*/
                                  /* Clean out any allocated space     */
                                  /* referenced from the OCB.          */
                                  /*-----------------------------------*/

   if( ocb->lastdbp  != S_NULL ) { s_drop( &(ocb->lastdbp )  ); }
   if( ocb->last_key != S_NULL ) { s_drop( &(ocb->last_key)  ); }
   if( ocb->last_ocd != S_NULL ) { s_drop( &(ocb->last_ocd)  ); }
   if( ocb->gfd      != S_NULL ) { s_drop( &(ocb->gfd     )  ); }
   if( ocb->server   != S_NULL ) { s_drop( &(ocb->server  )  ); }
   if( ocb->last_seg != NIL    ) { os_memd(  ocb->last_seg   ); }

                                  /* - - - - - - - - - - - - - - - - - */
                                  /* Reset prev&next pointers in OCB's */
                                  /* If this is the ONLY OCB in the    */
                                  /* chain then skip the pointer reset */
                                  /* logic.                            */
                                  /* - - - - - - - - - - - - - - - - - */

   if( ocb->prevocb == NIL  && ocb->nextocb == NIL  )
   {

      TM_master->OBJcb = NIL;

   }
   else
   {

      if (ocb->prevocb != NIL)
      {

         (ocb->prevocb)->nextocb = ocb->nextocb ;

      }
      else
      {

         TM_master->OBJcb = ocb->nextocb ;

      }
      if (ocb->nextocb != NIL)
      {

         (ocb->nextocb)->prevocb = ocb->prevocb ;

      }
      else
      {

         (ocb->prevocb)->nextocb = ocb->nextocb ;

      }
   }
                                  /* Deallocate memory used by the OCB */
   os_memn( ocb, sizeof( OCB ) );
   os_memd (ocb) ;

}

   return(rc);
}
