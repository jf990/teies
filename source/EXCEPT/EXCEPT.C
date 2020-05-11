/************************************************************************
   <Doc>
   <Name>          EXCEPT()
   <DATE>          August 21, 1986
   <AUTHOR>        John L. Foster
   <PURPOSE>       Handle TEIES interface, control and data exceptions.

      > If the error code is 0, except() returns without doing anything.

      > If the error type indicates a fatal case and the error code is
        non-zero, after the information string is formatted a
        TEIES-stack traceback and a C-stack traceback are produced and
        exit() is called.

      > Currently, a call to with an error code in the range of 10000 to
        31999 will produce a log entry of the following format:

          EXCEPT(hh:mm:ss): code=X, type=Y, args={1}, {2}, {3}, {4}, {5}}

        which is just an echoing of the arguments with the time.  If the
        error code is in the user reportable range (1000-9999) *and* the
        code cannot be found in the SERRI then this message will be
        displayed as well as logged.

      > If the error code is in the range 1000 to 9999 the error code
        is looked up in the SERRI through a call to t_seterr() and a
        message is displayed on all active display devices with the
        following severity type class rules:

            USER_INFO : Message is displayed in the error window in
                        normal (low) intensity.

            USER_WARNING : Message is displayed in the error window
                        highlighted.

            USER_ERROR : Message is displayed in the error window high-
                        lighted and t_alarm() is called.

   <Global>        TM_master     INOUT
   <PARAM>         code   sint   INPUT   TEIES error code, must be less
                                         than 32000.  If less than 0 the
                                         absolute value is used.
   <PARAM>         type   sint   INPUT   Severity level.
   <PARAM>         arg1   string INPUT   string parameter 1
   <PARAM>         arg2   string INPUT   string parameter 2
   <PARAM>         arg3   string INPUT   string parameter 3
   <PARAM>         arg4   string INPUT   string parameter 4
   <PARAM>         arg5   string INPUT   string parameter 5

   <OUTPUT>        sint                  Returns the error code or if
                                         except experienced an error a
                                         new error code.

   <Calls>         t_seterr(), exit(), s_cat(), os_dttm()

   <MODS>  Aug_09_88     JF         Added t_seterr() interface.
   </doc>
************************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"




sint    except( code, type, arg1, arg2, arg3, arg4, arg5 )

sint            code, type;
string                      arg1, arg2, arg3, arg4, arg5 ;


{

string   except_msg = S_NULL,
         s          = S_NULL,
         ex_code    = S_NULL,
         ex_type    = S_NULL,
         message    = S_NULL,             /* conversion work strings */
         err_return = S_NULL,
         template   = S_NULL,
         err_message= S_NULL,
         delim      = S_NULL,
         spill      = S_NULL,
         exc        = S_NULL,
        *str_vec    = S_NULL;
sint     flag    =      0,
         rc      = ERROR_CODE_NOT_IN_SERRI, /* function return code    */
         arg_num =      0,
         i1      =      0;
byte     *pointer=    NIL,
         *str    =    NIL;
datetime *e_time =   NULL;



if( TM_master->flags & IN_EXCEPT )
{

   return(0);

}
else
{

   TM_master->flags |= IN_EXCEPT;

}
if( code != 0 )
{

if ( code < 0 )
{
   code = 0 - code;
}

if( code != OUT_OF_MEMORY )
{

   s_init(   & except_msg, "%EXC"   );


   if( TM_master->current->exc_msg != S_NULL )
   {

      s_drop( & (TM_master->current->exc_msg) );

   }
   if( TM_master->usrtype == REAL_USER  &&  type >= USER_INFO )
   {

      rc = t_seterr( code , &err_return );

   }
   if( s_len( err_return ) < 1 )
   {
                                                  /*********************/
                                                  /*                   */
     e_time =(datetime*)os_mema(sizeof(datetime));/* Start by getting  */
                                                  /*  the OSK time     */
     os_dttm( e_time );                           /*                   */
     message = TM_master->current->exc_msg;       /*                   */
                                                  /* Start string msg  */
     s_init(&s, "EXCEPT(@#2i:@#2m:@#2s): code="); /*                   */
     rc = s_dttos(  & message, e_time, s );       /*                   */
                                                  /* Now format the    */
                                                  /*  exception code   */
     s_sitos(&ex_code,code);                      /*                   */
     s_cat(&message,message,ex_code,0,0);         /*                   */
                                                  /*                   */
     s_init(  &s,      ", type="       );         /* Stick in type=??? */
     s_cat(   &message,message, s, 0, 0);         /*                   */
     s_titos( &ex_type,           type );         /*                   */
     s_cat(   &message,message, ex_type, 0, 0 );  /*                   */
                                                  /*                   */
                                                  /*                   */
     s_init(  &s,      ", args={"      );         /* Show arguments    */
     s_cat(   &message,message, s, 0, 0);         /*  arg1 ... arg5    */
     s_init(  &s,      "}, {"          );         /*********************/

     s_cat(   &message, message, arg1, 0, 0);
     s_cat(   &message, message, s,    0, 0);

     s_cat(   &message, message, arg2, 0, 0  );
     s_cat(   &message, message, s,    0, 0  );

     s_cat(   &message, message, arg3, 0, 0  );
     s_cat(   &message, message, s,    0, 0  );

     s_cat(   &message, message, arg4, 0, 0  );
     s_cat(   &message, message, s,    0, 0  );

     s_cat(   &message, message, arg5, 0, 0  );
     s_init(  &s,       "}"                  );
     s_cat(   &message, message, s,    0, 0  );
     var_set( except_msg, message );    /* Tweek */

   }
   else
   {

      s_init   ( & template,   "@n@@@~..@#n@n@@." );
      s_valloc ( & str_vec,    14 );
      s_parse  ( & err_return, template, delim, str_vec, & spill );
      for ( i1 = 1; i1 <= s_len ((string) str_vec); i1++ )
      {
         switch ( i1 )
         {
            case  1:
            case  3:
            case  5:
            case  9:
            case 11:
            case 13:
               s_cat ( &err_message, err_message, str_vec [i1],0,0 );
               break;
            case  2:
            case  4:
            case  6:
            case  8:
            case 10:
            case 12:
               s_stosi ( &arg_num, str_vec [i1], 0, 0 );
               switch ( arg_num )
               {
                  case 1:
                     s_cat ( &err_message, err_message, arg1, 0, 0 );
                     break;
                  case 2:
                     s_cat ( &err_message, err_message, arg2, 0, 0 );
                     break;
                  case 3:
                     s_cat ( &err_message, err_message, arg3, 0, 0 );
                     break;
                  case 4:
                     s_cat ( &err_message, err_message, arg4, 0, 0 );
                     break;
                  case 5:
                     s_cat ( &err_message, err_message, arg5, 0, 0 );
                     break;
                  default :
                     break;
               }
         }
      }
      message = err_message;
      var_set( except_msg, message );    /* Tweek */
      s_vdrop( & str_vec  );
      s_drop(  & template );
      s_drop(  & delim    );
      s_drop(  & spill    );

   }                                       /* Stuff values into the */
   TM_master->current->except   = code;    /* current TM process    */
   TM_master->current->exc_type = type;    /* structure for error   */
   TM_master->current->exc_msg  = message; /* code, error type, and */
                                           /* a ptr to the message. */
                                           /* Its now TM's          */
                                           /* responsibility.       */
                                           /*************************/



   /* At this point we should LOG the error message to the error log */

}


   if( type == FATAL_ERROR || type == RESET_PROCESS )
   {

       /* t_stop( process_id ); */

   }
   else if( type == USER_ERROR )
   {

      t_alarm( 0 );

   }
   if(  code == OUT_OF_MEMORY || type == FATAL_ERROR  )
   {

       t_shutdn();
   exit(code);

   }

   s_drop( & s          );
   s_drop( & ex_type    );
   s_drop( & ex_code    );
   s_drop( & except_msg );
   os_memd(  e_time     );

}


TM_master->flags &= ~ IN_EXCEPT;


   return(code);

}
