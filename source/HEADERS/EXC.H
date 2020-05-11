/*
+--------------------------------------------------------------------+
|  <DOC>                                                             |
|  <Name>     except.h                                               |
|  <Date>     March 27, 1988                                         |
|  <Author>   John L. Foster                                         |
|  <Purpose>  Contains all definitions needed for use with the       |
|             exception handler.                                     |
|  <MODS>                                                            |
|  </DOC>                                                            |
+--------------------------------------------------------------------+
*/

extern sint except( sint code, sint type, string arg1, string arg2,
                              string arg3, string arg4, string arg5 )  ;


/*

    The following are the standard error types.

*/


#define INFO            0         /* Place an entry in the error       */
#define INFORMATIONAL   0         /* log but do not report and error   */
                                  /* or change control of the system   */
#define DATA_ERROR      1         /* Typically, on user input          */
#define PROG_ERROR      2         /* IE, invalid REXX call             */
#define PROGRAM_ERROR   2         /*                                   */
#define UNKNOWN_COMMAND 3         /*                                   */
#define UNKNOWN_CMD     3         /* User command unknown to           */
                                  /* component making call             */
#define HELP_WANTED     4         /* User help request                 */
#define FATAL_ERROR     5         /* Fatal system error detected       */
#define CONTROL_ERROR   6         /* Error in TEIES state logic        */
#define RESET_PROCESS   7         /* Process must be halted, return    */
                                  /*   to the scheduler.               */

/*

    The following are type codes that tell the exception handler that
    a message should go to the user on the display.  Increasing value
    indicates increasing severity.  eg. USER_ERROR forces a call to
    t_alarm( sint process_id ).

*/

#define USER_INFORMATION    8
#define USER_INFO           8     /* Tell the user about something     */
                                  /*   that is not an error.           */
#define USER_WARNING        9     /* User did something that may       */
                                  /*   eventually cause trouble or     */
                                  /*   is ambiguous.                   */
#define USER_LEVEL_ERROR   10     /*                                   */
#define USER_ERROR         10     /* User did something that is no     */
                                  /*   doubt an error and the system   */
                                  /*   can go no further until correct.*/

#define NOTIFICATION_MESSAGE 11   /*                                   */
#define NOTIF_MESSAGE        11   /* Any Notification messages are     */
                                  /*   handled when the except() is    */
                                  /*   called with this type.          */


/*       TEIES Levels recognized by the exception handler:             */

                                                              /*********/
#define            OPERATING_SYSTEM_KERNEL_LEVEL     11000    /* OSK_  */
#define            VIRTUAL_DEVICE_INTERFACE_LEVEL    14000    /* VDI_  */
#define            GRAPHICS_KERNEL_SYSTEM_LEVEL      15000    /* GKS_  */
#define            STRING_LEVEL                      16000    /* STR_  */
#define            INTERFACE_STRING_VECTOR_LEVEL     16000    /* ISV_  */
#define            OBJECT_LEVEL                      20000    /* OBJ_  */
#define            DATA_BASE_MANAGEMENT_SYSTEM_LEVEL 22000    /* DMS_  */
#define            FORMS_LEVEL                       23000    /* F_    */
#define            HASH_TABLE_MANAGEMENT_LEVEL       24000    /* HSH_  */
#define            EXTERNAL_INTERFACE_LEVEL          25000    /* EI_   */
#define            PRIVILEGE_LEVEL                   26000    /* PRV_  */
#define            MEMORY_MANAGEMENT_LEVEL           27000    /* MEM_  */
#define            TRANSACTION_MANAGER_LEVEL         28000    /* TM_   */
#define            SEGMENT_INDEX_MANAGEMENT_LEVEL    29000    /* SGI_  */
#define            INTER_SERVER_COMMUNICATION_LEVEL  30000    /* ISF_  */
#define            PERSONAL_COMPUTER_INTERFACE_LEVEL 31000    /* PC_   */
                                                              /*********/
#define            USER_INTERFACE_DATA_BASE_LEVEL     6000
#define            USER_INTERFACE_TRANSACTION_LEVEL   7000



/*

    The following are the known error codes
    Eventually there will be an additional #include for these.
    (see also TM_ERRS.H and OBJ_ERRS.H)

                                                                     */

#define OUT_OF_MEMORY                 27999
#define LOWEST_USER_LEVEL_EXCEPTION    1000
#define HIGHEST_USER_LEVEL_EXCEPTION   9999
#define LOWEST_PROGRAM_FUNCTION_KEY     700
#define HIGHEST_PROGRAM_FUNCTION_KEY    899

/*
+--------------------------------------------------------------------+
| Define some macros to simplify error code checking.  User level    |
| errors are things the interface determines the user has done wrong.|
| System level errors are problems with teies.                       |
+--------------------------------------------------------------------+
*/

#define User_Level_Error( err )   ((err>=USER_INTERFACE_DATA_BASE_LEVEL)\
                          && (err<=USER_INTERFACE_TRANSACTION_LEVEL+999))

#define System_Level_Error( err )     \
                               (err>USER_INTERFACE_TRANSACTION_LEVEL+999)

#define Program_Function_Request( err ) ((err>=LOWEST_PROGRAM_FUNCTION_KEY)\
                                      || (err<=HIGHEST_PROGRAM_FUNCTION_KEY))
