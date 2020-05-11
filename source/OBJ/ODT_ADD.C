/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_add                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    June  4, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   **odt    odtrecord   IN/OUT   Pointer to the pointer   |
  |                                          to the current odtrec    |
  |                                          that is to be filled in  |
  |  :PARAM.    *n      sint        IN/OUT   A pointer to a sint that |
  |                                          is used to keep count of |
  |                                          the number of odtrecs in |
  |                                          the ODT table            |
  |  :PARAM.     str    string      INPUT    A pointer to a TEIES str |
  |                                          that contains the name   |
  |                                          of the ODTrecord         |
  |  :PARAM.     lvl    byte        INPUT    The level of the ODTrec  |
  |                                          (0 is the lowest)        |
  |  :PARAM.     off    hword       INPUT    Offset from the beginn-  |
  |                                          ing of the current level |
  |  :PARAM.     next   hword       INPUT    Numerical offset of the  |
  |                                          next same-level entry    |
  |                                          in the ODT table         |
  |  :PARAM.     prv    hword       INPUT    Numerical offset of the  |
  |                                          previous same-level en-  |
  |                                          try in the ODT table     |
  |  :PARAM.     type   hword       INPUT    Type of ODTrecord as     |
  |                                          #defined in OBJ_COMM.H   |
  |  :PARAM.    *priv   byte        INPUT    Pointer to a 6 element   |
  |                                          array containing privi-  |
  |                                          ledge information.       |
  |                                                                   |
  |  :PURPOSE. To fill in the data for the next ODTrecord in memory   |
  |            and to increment the ODT pointer.                      |
  |                                                                   |
  |  :OUTPUT.  Always returns 0.                                      |
  |                                                                   |
  |  :CALLS.   s_init (), s_drop (), s_char_s ()                      |
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
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"





sint odt_add ( odt, n, str, lvl, off, nxt, prv, type, priv )

ODT       **odt           ;       /* Ptr to the ptr of the current OCD */
sint      *n              ;       /* Ptr to sint used to keep count    */
string     str            ;       /* Ptr to C-str with name            */
byte       lvl            ;       /* Level number in the ODT           */
hword      off            ,       /* Offset of OCD in the segment      */
           nxt            ,       /* Offset of next same-level entry   */
           prv            ,       /* Offset of prev same-level entry   */
           type           ;       /* Type of the Object Component      */
PRIVS     *priv           ;       /* 6 byte matrix containing privs    */



{


   hword     str_used  = 0,
             str_alloc = 0;
   sint      rc        = 0;






   os_memc(  str, &( (*odt)->name ),
                                MAX_COMPONENT_NAME_LENGTH + S_OVERHEAD );

   if( s_len( str ) > 16 )
   {

      s_smp_ul( ((string)(&((*odt)->name)))) = MAX_COMPONENT_NAME_LENGTH;

   }
   s_smp_al( ((string)(&((*odt)->name))) ) = MAX_COMPONENT_NAME_LENGTH;


                                  /* Copy the rest of the OCD info     */
   odt_lvl  (*odt) = lvl  ;
   odt_off  (*odt) = off  ;
   odt_nxto (*odt) = nxt  ;
   odt_prvo (*odt) = prv  ;
   odt_type (*odt) = type ;


   os_memc(  priv, &( (*odt)->privs ), sizeof( PRIVS )  );


   (*odt)++ ;                     /* Increment ODT pointer to nxt elem */
   (*n)++ ;                       /* Increment count of # of elements  */


   return(rc);
}
