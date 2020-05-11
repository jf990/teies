/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||                                                                   ||
 ||                                                                   ||
 ||  For the following object-action primitives, the routine naming   ||
 ||  convention is:                                                   ||
 ||                                                                   ||
 ||        ACTIONS                          OBJECTS                   ||
 ||                                                                   ||
 ||   cr = Create                      notif  = Notifications         ||
 ||   fn = Find                        mail   = Mail Message          ||
 ||   rm = Delete/Remove               cc     = Conference Comment    ||
 ||   jn = Join                        cr     = Conference Root       ||
 ||   md = Modify                      user   = User Directory        ||
 ||   rc = Receive                     grup   = Group Directory       ||
 ||   sc = Scan                        memb   = Membership List       ||
 ||   vw = View                        list   = List of Objects       ||
 ||   gt = Get                         ntdr   = Network Directory     ||
 ||   do = Do                          sx     = System Index (Meta)   ||
 ||   rv = Review                      act    = Activity              ||
 ||   or = Organize                    tick   = Ticket                ||
 ||   lk = Link                        Name   = Name                  ||
 ||   ex = Exist                                                      ||
 ||   ab = abstract                                                   ||
 ||                                                                   ||
 ||        ... continued in OBJ_PRM2.H ...                 $EXTRACT   ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern sint vw_notif () ; /* NOTIFICATION PRIMITIVES                  */
extern sint cr_notif () ;

                          /* Create notification level indicators     */
#define INTERACTIVE_CREATION 1
#define AUTOMATIC_CREATION   2

extern sint sc_notif () ;
extern sint fn_notif () ;
extern sint md_notif () ;
extern sint rc_notif () ;
extern sint jn_notif () ;
extern sint rm_notif () ;
extern sint do_notif () ;
extern sint lk_notif () ;
extern sint rv_notif () ;
extern sint gt_notif () ;
extern sint or_notif () ;
extern sint ex_notif () ;
extern sint ab_notif () ;

/*                                                         $ENDEXTRACT */
