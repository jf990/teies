/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    obj_import()                                           |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Ashish K. Mehta                        |
  |                                                                   |
  |  :DATE.    July 20, 1989                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   target_ocb       OCB *   INPUT -  target object        |
  |  :PARAM.   source_ocb       OCB *   INPUT -  source obj pointer   |
  |                                                                   |
  |  :PURPOSE. import foreign data from an operating system file or   |
  |            data stream into a TEIES object.                       |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   obj_stoc() , obj_grec()                                |
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


#define  STR_VEC_LEN   5


sint   obj_import( target_ocb, source_ocb )

OCB               *target_ocb;  /* object where to put the data      */
OCB               *source_ocb;  /* OS_FILE pointer from where to get */

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

#D_begin(  obj_import,  OBJ  )

if ( OCB_POINTER_OK ( source_ocb )  )

{

   s_valloc ( & str_vec   , STR_VEC_LEN );
   s_valloc ( & str_choice, STR_VEC_LEN );
   s_init   ( & template  , " @n = @n " );
   s_init   ( & str_choice(: 1 :) , "OBJECTTYPE" );
   s_init   ( & str_choice(: 2 :) , "LINK" );

   ocd_new( &source_ocd, "nothing");

   ocd_new ( &ocd_ptr,  "nothing"  );

   while ( obj_grec ( source_ocb, source_ocd, &line )  )

   {

      #D_str ('line','line');

      if ( s_char_s ( line , 0 ) == '@' )
      {
         /*+--------------------------------------------+*/
         /*|     case when user has entered the command |*/
         /*|     - remove '@' sign from line            |*/
         /*+--------------------------------------------+*/
          #D_say (' @ command is entered ');

          s_clip    ( &clip_str, &line   , 0, 1 );

          s_parse   ( &line, template, deli_str, str_vec, &spill_str );

          #D_strvec ( str_vec, 'from s_parse');

          if ( s_comp ( str_vec (: 1 :), str_choice(: 1 :), 0, 0,
                                                      'M',&rc ) == 0 )
          {

              #D_say (' Object type is specified ');

              obj_type   =  obj_inta ( str_vec(: 2 :) );

              target_ocb =  obj_all ( obj_type, LOCAL_OBJECT, NIL );

              oid_ptr    = &( target_ocb -> oid );

              #D_oid ('oid_ptr');
          }
          else if ( s_comp ( str_vec (: 1 :), str_choice(: 2 :), 0, 0,
                                                      'M',&rc ) == 0 )
          {

              #D_say (' LINK is specified ');
              rc = t_jmpst (  target_ocb -> type , LINK_ACT, 0, S_NULL,
                                                   target_ocb, &buffer );
              #D_show (' rc returned from t_jmpst = %d', 'rc');
          }
       }

       else if ( OCB_POINTER_OK ( target_ocb ) )

       {

       /*+-----------------------------------------------------------+*/
       /*|   case when user has entered ocd and data in the file     |*/
       /*+-----------------------------------------------------------+*/

          s_parse ( &line, template, deli_str, str_vec, &spill_str );

          #D_strvec ( str_vec, 'from s_parse');

          s_copy  ( & ( ocd_ptr -> name ),  str_vec(:1:), 0, 0 );

          rc = ocd_dcd ( ocd_ptr );
          #D_show (' returned code from ocd_dcd = %d', rc );


          #D_say (' Trying to replace the component ');

          /*+-------------------------------------+*/
          /*| replace the component in the object |*/
          /*+-------------------------------------+*/

          obj_set_mode ( target_ocb, OBJ_RAW );

          ocd_stoc ( target_ocb, ocd_ptr, str_vec(:2:) );

          obj_reset_mode ( target_ocb, OBJ_RAW );

          #D_say (' Component is replaced');

        }
        else
        {
          #D_say (' Could not find target object - going to next line');
        }
     }
  }
  else

  {


    rc = INVALID_OCB ;


   }

#D_return( rc );

}
