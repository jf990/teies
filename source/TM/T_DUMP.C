/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  :NAME.    T_dump                                                 |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 13, 1988                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Display all TM level structures, including:            |
  |                                                                   |
  |                User Control Block                                 |
  |                Process Table                                      |
  |                Connection Table                                   |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"


sint     t_dump(  )



{



              tmentry                  *process    = NIL;
              struct _ISCF_TAB         *connection = NIL;
              sint                      rc         =   0,
                                        i          =   0;





 if( TM_master == NIL )
 {

     rc = MASTER_PROCESS_IS_NOT_ACTIVE;

 }
 else
 {

    if( TM_master->sys_init  !=  NIL  )
    {


    }
    else
    {


    }

    if(  TM_master->first  ==  NIL  )
    {


    }
    else
    {


       process = TM_master->first;
       i       =                0;

       while( process != NIL )
       {

          i ++ ;


          process = process->next;

       }
    }

    if( TM_master->conn_tab != NIL )
    {


       connection = TM_master->conn_tab;
       i          =                   0;

       while( connection != NIL )
       {

          i ++ ;


          connection = connection->next;

       }
    }
    else
    {


    }
 }


   return(rc);


}
