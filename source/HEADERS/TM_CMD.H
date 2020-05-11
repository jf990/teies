/************************************************************************
*|  <DOC>                                                              |*
*|  :NAME.         tm_cmd header                                       |*
*|  :LEVEL.        TM                                                  |*
*|  :DATE.         March 7, 1987.                                      |*
*|  :AUTHOR.       Anoop K. Srivastava                                 |*
*|  :GLOBAL.       None.                                               |*
*|  :OUTPUT.       None.                                               |*
*|  :PURPOSE.      To create structured node for a Command to          |*
*|                 fascilitate storing it in a B-tree structure.       |*
*|                                                                     |
*|  :CALLS.        See below.                                          |*
*|  :ERRORS.       None.                                               |*
*|  :LIMITS.       Constants defined here must align with TEIES CMDDATA|*
*|  :MODS.         None.                                               |*
*|  </DOC>                                                             |*
************************************************************************/

extern struct _tm_cmd                  /*   The cmd node contains :    */
       {                               /*                              */
              hword  typeid [ 5 ];   /*      typeid field,           */
              hword  objcode[ 5 ];   /*      object code field and   */
              sint   ( * fn )  (  );   /*     a pointer to desired     */
       }  ;                            /*      function.               */

typedef struct _tm_cmd COMMAND ;


#define  SINGLE_WORD_COMMAND         1
#define  COMMAND_ACTION              2
#define  COMMAND_OBJECT              3
#define  COMMAND_MODIFIER            4
#define  COMMAND_QUALIFIER           5
#define  SYSTEM_COMMAND              6

                        /*+------------------------+
                          |  single word commands  |
                          +------------------------+*/

#define  COMMAND_CANCEL              1
#define  COMMAND_QUIT                2
#define  COMMAND_PROCESS             3
#define  COMMAND_HOMEBASE            4
