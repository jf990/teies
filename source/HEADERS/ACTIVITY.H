/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  :NAME.    Activity Header                                        |
  |  :AUTHOR.  Heidi Harting         Tanmay S. Kumar                  |
  |  :DATE.    August 13, 1987       March 1, 1989.                   |
  |  :LEVEL.   ACTIVITY                                               |
  |  :PURPOSE. Define the Activity level routines, constants and      |
  |            structures.                                            |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/


/* Definitions for the Activity Types */

#define  READ               1
#define  QUESTION           2
#define  SELECTION          3
#define  GRADEBOOK          4
#define  EXAM               5
#define  SPREADSHEET        6

/* Definitions for LINKing Activity Types */

#define  ACT_LINK_MAIL      6
#define  ACT_LINK_CC        7
#define  ACT_LINK_ACT       8
#define  ACT_LINK_RESPONSE  9
#define  ACT_LINK_READ      10
#define  ACT_READ_READ      11
#define  ACT_LINK_SELECTION 12

#define  NTF_FIRST         0x80       /* 10000000 */
#define  NTF_EACH          0x40       /* 01000000 */
#define  NTF_LAST          0x20       /* 00100000 */
#define  NTF_SPECL         0x10       /* 00010000 */

#define  VW_PER            0x80       /* 10000000 */
#define  BY_PER            0x40       /* 01000000 */

#define  RESPONDED         9999

/*$EXTRACT $MAKER-PROTO*/

extern sint cr_act( OCB *atch_ocb , string modifier , hword level,
                                                         OCB **result);
extern sint a_crrd(OCB *atch_ocb, OID *act_oid);
extern sint a_crqs(OCB *atch_ocb, OID *act_oid);
extern sint a_crsl(OCB *atch_ocb, OID *act_oid);
extern sint a_adact(OCB *ocb , sint act_type);
extern sint a_admem(OCB *attach_ocb, OCB *memb_ocb);
extern sint a_slnot(OCB * ct_ocb, sint notify, sint type);
extern sint a_crpt( string sp_loc , OCB *act_ocb , tint part_num,
                                     OCB **part_ocb, hword act_type);

extern sint do_act( OCB *atch_ocb , string modifier , hword level,
                                                         OCB **result);
extern sint a_dord( OCB *act_ocb , sint attach_type, OCB *attach_ocb,
                                                       OCB **return_ocb);
extern sint a_doqs( OCB *act_ocb , sint attach_type, OCB *attach_ocb,
                                                       OCB **return_ocb);
extern sint a_dosl( OCB *act_ocb , sint attach_type, OCB *attach_ocb,
                                                       OCB **return_ocb);
extern sint a_gtpt( OCB *act_ocb , sint act_type , sint part_num,
                                          sint attach_type, string id);

extern sint vw_act( OCB *atch_ocb , string modifier , hword level ,
                                                        OID  *act_oid);
extern sint a_crntf( OCB *act_ocb, sint part_num, hword act_type,
                                                      sint attach_type);
extern sint rc_act( OCB *act_ocb, string modifier, hword level,
                                                       byte *params) ;
extern sint a_addsel( OCB *ocb, string modifier, hword level,
                                              sint itm_num, OCD *ocd);

extern sint sc_act( string modifier) ;
extern sint fn_act( string modifier) ;
extern sint md_act( string modifier) ;
extern sint jn_act( string modifier) ;
extern sint rm_act( string modifier) ;
extern sint lk_act( OCB *attach_ocb , string packet , hword level ,
                                                     string  *result) ;
extern sint gt_act( string modifier) ;
extern sint rv_act( string modifier) ;
extern sint or_act( string modifier) ;
extern sint ex_act(OCB *ocb,string modifier, hword level, byte *params);
extern sint ab_act(OCB *ocb,string modifier, hword level, byte *params);



/*$ENDEXTRACT */
