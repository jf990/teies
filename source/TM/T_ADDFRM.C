/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_addfrm.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 26, 1988                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   form_id     string    INPUT   Form ID to index.        |
  |                                                                   |
  |  :PURPOSE. A form has been requested by the forms processor but   |
  |            does not exist.  This routine will put up a form to    |
  |            ask the user what file contains the form that should be|
  |            indexed.  The new form is created in to a CC object,   |
  |            added to the System Forms Conference, added to the SFI |
  |            and ??.                                                |
  |                                                                   |
  |  :OUTPUT.  0 if form added into the SFI.                          |
  |            28009 : file requested cannot be located.              |
  |            28010 : insufficient privilege to add forms to TEIES.  |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  The MS database should be queried first for the form,  |
  |            if it exists there index it here and return.  Otherwise|
  |            ask for the new file.                                  |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"





sint   t_addfrm(  form_id  )

string            form_id;


{


string      file_id   = S_NULL,
            file_ft   = S_NULL,
            form_var  = S_NULL,
            import    = S_NULL,
            addform   = S_NULL,
            edit      = S_NULL,
            temp      = S_NULL,
            contents_ocd = S_NULL,
            file_var  = S_NULL;
OCB        *sfi       =    NIL,
           *cc        =    NIL,
           *conf_root =    NIL,
           *ocb       =    NIL;
OCD        *ocd       =    NIL;
sint        rc        =      0;
boolean     s_pad     =  FALSE;




   sfi = t_sysndx( SFI_OBJ );
   if( sfi == NIL )
   {

      except( SFI_MISSING_OR_INVALID, DATA_ERROR, form_id, S_NULL,
                                           S_NULL, S_NULL, S_NULL);

   }
   else
   {

      var_init(& form_var, "FORM_ID", (byte *)form_id, OC_STR );
      s_init(  & addform,  "ADDFORM"         );
      s_init(  & file_var, "FILE_ID"         );
      s_init(  & import,   "IMPORT"          );
      f_setfld( addform,   form_var, form_id );
      f_rstfld( addform,   NO_FLAGS, file_var);

      t_form( addform, NIL, NIL   );

      s_copy( &file_id, (string) var_get( file_var ), 0, 0 );
      s_upper( file_id, 0, 0 );
      s_trim( &file_id, 'L'  );
      s_init( &edit, "EDIT" );
      if( s_comp( file_id, edit, 0, 4, 'M', &rc ) )
      {

         string   form_ft   = S_NULL,
                  mode_path = S_NULL;


         s_init( &mode_path, "* * "  );
         s_cat(  &file_id, mode_path, file_id,  0, 0 );
         s_init( &form_ft, " FORM"   );
         s_cat(  &file_id, file_id,   form_ft,  0, 0 );
         s_drop( &form_ft   );
         s_drop( &mode_path );

      }
      else
      {

         s_pad = TRUE;
         s_init( &file_id, "A0 * TEIES $SP" );
         t_spad(  file_id, NIL  );

      }
      s_drop( & edit );
      ocb = obj_opn( (OID *) file_id, OBJ_READ | OS_FILE );

      if( ocb == NIL )
      {


      }
      else
      {


/*+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  +                                                                   +
  +  Create a conference comment and fill in its contents with the    +
  +  contents of the file the user indicated on the form.  Then link  +
  +  the conference comment into the conference containing the forms  +
  +                                                                   +
  + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +*/
         cc  = obj_all( CC_OBJ, LOCAL_OBJECT, NIL  );
         ocd = ocd_new( &ocd, "ITEM.CONTENTS.TEXT" );
         rc  = ocd_bse( cc, ocd, ocb     );
         if( rc != 0 )
         {

            rc = lk_cc( conf_root, NIL, 0, NIL );


/*+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  +                                                                   +
  +  After the conference is squared away link the form into the forms+
  +  index under the current state of the interface (internal).       +
  +                                                                   +
  + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +*/
            ocd_new( &ocd, "SFI.INDEX.OID"     );
            ocd->key = form_id;
            rc = obj_rep( sfi, ocd, &(cc->oid), SEG_HOLD );

            ocd->key = S_NULL;
            ocd_new( &ocd, "SFI.INDEX.OCD"     );
            ocd->key = form_id;
            s_init( &contents_ocd, "ITEM.CONTENTS.TEXT"         );
            rc = obj_rep( sfi, ocd, contents_ocd, SEG_HOLD );
            s_drop( &contents_ocd );

            ocd->key = S_NULL;
            ocd_new( &ocd, "SFI.INDEX.FORMID"  );
            ocd->key = form_id;
            s_alloc( &temp, 8 );
            os_memn(   s_buffer( temp ), 8 );
            rc = obj_rep( sfi, ocd, temp, SEG_UPDATE );
            ocd->key = S_NULL;
            s_drop( &temp   );

         }
/*+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  +                                                                   +
  +  At last, close all the objects to make sure the updates are made +
  +  and clean up memory allocation.                                  +
  +                                                                   +
  + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +*/

         obj_cls( cc );
         obj_cls(sfi );
         ocd_drp( &ocd );
         if( s_pad )
         {

            t_delsp( file_id );

         }
      }
   }
   s_drop( &file_id );
   s_drop( &file_var);
   s_drop( &addform );
   s_drop( &form_var);



   return(0);

}
