/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||  <DOC>                                                            ||
 ||  <NAME>     OBJ_MAIL Header                                       ||
 ||  <LEVEL>    OBJ                                                   ||
 ||  <DATE>     February 9, 1989                                      ||
 ||  <AUTHOR>   JF                                                    ||
 ||  <PURPOSE>  Function definitions and constants for use by object  ||
 ||             level routines dealing with the MAIL object.          ||
 ||  <ERRORS>   Defined in OBJ_ERRS Header                            ||
 ||  </DOC>                                                           ||
 ||                                                                   ||
 ||                                  $EXTRACT   $MAKER-PROTO          ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern sint vw_mail( OCB * some_ocb, string modifier, hword level,
                                                         byte * params) ;

#define VW_MAIL_INTERACTIVE   98

extern sint cr_mail( OCB *ocb, string modifier, hword level,
                                                     OCB ** result_ocb) ;
                          /* Create Mail level identifiers            */
#define CREATE_NEW_MAIL              0
#define CREATE_MAIL_NEW_ROOT         1
#define CREATE_MAIL_NEW_REPLY        2
#define CREATE_MAIL_SEND_TO_MODIFIER 3

extern sint mail_reply( OCB *mail_object, OCB **reply, string reply_id );

extern sint sc_mail  ( void) ;
extern sint fn_mail( OCB * ocb, string modifier, hword level,
                                                     byte * parameters) ;
extern sint md_mail( OCB *, string, hword, void * ) ;
extern sint rc_mail( OCB * mail_ocb, string modifier, hword level,
                                                    OCB * receiver_ocb) ;

#define LINK_MAIL_KEY_PROVIDED      97
#define LINK_MAIL_VALIDATE_MODIFIER 96
#define LINK_MODIFIED_MAIL          95

extern sint jn_mail  ( void) ;
extern sint rm_mail( OCB * object, string modifier, hword level,
                                                         byte * params) ;
extern sint do_mail  ( void) ;
extern sint lk_mail( OCB * mail_ocb, string packet, hword level,
                                                   string * result_str) ;
extern sint rv_mail  ( string modifier) ;
extern sint gt_mail( OCB * ocb, string modifier, hword level,
                                                         byte * params) ;
extern sint or_mail  ( string modifier) ;
extern sint ex_mail( OCB * ocb, string modifier, hword level,
                                                         void * result );
/* Possible LEVEL bit settings */

#define    EX_MAIL_INTERACTIVE             0x01
#define    EX_MAIL_RETURN_MANY_OIDS        0x02
#define    EX_MAIL_RETURN_ONE_OID          0x04
#define    EX_MAIL_ITEM_IDS_PROVIDED       0x08


extern sint ab_mail( OCB * ocb, string modifier, hword level,
                                                         byte * params) ;

/*                                                         $ENDEXTRACT */

#define  MAIL_ITEM_ROOT    345
#define  MAIL_ITEM_REPLY   346
#define  MAIL_ITEM_TOTAL   347
#define  MAIL_ITEM_NEXT    348
