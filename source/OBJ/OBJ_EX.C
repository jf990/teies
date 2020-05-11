/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    obj_export()                                           |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Ashish K. Mehta                        |
  |                                                                   |
  |  :DATE.    July 28, 1989                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   target_ocb       OCB *   INPUT -  target OS_FILE ptr   |
  |  :PARAM.   source_ocb       OCB *   INPUT -  source obj pointer   |
  |                                                                   |
  |  :PURPOSE. export data from a TEIES object to an operating system |
  |            file.                                                  |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   obj_rep () , obj_next() , isv_get (),                  |
  |            odt_find() , ocd_root() , ocd_ctos(),                  |
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
#include "isv.h"




sint   obj_export( target_ocb, source_ocb )

OCB               *target_ocb;  /* OS_FILE pointer where to export   */
OCB               *source_ocb;  /* TEIES object from where to export */

{

string       line        =    S_NULL    , /* line which we want to copy*/
                                          /* into the file             */
             data_str    =    S_NULL    , /* has data from obj_next    */
             ndx_comp    =    S_NULL    , /* has index component in it */
             isv_str     =    S_NULL    , /* contains str from isv_get */
             key_str     =    S_NULL    , /* contains ocd -> key       */
             clip_ocd    =    S_NULL    , /* contains ocd -> key       */
             lef_paren   =    S_NULL    , /* string has one char '('   */
             rgt_paren   =    S_NULL    , /* string has one char ')'   */
             zero_str    =    S_NULL    , /* string has one char '0'   */
             coma_str    =    S_NULL    , /* string has one char ','   */
             equal_to    =    S_NULL    ; /* string has one char '='   */


 OCD       * ocd_ptr     =    NIL       ;

 ODT       * odt_ptr     =    NIL       ;

 OID       * oid_ptr     =    NIL       ;

 sint        rc          =    0          , /* return code of routine   */
             length      =    0          , /* length from obj_next     */
             ret_code    =    0          , /* return code of obj_next  */
             offset      =    0          ; /* offset passed to obj_next*/

 byte      * buffer      =    NIL        ; /* ptr to data for obj_next */

 hword       what_to_get = ANY_COMPONENT ,
             object      = 0             ;


#D_begin(  obj_export,  OBJ  )
#D_show('source OCB = 0x%X  target OCB = 0x%X','source_ocb,target_ocb');

if ( OCB_POINTER_OK ( source_ocb )  )
{

    #D_say (' passed source pointer is ok.');
    ocd_new( &ocd_ptr, NIL );
    offset    = 1 ;
    s_init ( & equal_to, "=" );
    s_init ( & zero_str, "0" );
    s_init ( & lef_paren, "(" );
    s_init ( & rgt_paren, ")" );
    s_init ( & coma_str, "," );

    oid_ptr  =  ( OID* ) os_mema ( sizeof(OID));
    oid_ptr  = & ( source_ocb -> oid ) ;
    #D_oid ( 'oid_ptr' );

    object  = obj_type ( oid_ptr );
    rc      = isv_get ( &isv_str ,( (isv) ( object + 10)) , 0 );
    #D_str ('isv_str','isv_str');

    s_init ( &line , "@OBJECTTYPE");
    s_cat  ( &line, line, equal_to, 0, 0);
    s_trim ( &line, 'B' ) ;
    s_cat  ( &line , line , isv_str , 0 , 0    );
    s_trim ( &line  , 'B'                        );
    #D_str (  'line', 'line'                     );

    ocd_ptr -> bias = 0 ;
    rc = obj_rep ( target_ocb, ocd_ptr,  line  ,  SEG_UPDATE );
    #D_show ( ' return code from obj_rep = %d', 'rc');

    /*+-----------------------------------------------+*/
    /*| To get the first component from the object    |*/
    /*+-----------------------------------------------+*/

    s_copy ( &(ocd_ptr->name), odt_name((source_ocb->odt) + 1), 0 , 0 );
    #D_str ('ocd_ptr->name','ocd_ptr->name');

    while ( ret_code != IMPROPER_DIRECTION_IN_ODT )
    {

         /*+-------------------------------------------------------+*/
         /*| get OCD from TEIES object, if invalid OCD get the next|*/
         /*+-------------------------------------------------------+*/

         ret_code = obj_next ( source_ocb, ocd_ptr, (void **) &buffer,
                                           offset, what_to_get, &length);
         offset    = 1 ;
         #D_show ( ' rc from obj_next = %d','ret_code');

         odt_ptr = odt_find ( source_ocb, ocd_ptr );

         if ( odt_type ( odt_ptr ) == OC_NDX  )
         {

             #D_say (' WE ARE IN INDEX SEGMENT ');
             s_copy ( &ndx_comp, ( ocd_ptr -> name ) , 0, 0 );
             s_trim ( &ndx_comp, 'B'   );
             #D_str ('ndx_comp', 'ndx_comp');

             offset   = 2    ;

         }
         else if ( odt_type ( odt_ptr ) >= OC_SEG  )
         {

             #D_str ('ocd_ptr -> name ', 'ocd_ptr -> name');
             #D_say (' NOT A COMPONENT ');

         }
         else
         {

            #D_str ('ocd_ptr -> name ', 'ocd_ptr -> name');
            #D_say (' VALID COMPONENT ');

            s_nulout ( &data_str );

            odt_ptr = ocd_root ( source_ocb, ocd_ptr );

            if (  ( ! obj_isnull( source_ocb, ocd_ptr ) )  ||
                  (   odt_type  ( odt_ptr ) == OC_NDX   )
               )
            {

                #D_say (' data found or we are in index segment ');

                ocd_ptr -> bias = 1 ;

                obj_set_mode( source_ocb, OBJ_RAW );

                rc = ocd_ctos ( source_ocb, ocd_ptr, &data_str );

                obj_reset_mode( source_ocb, OBJ_RAW );

                ocd_ptr -> bias = 0 ;

            }

           /*+----------------------------------------------------+*/
           /*|  Case when data found for the component            |*/
           /*-----------------------------------------------------+*/

           if ( s_len ( data_str ) > 0 )
           {

               if ( odt_type (odt_ptr) == OC_NDX )
               {

                  #D_say (' Index segment found ');

                  while ( rc != NO_DATA_FOUND )
                  {

                      #D_say (' Index segment found ');

                      s_mtoxs ( &key_str, s_buffer( ocd_ptr->key ),
                                          s_len   ( ocd_ptr->key ) );
                      s_copy ( &clip_ocd, ( ocd_ptr -> name ) , 0, 0 );
                      s_trim ( &clip_ocd, 'B'   );
                      #D_str ('clip_ocd', 'clip_ocd');

                      s_clip ( &ndx_comp, &clip_ocd, 0, s_len(ndx_comp));
                      #D_str('ndx_comp','ndx_comp');
                      #D_str('clip_ocd','clip_ocd');

                      s_cat ( &line, ndx_comp, lef_paren, 0, 0);
                      #D_str('line','line');


                      s_cat ( &line, line, key_str, 0, 0);
                      #D_str('line','line');

                      s_cat ( &line, line, coma_str, 0, 0);
                      #D_str('line','line');

                      s_cat ( &line, line, zero_str, 0, 0);
                      #D_str('line','line');

                      s_cat ( &line, line, rgt_paren, 0, 0);
                      #D_str('line','line');

                      s_cat ( &line, line, clip_ocd, 0, 0);
                      #D_str('line','line');

                      s_cat ( &line, line, equal_to, 0, 0);
                      #D_str('line','line');

                      s_cat  (  &line , line , data_str , 0 , 0    );
                      s_trim ( &line  , 'B'                        );
                      #D_str (  'line', 'line'                     );

                      ocd_ptr -> bias = 0 ;
                      rc = obj_rep(target_ocb,ocd_ptr,line,SEG_UPDATE);
                      #D_show ( ' return code from obj_rep = %d', 'rc');

                      ocd_ptr -> bias = 1 ;
                      obj_set_mode( source_ocb, OBJ_RAW );
                      rc = ocd_ctos ( source_ocb, ocd_ptr, &data_str );
                      obj_reset_mode( source_ocb, OBJ_RAW );
                      #D_show ( ' return code from obj_ctos = %d', 'rc');
                   }


               }
               else
               {

                  #D_say (' non-index segment ');

                  s_copy ( &line, ( ocd_ptr -> name ) , 0, 0 );
                  s_trim ( &line, 'B'   );
                  #D_str ('line', 'line');

                  s_cat  ( &line, line, equal_to, 0, 0);
                  s_trim ( &line, 'B' ) ;
                  #D_str ('line', 'line_str');

                  ocd_ptr -> bias = 0 ;

                  #D_str ('data_str', 'data_str');


                  s_cat  (  &line , line , data_str , 0 , 0    );
                  s_trim ( &line  , 'B'                        );
                  #D_str (  'line', 'line'                     );

                  ocd_ptr -> bias = 0 ;
                  rc = obj_rep(target_ocb,ocd_ptr,line,SEG_UPDATE);
                  #D_show ( ' return code from obj_rep = %d', 'rc');

               }


           }
        }

   }

}
else
{

       rc = INVALID_OCB ;

}
#D_return( rc );

}
