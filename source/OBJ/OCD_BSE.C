/********************************************************************
    <DOC>
    <NAME>               OCD_BSE.C
    <LEVEL>              OBJ
    <AUTHOR>             John L. Foster
    <DATE>               Mar 29, 1988
    <PARAMS>
           *ocb OCB             pointer to object control block
           *ocd OCD             pointer to ocdstr and index key
           *gfd OCB             object of the input data.
    <OUTPUT> sint error code.
    <GLOBAL>
    <PURPOSE>
            convert a CMS file,given the gfd,to the Basic Storage
            Element (BSE) and put it into DMS by the given pointer
            of the ocd record of the object definition table pointed
            to by the given pointer which points to the object control
            table.

    <CALLS>            obj_get(), obj_rep(), s_copy(), s_cat()
    <ERRORS>
    <LIMITS>
    </DOC>
 ********************************************************************/

#include <stdio.h>
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "exc.h"
#include "bse.h"






sint   ocd_bse (  ocb, ocd, gfd  )

OCB              *ocb    ;
OCD              *ocd    ;
OCB              *gfd    ;



{


   sint      rc        = 0     ,
             bse_value = 0     ,
           * bse_line_num = NIL,
           * txt_line_num = NIL;
   ODT     * odt_entry = NIL   ;
   string    line      = S_NULL;
   OCD     * gfd_ocd   = S_NULL,
           * bse_ocd   = S_NULL,
           * txt_ocd   = S_NULL;





   if( ! OCB_POINTER_OK( gfd ) )
   {

      except(   INVALID_OCB,    DATA_ERROR, S_NULL, S_NULL, S_NULL,
                                           S_NULL, S_NULL          );
   return(INVALID_OCB);

   }
   if( ! OCB_POINTER_OK( ocb ) )
   {

      except( INVALID_OCB,    DATA_ERROR, S_NULL, S_NULL, S_NULL,
                                           S_NULL, S_NULL          );
   return(INVALID_OCB);

   }


   odt_entry = odt_find( ocb, ocd );

   if(  ODT_POINTER_OK( odt_entry )  )
   {

      if( odt_type( odt_entry ) != OC_NDX )
      {

         rc = INVALID_OCD_TYPE;

      }
      else
      {

         sint  len = 0;

         obj_set_mode( gfd, OBJ_FLUSH );
         obj_cls(      gfd            );

         ocd_new( & gfd_ocd, NIL );
         ocd_new( & bse_ocd, NIL );
         ocd_new( & txt_ocd, NIL );

         len = s_len( ocd->name );
         s_alloc( & ( bse_ocd->name ), len + 4 );
         os_memc( s_buffer( ocd->name ), s_buffer( bse_ocd->name ), len );
         os_memc( ".BSE", s_buffer( bse_ocd->name )+ len, 4 );
         s_smp_ul( bse_ocd->name ) = len + 4;

         s_alloc( & ( txt_ocd->name ), len + 5 );
         os_memc( s_buffer( ocd->name ), s_buffer( txt_ocd->name ), len );
         os_memc( ".TEXT", s_buffer( txt_ocd->name )+ len, 5 );
         s_smp_ul( txt_ocd->name ) = len + 5;


         s_alloc(       & ( txt_ocd->key ) , BSE_MAX_KEY_SIZE );
         os_memn( s_buffer( txt_ocd->key ) , BSE_MAX_KEY_SIZE );
         s_smp_ul(          txt_ocd->key ) = BSE_MAX_KEY_SIZE  ;

         txt_line_num = (sint *) s_buffer( txt_ocd->key ) ;

         s_alloc(       & ( bse_ocd->key ) , BSE_MAX_KEY_SIZE );
         os_memn( s_buffer( bse_ocd->key ) , BSE_MAX_KEY_SIZE );
         s_smp_ul(          bse_ocd->key ) = BSE_MAX_KEY_SIZE  ;

         bse_line_num = (sint *) s_buffer( bse_ocd->key ) ;

         bse_value = B_RFIRST | B_PRINTABLE ;

         while( obj_grec( gfd, gfd_ocd, & line ) )
         {

            *(txt_line_num) += 1;
            *(bse_line_num) += 1;


            rc = obj_rep( ocb, txt_ocd, line, SEG_UPDATE );
            if( rc != 0 )
            {

               break;

            }

            rc = obj_rep( ocb, bse_ocd, & bse_value, SEG_UPDATE );

            if( rc != 0 )
            {

               break;

            }

         }

         s_drop(  & line    );
         ocd_drp( & gfd_ocd );
         ocd_drp( & bse_ocd );
         ocd_drp( & txt_ocd );


      }
   }
   else
   {


      rc = OCD_NOT_IN_ODT;

   }

   return(rc);
}
