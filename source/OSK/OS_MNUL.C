/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_mnul.c                                              |
  |                                                                   |
  |  :AUTHOR.  Ken Ng, John Foster                                    |
  |                                                                   |
  |  :DATE.    March 19, 1988                                         |
  |                                                                   |
  |  :LEVEL.   OSK                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   pointer     byte *     INPUT  Pointer to first byte of |
  |                                          memory area to NULL out. |
  |                                                                   |
  |  :PARAM.   length      sint       INPUT  Number of bytes to NULL. |
  |                                                                   |
  |  :PURPOSE. Given a pointer to a memory area and a length, fill the|
  |            memory with NULLs.  This is a blind operation so the   |
  |            pointer and amount of memory at that location are      |
  |            assumed to be valid.                                   |
  |                                                                   |
  |  :OUTPUT.  VOID function.                                         |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :MODS.    02/16/89       Sal    Rename the routine from os_memn  |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"



void   os_mnul(  pointer, length  )

byte            *pointer;
sint             length;


{

byte  * end   =  NIL;





   for( end = pointer + length ; pointer < end; pointer++)
   {

      *pointer = NULL;

   }



}
