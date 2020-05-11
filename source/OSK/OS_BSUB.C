/*+------------------------------------------------------------------+
  |                                                                  |
  |   :NAME.       os_bsub.c                                         |
  |                                                                  |
  |   :AUTHOR.     Rana Ajaz                                         |
  |                                                                  |
  |   :DATE.       Feb. 3, 1988                                      |
  |                                                                  |
  |   :LEVEL.      OSK                                               |
  |                                                                  |
  |   :GLOBAL.     None                                              |
  |                                                                  |
  |   :PARAM.      ptr    BYTE   INOUT    Pointer to a string of     |
  |                                       of the type byte.          |
  |                                                                  |
  |   :PARAM       len    SINT   INPUT    Length of the string       |
  |                                                                  |
  |   :PARAM.      Const  BYTE   INPUT    A subconstanat to be subtrac- |
  |                                       ted from each byrt of the  |
  |                                       string.                    |
  |                                                                  |
  |   :PURPOSE.    To subtract the byte subconstant from each byte of   |
  |                the string.                                       |
  |                                                                  |
  |   :OUTPUT.     Return 0 if 'subconst' or 'len' are 0.               |
  |                If ptr = -1, Pointer out of range.                |
  |                If ptr =  0, Operation complete.                  |
  |                                                                  |
  |   :CALLS.      None.                                             |
  |                                                                  |
  |   :ERRORS.     None.                                             |
  |                                                                  |
  |   :LIMITS.     Not known.                                        |
  |                                                                  |
  |   :MODS.       Casted -1 comparison with *ptr                    |
  |                                                                  |
  |                                                                  |
  |                                                                  |
  |   :END.                                                          |
  |                                                                  |
  +------------------------------------------------------------------+*/



#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"



 sint os_bsub(ptr, len, subconst)
 byte *ptr;
 byte subconst;
 sint len;
 {


     if (subconst == 0)
        {
   return(0);
        }
     if (len == 0)
        {
   return(0);
        }
     if (( ptr == NULL) || ( ptr == (byte *) MINUS_ONE_PTR))
         {
   return(-1);
         }

     do
     {
         *ptr -= subconst;
         ++ptr;
         --len;
     }
     while (len != 0);
   return(0);
 }
