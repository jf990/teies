/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ODTTEST.C                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 01, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Test the ODT functions:                                |
  |            odt_make() to make an ODT;  odt_save to store an ODT;  |
  |            odt_ptr() to locate an ODT;  odt_load to load an ODT;  |
  |            odt_link() to link ODT's; odt_find() to locate an OCD. |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/



#define D_OUTFILE "teies bugout B"
#define D_DUALOUT FALSE


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf.h"
#include "obj.h"
#include "form.h"
#include "tm.h"
#include "teies.h"
#include "version.h"
#include "debugi.h"

extern void printf();




main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


    sint       rc          =      0;
    string     holder      = S_NULL;
    string     config_file = S_NULL,
               version     = S_NULL,
               spacer      = S_NULL;


#D_main  ( ODTTEST,  OBJ  )



s_init( &version, VERSION_ID );


if( argc > 1 )
{

   #D_show( 'ODTTEST Started with %d args','argc' );
   s_init( &spacer, " " );
   while( --argc )
   {

      ++argv;
      s_init( &holder, *argv );
      s_cat(  &config_file, config_file, holder, 0, 0 );
      s_cat(  &config_file, config_file, spacer, 0, 0 );

   }
}
else
{

   s_init( &config_file, "* * SYSTEM OBJECT" );

}
#D_str( 'config_file','Input arguments'            );


   t_tmini();


   #D_say( 'Allocating ODT list table space:' );
   TM_master->odt = os_mema( sizeof( struct odtlist )*NUMBER_OF_ODTs );
   #D_show( 'TM_master->odt = 0x%x',  'TM_master->odt' );

                                  /*  - - - - - - - - - - - - - - - -  */
                                  /* Now load the system ODT's         */
                                  /* NOTE: When ODT's come from TEIES  */
                                  /* objects we have to run o_odt().   */
                                  /*  - - - - - - - - - - - - - - - -  */

   (TM_master->odt+ 0)->odt_ptr  = odt_load(  SODTI_OBJ) ;
   (TM_master->odt+ 0)->obj_type =            SODTI_OBJ  ;

   (TM_master->odt+ 1)->odt_ptr  = odt_load(    PSI_OBJ) ;
   (TM_master->odt+ 1)->obj_type =              PSI_OBJ  ;

   (TM_master->odt+ 2)->odt_ptr  = odt_load(  SOIDI_OBJ) ;
   (TM_master->odt+ 2)->obj_type =            SOIDI_OBJ  ;

   (TM_master->odt+ 3)->odt_ptr  = odt_load(  SERRI_OBJ) ;
   (TM_master->odt+ 3)->obj_type =            SERRI_OBJ  ;

   (TM_master->odt+ 4)->odt_ptr  = odt_load(    SYS_OBJ) ;
   (TM_master->odt+ 4)->obj_type =              SYS_OBJ  ;

   (TM_master->odt+ 5)->odt_ptr  = odt_load(   USER_OBJ) ;
   (TM_master->odt+ 5)->obj_type =             USER_OBJ  ;

                                  /* Null out the rest of the odtLIST  */

   for (count = 6 ; count <= NUMBER_OF_ODTs - 1 ; count ++)
   {
      (TM_master->odt+count)->odt_ptr  = NIL  ;
      (TM_master->odt+count)->obj_type = 0    ;
   }
#D_return( 0 );

}
