/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    mk_psi   ()                                            |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka, John L. Foster                         |
  |                                                                   |
  |  :DATE.    June  1, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :PURPOSE. This main routine creates and saves the PSI and SOIDI  |
  |            object definitions without starting up the entire OBJ  |
  |            level.  This is important if those objects or the ODT  |
  |            structure ever changes.                                |
  |                                                                   |
  |  :OUTPUT.  PSI ODT and SOIDI ODT files                            |
  |                                                                   |
  |  :CALLS.   odt_disp (), odt_save ()                               |
  |                                                                   |
  |  :ERRORS.  None.                                                  |
  |                                                                   |
  |  :LIMITS.  Blindly overwrites the file if it already exists.      |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "isv.h"
#include "tm.h"
#include "teies.h"



main ()
{

   string        odt          = S_NULL;
   string        component    = S_NULL;
   odtrecord    *odt_psi      =   NULL;
   odtrecord    *odt_soidi    =    NIL;
   odtrecord    *odt_top     =    NIL;
   byte          prvset (:6:)         ;
   sint          n            = 0     ;

   #D_main(MK_PSI   , OBJ );






   odt_psi = (ODT *) os_mema (sizeof (odtrecord) * 8) ;
   os_memn( odt_psi, sizeof( odtrecord ) * 8 );
   odt_top = odt_psi;

   prvset (:0:) = '\0' ;          /* Null out the privs field          */
   prvset (:1:) = '\0' ;
   prvset (:2:) = '\0' ;
   prvset (:3:) = '\0' ;
   prvset (:4:) = '\0' ;
   prvset (:5:) = '\0' ;

   #D_show ('odt_top=0x%X after os_mema','odt_top') ;

   s_init( &component, "PSI"  );
   odt_add(&odt_psi,&n,component  ,000,00000,00000,00000,PSI_OBJ,prvset);

   n = 0 ;                        /* Zero counter                      */

 /*                 "NAME        ",lvl,offst,next ,prev ,type   ,privs */
   s_init( & component,"PSI" );
   odt_add(&odt_psi,&n,component  ,0  ,00000,    1,    1,OC_HSEG,prvset);
   s_init( &component, "INDEX" );
   odt_add(&odt_psi,&n,component  , 1 ,00000,    2,    2,OC_NDX ,prvset);
   s_init( &component, "SYS_ID" );
   odt_add(&odt_psi,&n,component  ,  2,0    ,    4,    7,OC_STR ,prvset);
   s_init( &component, "TYPE"   );
   odt_add(&odt_psi,&n,component  ,  2,    0,    5,    3,OC_HWRD,prvset);
   s_init( &component, "OID"    );
   odt_add(&odt_psi,&n,component  ,  2,    2,    6,    4,OC_OID ,prvset);
   s_init( &component, "LFD"    );
   odt_add(&odt_psi,&n,component  ,  2,   10,    7,    5,OC_HWRD,prvset);
   s_init( &component, "GOID"   );
   odt_add(&odt_psi,&n,component  ,  2,   12,    3,    6,OC_INT ,prvset);
   s_drop( &component );
   odt_lvl (odt_top) =  1    ;  /* Number of level 0's in the ODT    */
   odt_off (odt_top) =  n    ;  /* Number of elements in the ODT     */
   odt_nxto(odt_top) =  0    ;  /* ISV reference number              */

   #D_show ('there are %3d elements in the PSI table','n') ;
   #D_show ('should be %3d elements in the PSI table','7') ;

   odt_disp ( odt_top ) ;
                                  /* Save the ODT                      */
   s_init( &odt, "PSI" );
   odt_save ( odt_top  , odt ) ;
   s_drop( &odt );
   os_memd( odt_top );


   odt_soidi = (ODT *) os_mema (sizeof (odtrecord) * 7) ;
   odt_top  = odt_soidi;
   s_init( &component, "SOIDI");
   odt_add(&odt_soidi,&n,component,0,0,0,0,SOIDI_OBJ,prvset);

   n = 0 ;                        /* Zero counter                      */

 /*                 "NAME        ",lvl,offst,next ,prev ,type   ,privs */
   s_init( & component,"SOIDI" );
   odt_add(&odt_soidi,&n,component  ,0  ,00000,    1,    1,OC_HSEG,prvset);
   s_init( &component, "INDEX" );
   odt_add(&odt_soidi,&n,component  , 1 ,00000,    2,    2,OC_NDX ,prvset);
   s_init( &component, "OID"    );
   odt_add(&odt_soidi,&n,component  ,  2,0    ,    4,    6,OC_OID ,prvset);
   s_init( &component, "LFD"    );
   odt_add(&odt_soidi,&n,component  ,  2,    0,    5,    3,OC_HWRD,prvset);
   s_init( &component, "GOID"   );
   odt_add(&odt_soidi,&n,component  ,  2,    2,    6,    4,OC_INT ,prvset);
   s_init( &component, "TYPE"   );
   odt_add(&odt_soidi,&n,component  ,  2,   10,    3,    5,OC_HWRD,prvset);
   s_drop( &component );
   odt_lvl (odt_top  ) =  1    ;  /* Number of level 0's in the ODT    */
   odt_off (odt_top  ) =  n    ;  /* Number of elements in the ODT     */
   odt_nxto(odt_top  ) =  0    ;  /* ISV reference number              */

   #D_show ('there are %3d elements in the SOIDI table','n') ;
   #D_show ('should be %3d elements in the SOIDI table','6') ;

   odt_disp ( odt_top ) ;
                                  /* Save the ODT                      */
   s_init( &odt, "SOIDI" );
   odt_save ( odt_top,   odt ) ;
   s_drop( &odt );
   os_memd( odt_top   );


   #D_return(0) ;
}
