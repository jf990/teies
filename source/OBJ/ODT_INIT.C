/*+------------------------------------------------------------------+*/
/*|                                                                  |*/
/*|  :NAME.      odt_init ()                                         |*/
/*|                                                                  |*/
/*|  :LEVEL.     OBJ                                                 |*/
/*|                                                                  |*/
/*|  :GLOBAL.    odt_t_r, TM_master->odt_t_r                         |*/
/*|                                                                  |*/
/*|  :PARAM.                                                         |*/
/*|                                                                  |*/
/*|  :PURPOSE.   To initialize odt_t_r for use with odt_make.        |*/
/*|                                                                  |*/
/*|  :OUTPUT.    odt_t_r                                             |*/
/*|                                                                  |*/
/*|  :CALLS.     s_init                                              |*/
/*|                                                                  |*/
/*|  :ERRORS.                                                        |*/
/*|                                                                  |*/
/*|  :LIMITS.                                                        |*/
/*|                                                                  |*/
/*|  :MODS.     19 Jan 88    Steve Muccione  Preliminary version     |*/
/*|                                                                  |*/
/*|  :END.                                                           |*/
/*|                                                                  |*/
/*+------------------------------------------------------------------+*/

#include "defs.h"
#include "isv.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"



sint odt_init(  )

{



   sint cntr = 0;





   if( TM_master->odt_t_r == NIL )
   {


   }
   for (cntr=0;cntr<ODT_TYPES+1;cntr++)
   {
      odt_t_r [cntr].name=S_NULL;
   }

   s_init (&(odt_t_r [0].name),"BYTE");
   odt_t_r [0].type = OC_BYTE;
   odt_t_r [0].length = 1;

   s_init (&(odt_t_r [1].name),"INT");
   odt_t_r [1].type = OC_INT;
   odt_t_r [1].length = 4;

   s_init (&(odt_t_r [2].name),"REAL");
   odt_t_r [2].type = OC_REAL;
   odt_t_r [2].length = 4;

   s_init (&(odt_t_r [3].name),"STR");
   odt_t_r [3].type = OC_STR;
   odt_t_r [3].length = 65535;

   s_init (&(odt_t_r [4].name),"D/T");
   odt_t_r [4].type = OC_D_T;
   odt_t_r [4].length = 8;

   s_init (&(odt_t_r [5].name),"PRIV");
   odt_t_r [5].type = OC_PRIV;
   odt_t_r [5].length = 6;

   s_init (&(odt_t_r [6].name),"OID");
   odt_t_r [6].type = OC_OID;
   odt_t_r [6].length = 8;

   s_init (&(odt_t_r [7].name),"HWRD");
   odt_t_r [7].type = OC_HWRD;
   odt_t_r [7].length = 2;

   s_init (&(odt_t_r [8].name),"SEG");
   odt_t_r [8].type = OC_SEG;
   odt_t_r [8].length = 0;

   s_init (&(odt_t_r [9].name),"INDEX");
   odt_t_r [9].type = OC_NDX;
   odt_t_r [9].length = 0;

   s_init (&(odt_t_r [10].name),"ARRAY");
   odt_t_r [10].type = OC_ARY;
   odt_t_r [10].length = 0;

   s_init (&(odt_t_r [11].name),"HSEG");
   odt_t_r [11].type = OC_HSEG;
   odt_t_r [11].length = 0;

   s_init (&(odt_t_r [12].name),"RSEG");
   odt_t_r [12].type = OC_HHSG;
   odt_t_r [12].length = 0;

   s_init (&(odt_t_r [13].name),"INHR");
   odt_t_r [13].type = OC_INHR;
   odt_t_r [13].length = 8;

   s_init (&(odt_t_r [14].name),"ISV");
   odt_t_r [14].type = OC_ISV;
   odt_t_r [14].length = 2;

   s_init (&(odt_t_r [15].name),"ROOT");
   odt_t_r [15].type = OC_HHSG;
   odt_t_r [15].length = 0;




   return(0);
}

