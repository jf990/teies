/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    form.c                                                 |
  |  <AUTHOR>  John L. Foster, Nestor Voronka                         |
  |  <DATE>    October 21, 1987                                       |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   form_id  string       INPUT   Form Identifier of the   |
  |                                          form's source.           |
  |  <PARAM>  *ob_ocb   struct OCB   INPUT   Pointer to the ocb of the|
  |                                          object that is linked to |
  |                                          the fields.              |
  |  <PARAM>   flags    hword        INPUT   Bit flags for forms      |
  |                                          control.                 |
  |  <PURPOSE> This is the entry point from the Object Level and TM   |
  |            to use the Forms Processor. First we determine if the  |
  |            form has been parsed and still can be found in the GKS |
  |            world.  If so, then the Presentation Manager is called |
  |            and input is requested until a non-resolvable command  |
  |            key is hit.  If the form has not been parsed, then it  |
  |            is located and opened, the forms profile is set up,    |
  |            the form is parsed and rendered in the GKS world, and  |
  |            the the Presentation Manager is called.                |
  |                                                                   |
  |  <OUTPUT>  A pointer to a string containing the command key that  |
  |            was hit to leave the PM or S_NULL if an error occured. |
  |  <CALLS>   f_resolv (), f_fbeg (), f_profil (), f_parse (),       |
  |            f_fend (), p_disp ()                                   |
  |  <ERRORS>  * none *                                               |
  |  <LIMITS>  This version does not handle forms to a printer or     |
  |            to a storage device.                                   |
  |  <MODS>    January 7, 1988   Nestor Voronka.  Restructuring of    |
  |            the Form Processor's higher levels of logic.           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "debug.h"
#include "form_i.h"


sint   form (  form_id, obj_ocb, flags  )

string         form_id  ;
OCB          * obj_ocb  ;
hword          flags    ;

{
   sint        com_key  =      0; /* PF value returned from the PM     */
   form_dat   *formdata =   NULL; /* Structure needed to display Form  */
   OCB        *form_ocb =   NULL; /* Form's Object Control Block       */
   OCD        *form_ocd =    NIL; /* Obj Comp Desc of the forms text   */
   sint        rc       =      0; /* Return code from other functions  */


                                  /* Check to see if form is in world  */
   formdata = (form_dat *)f_resolv (form_id, flags) ;
                                  /* If it is not then                 */
   if (formdata == NIL)
   {                              /* Find and open the form            */
      rc = f_fbeg  (form_id, flags, &form_ocb, &form_ocd) ;
      if (rc != 0)
      {
         return( FORM_NOT_FORMED );
      }
                                  /* Set up the forms profile          */
      formdata = (form_dat *) f_profil (
                             form_id, form_ocb, form_ocd, obj_ocb, flags
                                       );
                                  /* Parse the form                    */
      f_parse ( form_ocb, form_ocd );
                                  /* Discard the profile & update PM's */
      f_fend  ( form_id, formdata, form_ocb ) ;
      ocd_drp( &form_ocd );

   }

   if ( ! ( CHK_MASK (  flags, DO_NOT_DISPLAY  ) ) )
   {
                                 /* Call the Presentation Manager     */
      com_key = (sint) p_disp ( formdata, flags ) ;

   }

                                  /* Return Command Key from the PM    */
   return( com_key );

}
