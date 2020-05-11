/*+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+
  |   <DOC>                                                           |
  |   <Name>         OBJ_ERRS Header                                  |
  |   <Date>         July 15, 1988                                    |
  |   <Author>       JF                                               |
  |   <level>        OBJ                                              |
  |   <Purpose>      Define all Object Level error codes by level:    |
  |                                                                   |
  |                          22xxx  - DMS Interface                   |
  |                          20xxx  - Obj Level                       |
  |                           7xxx  - User Interface                  |
  |                           6xxx  - User Interface                  |
  |   </DOC>                                                          |
  + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +*/
#define     OBJ_NO_ERROR                                   0
#define     NO_ERROR                                       0
#define     NO_ERR                                         0
#define     DMS_LEVEL_ERROR                            22000
#define     OBJ_LEVEL_ERROR                            20000
#define     IMPROPER_OBJECT_TYPE                       20001
#define     OID_NOT_IN_SOIDI                           20002
#define     OID_IS_OPEN                                20003
#define     OID_IS_NOT_OPEN                            20004
#define     INVALID_OID_POINTER                        20005
#define     NOT_A_SYSTEM_INDEX                         20006
#define     OCD_POINTER_INVALID                        20007
#define     GFD_DOES_NOT_EXIST                         20008
#define     ODT_DOES_NOT_EXIST                         20009
#define     ODT_IS_INVALID                             20010
#define     INVALID_OCD_STRING                         20011
#define     ODT_MEMORY_FULL                            20013
#define     ODT_CANNOT_BE_LOADED                       20014
#define     LINK_EXISTS_BUT_TYPE_MISMATCH              20015
#define     ODT_CANNOT_BE_SAVED                        20016
#define     NO_SEGMENT_TO_REPLACE_DATA                 20017
#define     INDEX_TO_REMOVE_DOES_NOT_EXIST             20018
#define     FILE_FUNCTION_ERROR                        20019
#define     OCD_NOT_IN_ODT                             20020
#define     INVALID_DATA_POINTER                       20021
#define     INVALID_OCB                                20022
#define     NO_DATA_FOUND                              20023
#define     NO_MORE_KEYS                               20024
#define     DATA_NOT_REPLACED                          20025
#define     NO_DATA_PROVIDED                           20026
#define     OCD_NOT_OF_PROPER_TYPE                     20027
#define     INVALID_OCD_TYPE                           20028
#define     INVALID_OCD_PACKET                         20029
#define     INVALID_OID                                20030
#define     CANNOT_CREATE_OBJECT                       20031
#define     DMS_PACK_FAILED                            20032
#define     DMS_UNPACK_FAILED                          20033
#define     OBJECT_PACKET_NOT_PROPER_FORMAT            20034
#define     GENERIC_FILE_DESCRIPTOR_ERROR              20035
#define     NO_INHERITANCE_POINTER_TO_FOLLOW           20036
#define     DATA_OVERFLOW                              20037
#define     OCD_LEVELS_DO_NOT_MATCH                    20040
#define     IMPROPER_DIRECTION_IN_ODT                  20041
#define     CANNOT_DETERMINE_OID                       20042
#define     CANNOT_STORE_RESULT_FLAGS                  20043
#define     CANNOT_STORE_RESULT_PACKET                 20044
#define     INVALID_DATA_PACKET                        20045
#define     INVALID_OWNER_OCB                          20046
#define     NEXT_STEP_NOT_DEFINED_IN_CR_CR             20047
#define     NO_SERVER_SPECIFIED                        20048
#define     CANNOT_STORE_RESULT_OID                    20049
#define     CANNOT_STORE_RESULT_OCB                    20050
#define     OBJECT_IS_READ_ONLY                        20051
#define     DATA_BASE_INDEX_ANOMALY                    20052
#define     INDEX_IS_EMPTY                             20053
#define     OBJECT_NOT_LINKED                          20054
#define     OS_FILE_I_O_ERROR                          20055
#define     SOIDI_NOT_SECOND_ALLOCATED_OCB             20092
#define     SOIDI_IS_NOT_OPEN                          20093
#define     FUNCTION_NOT_SUPPORTED                     20222
#define     OBJ_FUNCTION_NOT_SUPPORTED                 20222
#define     OBJ_LEVEL_UNSUPPORTED                      20223
#define     FUNCTION_NOT_IMPLEMENTED                   20224
#define     DMS_OPEN_ERROR                             20700
#define     PSI_IS_LOCKED                              20983
#define     SOIDI_IS_LOCKED                            20984
#define     OBJ_NECESSARY_ODT_NOT_FOUND                20985
#define     OBJ_OBJECT_TABLE_FULL                      20986
#define     SERRI_DOES_NOT_EXIST                       20987
#define     SMI_MISSING_OR_INVALID                     20988
#define     SGI_MISSING_OR_INVALID                     20989
#define     SOIDI_MISSING_OR_INVALID                   20990
#define     PSI_MISSING_OR_INVALID                     20991
#define     SNI_MISSING_OR_INVALID                     20992
#define     SOIDI_CANNOT_BE_UPDATED                    20993
#define     DATA_BASE_IS_CORRUPT                       20994
#define     CRI_MISSING_OR_INVALID                     20995
#define     SFI_MISSING_OR_INVALID                     20996
#define     LFD_1_IS_MISSING                           20997
#define     SOIDI_NOT_IN_PSI                           20998
#define     CANNOT_CREATE_LFD_1                        20999


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |                                                                   |
 |                                                                   |
 |                 U S E R    L E V E L    E R R O R S               |
 |                                                                   |
 |               Total range is codes 6000 through 7799.             |
 |           6000 through 6099 reserved for general messages.        |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define Item_Does_Not_Exist                                 6001
#define REQUIRED_INFO_NEEDED                                6002
#define UNKNOWN_OBJECT_TYPE                                 6003
#define UNKNOWN_ACTION_TYPE                                 6004
#define FEATURE_NOT_AVAILABLE                               6005
#define BAD_ASSOCIATION                                     6006
#define OBJECT_RECEIVED                                     6007
#define ITEM_ALREADY_RECEIVED                               6008

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Conference Comments are range 6200 to 6249.                      |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define COMMENT_OBJECT_RECEIVED                             6200
#define NO_COMMENTS_TO_LIST                                 6201

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Notification error messages are range 6250 to 6299.              |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define NTF_DOES_NOT_EXIST                                  6250
#define NTF_OBJECT_RECEIVED                                 6251
#define INVALID_NOTIFICATION_TYPE                           6252

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Membership object error messages are range 6300 to 6319.         |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define MEMBERSHIP_ALREADY_EXISTS                           6300
#define MEMBERSHIP_DOES_NOT_EXIST                           6301

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  User object error messages are range 6320 to 6359.               |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define USER_ALREADY_EXISTS                                 6320
#define NO_ASSOCIATED_ITEM                                  6321
#define CANNOT_MODIFY_THIS_USER                             6322
#define MEMBER_DOES_NOT_EXIST                               6323

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Mail messages are range 6360 to 6399.                            |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define MAIL_DOES_NOT_EXIST                                 6360
#define MAIL_OBJECT_RECEIVED                                6361
#define NO_MAIL_TO_RECEIVE                                  6362
#define UNKNOWN_USER_ID                                     6363
#define NOT_ALLOWED_TO_SEND_TO                              6364
#define IMPROPER_KEYWORD                                    6365
#define INCOMPLETE_NAME_SPECIFIED                           6366
#define USER_NUMBER_NOT_PROVIDED                            6367
#define NO_ADDRESSES_SPECIFIED                              6368
#define MAIL_CANNOT_ASSOCIATE_REPLY                         6369
#define MAIL_ITEMS_DO_NOT_EXIST                             6370

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Activities are range 6400 to 6429.                               |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define ACTIVITY_ERROR                                      6400
#define CANNOT_REPLY_TO_ACTIVITY                            6401
#define ACTIVITY_DONE                                       6402
#define NO_ASSOCIATED_ACTIVITY                              6403
#define ACTIVITY_IS_CLOSED                                  6404
#define MULTIPLE_RESPONSES_NOT_ALLOWED                      6405
#define NOT_ALLOWED_TO_VIEW_RESPONSE                        6406
#define PERMISSION_DENIED                                   6407
#define SELECTION_NOT_AVAILABLE                             6408
#define SELECTION_ACCEPTED                                  6409
#define SELECTION_LIMIT_REACHED                             6410

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Conference Roots (CR) are range 7660 to 7679.                    |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define CONF_ID_ALREADY_IN_CRI                              7661
#define CONF_ID_NOT_IN_CRI                                  7662
#define CONF_LABEL_ALREADY_IN_CRI                           7663
#define CONF_LABEL_NOT_IN_CRI                               7664
#define CONF_ALREADY_EXISTS                                 7665
#define CONF_DOES_NOT_EXIST                                 7666
#define CONF_ID_NOT_PROVIDED                                7667
#define CONF_LABEL_NOT_PROVIDED                             7668
#define CONF_ID_AND_LABEL_ALREADY_IN_CRI                    7669
#define CONF_DOES_NOT_HAVE_MEMBERS                          7670
#define CONF_ACCESS_NOT_ALLOWED                             7671
#define CONF_OWNER_INVALID                                  7672
#define CONF_NOT_MEMBER_OF_ANY                              7673
#define CONF_ID_AND_LABEL_NOT_IN_CRI                        7674

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Groups are range 7680 to 7699.                                   |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define GROUP_ID_ALREADY_IN_SGI                             7680
#define GROUP_ID_NOT_IN_SGI                                 7681
#define GROUP_LABEL_ALREADY_IN_SGI                          7682
#define GROUP_LABEL_NOT_IN_SGI                              7683
#define GROUP_ALREADY_EXISTS                                7684
#define GROUP_DOES_NOT_EXIST                                7685
#define GROUP_ID_NOT_PROVIDED                               7686
#define GROUP_LABEL_NOT_PROVIDED                            7687
#define GROUP_ID_AND_LABEL_ALREADY_IN_SGI                   7688
#define GROUP_DOES_NOT_HAVE_MEMBERS                         7689
#define GROUP_ACCESS_NOT_ALLOWED                            7690
#define GROUP_OWNER_INVALID                                 7691
#define GROUP_CREATED                                       7692
#define GROUP_ID_OR_LABEL_NOT_PROVIDED                      7693
#define GROUP_LABEL_OR_ID_NOT_IN_SGI                        7694

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Tickets and object level privileges are range 7700 to 7729.      |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define ACCESS_DENIED_FOR_OBJECT                            7700
#define PRIVILEGE_CONFLICT                                  7701
#define TICKET_COUNT_EXCEEDED                               7702
#define TICKET_DATE_EXPIRED                                 7703
#define TICKET_DOES_NOT_EXIST                               7704
#define UNDEFINED_ROLE                                      7705
#define UNDEFINED_PRIVILEGE                                 7706

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Contents objects are error range 7730 to 7739.                   |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define CONTENTS_NOT_FOUND                                  7730

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  Network directories are range 7740 to 7769.                      |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define NETWORK_DIRECTORY_NOT_FOUND                         7740

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  List object are range 7770 to 7799.                              |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

#define LIST_OBJECT_NOT_FOUND                               7770
#define LIST_OBJECT_IS_EMPTY                                7771
#define OBJ_LIST_KEY_REQUIRED                               7772

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *\
 |  The whole range 7800 - 7899 is reserved for NAME NOT FOUND of    |
 |  ex_user to reflect the number of names not found in the error    |
 |  code (rc - NAME_NOT_FOUND = # of unverified names).              |
\* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
#define NAME_NOT_FOUND                                      7800
