/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_QRY.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 pid:  connection number

:OUTPUT.                rc:   if above 55,000 then error else status bits

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               Give us some time, we'll find something!!!

:CALLS.                 -none-

:ERRORS.                55800 : general error
                        55801 : isf_open() never called for this system

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

sint isf_qry( id, flags, int_dat, str_dat)

   string     id;
   sint           flags;
   sint                  *int_dat;
   string                         *str_dat;

{

      sint rc = 0;

      #D_begin( isf_qry, ISCF)

      rc = scv_qry( id, flags, int_dat, str_dat);

      #D_return( rc )

}
