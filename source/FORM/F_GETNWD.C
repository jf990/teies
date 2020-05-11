/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_getnwd ()                                            |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    May 17, 1988                                           |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  None.                                                  |
  |                                                                   |
  |  :PARAM.   f_prof  struct f_prof_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains default & parsed  |
  |                                        qualifier values.          |
  |                                                                   |
  |  :PARAM.   f_fmat  struct f_fmat_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains info required to  |
  |                                        render data properly in    |
  |                                        the GKS world.             |
  |                                                                   |
  |  :PURPOSE. purpose                                                |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   None.                                                  |
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
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "debug.h"
#include "form_i.h"


tint f_getnwd (wordout, linein, start)
string *wordout ;
string  linein  ;
tint    start   ;
{
   tint pos    =   0 ;
   tint beg    =   0 ;
   byte charin = ' ' ;

   pos = start ;






