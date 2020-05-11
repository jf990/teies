/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_LI.C
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.               f_prof :  pointer to the structure f_prof_s

                      f_fmat :  pointer to the structure f_fmat_s


:OUTPUT.                  rc :  a SINT indicating if the list item
                                was rendered properly.

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.

          This routine will first determine if there are any list
          types presently active, and if so, what the current list type
          is.  After determining this information, it will calculate how
          the list item should be prefixed ( with a bullet, number/alpha,
          or nothing ), and will determine the correct amount of
          indentation needed for that item.

:CALLS.   f_render.c  s_cat.c  s_sitos.c  stk_qry.c  stk_psh.c
          stk_pop.c  osmemd.c  s_drop.c


:ERRORS.

:LIMITS.

<MODS>    May 8, 1989     JF     Started to add repeating list.
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "bse.h"
#include "form_i.h"

#define  sl_ind  2

sint    f_li (    f_prof, f_fmat   )

struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;

{
    struct  f_lst_s  *list = NIL;
    string  alph_seq       = S_NULL,
            bullet         = S_NULL,
            prefix         = S_NULL,
            seq            = S_NULL,
            dot            = S_NULL;
    sint    rc             = 0;
    real    down           = 0.0 ;
    boolean end_mark       = FALSE;


    s_init( & alph_seq, "abcdefghijklmnopqrstuvwxyz");
    s_init( & bullet,   "o+*-#@"                    );
    s_init( & dot,      "."                         );

    if ( stk_qry( (STACK *) f_fmat->list_stk )  ) {


       list = (struct f_lst_s *)  stk_pop( (STACK *) f_fmat->list_stk );

       switch ( list->type ) {

          case ORDERED_LIST:

               list->seqno += 1;         /* incriment sequence by 1    */

               if ( ((list->level) % 2) ) {
                                             /* odd levels get numbers */
                  s_sitos( &seq, list->seqno );

                  s_cat( &prefix, seq, dot, 0, 0);
                  }

               else {                        /* even levels get alphas */
                  s_copy( & prefix, alph_seq, list->seqno, 1 );
                  s_cat(  & prefix, prefix,   dot,      0, 0 );
                  }

               break;

          case SIMPLE_LIST:

               break;

          case UNORDERED_LIST:

               s_copy( & prefix, bullet, list->level, 1 );
               break;

          case REPEATING_LIST:

               rc = f_rl_li (    f_prof, f_fmat   );
               break;

          default :

               break;

          }

                                  /* If 'cursor' is already at margin  */
       if ( f_fmat->cur_x != f_fmat->org_x + f_fmat->lef_mar ) {
          down = 2.0 ;
          }
       else {
          down = 1.0 ;
          }

       f_fmat->cur_y = f_fmat->cur_y - down * (real) f_fmat->spacing ;
       f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar - PRE_SIZ;

       f_render( f_fmat, prefix, F_RENDER_NCR, NIL ) ;

       f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar;

       stk_psh( (STACK *) f_fmat->list_stk, (byte *) list );
       os_memd( list );                  /* put updated list info back */

       rc = 1;                                /* successful completion */
       }

    else {                                     /* no list types active */

       f_li_err( f_prof, f_fmat, end_mark ); /* error, no lists active */

       rc = 0;                              /* unsuccessful completion */
       }

    s_drop( & alph_seq );
    s_drop( & bullet   );
    s_drop( & prefix   );
    s_drop( & dot      );
    s_drop( & seq      );

   return(rc);
}
