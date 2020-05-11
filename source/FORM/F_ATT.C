/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_attach                                               |
  |  <AUTHOR>  Nestor R. Voronka, John L. Foster                      |
  |  <DATE>    July 20, 1988                                          |
  |  <LEVEL>   FORMS.FFUN                                             |
  |  <PARAM>   f_prof  struct f_prof_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains default & parsed  |
  |                                        qualifier values.          |
  |  <PARAM>   f_fmat  struct f_fmat_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains info required to  |
  |                                        render data properly in    |
  |                                        the GKS world.             |
  |  <PURPOSE> To process the <attach> markup                         |
  |            <attach name=* reference="Object.Component">           |
  |                                                                   |
  |              to attach a component of the object Form() was called|
  |              with. If the component is an Index segment, use the  |
  |              contents.  If it is an OID, open the object and as-  |
  |              sume it will be a CONTENTS type object.  Otherwise   |
  |              we cannot process the component as an attachment.    |
  |                                                                   |
  |            <attach name="GFD" reference=*>                        |
  |                                                                   |
  |               to attach a local operating system file.            |
  |                                                                   |
  |            <attach name="ItemId" Reference="OCD">                 |
  |                                                                   |
  |               to resolve an item id to an object and attach the   |
  |               object component.                                   |
  |                                                                   |
  |            NOTE: where a * appears that qualifier can also be     |
  |                  left out of the markup.                          |
  |  <OUTPUT>  sint.             Updates the form object control      |
  |            block and form object component descriptor stacks.     |
  |  <CALLS>   obj_opn(), s_comp(), s_init(), s_drop(), s_copy()      |
  |            s_len(), stk_psh(), stk_pop()                          |
  |  <LIMITS>  Right now we have no way of closing an object we open  |
  |            in this routine.  Assume it will be closed by the      |
  |            parser when it is going to pop this OCB off the stack. |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "form_i.h"
#include "stack.h"




sint   f_att (  f_prof,  f_fmat  )

struct f_prof_s * f_prof;
struct f_fmat_s * f_fmat;


{

   OCB   * ocb           =    NIL;
   OCD   * ocd           =    NIL;
   ODT   * odt_entry     =    NIL;
   string  wild_card     = S_NULL;
   sint    rc            =      0;
   OID     contents_oid          ;
   boolean opened_object =  FALSE;






   s_init ( & wild_card, "*" );
                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;

                                     /*+------------------------------+
                                       | If no object is specified    |
                                       | use the one form() was called|
                                       | with.                        |
                                       +------------------------------+*/
   if (
       ( s_len ( f_prof->att_name ) < 1 )                            ||
       ( s_comp( f_prof->att_name, wild_card, 0, 0, 'M', &rc) == 0)
      )
   {

      ocb = f_fmat->ocb;
      ocd = ocd_new ( & ocd, NIL );
      s_copy ( & (ocd->name), f_prof->att_ref, 0, 0 );
      odt_entry = odt_find( ocb, ocd );
      if( odt_type( odt_entry ) == OC_OID )
      {

         rc = obj_get( ocb, ocd, & contents_oid );
         if( rc == 0  &&  ( ! OID_IS_NULL( & contents_oid ) )  )
         {

            ocb = obj_opn( & contents_oid, OBJ_READ | OBJ_REMOTE );
            opened_object = TRUE;
            s_init ( & (ocd->name), "CONTENT.CONTENTS" );

         }
         else
         {

            rc = INVALID_OID;

         }
      }
      else if ( odt_type( odt_entry ) != OC_NDX )
      {

          ocb = NIL;
          rc = OCD_NOT_OF_PROPER_TYPE;

      }
   }
   else
   {

      ocb = obj_opn( (OID *) f_prof->att_name, OBJ_READ | OS_FILE );
      opened_object = TRUE;

   }
   if ( OCB_POINTER_OK( ocb ) )
   {

      ocb->status = 1;
      f_parse (  ocb, ocd  );
      if ( obj_test_mode( ocb, OS_FILE )  ||  opened_object )
      {

         rc = obj_cls( ocb );

      }
   }
   s_drop( & wild_card );
   if( OCD_POINTER_OK( ocd ) )
   {

      ocd_drp( & ocd );

   }

   return(0);
}
