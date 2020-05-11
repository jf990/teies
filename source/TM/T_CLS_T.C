/*+--------------------------------------------------------------------+
  |                                                                    |
  |      :NAME.         t_cls_t.c                                      |
  |                                                                    |
  |      :AUTHOR.       Anoop K. Srivastava                            |
  |                                                                    |
  |      :DATE.         January 22, 1988.                              |
  |                                                                    |
  |      :LEVEL.        TM                                             |
  |                                                                    |
  |      :GLOBAL.       None.                                          |
  |                                                                    |
  |      :PARAM.        server_id   string  INPUT  Server to close.    |
  |                                                                    |
  |      :PARAM.        flag        sint    INPUT  Determine if we need|
  |                                                to close ISCF.      |
  |                                                                    |
  |      :PURPOSE.      To remove a record from the table of           |
  |                     active TM level connections. First the given   |
  |                     server_id is searched in the conn table and    |
  |                     if a match is found then the record containing |
  |                     it is removed. Also, the connection to the     |
  |                     server is closed.                              |
  |                                                                    |
  |                                                                    |
  |      :OUTPUT.       returns 0 if the operation is successful,      |
  |                     returns -1 if the server_id not found.         |
  |                                                                    |
  |      :CALLS.        t_conn(), s_comp(), os_memd(), isf_cls().      |
  |                                                                    |
  |      :ERRORS.       see output.                                    |
  |                                                                    |
  |      :LIMITS.       None known.                                    |
  |                                                                    |
  |      :END.                                                         |
  |                                                                    |
  +--------------------------------------------------------------------*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "tm.h"

extern sint isf_close();




sint t_cls_t( server_id, close_iscf_connection )

string        server_id;
sint          close_iscf_connection;

{

    sint           rc        = 0;
    string         server    = S_NULL;
    ISCF_TAB      *t_pointer = NIL;



    t_pointer = t_conn( server_id );
    s_copy( & server, server_id, 0, 0 );

    if ( ! ISF_TAB_POINTER_OK( t_pointer ) )
    {

         rc = USER_NOT_SIGNED_ON;

    }
    else
    {

         if (t_pointer->prev == NIL )
         {

              if ( t_pointer->next == NIL )
              {

                   TM_master->conn_tab = NIL;

              }
              else
              {

                   TM_master->conn_tab   = t_pointer->next;
                   t_pointer->next->prev = NIL;

              }
         }
         else if (t_pointer->next == NIL)
         {

              t_pointer->prev->next = NIL ;

         }

         else
         {

              t_pointer->prev->next = t_pointer->next;
              t_pointer->next->prev = t_pointer->prev;

         }
         s_drop( & (t_pointer->serv_name  ) );
         s_drop( & (t_pointer->acc_method ) );
         s_drop( & (t_pointer->user_name  ) );
         os_memd(   t_pointer               );
         rc = 0;

    }
    if( close_iscf_connection )
    {

       rc = isf_close( server, 0  );
       if( rc != 0 )
       {


       }
    }
    s_drop( & server );


   return(rc);
}

