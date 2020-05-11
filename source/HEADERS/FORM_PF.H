/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    form_pf header                                         |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 23, 1989                                        |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  None                                                   |
  |  <PURPOSE> Define the function key values and return codes        |
  |            from the forms processor.                              |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#ifndef FORM_PF_H_INCLUDED


#define FORM_PF_H_INCLUDED 1
                                            /*Default Key*/
#define  FORM_HELP           (   1 )        /*   PF 1   */
#define  FORM_CONFIRM        (   2 )        /*   PF 2   */
#define  FORM_QUIT           (   3 )        /*   PF 3   */
#define  FORM_PROCESS        (   4 )        /*   PF 4   */
#define  FORM_REPLY          (   5 )        /*   PF 5   */
#define  FORM_PRINT          (   6 )        /*   PF 6   */
#define  FORM_PAGE_DOWN      (   7 )        /*   PF 7   */
#define  FORM_PAGE_UP        (   8 )        /*   PF 8   */
#define  FORM_SCAN           (   9 )        /*   PF 9   */
#define  FORM_TEN            (  10 )        /*   PF10   */
#define  FORM_ELEVEN         (  11 )        /*   PF11   */
#define  FORM_HOMEBASE       (  12 )        /*   PF12   */
#define  FORM_ABANDON        (  12 )        /*   PF12   */
#define  FORM_ENTER          (  13 )        /*   Enter  */
#define  FORM_CLEAR          (  14 )        /*   Clear  */
#define  FORM_ESCAPE         (  15 )        /*   PA 1   */
#define  FORM_EDIT_HELP      (  16 )        /*   PA 2   */
#define  FORM_EDIT_FORM      (  16 )        /*   PA 2   */
#define  FORM_TOP            (  17 )        /*   pf16   */
#define  FORM_BOTTOM         (  18 )        /*   pf17   */
#define  FORM_HOME           (  19 )        /*   pf18   */
#define  FORM_HPREV          (  20 )        /*   pf19   */
#define  FORM_HNEXT          (  21 )        /*   pf20   */
#define  FORM_LEFT           (  22 )        /*   pf22   */
#define  FORM_RIGHT          (  23 )        /*   pf24   */

#define  FORM_NOT_FORMED     (  -1 )        /*          */


#endif
