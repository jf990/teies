/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  :NAME.    formp_x.h   ---  HEADER FILE                           |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |  :DATE.    March 21, 1988                                         |
  |  :LEVEL.   FORMS                                                  |
  |  :PURPOSE. To provide external declarations to internal forms     |
  |            functions and a source file for MAKER for the          |
  |            Form's Parser.                                         |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |  :LIMITS.  None known.                                            |
  |  :MODS.    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |            JF  Aug 28 89  Prototyped.                             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


/*                  $EXTRACT               $MAKER-PROTO                */


/* +-----------------------------------------------------------------+ */
/* |   FORMS SUBLEVEL     -----     PARSER                           | */
/* +-----------------------------------------------------------------+ */

extern  sint    f_parse  ( OCB * ocb, OCD * ocd ) ;
extern  sint    f_qual   ( string qual, f_qualifiers m_quals ) ;

extern  sint    fexe_cmd( string_vector str_vec, struct f_prof_s *f_prof,
                   struct f_fmat_s * f_fmat, struct f_prof_s * f_deft ) ;

                                  /* Identifies the Mark-up            */

extern  string  farg_var ( string line, struct f_prof_s * f_prof,
                                             struct f_fmat_s * f_fmat ) ;

                                  /* Gets any text embedded variables  */

extern sint farg_li   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* List Item arguments               */


extern sint farg_rl   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* Repeating list arguments          */

extern sint farg_hp   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* Highlighted Phrase arguments      */

extern sint farg_ctr  ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Center markup arguments      */

extern sint farg_h    ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Header markup arguments      */

extern sint farg_att  ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Attach markup arguments      */


extern sint farg_fld  ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Field markup arguments       */

extern sint farg_sp   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Space markup arguments       */


extern sint farg_xmp  ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /*      Example markup arguments     */


extern sint farg_pf   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* Program Function arguments        */

extern sint farg_dl   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* Definition List arguments         */

extern sint farg_if   ( string_vector str_vec, sint cmd_num,
                   struct f_prof_s * f_prof, struct f_prof_s * f_deft,
                                             struct _f_exe_cmd * tags ) ;

                                  /* If markup arguments               */


                                                        /* $ENDEXTRACT */
