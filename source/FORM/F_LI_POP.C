/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             POP_LIST.C
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.                type  :  a SINT representing the type of list
                                that is to be ended.  If type=0, means
                                most current list type should be ended.
                                Type=1,2, or 3 means a specific type of
                                list to be ended.

:OUTPUT.                 rc  :  a SINT indicating if the popping of the
                                list type was completed successfully. a
                                0 means no, and a 1 means yes.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.





:CALLS.  stk_qry.c  stk_pop.c  stk_psh.c

:ERRORS.

:LIMITS.

:MODS.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "form_i.h"

sint  f_li_pop ( f_prof, f_fmat, type )
struct  f_prof_s  *f_prof;
struct  f_fmat_s  *f_fmat;
sint  type;

{
    struct  f_lst_s  *list;
    STACK   *temp_stk=NULL;
    sint    rc=0;
    boolean end_mark=TRUE;


    if ( stk_qry( (STACK *) f_fmat->list_stk ) ) {

       if ( type == 0 ) {              /* pop current list type */

          list = (struct f_lst_s *) stk_pop( (STACK *)f_fmat->list_stk );

          switch ( list->type ) {

             case ORD_LIST:              /* ordered list popped */
                     f_fmat->ol_level -= 1;
                     break;

             case SIMP_LIST:            /* simple list popped */
                     f_fmat->sl_level -= 1;
                     break;

             case UNORD_LIST:           /* unordered list popped */
                     f_fmat->ul_level -= 1;
                     break;

             }
          os_memd( list );        /* deallocate memory for element */
          rc = 1;                   /* successful completion */
          }
       else {

          temp_stk = stk_ini( MAX_LIST_LEVELS, sizeof( struct f_lst_s) );

          while ( stk_qry( (STACK *) f_fmat->list_stk ) ) {

             list = (struct f_lst_s *)stk_pop((STACK*) f_fmat->list_stk);

             if ( list->type != type ) {   /* not type of list to pop  */
                stk_psh( temp_stk, (byte *) list );
                os_memd( list );      /* deallocate memory for element */
                }
             else {                               /* right type to pop */
                rc = 1;                     /* successful operation    */
                os_memd( list );      /* deallocate memory for element */

                while ( stk_qry( temp_stk ) ) { /* elements to restore */
                   list = (struct f_lst_s *) stk_pop( temp_stk );

                   switch ( list->type ) {

                      case ORD_LIST:            /* ordered list popped */
                         f_fmat->ol_level -= 1;
                         break;

                      case SIMP_LIST:            /* simple list popped */
                         f_fmat->sl_level -= 1;
                         break;

                      case UNORD_LIST:       /* unordered list popped */
                         f_fmat->ul_level -= 1;
                         break;

                      }
                   f_fmat->lef_mar = f_fmat->lef_mar - LVL_IND;
                   os_memd( list );   /* deallocate memory for element */
                   }
                stk_end( temp_stk );
                break;
                }
             }
             if ( rc == 0 ) {                   /* list type not found */
                while ( stk_qry( temp_stk ) ) { /* elements to restore */
                   list = (struct f_lst_s * ) stk_pop( temp_stk );
                   stk_psh( (STACK *)f_fmat->list_stk, (byte *)list );
                   os_memd( list );
                   }
                f_li_err( f_prof, f_fmat, end_mark );
                }
          }
       }
    else {           /* no lists active right now to end */
       f_li_err( f_prof, f_fmat, end_mark );
       }

   return(rc);
}
