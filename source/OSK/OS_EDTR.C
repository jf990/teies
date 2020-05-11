/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     OS_EDTR C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    OSK                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    file       string     IN   File name to edit.          |*/
/*| :PARAM.    rcode      sint*      OUT  Return code from editor.    |*/
/*| :PARAM.    init       bool       IN   Does initial help screen go |*/
/*|                                       up?                         |*/
/*|                                                                   |*/
/*| :PURPOSE.  Invokes an editor session for the given or default     |*/
/*|            file.                                                  |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     20_Apr_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     20_Apr_87   J.L.Foster  Added user defined editor and  |*/
/*|                                    default file name code.        |*/
/*|                                                                   |*/
/*| :MODS.     06_Jul_89   K.J.Walsh   Added ability to use a PROFILE |*/
/*|                                    for XEDIT as per user settings.|*/
/*|                                                                   |*/
/*| :MODS.     02_Aug_89   K.J.Walsh   Sets WIDTH to 132 (default 80) |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"


string os_edtr(file,caller_rc,init )

string  file   ;                       /* Name of file to be edited    */
sint    *caller_rc;                    /* Numeric RC (future use)      */
bool    init;                          /* Does editor display initial  */
                                       /* help screen                  */

{
   sint     our_rc   = 0      ;        /* rc from called routines      */
   string   realfile = S_NULL ;        /* Actual fully-qualified file  */
   string   invoke   = S_NULL ;        /* What we fork over to OS      */
   string   defalt   = S_NULL ;        /* Default file descriptor      */
   string   workst   = S_NULL ;        /* Options to the editor          */
   string   retstr   = S_NULL ;        /* String to be returned to call*/
   byte     *holder  = NIL;            /* Holds return str to s_init() */


   s_init(&defalt,"* * TEIES $SP");

   our_rc = os_qfd(&realfile,file,defalt);
   if ( our_rc != 0 )
   {
   }
   else
   {
      /*+-------------------------------------------------------------+*/
      /*| Now see what editor we are using.  First guess is XEDIT.    |*/
      /*+-------------------------------------------------------------+*/
      s_init(&invoke,"XEDIT");            /* setup for comp /cmd       */
      if( s_len(TM_master->ucb->editor) == 0
        || s_comp(invoke,TM_master->ucb->editor,0,0,'M',&our_rc ) == 0 )
      {

         /*+----------------------------------------------------------+*/
         /*| Set GLOBALV that tells TEIES XEDIT if initial screen     |*/
         /*| goes up; clear shuttle GLOBALV.                          |*/
         /*+----------------------------------------------------------+*/
         if ( init == TRUE )
            our_rc = putenv("STORAGE.TEIES.OS_EDINT=1");
         else
            our_rc = putenv("STORAGE.TEIES.OS_EDINT=0");
         our_rc = putenv("STORAGE.TEIES.OS_EDSHT=");

         /*+----------------------------------------------------------+*/
         /*| If a profile string exists then create a parameter list. |*/
         /*| If not we still create a blank string for padding later. |*/
         /*+----------------------------------------------------------+*/
         if ( s_len(TM_master->ucb->editor_profile) > 0 )
         {
            s_init( &workst, " ( WIDTH 132 PROFILE ");
            s_cat(&workst,workst,TM_master->ucb->editor_profile,0,0);
         }
         else
         {
            s_init( &workst, " WIDTH 132");
         }

         /*+----------------------------------------------------------+*/
         /*| Now we pretty much set up a regular invocation of XEDIT  |*/
         /*| with the indicated profile.                              |*/
         /*+----------------------------------------------------------+*/
         s_cat(&invoke,invoke,workst,1,1);   /* Add a blank to "XEDIT" */
         s_cat(&invoke,invoke,realfile,0,0); /* Add FD to "XEDIT "     */
         s_cat(&invoke,invoke,workst,0,0);   /* Tack on param list     */

         our_rc = os_call(invoke);

         s_init(&retstr,getenv("TEIES.OS_EDSHT"));

         s_init(&workst,"%CMD");
         var_set(workst,retstr);             /* Go set %CMD line var   */

         *caller_rc = 0;                     /* Clear return code      */

         switch(our_rc)
         {
            case 55000:                      /* Enter key / command    */
               holder = "$ENTER";
               break;

            case 55001:                      /* PF01 (shouldn't happen)*/
               holder = "HELP";
               break;

            case 55002:                      /* PF02                   */
               holder = "CONFIRM";
               break;

            case 55003:                      /* PF03                   */
               holder = "QUIT";
               break;

            case 55004:                      /* PF04                   */
               holder = "PROCESS";
               break;

            case 55005:                      /* PF05 (shouldn't happen)*/
               holder = "PRINT";
               break;

            case 55006:                      /* PF06 (shouldn't happen)*/
               holder = "TOP";
               break;

            case 55007:                      /* PF07 (shouldn't happen)*/
               holder = "PREVIOUS";
               break;

            case 55008:                      /* PF08 (shouldn't happen)*/
               holder = "NEXT";
               break;

            case 55009:                      /* PF09 (shouldn't happen)*/
               holder = "BOTTOM";
               break;

            case 55010:                      /* PF10 (shouldn't happen)*/
               holder = "HNEXT";
               break;

            case 55011:                      /* PF11 (shouldn't happen)*/
               holder = "HPREVIOUS";
               break;

            case 55012:                      /* PF12                   */
               holder = "CONFIRM";
               break;

            case 55027:                      /* PA1                    */
               holder = "$PA1";
               break;

            case 55028:                      /* PA2                    */
               holder = "$PA2";
               break;

            case 55029:                      /* PA3                    */
               holder = "$PA3";
               break;

            default:                         /* unknown return code    */
               holder = "PROCESS";
               break;

         }
         s_init(&retstr,holder);

      }
      /*+-------------------------------------------------------------+*/
      /*| Too bad, we don't know about this kind of editor.           |*/
      /*| except() had dam well better return control to this routine |*/
      /*| so we can do the s_drop()'s.                                |*/
      /*+-------------------------------------------------------------+*/
      else
      {
      }
   }

   /*+----------------------------------------------------------------+*/
   /*| Time to boogie.                                                |*/
   /*+----------------------------------------------------------------+*/
   s_drop(&realfile);
   s_drop(&invoke);
   s_drop(&defalt);
   s_drop(&workst);

   return(retstr);
}
