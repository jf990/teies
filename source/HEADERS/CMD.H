/************************************************************************
*|                                                                     |*
*|  :NAME.         cmd.h                                               |*
*|                                                                     |*
*|  :LEVEL.        Transaction Management.                             |*
*|                                                                     |*
*|  :DATE.         March 7, 1987.                                      |*
*|                                                                     |*
*|  :AUTHOR.       Anoop K. Srivastava                                 |*
*|                                                                     |*
*|  :GLOBAL.       struct cmd          a command node                  |*
*|                                                                     |*
*|                                                                     |*
*|  :OUTPUT.       None.                                               |*
*|                                                                     |*
*|  :PURPOSE.      To create structured node for a Command to          |*
*|                 fascilitate storing it in a B-tree structure.       |*
*|                                                                     |
*|  :CALLS.        See below.                                          |*
*|                                                                     |*
*|  :ERRORS.       None.                                               |*
*|                                                                     |*
*|  :LIMITS.       None.                                               |*
*|                                                                     |*
*|  :MODS.         None.                                               |*
*|                                                                     |*
*|  :END.                                                              |*
*|                                                                     |*
************************************************************************/

extern struct cmd                      /*   The cmd node contains :    */
       {                               /*                              */
              hword  typeid[5];      /*      typeid field,           */
              hword  objcode[5];     /*      object code field and   */
              sint        (*fn)();     /*     a pointer to desired     */
       }  ;                            /*      function.               */

#define  S_CHAR    1
#define  ACTION    2
#define  OBJECT    3
#define  MODIFIER  4
#define  S_WORD    5
#define  SYS       6
#define  QUIT      2
