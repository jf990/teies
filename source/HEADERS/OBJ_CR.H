/*+-------------------------------------------------------------------+*/
/*| <Doc>                                                             |*/
/*| <NAME>     OBJ_CR H                                               |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <Author>   Sal Johar                                              |*/
/*| <Date>     August 19, 1989                                        |*/
/*| <PURPOSE.  Required for use of Conference Root Comment functions. |*/
/*| </Doc>                                                            |*/
/*+-------------------------------------------------------------------+*/

#ifndef OBJ_CR_H_INCLUDED
#  define OBJ_CR_H_INCLUDED

   /*+----------------------------------------------------------------+*/
   /*| $EXTRACT $MAKER-PROTO                                          |*/
   /*+----------------------------------------------------------------+*/

   /*+----------------------------------------------------------------+*/
   /*| Abstract Conference Root                                       |*/
   /*+----------------------------------------------------------------+*/
   extern sint ab_cr(OCB *ocb,string index_key,hword type,byte *params);

   /*+----------------------------------------------------------------+*/
   /*| Create Conference Root, with internal flags and subroutines.   |*/
   /*+----------------------------------------------------------------+*/
   extern sint cr_cr(OCB *ocb,string modifier,hword level,byte *params);

#  define REQUIRED_INFORMATION             0x0002  /* cfr_gtreq        */
#  define PUBLIC_INFORMATION               0x0004  /* cfr_gtpub        */
#  define DESCRIPTION_INFORMATION          0x0008  /* cfr_gtdes        */
#  define ASSIGNMENT_INFORMATION           0x0010  /* Call cr_memb     */
#  define ALL_INFORMATION (              \
               REQUIRED_INFORMATION      \
             |   PUBLIC_INFORMATION      \
             |   DESCRIPTION_INFORMATION \
             |   ASSIGNMENT_INFORMATION  )

#  define STARTING_POINT_CONFERENCE_CREATE 0x0001
#  define OK_TO_CREATE_CONFERENCE          0x0020   /* call lk_cr */
#  define CREATE_ANOTHER_CONFERENCE        0x0040
#  define DISPLAY_CONFERENCE_CREATED       0x0080

#  define ABANDON_CREATION                 0x0100
#  define CLEAN_UP                         0x0200

#  define PUBLIC_TITLE                     0x0001
#  define PUBLIC_DESC                      0x0002
#  define PUBLIC_MEMBER                    0x0004

   extern sint cfr_cfrm(OCB *ocb,string modifier,hword level,
      hword *todo);
   extern sint cfr_clear(OCB *conf_ocb,string modifier,hword level,
      OCB **new_conf_ocb);
   extern sint cfr_gtdes(OCB *ocb,string modifier,hword level,
      byte *params);
   extern sint cfr_info(OCB *ocb,string modifier,hword fld_done,
      hword *todo);
   extern sint cfr_gtpub(OCB *ocb,string modifier,hword level,
      byte *params);
   extern sint cfr_gtreq(OCB *ocb,string modifier,hword level,
      byte *params);

   /*+----------------------------------------------------------------+*/
   /*| Get Conference Root Id, with definitions of flags used.        |*/
   /*+----------------------------------------------------------------+*/
   extern sint cfr_gtid( sint err, string *cr_id, hword *flags );
   #  define DISPLAY_ALL_CONFS                0x0001
   #  define ALL_CONFS_DISPLAYED              0x0002
   #  define SCAN_ALL_CONFS                   0x0004
   #  define JOINING_PUBLIC_CONF              0x0008
   #  define CREATING_COMMENT                 0x0010
   #  define VIEWING_COMMENT                  0x0020

   /*+----------------------------------------------------------------+*/
   /*| Update users view of conference memberships as they may have   |*/
   /*| changed since signon/last update.                              |*/
   /*+----------------------------------------------------------------+*/

   extern sint cfr_updv( );
#  define cfr_update_conference_view  cfr_updv

   /*+----------------------------------------------------------------+*/
   /*| Update users conference marker by count, internal sequence #,  |*/
   /*| or date/time.                                                  |*/
   /*+----------------------------------------------------------------+*/

   extern sint cfr_set( OID * user_oid, OID * membership,
                       datetime * time_stamp, sint seqnum, sint ccount,
                                               OCB * conference_object );
#  define cfr_update_member_marker           cfr_set
#  define CONFERENCE_MARKER_PACKET_SIZE      32

   /*+----------------------------------------------------------------+*/
   /*| Do Conference Root                                             |*/
   /*+----------------------------------------------------------------+*/
   extern sint do_cr( void );

   /*+----------------------------------------------------------------+*/
   /*| Conference Root exists?  "type" definitions follow.            |*/
   /*+----------------------------------------------------------------+*/
   extern sint ex_cr(OCB *ocb,string modifier,hword type,OID **params);

#  define CHK_CONF_ID                      0x0001
#  define CHK_CONF_LABEL                   0x0002
#  define CHK_CONF_OID                     0x0004

   /*+----------------------------------------------------------------+*/
   /*| Find Conference Root                                           |*/
   /*+----------------------------------------------------------------+*/
   extern sint fn_cr(OCB *ocb,string modifier,hword level,byte *params);

   /*+----------------------------------------------------------------+*/
   /*| Get Conference Root                                            |*/
   /*+----------------------------------------------------------------+*/
   extern sint gt_cr(OCB *cr_ocb,string modifier,hword level,
      sint cc_count);

   /*+----------------------------------------------------------------+*/
   /*| Join Conference Root                                           |*/
   /*+----------------------------------------------------------------+*/
   extern sint jn_cr(OCB *ocb,string modifier,hword level,OCB *memb_ocb);

   /*+----------------------------------------------------------------+*/
   /*| Link Conference Root                                           |*/
   /*+----------------------------------------------------------------+*/
   extern sint lk_cr(OCB *ocb,string modifier,hword level,
      string *params);

   /*+----------------------------------------------------------------+*/
   /*| Modify Conference Root                                         |*/
   /*+----------------------------------------------------------------+*/
   extern sint md_cr( void);

   /*+----------------------------------------------------------------+*/
   /*| Organize Conference Root                                       |*/
   /*+----------------------------------------------------------------+*/
   extern sint or_cr( string modifier);

   /*+----------------------------------------------------------------+*/
   /*| Receive Conference Root                                        |*/
   /*+----------------------------------------------------------------+*/
   extern sint rc_cr( void);

   /*+----------------------------------------------------------------+*/
   /*| Remove Conference Root                                         |*/
   /*+----------------------------------------------------------------+*/
   extern sint rm_cr( string modifier);

   /*+----------------------------------------------------------------+*/
   /*| Review Conference Root                                         |*/
   /*+----------------------------------------------------------------+*/
   extern sint rv_cr( string modifier);

   /*+----------------------------------------------------------------+*/
   /*| Scan Conference Root                                           |*/
   /*+----------------------------------------------------------------+*/
   extern sint sc_cr( void);

   /*+----------------------------------------------------------------+*/
   /*| View Conference Root                                           |*/
   /*+----------------------------------------------------------------+*/
   extern sint vw_cr(OCB *ocb,string modifier,hword level,byte *params);

   /*+----------------------------------------------------------------+*/
   /*| $ENDEXTRACT                                                    |*/
   /*+----------------------------------------------------------------+*/

#define   CR_CONFERENCE_EXISTS( rc )                           \
     ( ( rc == CONF_ID_ALREADY_IN_CRI           )   ||         \
       ( rc == CONF_LABEL_ALREADY_IN_CRI        )   ||         \
       ( rc == CONF_ALREADY_EXISTS              )   ||         \
       ( rc == CONF_ID_AND_LABEL_ALREADY_IN_CRI )      )
#endif
