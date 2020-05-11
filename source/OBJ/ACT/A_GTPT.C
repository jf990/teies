/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                              A_GTPT.C
<LEVEL>                             ACTIVITY
<AUTHOR>                            Heidi E. Harting
<DATE>                              July 19, 1988

<Params>                  form  : a string containing a form id, It is
                                  used in case the form was already
                                  rendered, F_SETFLD needs as a param.

                       act_ocb  : a pointer to the OCB where the activity
                                  information is stored.

                       act_type : a sint.

<OUTPUT>                        : - none -
<PURPOSE>
         This routine is used to extract the responses of a Question
         Activity, the Sections of a Read Activity, and the Selection
         Explanations of a Selection Activity, from the Database.
<ERRORS>
                       INVALID_OCB                20022
                       FUNCTION_NOT_SUPPORTED     20222
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "isv.h"
#include "activity.h"

sint a_gtpt( act_ocb, act_type, part_num, attach_type, id )

OCB     *act_ocb;
sint    act_type;
sint    part_num;
sint    attach_type;
string  id;

{
   sint     rc             =         0;

   OCB      *part_ocb      =       NIL;

   OCD      *cc_ocd        =       NIL,
            *act_ocd       =       NIL;

   OID      *part_oid      =       NIL;

   hword    half           =         0;

   byte     x_byte         =         0;

   string   sect           =    S_NULL,
            act_id         =    S_NULL,
            form           =    S_NULL,
            part           =    S_NULL;


   s_init( &sect,  "SECT" );
   s_init( &act_id, "ACT_ID");

   part_oid = ( OID * )os_mema( sizeof (OID) );

   ocd_new( &act_ocd, "ACTIVITY.PART_LIST.LOCATION" );
   s_alloc(&(act_ocd->key), 4);
   os_memn(s_buffer(act_ocd->key), 4);
   s_smp_ul(act_ocd->key) = 4;
   os_memc( &part_num, s_buffer( act_ocd->key ), 4 );    /* assign key */
   obj_get( act_ocb, act_ocd, part_oid );
   part_ocb = obj_opn( part_oid, OBJ_UPDATE | OBJ_REMOTE );

   s_sitos( &part, part_num );


   if ( OCB_POINTER_OK( part_ocb ) )
   {

      switch ( act_type )
      {

         case READ:
                   s_init( &form, "RD_PART" );
                   break;
         case QUESTION:
                   s_init( &form, "QS_PART" );
                   break;
         case SELECTION:
                   s_init( &form, "SL_PART" );
                   break;
         default:
                   rc = FUNCTION_NOT_SUPPORTED;
                   break;

      }
      if (rc == 0)
      {
         var_set( sect, part );
         f_setfld( form, sect, part );

         var_set( act_id, id );
         f_setfld( form, act_id, id );

         rc = t_form( form, S_NULL, part_ocb );
         if (rc != USER_REQUESTED_QUIT)
         {
            rc = 0;
         }

      }


      f_close( form, NO_FLAGS );

      obj_cls( part_ocb );
   }
   else
   {
      rc = INVALID_OCB;
   }


   ocd_drp( &act_ocd );

   os_memd(part_oid);


   s_drop( &part );
   s_drop( &sect );
   s_drop( &form );
   s_drop( &act_id);

   return(rc);
}
