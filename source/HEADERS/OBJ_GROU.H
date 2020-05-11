/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ_GROUP Header                                       |
  |  <AUTHOR>  John L. Foster, Tanmay S. Kumar, Ajaz Rana             |
  |  <DATE>    February 13, 1990                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Definitions for object level Group functions.          |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#define    CHECK_GROUP_ID         0x0001
#define    CHECK_GROUP_LABEL      0x0002
#define    CHECK_GROUP_OID        0x0003
#define    CHECK_GROUP_ALL        0x0004

#define     G_ON_MS_ALREADY       0x0100

#define     G_NEED_ONE_OID        0x1000
#define     G_NEED_MANY_OID       0x2000

#define     G_PUBLIC_TITLE        0x0001
#define     G_PUBLIC_DESC         0x0002
#define     G_PUBLIC_MEMBER       0x0004
#define     G_PUBLIC_CONF         0x0008

#define     REQUIRED_INFORMATION         0x0002    /* grp_req         */
#define     PUBLIC_INFORMATION           0x0004    /* grp_pub         */
#define     DESCRIPTION_INFORMATION      0x0008    /* grp_des         */
#define     ASSIGNMENT_INFORMATION       0x0010    /* Call cr_memb    */

#define     ALL_INFORMATION            ( REQUIRED_INFORMATION     \
                                     |   PUBLIC_INFORMATION       \
                                     |   DESCRIPTION_INFORMATION  \
                                     |   ASSIGNMENT_INFORMATION )

#define     STARTING_POINT_GROUP_CREATE  0x0001
#define     OK_TO_CREATE_GROUP           0x0020   /* call lk_grup */
#define     CREATE_ANOTHER_GROUP         0x0040
#define     DISPLAY_GROUP_CREATED        0x0080

#define     ABANDON_CREATION             0x0100
#define     CLEAN_UP                     0x0200


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||                                             $EXTRACT  $MAKER-PROTO||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern sint cr_grup( OCB * ocb, string modifier, hword level,
                                                         byte * params );

extern sint grp_srt( OCB * group_ocb, string modifier, hword level,
                                                     hword  *to_do_list);
extern sint grp_req( OCB * group_ocb, string modifier, hword level,
                                                           byte *params);
extern sint grp_pub( OCB * group_ocb, string modifier, hword level,
                                                           byte *params);
extern sint grp_des( OCB * group_ocb, string modifier, hword level,
                                                           byte *params);
extern sint grp_dis( OCB * group_ocb, string modifier, hword level,
                                                      hword *to_do_list);
extern sint lk_grup  ( OCB * group_ocb, string modifier, hword level,
                                                           byte *params);
extern sint ex_grup  ( OCB **list_ocb,string what_to_get,
                                       hword search_type, void **params);

extern sint grp_clr(OCB *group_ocb,string modifier,hword level,
                                                    OCB **new_group_ocb);

extern sint fn_grup  ( OCB * ocb, string modifier, hword level, byte *
                                                           parameters ) ;
extern sint ab_grup  ( OCB * ocb, string modifier, hword level,
                                                           byte *params);
extern sint vw_grup  ( void ) ;
extern sint gt_grup  ( string modifier ) ;
extern sint rv_grup  ( string modifier ) ;
extern sint or_grup  ( string modifier ) ;
extern sint md_grup  ( void ) ;
extern sint rc_grup  ( void ) ;
extern sint jn_grup  ( void ) ;
extern sint rm_grup  ( string modifier ) ;
extern sint do_grup  ( void ) ;
extern sint sc_grup  ( void ) ;
/*                                                         $ENDEXTRACT */
