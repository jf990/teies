/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_qwind()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    January 31, 1988                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   formid      string    INPUT   Form identifier of the   |
  |                                          form to query window info|
  |  :PARAM.   flags       hword     INPUT   Form type flag.          |
  |  :PARAM.   win_width   real  *   OUTPUT  Width of the window      |
  |  :PARAM.   win_len     real  *   OUTPUT  Length of the window     |
  |  :PARAM.   min_x       real  *   OUTPUT  NC of the minimum X value|
  |  :PARAM.   max_x       real  *   OUTPUT  NC of the maximim X value|
  |  :PARAM.   min_y       real  *   OUTPUT  NC of the minimum Y value|
  |  :PARAM.   max_y       real  *   OUTPUT  NC of the maximum Y value|
  |  :PARAM.   border      boolean   OUTPUT  TRUE if widow gets a     |
  |                                          border.                  |
  |                                                                   |
  |  :PURPOSE. Given the Form ID, return where the form is going to   |
  |            appear on the device.  Various window/viewport attri-  |
  |            butes are returned, such as width and length of the    |
  |            viewport as well as minimum and maximum X and Y points |
  |            and whether the window will contain a border.          |
  |                                                                   |
  |  :OUTPUT.  sint             0 if information returned.            |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "ti.h"



sint   t_qwind( formid, flags,
                        win_width, win_len, min_x, max_x, min_y, max_y,
                border )

string          formid;
hword           flags;
real                   *win_width,*win_len,*min_x,*max_x,*min_y,*max_y;
boolean        *border;




{


sint      wk_id =   1,
          du    =   0;
real      wid   = 0.0,
          len   = 0.0,
          xras  = 0.0,
          yras  = 0.0;





   gqmax_disp (wk_id, &du, &wid, &len, &xras, &yras) ;
   *win_width  =  wid - 1.0 ;
   *win_len    =  len - 1.0 ;
   *min_x      =  0.0 ;
   *max_x      =  1.0 ;
   *min_y      =  0.0 ;
   *max_y      =  1.0 ;
   *border     = FALSE;



   return(0);

}
