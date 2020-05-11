/************************************************************************

    <DOC>

    <NAME>         a_admem(OCB *attach_ocb, OCB *memb_ocb);

    <LEVEL>        Activity

    <DATE>         June 9, 1989

    <AUTHOR>       Tanmay S. Kumar

    <GLOBAL>       None.

    <PARAM>        attach_ocb  OCB *     INPUT   OCB of the attached object
                   memb_ocb    OCB *     INPUT   OCB of the membership object

    <OUTPUT>       Sint.

    <PURPOSE>      To create the membership object for the activity

    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr(), except().

    <ERRORS>       None Known.

    <FORMS>        None

    <LIMITS>       None.

    <MODS>         None.

    <END>

    </DOC>

************************************************************************/



#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"


sint     a_admem( attach_ocb, memb_ocb )

OCB      *attach_ocb;
OCB      *memb_ocb;
{



   return(0);

}
