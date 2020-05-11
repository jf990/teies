/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    obj_import()                                           |
  |                                                                   |
  |  <AUTHOR>  John L. Foster, Ashish K. Mehta                        |
  |                                                                   |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <DATE>    July 20, 1989                                          |
  |                                                                   |
  |  <PARAM>   target_ocb       OCB *   INPUT -  target object        |
  |  <PARAM>   source_ocb       OCB *   INPUT -  source obj pointer   |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PURPOSE> import foreign data from an operating system file or   |
  |            data stream into a TEIES object.                       |
  |                                                                   |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   obj_stoc() , obj_grec()                                |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <LIMITS>  '@LINK does not work for this version.'                |
  |                                                                   |
  |  <END>                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"


#define  STR_VEC_LEN   5


sint   obj_import( target_ocb, source_ocb )

OCB               *target_ocb;  /* object where to transfer data       */
OCB               *source_ocb;  /* OS_FILE pointer from where to import*/

{

string       line        =    S_NULL  ,
             deli_str    =    S_NULL  ,
             spill_str   =    S_NULL  ,
             template    =    S_NULL  ,
           * str_vec     =    S_NULL  ,
           * str_choice  =    S_NULL  ,
             ourform     =    S_NULL  ,
             clip_str    =    S_NULL  ;


 OCD       * ocd_ptr     =    NIL     ,
           * source_ocd  =    NIL     ;

 OID       * oid_ptr     =    NIL     ;

 hword       obj_type    =    0       ;

 sint        rc          =    0       ;

 byte      * buffer      =    NIL     ;


if ( OCB_POINTER_OK ( source_ocb )  )

{

   s_valloc ( & str_vec   , STR_VEC_LEN );
   s_valloc ( & str_choice, STR_VEC_LEN );
   s_init   ( & template  , " @n = @n " );
   s_init   ( & str_choice[ 1 ] , "OBJECTTYPE" );
   s_init   ( & str_choice[ 2 ] , "LINK" );

   ocd_new( &source_ocd, NIL );

   ocd_new ( &ocd_ptr, NIL );

   while ( obj_grec ( source_ocb, source_ocd, &line ) )

   {


      if ( s_char_s ( line , 0 ) == '@' )
      {
         /*+--------------------------------------------+*/
         /*|     case when user has entered the command |*/
         /*|     - remove '@' sign from line            |*/
         /*+--------------------------------------------+*/

          s_clip    ( &clip_str, &line   , 0, 1 );

          s_parse   ( &line, template, deli_str, str_vec, &spill_str );


          if ( s_comp ( str_vec [ 1 ], str_choice[ 1 ], 0, 0,
                                                      'M',&rc ) == 0 )
          {


              obj_type   =  obj_inta ( str_vec[ 2 ] );

              target_ocb =  obj_all ( obj_type, LOCAL_OBJECT, NIL );

              oid_ptr    = &( target_ocb -> oid );

          }
          else if ( s_comp ( str_vec [ 1 ], str_choice[ 2 ], 0, 0,
                                                      'M',&rc ) == 0 )
          {

              rc = t_jmpst (  target_ocb -> type , LINK_ACT, 0, S_NULL,
                                                   target_ocb, &buffer );
          }
       }

       else if ( OCB_POINTER_OK ( target_ocb ) )

       {

       /*+-----------------------------------------------------------+*/
       /*|   case when user has entered ocd and data in the file     |*/
       /*+-----------------------------------------------------------+*/

          s_parse ( &line, template, deli_str, str_vec, &spill_str );


          s_copy  ( & ( ocd_ptr -> name ),  str_vec[1], 0, 0 );

          rc = ocd_dcd ( ocd_ptr );



          /*+-------------------------------------+*/
          /*| replace the component in the object |*/
          /*+-------------------------------------+*/

          obj_set_mode ( target_ocb, OBJ_RAW );

          ocd_stoc ( target_ocb, ocd_ptr, str_vec[2] );

          obj_reset_mode ( target_ocb, OBJ_RAW );


        }
        else
        {
        }
     }
  }
  else

  {


    rc = INVALID_OCB ;


   }

   return(rc);

}
