/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_gtdes()                                            |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INOUT  Pointer to Conference    |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_CR()     |
  |                                                                   |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |                                                                   |
  |  <PARAM>   cr_params   struct *   INOUT  defined in obj_prm1.h.   |
  |                                          Only the "control" field |
  |                                          of this structure is in  |
  |                                          use here                 |
  |                                          (i.e. cr_params->control)|
  |                                                                   |
  |  <PURPOSE> Create Conference Root. This routine handles the       |
  |            adding of DESCRIPTION component to the conference,     |
  |            It calls up the "s_pad", implements TEIES function     |
  |            keys, and returns a proper code depending upon the     |
  |            value of cr_params->control so that the next routine   |
  |            or form could be called in the process.                |
  |                                                                   |
  |  <OUTPUT>  returns  2  if QUIT requested, and "control" = 10 or 11|
  |                        otherwise returns 3 on quit                |
  |            returns -1  if ABANDON is requested                    |
  |            returns  1  if "control" is 10 so that CFR_INFO called |
  |            returns  5  if "control" is 11 so that t_jmpst() called|
  |                                           which calls cr_memb()   |
  |            returns  1  if "control" is 14 so that CFR_INFO calld| |
  |                                     so that CFR_GTDES is called   |
  |            returns  5  if "control" is 15 so that t_jmpst() called|
  |                                           which calls cr_memb     |
  |                                                                   |
  |  <CALLS>   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "obj_cr.h"


sint cfr_gtdes( conf_ocb, modifier, level, params )

OCB     *conf_ocb;
string   modifier;
hword    level;
byte    *params;

{
string   sp_string      = S_NULL;

sint     rc             = NO_ERR,
         junk           = 0;
sint     end_function   = 0;

OCB     *sp_ocb         = NULL;

OCD     *conf_ocd       = NULL;



   end_function = t_spad( S_NULL, &sp_string );

   if ( end_function == USER_REQUESTED_ABANDON
      || end_function == USER_REQUESTED_QUIT )
   {

      rc = end_function;

   }
   else
   {

      sp_ocb = obj_opn( (OID *) sp_string, OBJ_READ | OS_FILE );
      if( OCB_POINTER_OK( sp_ocb ) )
      {

         ocd_new(  &conf_ocd, "CONFERENCE.DESCRIPTION" );
         rc = ocd_bse( conf_ocb, conf_ocd, sp_ocb );
         obj_cls( sp_ocb );
         t_delsp( sp_string );

      }  /* if pointer is good */

   }  /* if rc_form was not quit or abandon */

   s_drop( &sp_string );

   ocd_drp( &conf_ocd );


   return(rc);

}
