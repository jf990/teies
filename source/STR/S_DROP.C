/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    s_drop.c                                               |*/
/*|  <LEVEL>   str                                                    |*/
/*|  <PARAM>   pointer     string*  I/O      pointer to memory to be  |*/
/*|                                          dropped                  |*/
/*|                                                                   |*/
/*|  <PURPOSE> Get rid of string memory and zero pointer.             |*/
/*|  <OUTPUT>  sint                                                   |*/
/*|  <LIMITS>  Does no checking to make sure that a string is being   |*/
/*|            dropped.                                               |*/
/*|  <MODS>    06_Aug_86   N.Voronka   Authored.                      |*/
/*|  <MODS>    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  <MODS>    19_Feb_90   JF          Removed pcheck(), added test for*/
/*|                                    bad pointer input, added show of*/
/*|                                    the pointer for debugging.     |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "str_i.h"
#include "osk.h"


sint s_drop (pointer)
string *pointer ;
{

   if( pointer != NIL && pointer != (string *) -1 )
   {


      if ( *pointer == S_NULL )
      {
      }
      else
      {
         s_smp_al(*pointer) = 0;       /* Trash alloc & used lens so   */
         s_smp_ul(*pointer) = 0xDEAD;  /* str can't be used again      */
         os_memd (*pointer) ;
         *pointer = S_NULL ;
      }
   }

   return(0);
}
