/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_gtpub()                                            |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INOUT  Pointer to Conference    |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_CR()     |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   cr_params   struct *   INOUT  defined in obj_prm1.h.   |
  |                                          Only the "control" field |
  |                                          of this structure is in  |
  |                                          use here                 |
  |                                          (i.e. cr_params->control)|
  |                                                                   |
  |  <PURPOSE> Create Conference Root. This routine handles the       |
  |            adding of PUBLIC components of the conference, namely  |
  |            TITLE and KEYS.                                        |
  |            It calls up the "3142 form", validates the user inputs,|
  |            implements function keys like QUIT, ABANDON, etc.,     |
  |            and returns a proper code depending upon the           |
  |            value of cr_params->control so that the next routine   |
  |            or form could be called in the process.                |
  |                                                                   |
  |  <OUTPUT>  returns  2  if QUIT requested, so call CFR_GTREQ()     |
  |            returns -1  if ABANDON is requested                    |
  |            returns  1  if "control" is 12, so call CFR_INFO()     |
  |            returns  5  if "control" is 13 so that t_jmpst() called|
  |                                           which calls cr_memb()   |
  |                                                                   |
  |  :CALLS.   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "form.h"
#include "obj_cr.h"


sint cfr_gtpub( conf_ocb, modifier, level, params )

OCB     *conf_ocb;
string   modifier;
hword    level;
byte    *params;

{

string   form_id        =  S_NULL,
         tmp_str        =  S_NULL,

         CONF_TITLE     =  S_NULL,
         CONF_KEYS      =  S_NULL;

OCD      *conf_ocd      =  NULL;


sint     rc             =  NO_ERR,
         rc_form        =  0,
         junk           =  0;


s_init( &form_id,     "3142"    );

s_init( &CONF_TITLE,  "CONF_TITLE" );
s_init( &CONF_KEYS,   "CONF_KEYS"  );


rc_form = t_form( form_id, modifier, conf_ocb );

switch(rc_form)
{
case FORM_NOT_FORMED:
case USER_REQUESTED_ABANDON:
case USER_REQUESTED_QUIT:

   rc = rc_form;

break;

default:

   tmp_str = var_get( CONF_TITLE );
   if( s_len( tmp_str ) != 0 )
   {

      obj_update( conf_ocb, "CONFERENCE.TITLE", tmp_str );

   }

   tmp_str = var_get( CONF_KEYS );
   if( s_len( tmp_str ) != 0 )
   {

      t_ckkeys( &tmp_str, tmp_str );
      obj_update( conf_ocb, "CONFERENCE.KEYS", tmp_str );

   }

break;
}  /* switch of t_form RC */

s_drop( &form_id     );

s_drop( &CONF_TITLE  );
s_drop( &CONF_KEYS   );

   return(rc);

}
