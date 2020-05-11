/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||  <DOC>                                                            ||
 ||  <NAME>     OBJ_USER Header                                       ||
 ||  <LEVEL>    OBJ                                                   ||
 ||  <DATE>     January 25, 1989                                      ||
 ||  <AUTHOR>   JF                                                    ||
 ||  <PURPOSE>  Function definitions and constants for use by object  ||
 ||             level routines dealing with the USER object.          ||
 ||  <ERRORS>   Defined in OBJ_ERRS Header                            ||
 ||  </DOC>                                                           ||
 ||                                                                   ||
 ||   $MAKER-PROTO                                         $EXTRACT   ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern sint vw_user( OCB * some_ocb, string names_from_cmd_line,
                                    hword type_of_search, byte * params);

#define     SEARCH_FOR_USERS_MATCHING_STRING        0
#define     OBJ_ALL_USERS                           1
#define     ALL_USERS_IN_SNI                        1
#define     ALL_USERS_LINKED_TO_OBJECT              3
#define     USERS_LINKED_TO_OBJECT_MATCHING_STRING  4
#define     USER_OBJECT_ONLY                        5

extern sint cr_user( OCB * dummy_ocb, string modifier, hword level,
                                                         OCB ** params) ;
extern sint sc_user  ( void) ;
extern sint fn_user( OCB * some_ocb, string modifier, hword level,
                                                     byte * parameters) ;
extern sint md_user( OCB * some_ocb, string names_from_cmd_line,
                                            hword level, byte * params) ;
extern sint rc_user( void) ;
extern sint jn_user( void) ;
extern sint rm_user( string modifier) ;
extern sint do_user( void) ;
extern sint lk_user( OCB * user, string modifier, hword level,
                                                   string * result_str) ;

#define     LINK_FOR_CREATE_USER         0
#define     LINK_FOR_NEW_USER            2
#define     LINK_FOR_MODIFY_USER         3

#define     SNI_MAXIMUM_KEY_LENGTH      32
#define     USER_MAXIMUM_ID_LENGTH       4
#define     USER_MAXIMUM_ID           9999
#define     USER_MINIMUM_ID              0

extern sint rv_user  ( string modifier) ;
extern sint gt_user( OCB * user, string modifier, hword level,
                                                          byte * params);
extern sint or_user  ( string modifier) ;
extern sint ab_user( OCB * ocb, string modifier, hword level,
                                                        byte  * params );
extern sint ex_user( OCB * list_ocb, string usr_name, hword level,
                                                        string * params);

/*
   This routine is used to add or remove an entry to a list referenced
   from the user object.  If the OID is NIL then the list entry accessed
   by the key is removed, otherwise it is added to the list.  Number and
   data are list object fields in case they are needed.  The key must be
   a string, and the user object must be provided.
 */
extern sint user_list( OCB * user_object, byte * list_component,
                       string key, OID * oid, sint number, string data );

#define add_to_user_list       user_list
#define remove_from_user_list  user_list

/*                                                         $ENDEXTRACT */

#define     MAX_FULLNAME_LEN         20
#define     STRING_TO_OID            ((hword)0x0001)
#define     OID_TO_STRING            ((hword)0x0002)
#define     NO_NEED_TO_XMIT          ((hword)0x0100)
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
 ||  Format for a User Statistic entry in the list object referenced  ||
 ||  by INDEXES.STATS.ENTRY  (key is Date of entry which is time_off) ||
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
extern struct user_statistic_entry {

                                      datetime      time_on;
                                      sint          count_oid_created,
                                                    count_oid_modified,
                                                    count_oid_received;
                                      sint          access_type;
                                   };
typedef struct user_statistic_entry USER_STAT_ENTRY;
