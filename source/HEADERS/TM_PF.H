 /**********************************************************************\
 |  :Name.   TM_PK Header                                               |
 |  :Date.   August 30, 1989                                            |
 |  :Author. John L. Foster                                             |
 |  :Purpose.                                                           |
 |                                                                      |
 |  Define the return codes for the command line (t_command()), the     |
 |  form processor (t_form()), and the various defined program functions|
 |  available in TEIES.                                                 |
 \**********************************************************************/
 /**********************************************************************\
 |                                                                      |
 |  t_form(), t_command() both return a special code if the user        |
 |  selects a numeric Reactive Menu option on the command line. This    |
 |  code is in the range of 700 + 1 ... 700 + 99.  Therefore only       |
 |  99 menu choices can be supported on TEIES.                          |
 |                                                                      |
 \**********************************************************************/

#ifndef TM_PF_H_INCLUDED

#define TM_PF_H_INCLUDED 1

#define   TM_REACTIVE_MENU_SELECTION         700
#define   TM_MIN_REACTIVE_MENU_SELECTION     701
#define   TM_MAX_REACTIVE_MENU_SELECTION     799
#define   USER_REQUESTED_SOME_HELP           796
#define   USER_REQUESTED_MORE_HELP           797
#define   USER_REQUESTED_CHAT                798
#define   USER_REQUESTED_SIGNOFF             799

 /**********************************************************************\
 |                                                                      |
 |  t_form(), t_command() both return a special code if the user        |
 |  pressed or selected a Control Function from a Function Key. This    |
 |  code is in the range of 800 + 1 ... 800 + 99.  Therefore only 99    |
 |  function keys can be supported on TEIES.                            |
 |                                                                      |
 \**********************************************************************/

#define   TM_CONTROL_MENU_SELECTION          800
#define   TM_MIN_CONTROL_MENU_SELECTION      801
#define   TM_MAX_CONTROL_MENU_SELECTION      899
#define   USER_REQUESTED_HELP      (TM_CONTROL_MENU_SELECTION+FORM_HELP)
#define   USER_REQUESTED_CONFIRM   (TM_CONTROL_MENU_SELECTION+FORM_CONFIRM)
#define   USER_REQUESTED_QUIT      (TM_CONTROL_MENU_SELECTION+FORM_QUIT)
#define   USER_REQUESTED_CANCEL    (TM_CONTROL_MENU_SELECTION+FORM_QUIT)
#define   USER_REQUESTED_PROCESS   (TM_CONTROL_MENU_SELECTION+FORM_PROCESS)
#define   USER_REQUESTED_REPLY     (TM_CONTROL_MENU_SELECTION+FORM_REPLY)
#define   USER_REQUESTED_PRINT     (TM_CONTROL_MENU_SELECTION+FORM_PRINT)
#define   USER_REQUESTED_KEEP      (TM_CONTROL_MENU_SELECTION+FORM_PRINT)
#define   USER_REQUESTED_SCAN      (TM_CONTROL_MENU_SELECTION+FORM_SCAN)
#define   USER_REQUESTED_GET       (TM_CONTROL_MENU_SELECTION+FORM_SCAN)
#define   USER_REQUESTED_ABANDON   (TM_CONTROL_MENU_SELECTION+FORM_ABANDON)
#define   USER_REQUESTED_HOMEBASE  (TM_CONTROL_MENU_SELECTION+FORM_ABANDON)
#define   USER_REQUESTED_EDIT_HELP (TM_CONTROL_MENU_SELECTION+FORM_EDIT_HELP)
#define   USER_REQUESTED_CLEAR     (TM_CONTROL_MENU_SELECTION+FORM_CLEAR)
#define   USER_REQUESTED_ESCAPE    (TM_CONTROL_MENU_SELECTION+FORM_ESCAPE
#define   USER_REQUESTED_ENTER     (TM_CONTROL_MENU_SELECTION+FORM_ENTER)


#endif
