/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||  <Doc>                                                            ||
 ||  <Name>     OBJ_PRIM  Header                                      ||
 ||  <Level>    OBJ                                                   ||
 ||  <Author>   JF                                                    ||
 ||  <Date>     February 16, 1989                                     ||
 ||                                                                   ||
 ||  <Purpose>                                                        ||
 ||  For the following object-action primitives, the routine naming   ||
 ||  convention is:                                                   ||
 ||                                                                   ||
 ||        ACTIONS                          OBJECTS                   ||
 ||                                                                   ||
 ||   cr = Create                      ntf    = Notifications         ||
 ||   fn = Find                        mail   = Mail Message          ||
 ||   rm = Delete/Remove               cc     = Conference Comment    ||
 ||   jn = Join                        cr     = Conference Root       ||
 ||   md = Modify                      user   = User Directory        ||
 ||   rc = Receive                     grp    = Group Directory       ||
 ||   sc = Scan                        memb   = Membership List       ||
 ||   vw = View                        list   = List of Objects       ||
 ||   gt = Get                         ntdr   = Network Directory     ||
 ||   do = Do                          sx     = System Index (Meta)   ||
 ||   rv = Review                      act    = Activity              ||
 ||   or = Organize                                                   ||
 ||   lk = Link                                                       ||
 ||   ab = abstract                                                   ||
 ||                                                                   ||
 ||   </Doc>                                                          ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#ifndef OBJ_PRIM_H_INCLUDED


#define  OBJ_PRIM_H_INCLUDED 1
#include "obj_mail.h"
#include "obj_user.h"
#include "obj_cc.h"
#include "obj_cr.h"
#include "obj_group.h"
#include "obj_tick.h"
#include "obj_list.h"
#include "obj_ntf.h"
#include "obj_memb.h"
#include "obj_sys.h"
#include "obj_name.h"
#include "activity.h"


#endif
