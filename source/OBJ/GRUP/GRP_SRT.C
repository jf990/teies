/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_srt()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman,  Tanmay S. Kumar  |
  |                                                                   |
  |  :DATE.    June 23, 1988                         August 15, 1989  |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *      INOUT  Pointer to Group         |
  |                                          object control block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_GRUP()   |
  |                                                                   |
  |  :PARAM.   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  :PARAM.   level       hword      INPUT  has the done state list  |
  |                                                                   |
  |  :PARAM.   to_do_list  hword  *   INOUT  has the to_do state list |
  |                                                                   |
  |            to_do_list           is used to maintain the user      |
  |            interaction history, how a user got to a particular    |
  |            screen in the group      create process, what will     |
  |            be the next screen, and what screen he/she will reach  |
  |            if a QUIT is requested etc. It is initialized to a     |
  |            a certain value depending upon the user's responses    |
  |            on the "3120 form" to the following questions.         |
  |                                                                   |
  |            ALL_INFORMATION         (ALL)    3140 form  grp_srt()  |
  |            REQUIRED_INFORMATION    (REQ)    3141 form  grp_reg()  |
  |            PUBLIC_INFORMATION      (PUB)    3142 form  grp_pub()  |
  |            DESCRIPTIVE_INFORMATION (DES)    s_pad      grp_des()  |
  |            MEMBERSHIP_INFORMATION  (MEM)    3190 form  cr_memb()  |
  |                                                                   |
  |            if ALL is "Y", do not care about REQ, PUB, DES, & MEM  |
  |            because ALL means going through all the screens.       |
  |                                                                   |
  |            Providing REQ is mendatory. Any combination of REQ     |
  |            with the rest three ( PUB, DES, MEM ) is allowed.      |
  |                                                                   |
  |            Follwing will help explain the possible combinations   |
  |            and initialization of to_do_list and done list         |
  |                                                                   |
  |            ALL  REQ  PUB  DES  MEM                                |
  |                                                                   |
  |            1     d    d    d    d      16   (d = don't care)      |
  |            0     1    0    0    0       8   (1 = Y)               |
  |            0     1    0    0    1       9   (0 = N)               |
  |            0     1    0    1    0      10                         |
  |            0     1    0    1    1      11                         |
  |            0     1    1    0    0      12                         |
  |            0     1    1    0    1      13                         |
  |            0     1    1    1    0      14                         |
  |            0     1    1    1    1      15--this combination is    |
  |                                            same as ALL = Y        |
  |                                                                   |
  |  :PURPOSE. Create a GROUP.         This routine calls up the      |
  |            first form (3140 form) of this process. Does the input |
  |            validation.                                            |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
  |                                                                   |
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
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_group.h"



sint grp_srt( group_ocb, modifier, done_list, to_do_list )

OCB      *group_ocb;
string   modifier;
hword    done_list;
hword   *to_do_list;

{


sint     rc             =  0,
         junk           =  0,
         var_yn         =  0;

boolean  show_form      = TRUE;

string   ALL_INF        =  S_NULL,      /* Define fields that appear on*/
         ALL_DONE       =  S_NULL,
         REQ_INF        =  S_NULL,      /* form "3120"                 */
         REQ_DONE       =  S_NULL,
         PUB_INF        =  S_NULL,
         PUB_DONE       =  S_NULL,
         DESC_INF       =  S_NULL,
         DESC_DONE      =  S_NULL,
         ASSIGN_INF     =  S_NULL,
         ASSIGN_DONE    =  S_NULL,
         REVIEW_INPUTS  =  S_NULL,
         OK_TO_ESTAB    =  S_NULL,

         done           =  S_NULL,
         not_done       =  S_NULL,
         var_value      =  S_NULL,

         form_id        =  S_NULL;


s_init( &ALL_INF,          "ALL_INF"         );
s_init( &ALL_DONE,         "ALL_DONE"        );
s_init( &REQ_INF,          "REQ_INF"         );
s_init( &REQ_DONE,         "REQ_DONE"        );
s_init( &PUB_INF,          "PUB_INF"         );
s_init( &PUB_DONE,         "PUB_DONE"        );
s_init( &DESC_INF,         "DESC_INF"        );
s_init( &DESC_DONE,        "DESC_DONE"       );
s_init( &ASSIGN_INF,       "ASSIGN_INF"      );
s_init( &ASSIGN_DONE,      "ASSIGN_DONE"     );
s_init( &REVIEW_INPUTS,    "REVIEW_INPUTS"   );
s_init( &OK_TO_ESTAB,      "OK_TO_ESTAB"     );

s_init( &form_id,          "3120"            );

isv_get( &done,     ISV_YES,  0 );
isv_get( &not_done, ISV_NO,   0 );


if(    (done_list & REQUIRED_INFORMATION)
    && (done_list & PUBLIC_INFORMATION)
    && (done_list & DESCRIPTION_INFORMATION)
    && (done_list & ASSIGNMENT_INFORMATION)    )
{                                      /* If ALL information is already*/
                                       /* processed                    */
   var_set( ALL_DONE   , done );       /* Then turn done field to YES  */
   var_set( REQ_DONE   , done );
   var_set( PUB_DONE   , done );
   var_set( DESC_DONE  , done );
   var_set( ASSIGN_DONE, done );

   f_setfld( form_id, ALL_DONE   , done );  /* Tell form of new values */
   f_setfld( form_id, REQ_DONE   , done );
   f_setfld( form_id, PUB_DONE   , done );
   f_setfld( form_id, DESC_DONE  , done );
   f_setfld( form_id, ASSIGN_DONE, done );

   var_set( ALL_INF   , not_done );     /* And other fields to NO      */
   var_set( REQ_INF   , not_done );
   var_set( PUB_INF   , not_done );
   var_set( DESC_INF  , not_done );
   var_set( ASSIGN_INF, not_done );

   f_setfld( form_id, ALL_INF   , not_done );
   f_setfld( form_id, REQ_INF   , not_done );
   f_setfld( form_id, PUB_INF   , not_done );
   f_setfld( form_id, DESC_INF  , not_done );
   f_setfld( form_id, ASSIGN_INF, not_done );

}
else
{

   if( done_list & REQUIRED_INFORMATION )
   {

      var_set( REQ_DONE  , done );
      var_set( REQ_INF   , not_done );
      f_setfld( form_id, REQ_DONE   , done );
      f_setfld( form_id, REQ_INF   , not_done );


   }

   if( done_list & PUBLIC_INFORMATION )
   {

      var_set( PUB_DONE  , done );
      var_set( PUB_INF   , not_done );
      f_setfld( form_id, PUB_DONE   , done );
      f_setfld( form_id, PUB_INF   , not_done );

   }

   if( done_list & DESCRIPTION_INFORMATION )
   {

      var_set( DESC_DONE , done );
      var_set( DESC_INF  , not_done );
      f_setfld( form_id, DESC_DONE  , done );
      f_setfld( form_id, DESC_INF  , not_done );


   }

   if( done_list & ASSIGNMENT_INFORMATION )
   {

      var_set( ASSIGN_DONE   , done );
      var_set( ASSIGN_INF, not_done );
      f_setfld( form_id, ASSIGN_DONE, done );
      f_setfld( form_id, ASSIGN_INF, not_done );

   }

}  /* If All Information was set */

if( done_list & REQUIRED_INFORMATION )  /* If done then put cursor on   */
{                                      /* OK to establish field. Also  */
                                       /* we should turn it to YES     */
   f_curpos( form_id, NO_FLAGS, OK_TO_ESTAB, 0.0, 0.0 );

}

while( show_form && (rc == NO_ERR) )
{
   *to_do_list = 0;

   rc = t_form( form_id, S_NULL, NULL );

   if ( ( rc == USER_REQUESTED_ENTER )
       || ( rc == USER_REQUESTED_PROCESS ) )
   {
      rc = 0;
      if ( (t_valyn( var_get(ALL_INF))) == -1 )
      {
         except( INPUT_EXCEPTION, USER_ERROR,
                      var_get(ALL_INF), S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, ALL_INF, 0.0, 0.0 );
         continue;
      }
      else if ( (t_valyn( var_get(ALL_INF))) == 1 )
      {
         *to_do_list |= ALL_INFORMATION;
      }
      else   /* ALL_INF field had a No in it */
      {
         if ( (t_valyn( var_get(REQ_INF))) == -1 )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, var_get(REQ_INF),
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, REQ_INF, 0.0, 0.0 );
            continue;
         }
         else if ( (t_valyn( var_get(REQ_INF))) == 1 )
         {
            *to_do_list |= REQUIRED_INFORMATION;
         }
         else ;

         if ( (t_valyn( var_get(PUB_INF))) == -1 )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, var_get(PUB_INF),
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, PUB_INF, 0.0, 0.0 );
            continue;
         }
         else if ( (t_valyn( var_get(PUB_INF))) == 1 )
         {
            *to_do_list |= PUBLIC_INFORMATION;
         }
         else ;

         if ( (t_valyn( var_get(DESC_INF))) == -1 )
         {
            except(Yes_Or_No_Was_Expected, USER_ERROR, var_get(DESC_INF),
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, DESC_INF, 0.0, 0.0 );
            continue;
         }
         else if ( (t_valyn( var_get(DESC_INF))) == 1 )
         {
            *to_do_list |= DESCRIPTION_INFORMATION;
         }
         else ;

         if ( (t_valyn( var_get(ASSIGN_INF))) == -1 )
         {
            except(Yes_Or_No_Was_Expected, USER_ERROR,
                   var_get(ASSIGN_INF), S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, ASSIGN_INF, 0.0, 0.0 );
            continue;
         }
         else if ( (t_valyn( var_get(ASSIGN_INF))) == 1 )
         {
            *to_do_list |= ASSIGNMENT_INFORMATION;
         }
         else;

         if ( (t_valyn( var_get(REVIEW_INPUTS))) == -1 )
         {
            except(Yes_Or_No_Was_Expected, USER_ERROR,
                var_get(REVIEW_INPUTS), S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, REVIEW_INPUTS, 0.0, 0.0 );
            continue;
         }
         if ( (t_valyn( var_get(OK_TO_ESTAB))) == -1 )
         {
            except(Yes_Or_No_Was_Expected, USER_ERROR,
                var_get(OK_TO_ESTAB), S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, OK_TO_ESTAB, 0.0, 0.0 );
            continue;
         }
         else if ( (t_valyn( var_get(OK_TO_ESTAB))) == 1 )
         {

            if(    (*to_do_list & REQUIRED_INFORMATION)
                || (t_valyn(var_get( REQ_DONE )) == TRUE) )
            {

               *to_do_list |= OK_TO_CREATE_GROUP;

            }
         }
         else ;


      }  /* else No typed in all info */
   }
   else   /* invalid pf key typed */
   {
       continue;
   }
   show_form = FALSE;

}  /* while */
f_close(form_id, NO_FLAGS);    /* MAY NEED TO TAKE THIS OUT */


s_drop( &ALL_INF       );
s_drop( &ALL_DONE      );
s_drop( &REQ_INF       );
s_drop( &REQ_DONE      );
s_drop( &PUB_INF       );
s_drop( &PUB_DONE      );
s_drop( &DESC_INF      );
s_drop( &DESC_DONE     );
s_drop( &ASSIGN_INF    );
s_drop( &ASSIGN_DONE   );
s_drop( &REVIEW_INPUTS );
s_drop( &OK_TO_ESTAB   );
s_drop( &form_id       );

   return(rc);
}
