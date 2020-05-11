/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ_NTF Header                                         |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 13, 1990                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Define Notification routines and constants.            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
               /* Create notification level indicators     */

#define INTERACTIVE_CREATION  1
#define AUTOMATIC_CREATION    2
#define AUTO_MAIL_NTF        51
#define AUTO_MEMB_NTF        52
#define AUTO_ACT_NTF         53
#define VIEW_NTF_NEW          1
#define VIEW_NTF_OLD          2

/*                                                $EXTRACT $MAKER-PROTO*/

extern sint cr_ntf(OCB *ocb, string modifier, hword level, OCB **params);



extern sint lk_ntf(OCB *ocb, string modifier, hword level,
                                                      byte **result_str);

extern sint ntf_valto(OCB *notif_ocb, OCB *list_ocb, OID *author_oid,
                                 sint *num_of_recip, string *result_str);

#define  notif_validate_recipients     ntf_valto

extern sint ntf_auto(OCB *ocb, OCB *list_ocb, OID *author_oid,
                       OCB **n_ocb, sint *num_of_recip, string modifier);

#define  notif_automatic_notif         ntf_auto

extern sint ntf_memb(OCB *memb_ocb, string modifier, OID *author_oid,
                                                              OCB **ocb);

#define  notif_membership_notif        ntf_memb



extern sint vw_ntf(OCB *some_ocb, string modifier, hword level,
                                                           byte *params);

extern sint gt_ntf(OCB *notif_ocb, string modifier, hword level,
                                                           byte *params);

extern sint rc_ntf(OCB *notif_ocb, string modifier, hword level,
                                                           byte *params);

extern sint rm_ntf(OCB *notif_ocb, string modifier, hword level,
                                                           byte *params);

extern sint ntf_ndx( void );
extern sint ntf_get( sint ntf_entry, string * result );


extern sint sc_ntf(void);
extern sint fn_ntf(void);
extern sint md_ntf(void);
extern sint jn_ntf(void);
extern sint do_ntf(void);
extern sint rv_ntf(void);
extern sint or_ntf(void);
extern sint ex_ntf(void);
extern sint ab_ntf(void);

/*                                                         $ENDEXTRACT */

#define NTF_MAX_NOTIF_ID       9999
#define NTF_MIN_NOTIF_ID          1
#define NTF_KEY_LENGTH            4
/*+-------------------------------------------------------------------+
  | Set up the Notification ID numbers for all the notification mes-  |
  | sages.  Use these constants to look up Notification messages with |
  | ntf_get().  See TEIES NTFDATA for originial input data and language
  | differences and SNTFI OBJECT for the System Notification Index    |
  | layout.                                                           |
  | A CHANGE HERE MUST ALSO BE REFLECTED IN TEIES NTFDATA !           |
  +-------------------------------------------------------------------+*/
#define   NTF_AGREE_WITH_CONTENT                          1
#define   NTF_DISAGREE_WITH_CONTENT                       2
#define   NTF_DESIRABLE_PROPOSAL                          3
#define   NTF_UNDESIRABLE_PROPOSAL                        4
#define   NTF_APPROVE_ACTION                              5
#define   NTF_DISAPPROVE_WITH_ACTION                      6
#define   NTF_FEASIBLE_CONCEPT                            7
#define   NTF_UNFEASIBLE_CONCEPT                          8
#define   NTF_VALID_ARGUMENT                              9
#define   NTF_INVALID_ARGUMENT                           10
#define   NTF_APPLAUD_CONTENT                            11
#define   NTF_BOO_CONTENT                                12
#define   NTF_UNCLEAR_CONTENT                            13
#define   NTF_INTERESTED_IN_SUBJECT                      14
#define   NTF_WILL_HANDLE_TASK                           15
#define   NTF_HOLD_OFF_ACTION                            16
#define   NTF_RECEIVED_MESSAGE                           17
#define   NTF_RECEIVED_CONFERENCE_COMMENT                18
#define   NTF_DID_TICKET                                 19
#define   NTF_DID_ACTIVITY                               20
#define   NTF_YOU_HAVE_BEEN_ADDED_TO                     21
#define   NTF_YOU_HAVE_BEEN_REMOVED_FROM                 22
#define   NTF_YOUR_MEMBERSHIP_ROLE_HAS_BEEN_MODIFIED     23
