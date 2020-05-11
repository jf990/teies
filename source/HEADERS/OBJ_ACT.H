/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ_ACT.H                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 26, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Define and fill in the global Object/Action state      |
  |            table.  It is a matrix of routines defining what       |
  |            routine to invoke when an object/action combination    |
  |            is selected.                                           |
  |  <LIMITS>  Cannot handle non-C language coded routines.           |
  |                                                                   |
  |            Should handle error return codes from the routines,    |
  |            which should pass back a return code.                  |
  |                                                                   |
  |            NOTE: If the number of actions or objects changes, both|
  |            this file and OBJ_COMM must be changed as they are de- |
  |            fined in both headers due to non-intersetions.  Should |
  |            corrected (once the system runs, ok?)                  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
                              /*****************************************/
                              /*   WATCH OBJ_COMM.H since it has these */
#define NUMBER_OF_ACTIONS 15  /*   2 defines also and they *MUST* match*/
#define NUMBER_OF_OBJECTS 12  /*                                       */
                              /*****************************************/

       struct _act_obj
       {

          sint (*(act_obj))();

       }  AOM[ NUMBER_OF_ACTIONS ][ NUMBER_OF_OBJECTS ]    =    {

       {                                                       /* VIEW */
            vw_user,
            vw_grup,
            NIL,        /* Network Directory */
            vw_cr,
            vw_cc,
            vw_mail,
            vw_ntf,
            vw_act,
            vw_memb,
            vw_list,
            vw_tick,
            NIL,        /* Contents */
       },
       {                                                       /* SCAN */
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            sc_mail,
            NIL,
            NIL,
            NIL,
            sc_list,
            sc_tick,
            NIL,
       },
       {                                                     /* CREATE */
            cr_user,
            cr_grup,
            NIL,
            cr_cr,
            cr_cc,
            cr_mail,
            cr_ntf,
            cr_act,
            cr_memb,
            cr_list,
            cr_tick,
            NIL,
       },
       {                                                     /* MODIFY */
            md_user,
            NIL,
            NIL,
            md_cr,
            md_cc,
            md_mail,
            NIL,
            md_act,
            md_memb,
            md_list,
            md_tick,
            NIL,
       },
       {                                                     /* REMOVE */
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            rm_mail,
            rm_ntf,
            NIL,
            NIL,
            rm_list,
            rm_tick,
            NIL,
       },
       {                                                     /* REVIEW */
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
       },
       {                                                       /* FIND */
            fn_user,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            fn_list,
            fn_tick,
            NIL,
       },
       {                                                       /* JOIN */
            NIL,
            jn_grup,
            NIL,
            jn_cr,
            NIL,
            NIL,
            NIL,
            NIL,
            jn_memb,
            jn_list,
            jn_tick,
            NIL,
       },
       {                                                    /* RECEIVE */
            NIL,
            NIL,
            NIL,
            NIL,
            rc_cc,
            rc_mail,
            rc_ntf,
            rc_act,
            NIL,
            rc_list,
            rc_tick,
            NIL,
       },
       {                                                         /* DO */
            NIL,
            NIL,
            NIL,
            NIL,
            do_cc,
            do_mail,
            do_ntf,
            do_act,
            NIL,
            do_list,
            do_tick,
            NIL,
       },
       {                                                        /* GET */
            gt_user,
            NIL,
            NIL,
            gt_cr,
            gt_cc,
            gt_mail,
            gt_ntf,
            NIL,
            NIL,
            gt_list,
            gt_tick,
            NIL,
       },
       {                                                   /* ORGANIZE */
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
       },
       {                                                       /* LINK */
            lk_user,
            lk_grup,
            NIL,
            lk_cr,
            lk_cc,
            lk_mail,
            lk_ntf,
            lk_act,
            lk_memb,
            lk_list,
            lk_tick,
            NIL,
       },
       {                                                   /* ABSTRACT */
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            NIL,
            ab_list,
            ab_tick,
            NIL,
       },
       {                                                      /* EXIST */
            ex_user,
            ex_grup,
            NIL,
            ex_cr,
            NIL,
            NIL,
            NIL,
            NIL,
            ex_memb,
            ex_list,
            ex_tick,
            NIL,
       }                                                };
