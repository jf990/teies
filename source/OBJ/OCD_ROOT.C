/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_root.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 11, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   ocb   OCB *           INPUT   OCB of the object to     |
  |                                          find next OCD in.        |
  |  <PARAM>   ocd   OCD *           INPUT   OCD to update with next  |
  |                                          OCD.                     |
  |  <PURPOSE> Given the object, return the ROOT OCD positioned from  |
  |            the current OCD for that object.  If there is no root, |
  |            return the first.  Returns both the OCD and the content|
  |            of that OCD.  If the OCD pointer in NIL, a new OCD is  |
  |            created.  If data is NIL, a data buffer is allocated.  |
  |            If these pointers are not NIL, -1, then they are assum-|
  |            ed valid.                                              |
  |                                                                   |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>  0 : Operation completed as expected.                   |
  |            20453 : Cannot comput the root for the current OCD.    |
  |  <LIMITS>  None known.                                            |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"





ODT  * ocd_root( ocb, ocd )

OCB             * ocb;
OCD                  * ocd;



{

   ODT      * odt_entry     =    NIL,
            * odt_table     =    NIL,
            * original_odt  =    NIL;






odt_table    = ocb->odt;
odt_entry    = odt_find ( ocb, ocd );
original_odt = odt_entry ;

if( ODT_POINTER_OK( odt_entry ) )
{

   original_odt = odt_entry ;
   while( odt_level( odt_entry ) == odt_level( original_odt )
      &&  odt_entry > odt_table )
   {

      odt_entry -- ;

   }
}

   return(odt_entry);

}
