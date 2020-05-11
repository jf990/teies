/*+-------------------------------------------------------------------+*/
/*|     <DOC>                                                         |*/
/*|                                                                   |*/
/*|     <NAME>             DBM_EDIT.C                                 |*/
/*|                                                                   |*/
/*|     <LEVEL>            OBJ                                        |*/
/*|                                                                   |*/
/*|     <AUTHOR>           John L. Foster, Ashish K. Mehta            |*/
/*|                                                                   |*/
/*|     <DATE>             May 30, 1989                               |*/
/*|                                                                   |*/
/*|     <PARAMS>           *oid     struct OID  INPUT  Pointer to OID |*/
/*|                                                                   |*/
/*|     <OUTPUT>           *return_code    sint OUTPUT   0 - Quit     |*/
/*|                                                      1 - Abandon  |*/
/*|                                                                   |*/
/*|     <GLOBALS>          - none -                                   |*/
/*|                                                                   |*/
/*|     <PURPOSE>          DATABASE MANAGEMENT EDITOR                 |*/
/*|                        This  routine gives a SCREEN to            |*/
/*|                        edit the components of the object          |*/
/*|                        asked in 'oid'.                            |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|     <CALLS>            obj_opn (), soidi_gt (), ocd_new (),       |*/
/*|                        t_form  (), ocd_ctos (), ocd_stoc()        |*/
/*|                        obj_next(), odt_find (),                   |*/
/*|                                                                   |*/
/*|     <ERRORS>           -not known                                 |*/
/*|                                                                   |*/
/*|     <LIMITS>                                                      |*/
/*|                                                                   |*/
/*|     <MODS>                                                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "dbm_h.h"
#include "vars.h"



sint  dbm_edit( oid )

OID  * oid ;

{

     sint        rc             =    0        ,
                 flag           =    0        ,
                 offset         =    0        ,
                 select         =    0        ,
                 key_select     =    0        ,
                 key_flag       =    0        ,
                 process_id     =    0        ,
                 return_code    =    0        ,
                 length         =    0        ;
     string      form_id        =    S_NULL   ,
                 return_string  =    S_NULL   ,
                 abandon        =    S_NULL   ,
                 lfd_str        =    S_NULL   ,
                 data_str       =    S_NULL   ,
                 goid_str       =    S_NULL   ,
                 obj_str        =    S_NULL   ,
                 oid_str        =    S_NULL   ,
                 component      =    S_NULL   ,
                 user_data      =    S_NULL   ,
                 prev_data      =    S_NULL   ,
                 out_data       =    S_NULL   ,
                 oid_form       =    S_NULL   ,
                 lfd_form       =    S_NULL   ,
                 goid_form      =    S_NULL   ,
                 key_form       =    S_NULL   ,
                 key_str        =    S_NULL   ,
                 offset_str     =    S_NULL   ,
                 object         =    S_NULL   ,
                 message        =    S_NULL   ,
                 no_comp_msg    =    S_NULL   ,
                 no_data_msg    =    S_NULL   ,
                 no_rep_msg     =    S_NULL   ,
                 rep_comp_msg   =    S_NULL   ,
                 current_dt     =    S_NULL   ,
                 same_comp_str  =    S_NULL   ,
                 same_key_str   =    S_NULL   ,
                 same_offset    =    S_NULL   ,
                 DATE           =    S_NULL   ,
                 next_comp      =    S_NULL   ,
                 prev_comp      =    S_NULL   ,
                 lst_cmp_obj    =    S_NULL   ,
                 fst_cmp_obj    =    S_NULL   ,
                 enter_comp     =    S_NULL   ,
                 no_keys        =    S_NULL   ,
                 quit           =    S_NULL   ;


     tint        goid           =    0        ;

     hword       lfd            =    0        ,
                 obj_type       =    0        ,
                 what_to_get    =    ANY_COMPONENT;

     OCB         *ocb_ptr       =    NIL      ;

     OCD         *ocd_ptr       =    NIL      ;

     ODT         *odt_ptr       =    NIL      ;


     byte        *buffer        =    NIL      ,
                 *byte_ptr      =    NIL      ;




     #D_begin ( dbm_edit, OBJ                                          );
     #D_say   ( " BEGIN DBM_EDIT "                                     );

     #D_oid   ( 'oid');

     s_init   ( &quit     , "QUIT"                                     );
     s_init   ( &prev_comp, "PROCESS"                                  );
     s_init   ( &next_comp, "REPLY"                                    );
     s_init   ( &DATE     , "%TIMEON"                                  );
     s_init   ( &abandon  , "ABANDON"                                  );
     s_init   ( &component, "COMPONENT"                                );
     s_init   ( &out_data , "DATA2"                                    );
     s_init   ( &user_data, "DATA1"                                    );
     s_init   ( &oid_form , "OID2"                                     );
     s_init   ( &lfd_form , "LFD2"                                     );
     s_init   ( &goid_form, "GOID2"                                    );
     s_init   ( &key_form , "KEY"                                      );
     s_init   ( &offset_str,"OFFSET"                                   );
     s_init   ( &object   , "OBJECT"                                   );
     s_init   ( &form_id  , "DBM_ED2"                                  );
     s_init   ( &message  , "MESSAGE"                                  );
     s_init   ( &no_comp_msg ,"   * * * Component Does Not Exist * * *");
     s_init   ( &no_rep_msg  ,"  * * * Component is NOT Replaced * * *");
     s_init   ( &no_data_msg ,"   * * *   Component is Empty     * * *");
     s_init   ( &rep_comp_msg,"   * * * Component is Replaced * * *   ");
     s_init   ( &lst_cmp_obj ,"   * * * Last Component of Object * * *");
     s_init   ( &fst_cmp_obj ,"  * * * First Component of Object * * *");
     s_init   ( &enter_comp  ,"   * * * Enter an Component First * * *");
     s_init   ( &no_keys     ,"   * * * No more keys in the index* * *");

     /*+--------------------------------------------------------------+*/
     /*|   (1). open the object asked in 'oid'                        |*/
     /*|   (2). get  the lfd, goid and object_type from system oid    |*/
     /*|        index.                                                |*/
     /*|   (3). Convert lfd, goid and object_type into string so that |*/
     /*|        we can display them on the form.                      |*/
     /*+--------------------------------------------------------------+*/

     ocb_ptr = obj_opn ( oid , OBJ_UPDATE );

     if ( OCB_POINTER_OK ( ocb_ptr ) )
     {

      t_gtdate ( &current_dt, NIL        );
      var_set  ( DATE       , current_dt );

     rc = soidi_gt ( oid,   &lfd,   &goid,   &obj_type    );

     s_htos  (   & lfd_str ,    lfd                       );
     s_sitos (   & goid_str,    goid                      );
     s_htos  (   & obj_str ,    obj_type                  );
     s_mtoxs (   & oid_str , (byte *) oid , sizeof( OID ) );


     var_set  (   oid_form,   oid_str   );
     f_setfld (   form_id, oid_form, oid_str );
     var_set  (   lfd_form,   lfd_str   );
     f_setfld (   form_id, lfd_form, lfd_str );
     var_set  (   goid_form,   goid_str );
     f_setfld (   form_id, goid_form, goid_str );
     var_set  (   object  ,   obj_str   );
     f_setfld (   form_id, object, obj_str );



     ocd_new  (   &ocd_ptr,  "nothing"  );

     /*+--------------------------------------------------------------+*/
     /*|  Call screen 2 :                                             |*/
     /*|                 User can enter component of the object,      |*/
     /*|                 receive the data of the component in the     |*/
     /*|                 object and change the data of the component  |*/
     /*|                 in the given object.                         |*/
     /*+--------------------------------------------------------------+*/

     return_string = t_form ( form_id , S_NULL, NIL );

     while ( ( s_comp ( return_string, prev_comp, 0, 0,
                                    'M', &rc ) == 0 ) ||
             ( s_comp ( return_string, next_comp, 0, 0,
                                    'M', &rc ) == 0 ) )
     {

        f_setfld ( form_id , message, enter_comp );
        return_string = t_form ( form_id , S_NULL, NIL );
     }

     f_rstfld ( form_id, NO_FLAGS, message );
     var_clr  ( message );

     while ( ! t_ifquit ( return_string ) )

     {

          /*+----------------------------------------------------+*/
          /*|       Fill in the OCD from the form                |*/
          /*+----------------------------------------------------+*/
          key_select = 0 ;
          key_flag   = 0 ;

          if ( select == 0 )
          {
              #D_say('Case when user enter the component-first time ');
              s_copy ( &(ocd_ptr->name), var_get(component) , 0, 0);
              #D_str ('ocd_ptr -> name','ocd_ptr->name');

          }
          else if ( select == 1 )
          {

              #D_say('Case when user enter the component-after');

              var_set  ( component, ( ocd_ptr -> name ) );
              f_setfld ( form_id, component, (ocd_ptr->name) );

              #D_str ('ocd_ptr -> name','ocd_ptr->name');

          }
         s_copy ( &same_comp_str, ( ocd_ptr->name) , 0, 0);

         s_upper(ocd_ptr->name, 0, 0);

         odt_ptr  =  odt_find ( ocb_ptr, ocd_ptr );

         s_trim(&(ocd_ptr->name), 'B');

         if ( odt_type ( odt_ptr ) >= OC_SEG )
         {
              /*+------------------------------------------------+*/
              /*| If entered  component is not a component       |*/
              /*| skip it.                                       |*/
              /*+------------------------------------------------+*/
              offset = 1 ;
              #D_say (' NOT A COMPONENT ');
              rc = obj_next ( ocb_ptr, ocd_ptr, (void **) &buffer,
                                         offset, what_to_get, &length );
              #D_show ( ' rc from obj_next =%d', 'rc');
              select = 1 ;
         }
         else
         {
           #D_say('SEEMS VALID COMPONENT');

           odt_ptr =  ocd_root ( ocb_ptr, ocd_ptr ) ;

           /*+---------------------------------------------------+*/
           /*|  case when we have an INDEX segment               |*/
           /*+---------------------------------------------------+*/

           if ( odt_type ( odt_ptr ) == OC_NDX )
           {

              /*+----------------------------------------------------+*/
              /*|  Case when user enters key(offset) on the form     |*/
              /*+----------------------------------------------------+*/
              #D_say (' User has entered the key or/and offset');

              key_flag = 1 ;
              s_copy ( &same_key_str, var_get ( key_form) , 0, 0 );
              #D_str ( 'same_key_str', 'same_key_str');

              s_upper( same_key_str, 0, 0);
              #D_str ( 'same_key_str', 'same_key_str');

              s_copy ( &same_offset ,var_get (offset_str), 0, 0);
              #D_str ( 'same_offset', 'same_offset');

              s_alloc ( &(ocd_ptr -> key), 32 );
              byte_ptr = s_buffer ( ocd_ptr -> key );
              s_sxtom ( &byte_ptr, var_get(key_form), 0, 0 );
              s_smp_ul( ocd_ptr->key ) = s_len ( var_get (key_form) ) / 2
                                       + s_len ( var_get (key_form) )% 2;

              #D_str ('ocd_ptr -> key','ocd_ptr->key');

              s_stosi( &(ocd_ptr->bias),var_get(offset_str), 0, 0);
              #D_show( ' ocd_ptr -> bias = %d ', 'ocd_ptr->bias' );
              key_select = 1 ;

              if ( ( s_len ( ocd_ptr->key)          == 0 )  &&
                 ( s_len ( var_get (offset_str) ) == 0 )
                 )
                 {
                   /*+---------------------------------------------+*/
                   /*|  if bias is zero set it                     |*/
                   /*+---------------------------------------------+*/
                   #D_str ( 'ocd_ptr -> key','ocd_ptr -> key');
                   ocd_ptr->bias = 1 ;

                 }
          }


          #D_ocd( 'ocd_ptr' );
          #D_str('ocd_ptr->name','ocd_ptr->name');

          s_nulout ( & data_str );
          #D_str  ('data_str','data_str');

          f_rstfld ( form_id, NO_FLAGS, key_form    );
          var_clr  ( key_form    );

          obj_set_mode( ocb_ptr, OBJ_RAW );

          rc = ocd_ctos( ocb_ptr, ocd_ptr, &data_str );

          obj_reset_mode( ocb_ptr, OBJ_RAW );

          if ( key_flag == 1   &&  ( s_len ( ocd_ptr -> key ) > 0 ) )
          {
              /*+----------------------------------------------------+*/
              /*|  Case when user enter only offset on the form      |*/
              /*+----------------------------------------------------+*/
              #D_say (' set key on the form ');
              s_mtoxs( &key_str, s_buffer( ocd_ptr -> key ) ,
                                 s_len   ( ocd_ptr -> key )  );
              var_set  ( key_form, key_str );
              f_setfld ( form_id, key_form, key_str);
          }
          ocd_ptr -> bias = 0 ;
          #D_str  ('data_str','data_str');

          /*+---------------------------------------------------------+*/
          /*|  if rc = 0 then component exists                        |*/
          /*|  else           does not exist                          |*/
          /*+---------------------------------------------------------+*/
          if ( s_len( data_str ) > 0  )

          {

              /*+----------------------------------------------------+*/
              /*|  Case when user enter data on the form             |*/
              /*+----------------------------------------------------+*/
              f_setfld ( form_id, out_data, data_str );
              #D_say   ( " Field on the form is set" );

          }
          else if( rc == NO_DATA_FOUND || rc == 0 )

          {

              /*+----------------------------------------------------+*/
              /*|  Case when component  is not filled yet.           |*/
              /*+----------------------------------------------------+*/
              f_setfld ( form_id, message, no_data_msg );
              t_alarm ( process_id );
              #D_say   ( " Component requested is Empty" );
          }
          else if  ( rc == NO_MORE_KEYS )

          {

              /*+----------------------------------------------------+*/
              /*|  Case when component  is not filled yet.           |*/
              /*+----------------------------------------------------+*/
              f_setfld ( form_id, message, no_keys );
              t_alarm ( process_id );
              #D_say   ( " No more keys in the index " );
          }
          else
          {

              f_setfld ( form_id, message, no_comp_msg );
              t_alarm ( process_id );
              #D_say   ( " Component does not exist" );

          }

          /*+---------------------------------------------------------+*/
          /*| case when user wants to edit the same component         |*/
          /*+---------------------------------------------------------+*/

          while ( s_comp( var_get ( component ) , same_comp_str, 0, 0,
                                                  'M', &rc ) == 0 )
          {

             return_string = t_form ( form_id , S_NULL, NIL );
             #D_str  ('return_string','return_string');

             /*+-----------------------------------------------------+*/
             /*| case  when user press (PrvScr) or (NxtScr)          |*/
             /*| and wants to change the component also.             |*/
             /*+-----------------------------------------------------+*/

             if ( return_string == S_NULL      ||
                  t_ifquit ( return_string)       )

             {
             /*+-----------------------------------------------------+*/
             /*| case  when user press ENTER only                    |*/
             /*+-----------------------------------------------------+*/
                 #D_say   ( " User Requested QUIT " );
                 key_select = 0 ;
                 break;
             }

             else if ( s_comp ( return_string, prev_comp, 0, 0,
                                                     'M', &rc ) == 0 )
             {
             /*+-----------------------------------------------------+*/
             /*| case  when user requests previous component         |*/
             /*+-----------------------------------------------------+*/
                 offset = -1 ;
                 rc = obj_next ( ocb_ptr, ocd_ptr, (void **) &buffer,
                                        offset, what_to_get, &length );
                 #D_show ( ' rc from obj_next =%d', 'rc');

                 odt_ptr = odt_find ( ocb_ptr, ocd_ptr );

                 if ( odt_type ( odt_ptr ) >= OC_SEG )
                 {
                   /*+------------------------------------------------+*/
                   /*| If previous component is not a component       |*/
                   /*| skip it.                                       |*/
                   /*+------------------------------------------------+*/

                   #D_say (' NOT A COMPONENT ');
                    rc = obj_next ( ocb_ptr, ocd_ptr, (void **) &buffer,
                                          offset, what_to_get, &length );
                    #D_show ( ' rc from obj_next =%d', 'rc');

                 }

                 if ( ( rc != 0 ) && ( rc != NO_DATA_FOUND ) &&
                                     ( rc != NO_MORE_KEYS  )   )
                 {
                   /*+------------------------------------------------+*/
                   /*| We are trying to get component before the first|*/
                   /*+------------------------------------------------+*/
                    #D_say ('Trying to get component before the FIRST');
                    f_setfld ( form_id, message, fst_cmp_obj );
                    rc = 0 ;
                 }
                 else
                 {
                   /*+------------------------------------------------+*/
                   /*|Previous component found,dispaly it on the form |*/
                   /*+------------------------------------------------+*/
                    rc = 0;
                    select = 1 ;
                    key_select = 0 ;
                    break;
                 }
             }
             else if ( s_comp ( return_string, next_comp, 0, 0,
                                                     'M', &rc ) == 0 )
             {
             /*+-----------------------------------------------------+*/
             /*| case  when user requests next component             |*/
             /*+-----------------------------------------------------+*/
                 offset = 1 ;
                 rc = obj_next ( ocb_ptr, ocd_ptr, (void **) &buffer,
                                        offset, what_to_get, &length );
                 #D_show ( ' rc from obj_next =%d', 'rc');

                 odt_ptr = odt_find ( ocb_ptr, ocd_ptr );

                 if ( odt_type ( odt_ptr ) >= OC_SEG )
                 {
                   /*+------------------------------------------------+*/
                   /*| If next component is not a component skip it.  |*/
                   /*+------------------------------------------------+*/

                    #D_say (' NOT A COMPONENT ');
                    rc = obj_next ( ocb_ptr, ocd_ptr, (void **) &buffer,
                                          offset, what_to_get, &length );
                    #D_show ( ' rc from obj_next =%d', 'rc');

                 }
                 if ( ( rc != 0 ) && ( rc != NO_DATA_FOUND ) &&
                                     ( rc != NO_MORE_KEYS  )   )
                 {
                   /*+------------------------------------------------+*/
                   /*| We are trying to get component after the last  |*/
                   /*+------------------------------------------------+*/
                    #D_say ('Trying to get component after the LAST');
                    f_setfld ( form_id, message, lst_cmp_obj );
                    rc = 0 ;
                 }
                 else
                 {
                   /*+------------------------------------------------+*/
                   /*| Next component found, dispaly it on the form   |*/
                   /*+------------------------------------------------+*/
                   rc     = 0 ;
                   select = 1 ;
                   key_select = 0 ;
                   break;
                 }
             }

             else if ( s_comp ( var_get (component), same_comp_str, 0, 0,
                                                       'M', &rc ) != 0 )
             {
                  #D_say(' component is changed on the form');
                  select = 0 ;
                  key_select = 0 ;
                  break;
             }

             else if((s_comp ( var_get ( key_form ), same_key_str, 0, 0,
                                                  'M', &rc ) != 0 ) ||

                    (s_comp ( var_get ( offset_str), same_offset,  0, 0,
                                                  'M', &rc ) != 0 ) )

             {
                  /*+------------------------------------------------+*/
                  /*| Case when component remains same but only      |*/
                  /*| key is changed on the form.                    |*/
                  /*+------------------------------------------------+*/
                  #D_say('key is changed on the form');
                  key_select = 1 ;

                  if ( s_len ( var_get ( key_form ) ) == 0 )
                  {
                          key_select = 2 ;
                  }

                  break;
             }

             else if ( ( s_comp ( var_get ( user_data ), prev_data, 0, 0,
                                                   'M', &rc ) != 0)  &&
                       ( s_len  ( var_get ( user_data ) ) != 0    )
                     )
             {
                /*+---------------------------------------------------+*/
                /*| Here user wants to change the data                |*/
                /*+---------------------------------------------------+*/
                select = 0;

                #D_say   ( " User Wants to change the COMPONENT ");

                obj_set_mode( ocb_ptr, OBJ_RAW );

                rc = ocd_stoc( ocb_ptr, ocd_ptr, var_get ( user_data ) );

                obj_reset_mode( ocb_ptr, OBJ_RAW );

                if ( rc == 0 )
                {
                   #D_say   ( " COMPONENT is replaced in the OBJECT");

                   obj_set_mode( ocb_ptr, OBJ_RAW );

                   #D_str  ('data_str','data_str');
                   rc = ocd_ctos( ocb_ptr, ocd_ptr, &data_str ) ;

                   obj_reset_mode( ocb_ptr, OBJ_RAW );
                   ocd_ptr -> bias = 0 ;
                   s_copy(&prev_data, data_str, 0, 0);
                   #D_str('data_str','data_str');
                   #D_mdump('data_str','length+4');


                   f_setfld ( form_id, out_data, data_str);

                   f_setfld ( form_id, message,   rep_comp_msg);
                   t_alarm  ( process_id );
                 }
                 else
                 {
                   #D_say   ( " COMPONENT is not replaced ");
                   f_setfld ( form_id, message, no_rep_msg );
                   t_alarm  ( process_id );

                   f_rstfld ( form_id, NO_FLAGS, user_data   );
                   var_clr  ( user_data   );

                   f_rstfld ( form_id, NO_FLAGS, out_data );
                   var_clr  ( out_data   );
                 }

              }

              else
              {

                #D_say   ( "User requested ENTER ");
             /* f_rstfld ( form_id, NO_FLAGS, message );  */
             /* var_clr  ( message );                     */

                f_rstfld ( form_id, NO_FLAGS, user_data   );
                var_clr  ( user_data   );

                if ( key_select == 2 )
                {
                    f_rstfld ( form_id, NO_FLAGS, key_form    );
                    var_clr  ( key_form    );
                    s_nulout ( & ( ocd_ptr -> key ) ) ;
                }
                else if ( key_select == 0 )
                {
                    f_rstfld ( form_id, NO_FLAGS, key_form    );
                    var_clr  ( key_form    );
                    s_nulout ( & ( ocd_ptr -> key ) ) ;

                    f_rstfld ( form_id, NO_FLAGS, offset_str  );
                    var_clr  ( offset_str );

                }
              }
          }

          f_rstfld ( form_id, NO_FLAGS, out_data  );
          var_clr  ( out_data );

          f_rstfld ( form_id, NO_FLAGS, user_data );
          var_clr  ( user_data );

          f_rstfld ( form_id, NO_FLAGS, message   );
          var_clr  ( message );

          if ( key_select == 2 )
          {
                f_rstfld ( form_id, NO_FLAGS, key_form    );
                var_clr  ( key_form    );
                s_nulout ( & ( ocd_ptr -> key ) ) ;
          }
          else if ( key_select == 0 )
          {

               f_rstfld ( form_id, NO_FLAGS, key_form  );
               var_clr  ( key_form    );
               s_nulout ( & ( ocd_ptr -> key ) ) ;

               f_rstfld ( form_id, NO_FLAGS, offset_str);
               var_clr  ( offset_str );
          }
        }
      }
      if ( s_comp ( abandon, return_string, 0, 0, 'M', &rc ) == 0 )
      {
         return_code = 1 ;
      }

      obj_cls ( ocb_ptr );


      f_rstfld( form_id, NO_FLAGS, component );
      var_clr ( component );

      f_rstfld( form_id, NO_FLAGS, user_data );
      var_clr ( user_data );

      f_rstfld( form_id, NO_FLAGS, out_data );
      var_clr ( out_data );

      f_rstfld ( form_id, NO_FLAGS, message   );
      var_clr  ( message );

      f_rstfld ( form_id, NO_FLAGS, key_form  );
      var_clr  ( key_form    );

      f_rstfld ( form_id, NO_FLAGS, offset_str);
      var_clr  ( offset_str );

      f_close ( form_id , NO_FLAGS );

      var_clr ( lfd_form );
      var_clr ( oid_form );
      var_clr ( goid_form);
      var_clr ( object   );

      s_drop   ( & form_id  );
      s_drop   ( & abandon  );
      s_drop   ( & lfd_str  );
      s_drop   ( &data_str  );
      s_drop   ( & goid_str );
      s_drop   ( & obj_str  );
      s_drop   ( & oid_str  );
      s_drop   ( & component);
      s_drop   ( & user_data);
      s_drop   ( & prev_data);
      s_drop   ( & out_data );
      s_drop   ( &oid_form  );
      s_drop   ( &lfd_form  );
      s_drop   ( &goid_form );
      s_drop   ( &object    );
      s_drop   ( &no_comp_msg);
      s_drop   ( &rep_comp_msg);
      s_drop   ( &quit);

 }
 else
 {

     return_code = INVALID_OID_POINTER ;

 }
#D_return( return_code );
}
