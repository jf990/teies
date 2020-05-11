/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||  <DOC>                                                            ||
 ||  <NAME>     OBJ_MEMB Header                                       ||
 ||  <LEVEL>    OBJ                                                   ||
 ||  <DATE>     January 25, 1989                                      ||
 ||  <AUTHOR>   Gurinder Sal Johar                                    ||
 ||  <PURPOSE>  Function definitions and constants for use by object  ||
 ||             level routines dealing with the MEMBERSHIP object.    ||
 ||  <ERRORS>   Defined in OBJ_ERRS Header                            ||
 ||  </DOC>                                                           ||
 ||                                                                   ||
 ||                          $MAKER-PROTO                  $EXTRACT   ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */



extern sint vw_memb( OCB *ocb,string modifier,hword level,  OCB *owner);
extern sint cr_memb(OCB *owner,string modifier,hword level,OCB **params);
extern sint ex_memb( OCB *ocb,OID *memb_oid,hword level,  PRIVS *params);
extern sint md_memb( OCB *ocb,string modifier,hword level, OCB **params);
extern sint jn_memb( OCB *ocb,string modifier,hword level, byte *params);

extern sint lk_memb(OCB *ocb,string modifier,hword level,string *params);
extern sint memb_fmt( OCB * membership );

extern sint sc_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint fn_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint rc_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint rm_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint do_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint gt_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint rv_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint or_memb( OCB *ocb,string modifier,hword level,byte *params);
extern sint ab_memb( OCB *ocb,string modifier,hword level,byte *params);

extern sint memb_gt( OCB *ocb,string modifier,hword level,sint itm_num,
                sint las_itm_num, sint **seqnum_arry,sint *arry_size );

#define  MEMBERSHIP_BIT_WINDOW_SIZE      ( 64 * 8 )

extern sint memb_rc( OCB *ocb,string modifier,hword level,sint itm_num,
                                                         OID *user_oid);

/*                                                         $ENDEXTRACT */


#define MEMB_LINK_TO_USER                       1
#define MEMB_LINK_TO_OWNER                      2

#define MEMB_MODIFIER_IS_NAME_TO_ADD       0x0010
#define MEMB_MODIFIER_IS_NAME_TO_REMOVE    0x0020
#define MEMB_MODIFIER_IS_NAME_TO_MODIFY    0x0030
#define MEMB_MODIFIER_IS_OID_TO_ADD        0x0040
#define MEMB_MODIFIER_IS_OID_TO_REMOVE     0x0050
#define MEMB_MODIFIER_IS_OID_TO_MODIFY     0x0060

#define MEMB_OCB_ALREADY_CHECKED           0x0001
#define MEMB_OCB_ALREADY_LINKED            0x0002
#define MEMB_USE_EXISTING_OBJECT           MEMB_OCB_ALREADY_LINKED

                                           /* If caller tells cr_memb  */
                                           /* to use existing object   */
                                           /* pass on to md_memb that  */
                                           /* object is also linked and*/
                                           /* there is no need to link */

#define MEMB_DOING_LINKING                 0x0004
#define MEMB_DOING_VALIDATION              0x0008
#define MEMB_DOING_CREATION                0x0008

                                           /* Don't send notifications */
                                           /* till owner says OK to the*/
                                           /* whole creation process.  */

#define MEMB_USE_EXISTING_LISTS            0x1000
                                           /* We already have created  */
                                           /* no need to create again  */

#define MEMB_SET_MARKER                    0x2000
                                           /* Set begining mrker       */
                                           /* used in rcv_itm          */

#define MEMB_ALL_UNREAD_ITEM               0x4000
                                           /* Used in get_itm          */

#define MEMB_SCANNING_ITEMS                0x8000
                                           /* Used in rcv_itm          */
