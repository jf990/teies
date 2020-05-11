/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_dlst                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 20, 1987                                        |
  |  <LEVEL>   OBJECT                                                 |
  |  <GLOBAL>  TM_master                                              |
  |  <PURPOSE> Dump out and list the Object control blocks, the ODTs  |
  |            active in the system, and the ISV reference to objects |
  |            available to the system at the time of invocation of   |
  |            this function.                                         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"


sint           obj_dlst()



{
   OCB        *ocb_fill=   NULL ; /* Pointer to the OCB of the PSI     */
   sint        sgoid   =   0    , /* GOID of the SOIDI                 */
               i       =   0    , /* counter                           */
               seglen  =   0    , /* Length  of segment retrieved      */
               j       =   0    , /*   "                               */
               rc      =   0    ;
   hword       slfd    =   0    ; /* LFD  of the SOIDI                 */
   string      dbp     = S_NULL ; /* DBpath to a segment in the PSI    */
   byte       *soid    =   NIL  , /* OID  of the SOIDI                 */
              *segptr  =   NIL  ; /* Pointer to segment retrieved      */






   i = 0;
   while((TM_master->odt+i)->odt_ptr  != NIL  )
   {

       i++;

   }
   for( j=0; j<=i; j++ )
   {


   }
   i = 0;
   while( (TM_master->OBJcb+i)->nextocb != NIL )
   {

       i++;

   }
   for( j=0; j<=i; j++ )
   {


   }
   return(0);
}
