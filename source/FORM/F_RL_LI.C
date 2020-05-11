/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_rl_li()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 8, 1989                                            |
  |  <LEVEL>   FFUN  (Forms Functions)                                |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   f_prof      struct f_prof_s *   INPUT                  |
  |  <PARAM>   f_fmat      struct f_fmat_s *   INPUT                  |
  |                                                                   |
  |  <PURPOSE> Process special object level interface handling of     |
  |            repeating list items.  We need to collect all the text |
  |            of the list item and apply it for each object ID we    |
  |            have in our list.                                      |
  |                                                                   |
  |            This routine is called from f_li() only if a <li> tag  |
  |            is encountered as part of a <rl>.                      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "form_i.h"




sint   f_rl_li(  f_prof, f_fmat  )

struct f_prof_s        * f_prof;
struct f_fmat_s        * f_fmat;


{

   sint        rc         =      0;




   return(rc);

}
