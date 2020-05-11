 /**********************************************************************\
 |  :Name.   TM_ERRS.h                                                  |
 |  :Date.   July 06, 1988                                              |
 |  :Author. John L. Foster                                             |
 |  :Purpose.                                                           |
 |                                                                      |
 |                                                                      |
 |  Define the exception handler error codes for use with the TM level  |
 |  and any necessary user interface level error codes needed.          |
 |                                                                      |
 |  Error code ranges 28000 ... 29999 for internal errors,              |
 |  range 7900 ... 7999 for user level errors (user intervention).      |
 |                                                                      |
 \**********************************************************************/

#define   UNEXPECTED_RESPONSE                 -1

 /**********************************************************************\
 \**********************************************************************/


#define   TM_INTERNAL_ERROR                28000
#define   PROCESS_IS_NOT_ACTIVE            28001
#define   MASTER_PROCESS_IS_NOT_ACTIVE     28002
#define   INVALID_PROCESS_ID               28003
#define   MASTER_SERVER_IS_NOT_AVAILABLE   28004
#define   NO_PROCESSES_ACTIVE              28005
#define   DO_NOT_CONFIRM_TRANSACTION       28006
#define   TM_USER_CONTROL_BLOCK_NOT_FOUND  28007
#define   TM_CANNOT_OPEN_COMMAND_DATA_FILE 28008
#define   TM_MASTER_PROCESS_ALREADY_ACTIVE 28009
#define   TM_EVENT_NOT_DEFINED             28010
#define   TM_SEND_CONFIRMATION             28011
#define   TM_CANNOT_STORE_RESULT           28012
#define   TM_SYS_INIT_NOT_ACTIVE           28013
#define   TM_SYS_INIT_REQUEST_OUT_OF_RANGE 28014


#define   TM_FIRST_SIGNON                  28101
#define   STATE_NOT_DEFINED                28121
#define   STATE_NOT_CODED                  28123

#define   SKI_INVALID_CHARACTER            28330
#define   SKI_INVALID_STRING               28331
#define   SKI_IMPROPER_KEYWORD_FORMAT      28332
#define   SKI_NO_ENTRY_FOUND               28333
#define   SKI_ONE_OF_MANY                  28334
#define   SKI_LAST_OF_MANY                 28335

#define   Internal_ISCF_Error              28000
#define   TM_ISF_LEVEL_ERROR               28400
#define   Server_Not_defined               28401
#define   Message_Limit_Exceeded           28403
#define   ISCF_Already_Initialized         28404
#define   Receive_Buffer_Too_Small_Or_Bad  28405
#define   Cannot_Access_Remote_Data        28406
#define   Remote_Data_Inaccessible_Or_Bad  28407
#define   ISCF_Is_Not_Initialized          28408
#define   Message_Purged_By_Remote_Server  28409
#define   Remote_User_Not_Active           28411
#define   Remote_User_Not_Initialized      28412
#define   Connection_Limit_Exceeded        28413
#define   Remote_Connection_Limit_Exceeded 28414
#define   Connection_Not_Authorized        28415
#define   ISCF_ERROR_36                    28436  /*<*****************/
#define   ISCF_ERROR_51                    28451  /*<                */
#define   ISCF_ERROR_52                    28452  /*< These are all  */
#define   ISCF_ERROR_53                    28453  /*< internal ISCF  */
#define   ISCF_ERROR_54                    28454  /*< errors. If we  */
#define   ISCF_ERROR_55                    28455  /*< get them then  */
#define   ISCF_ERROR_56                    28456  /*< there is an    */
#define   ISCF_ERROR_57                    28457  /*< ISCF problem.  */
#define   ISCF_ERROR_59                    28459  /*<*****************/
#define   No_Message_To_Receive            28462
#define   Remote_System_Disconnected       28463
#define   Connection_Already_In_Progress   28464
#define   ISCF_Not_Initialized             28465
#define   Server_Not_Known_To_ISCF         28466
#define   No_Message_Waiting               28467
#define   Cannot_Connect_To_Yourself       28468
#define   Nothing_New_Is_Waiting           28469
#define   No_New_Connections_Accepted      28470
#define   No_Server_Associated_To_PID      28479
#define   Query_Option_Not_Defined         28480
#define   Connection_Timeout               28481

#define   SIGNOFF_ABANDON_REQUESTED        28602
#define   USER_NOT_SIGNED_ON               28603

#define   System_ID_Not_In_SNI             28750
#define   Name_Not_in_SNI                  28751
#define   Password_Does_Not_Match_Name     28752
#define   Improper_Terminal_For_Signon     28753
#define   Not_A_TEIES_User_ID              28754

#define   NO_FORM_DEFINED                  28801
#define   NOT_A_REAL_USER                  28802
#define   ERROR_CODE_NOT_IN_SERRI          28803
#define   NO_MORE_MODIFIED_FIELDS          28804
#define   ERROR_IN_TM_CONNECTION_TABLE     28805
#define   TM_FORM_NOT_FORMED               28806
#define   SIGNOFF_REQUESTED                28833

#define   FORMS_LEVEL_NOT_ACTIVE           28835
#define   USER_ERRORS_ON_FORM              28836

#define   SCRATCHPAD_ERROR_OPENING_FILE    28837
#define   SCRATCHPAD_ERROR_ON_CALL         28838
#define   SCRATCHPAD_CANNOT_EDIT_FILE      28839

#define   SEARCH_METHOD_NOT_DEFINED        28873

#define   TEIES_COMMAND_TABLE_NOT_FOUND      28874
#define   TEIES_SYSTEM_ERROR_TABLE_NOT_FOUND 28875


#define   Unknown_TM_Function              28908

#define   VAR_UNDEFINED_NAME               28577
#define   VAR_UNKNOWN_TYPE                 28578

#define   TM_USER_LEVEL_ERROR              7900

#define   Noun_Not_Defined                 7901
#define   Verb_Not_Defined                 7902
#define   Word_Not_Defined                 7903
#define   Yes_Or_No_Was_Expected           7904
#define   INPUT_EXCEPTION                  7904
#define   YES_OR_NO_EXPECTED               7904
#define   INVALID_CHOICE                   7905
#define   NOTHING_TO_PROCESS               7906

#define   FORMS_HELP_NOT_AVAILABLE         7907
#define   FORM_TO_DISPLAY_NOT_FOUND        7908
#define   HELP_WAS_REQUESTED               7909
#define   NO_HELP_TO_PROCESS               7910
#define   HomeBase_Form_Not_Found          7911
#define   TM_BAD_CHAR_IN_NAME              7912
#define   TM_BAD_NAME                      7913
#define   TM_NONPRINTABLE_CHAR_IN_NAME     7914
