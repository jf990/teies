/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_gtinfo()                                           |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INOUT  Pointer to Conference    |
  |                                          object control block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_CR()     |
  |                                                                   |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |                                                                   |
  |  <PARAM>   cr_params   struct *   INOUT  defined in obj_prm1.h    |
  |                                          Only the "control" field |
  |                                          of this structure is in  |
  |                                          use here                 |
  |                                          (i.e. cr_params->control)|
  |                                          which is type "sint"     |
  |                                                                   |
  |            "cr_params->control" is used to maintain the user      |
  |            interaction history, how a user got to a particular    |
  |            screen in the conference create process, what will     |
  |            be the next screen, and what screen he/she will reach  |
  |            if a QUIT is requested etc. It is initialized to a     |
  |            a certain value depending upon the user's responses    |
  |            on the "3140 form" to the following questions. Called  |
  |           up by CFR_GTINFO(). Each one of these is a seprate TEIES|
  |            screen or form.                                        |
  |                                                                   |
  |            ALL_INFORMATION         (ALL)    3140 form  crf_gtinfo |
  |            REQUIRED_INFORMATION    (REQ)    3141 form  cfr_gtreq  |
  |            PUBLIC_INFORMATION      (PUB)    3142 form  cfr_gtpub  |
  |            DESCRIPTIVE_INFORMATION (DES)    s_pad      cfr_gtdes  |
  |            MEMBERSHIP_INFORMATION  (MEM)    3190 form  cr_memb()  |
  |                                                                   |
  |            if ALL is "Y", do not care about REQ, PUB, DES, & MEM  |
  |            because ALL means going through all the screens.       |
  |                                                                   |
  |            Providing REQ is mendatory. Any combination of REQ     |
  |            with the rest three ( PUB, DES, MEM ) is allowed.      |
  |                                                                   |
  |            Follwing will help explain the possible combinations   |
  |            and initialization of cr_params->control               |
  |                                                                   |
  |            ALL  REQ  PUB  DES  MEM     CR_PARAMS->CONTROL         |
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
  |  <PURPOSE> Create Conference Root. This routine calls up the      |
  |            first form (3140 form) of this process. Does the input |
  |            validation. Initializes cr_params->control depending   |
  |            upon user responses as explained above. Once user      |
  |            is done with REQ(cfr_gtreq), PUB(cfr_gtpub) or         |
  |            DES(crf_gtdes ) or MEM( cr_memb() ), he gets back      |
  |            to this same routine, and the user's request to        |
  |            "review" or "establish" is validated.                  |
  |                                                                   |
  |  <OUTPUT>  retirns 2  if everthing OK, so that CFR_GTRE() can be  |
  |                                                        called.    |
  |            returns -1 if abandon is requested                     |
  |            returns  0 if quit is requested                        |
  |                                                                   |
  |  <CALLS>   s_init(), isv_get(), s_comp(), t_valyn(), t_form(),    |
  |            f_rstfld().                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_cr.h"



sint cfr_gtinfo( conf_ocb, modifier, fld_done, fld_todo )

OCB      *conf_ocb;
string   modifier;
hword    fld_done;
hword   *fld_todo;

{


sint     rc             =  0,
         junk           =  0,
         var_yn         =  0;
sint     rc_form        =  0;

boolean  show_form      = TRUE;

string   ALL_INF        =  S_NULL,      /* Define fields that appear on*/
         ALL_DONE       =  S_NULL,
         REQ_INF        =  S_NULL,      /* form "3140"                 */
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

#D_start('cfr_gtinfo','OBJ_CR','Stuff done 0x%x','fld_done')

s_init( & ALL_INF,          "ALL_INF"         );
s_init( & ALL_DONE,         "ALL_DONE"        );
s_init( & REQ_INF,          "REQ_INF"         );
s_init( & REQ_DONE,         "REQ_DONE"        );
s_init( & PUB_INF,          "PUB_INF"         );
s_init( & PUB_DONE,         "PUB_DONE"        );
s_init( & DESC_INF,         "DESC_INF"        );
s_init( & DESC_DONE,        "DESC_DONE"       );
s_init( & ASSIGN_INF,       "ASSIGN_INF"      );
s_init( & ASSIGN_DONE,      "ASSIGN_DONE"     );
s_init( & REVIEW_INPUTS,    "REVIEW_INPUTS"   );
s_init( & OK_TO_ESTAB,      "OK_TO_ESTAB"     );

s_init(  & form_id,  "3140"      );
isv_get( & done,     ISV_YES,  0 );
isv_get( & not_done, ISV_NO,   0 );
#D_str(   'done', 'Should be YES');
#D_str('not_done','Should be NO')


if(    (fld_done & REQUIRED_INFORMATION)
    && (fld_done & PUBLIC_INFORMATION)
    && (fld_done & DESCRIPTION_INFORMATION)
    && (fld_done & ASSIGNMENT_INFORMATION)    )
{                                      /* If ALL information is already*/
                                       /* processed                    */
   #D_say('ALL information processed. turn done fields YES')
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

   if( fld_done & REQUIRED_INFORMATION )
   {

      #D_say('Required information processed. turn done fields YES')
      var_set( REQ_DONE  , done );
      var_set( REQ_INF   , not_done );
      f_setfld( form_id, REQ_DONE   , done );
      f_setfld( form_id, REQ_INF   , not_done );


   }

   if( fld_done & PUBLIC_INFORMATION )
   {

      #D_say('Public information processed. turn done fields YES')
      var_set( PUB_DONE  , done );
      var_set( PUB_INF   , not_done );
      f_setfld( form_id, PUB_DONE   , done );
      f_setfld( form_id, PUB_INF   , not_done );

   }

   if( fld_done & DESCRIPTION_INFORMATION )
   {

      #D_say('Description information processed. turn done fields YES')
      var_set( DESC_DONE , done );
      var_set( DESC_INF  , not_done );
      f_setfld( form_id, DESC_DONE  , done );
      f_setfld( form_id, DESC_INF  , not_done );


   }

   if( fld_done & ASSIGNMENT_INFORMATION )
   {

      #D_say('Assignment information processed. turn done fields YES')
      var_set( ASSIGN_DONE   , done );
      var_set( ASSIGN_INF, not_done );
      f_setfld( form_id, ASSIGN_DONE, done );
      f_setfld( form_id, ASSIGN_INF, not_done );

   }

}  /* If All Information was set */

if( fld_done & REQUIRED_INFORMATION )  /* If done then put cursor on   */
{                                      /* OK to establish field. Also  */
                                       /* we should turn it to YES     */
   f_curpos( form_id, NO_FLAGS, OK_TO_ESTAB, 0.0, 0.0 );

}

while( show_form && (rc == NO_ERR) )
{
   *fld_todo = 0;

   rc_form = t_form( form_id, S_NULL, NULL );
   #D_show('RC = %d from t_form()','rc_form');

   switch(rc_form)
   {

   case FORM_NOT_FORMED:
   case USER_REQUESTED_ABANDON:
   case USER_REQUESTED_QUIT:

      rc = rc_form;

   break;

   default:

      var_value = var_get( ALL_INF );
      var_yn    = t_valyn( var_value );

      if( var_yn == TRUE )        /* If field is YES then do NOT bother*/
      {                           /* looking at other fields.          */

         *fld_todo |= ALL_INFORMATION;

      }
      else
      if( var_yn == UNEXPECTED_RESPONSE )
      {

         except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, ALL_INF, 0.0, 0.0 );

         continue;
      }
      else
      {                          /* Now validate rest of the fields in */
                                 /* if un-expected field input, then   */
                                 /* return to the form and put cursor  */
                                 /* at the position of the error       */

         var_value = var_get( REQ_INF );
         var_yn    = t_valyn( var_value );

         if( var_yn == TRUE )     /* If field is YES then turn bits on */
         {

            *fld_todo |= REQUIRED_INFORMATION;

         }
         else
         if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, REQ_INF, 0.0, 0.0 );

            continue;
         }

         var_value = var_get( PUB_INF );
         var_yn    = t_valyn( var_value );

         if( var_yn == TRUE )     /* If field is YES then turn bits on */
         {

            *fld_todo |= PUBLIC_INFORMATION;

         }
         else
         if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, PUB_INF, 0.0, 0.0 );

            continue;
         }

         var_value = var_get( DESC_INF );
         var_yn    = t_valyn( var_value );

         if( var_yn == TRUE )     /* If field is YES then turn bits on */
         {

            *fld_todo |= DESCRIPTION_INFORMATION;

         }
         else
         if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, DESC_INF, 0.0, 0.0 );

            continue;
         }

         var_value = var_get( ASSIGN_INF );
         var_yn    = t_valyn( var_value );

         if( var_yn == TRUE )     /* If field is YES then turn bits on */
         {

            *fld_todo |= ASSIGNMENT_INFORMATION;

         }
         else
         if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, ASSIGN_INF, 0.0, 0.0 );

            continue;
         }

      }  /* If ALL_INF was true */

                                 /* And while we are at it, lets make  */
                                 /* sure other fields are correct too  */

      var_value = var_get( REVIEW_INPUTS );
      var_yn    = t_valyn( var_value );

      if( var_yn == TRUE )     /* If field is YES                   */
      {
#if 0
         /* Don't know what code should go here.  */
         /* We should display all the things which user should will */
         /* via directory view choice   */
#endif
      }
      else
      if( var_yn == UNEXPECTED_RESPONSE )
      {

         except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                     S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, REVIEW_INPUTS, 0.0, 0.0 );

         continue;
      }

      var_value = var_get( OK_TO_ESTAB );
      var_yn    = t_valyn( var_value );

      if( var_yn == TRUE )     /* If field is YES then check if at  */
      {                        /* least REQUIRED info field was done*/

         if(    (*fld_todo & REQUIRED_INFORMATION)
             || (t_valyn(var_get( REQ_DONE )) == TRUE) )
         {

            *fld_todo |= OK_TO_CREATE_CONFERENCE;

         }
         else                    /* If NOT, then let the user know  */
         {                       /* he has to say YES and put cursor*/
                                 /* on required information field   */
            except( REQUIRED_INFO_NEEDED, USER_ERROR,
                    S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

            f_curpos( form_id, NO_FLAGS, REQ_INF, 0.0, 0.0 );

         }

      }
      else
      if( var_yn == UNEXPECTED_RESPONSE )
      {

         except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                     S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, OK_TO_ESTAB, 0.0, 0.0 );

         continue;
      }

   break;

   }  /* switch(rc_form) */

   show_form = FALSE;

}  /* while */


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

#D_leave( rc, 'DONE WITH CR_GTINFO (get info)' );

}
