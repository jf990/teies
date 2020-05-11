/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    formd_x.h   ---  HEADER FILE                           |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    March 21, 1988                                         |
  |  <LEVEL>   FORMS                                                  |
  |  <PURPOSE> To provide external declarations to internal forms     |
  |            functions and a source file for MAKER for the          |
  |            Presentation Manager's functions.                      |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |            JF  Aug 28 89  Prototype, added pm routines.           |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
/*                       $EXTRACT                 $MAKER-PROTO         */
/* +-----------------------------------------------------------------+ */
/* |   FORMS SUBLEVEL     -----     PRESENTATION MANAGER             | */
/* +-----------------------------------------------------------------+ */

extern  hword   p_disp   ( form_dat * formdata, sint flags ) ;

                                    /* External interface to the PM    */

extern  sint    p_beg    ( string form_id, struct p_prof_s * p_prof ) ;

                                    /* Startup of the PM               */

extern  sint    p_end    ( struct p_prof_s * p_prof ) ;

                                    /* Termination of the PM           */

extern  boolean p_qhelp  ( form_dat * formdata, struct p_prof_s * p_prof,
                                                          hword flags ) ;

                                    /* Check input fields for '?'      */

extern  sint    p_help   ( form_dat * formdata, struct p_prof_s * p_prof ) ;
                                    /* get some HELP                   */

extern  sint    p_top    ( struct p_prof_s * p_prof ) ;

                                    /* TOP of form                     */

extern  sint    p_home   ( struct p_prof_s * p_prof ) ;

                                    /* HOME -- TOP and LEFT of form    */

extern  sint    p_bot    ( struct p_prof_s * p_prof ) ;

                                    /* BOTTOM of form                  */

extern  sint    p_next   ( struct p_prof_s * p_prof ) ;

                                    /* NEXT page in form               */

extern  sint    p_prev   ( struct p_prof_s * p_prof ) ;

                                    /* PREVIOUS page in form           */

extern  sint    p_hnext  ( struct p_prof_s * p_prof ) ;

                                    /* HALF-page NEXT in form          */

extern  sint    p_hprev  ( struct p_prof_s * p_prof ) ;

                                    /* HALF-page PREVIOUS in form      */

extern  sint    p_right  ( struct p_prof_s * p_prof ) ;

                                    /* RIGHT side of the form          */

extern  sint    p_left   ( struct p_prof_s * p_prof ) ;

                                    /* LEFT side of the form           */

extern  sint    p_show_p ( struct p_prof_s * p_prof ) ;

                                    /* Show current page               */

extern  sint    p_getpf  ( struct p_prof_s * profile_data, string function ) ;

                                    /* Given function return key value */

extern  boolean p_exepm  ( hword function_req, struct p_prof_s * p_prof ) ;

                                    /* Run a PM function               */


/*                                               $ENDEXTRACT           */
