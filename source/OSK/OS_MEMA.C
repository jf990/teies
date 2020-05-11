/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                     *
 *  This file will contain tempory memory allocation functions that    *
 *  will be used for debugging purposes only. They are:                *
 *                                                                     *
 *          os_mema     (number of bytes to allocate) -returns pointer *
 *          os_memd     (pointer to the structure to deallocate)       *
 *          os_memk     (PID of the process memory to deallocate)      *
 *                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
/*
+----------------------------------------------------------------------+
|                                                                      |
|  Author         : Nestor Voronka                                     |
|  Date Completed : Monday , August 18, 1986                           |
|  Routine Name   : os_mema   (size)                                   |
|  Globals Used   : None                                               |
|  Purpose        : Returns a pointer to 'size' bytes allocated        |
|                                                                      |
|  Calls          : malloc()                                           |
|  Limits         : no memory management yet!                          |
|  Mods:                                                               |
|    Author   Date      Description                                    |
|                                                                      |
|    JF       04/29/87  Added call to except so we check here for      |
|                         out-of-memory condition.                     |
|    JF       05/01/87  Added 7 bytes to the requested memory alloca-  |
|                         tion since the C malloc rounds DOWN to the   |
|                         first value divisable by 8.                  |
+----------------------------------------------------------------------+
                                                                      */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "osk_i.h"
#include "exc.h"

byte  *os_mema  (   size   )

sint                size;/* Amount of space to allocate in bytes      */

{
   byte buffer [ 200 ]; /* buffer for tracing functions */
   byte       *whereto = NIL; /* Pointer to space allocated here      */

   if( size > 0 )
   {

      whereto =  malloc (size+7) ;    /* See above for this tweek     */
      if (whereto == S_NULL)
      {

            except( 99999, FATAL_ERROR,
                    S_NULL,     S_NULL, S_NULL, S_NULL, S_NULL );
   return(whereto);

      }
   }



   return(whereto);
}
