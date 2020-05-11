/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_link                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 04, 1987                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block where we   |
  |                                          get all the linking info |
  |                                          necessary to perform this|
  |                                          task.                    |
  |                                                                   |
  |  <PURPOSE> Given the Object control block, link this object as    |
  |            as necessary for this object type.  Based on object    |
  |            the proper object level linking routine will be called.|
  |  <OUTPUT>  Sint                                                   |
  |  <CALLS>   lk_*() where * is the object designator.               |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "obj_prim.h"


sint obj_link(  ocb  )
struct OCB *ocb        ;          /* Pointer to the OBJ control block  */
{

#D_begin( obj_link, OBJ );


if( ocb == NIL )
{

   rc = 20000

}
else
{

   switch( ocb->type )
   {

       case    USER_OBJ : lk_user( ocb );  break;
       case   GROUP_OBJ : lk_grup( ocb );  break;
       case CONFCOM_OBJ : lk_cc(   ocb );  break;
       case  CONFER_OBJ : lk_cr(   ocb );  break;
       case    MAIL_OBJ : lk_mail( ocb );  break;
       case   NOTIF_OBJ : lk_notif(ocb );  break;
       case  NETDIR_OBJ : lk_ndir( ocb );  break;
       case    LIST_OBJ : lk_list( ocb );  break;
       case    MEMB_OBJ : lk_memb( ocb );  break;

   }
#D_return ( rc ) ;
}
