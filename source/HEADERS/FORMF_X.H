/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    formf_x.h   ---  HEADER FILE                           |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    March 21, 1988                                         |
  |  <LEVEL>   FORMS                                                  |
  |  <PURPOSE> To provide external declarations to internal forms     |
  |            functions and a source file for MAKER for the          |
  |            Form functions.                                        |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
/*                 $EXTRACT           $MAKER-PROTO                     */
/* +-----------------------------------------------------------------+ */
/* |   FORMS SUBLEVEL     -----     FFUN                               */
/* +-----------------------------------------------------------------+ */

                                  /* Process heading markup            */
extern  sint    f_h     ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_eh    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process highlight phrase mark     */
extern  sint    f_hp    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ehp   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process paragraph markup          */
extern  sint    f_p     ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ep    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process top matter markup         */
extern  sint    f_top_m ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_etop_m( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process bottom matter markup      */
extern  sint    f_bot_m ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ebot_m( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process space markup              */
extern  sint    f_sp    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );


                                  /* Process center text markup        */
extern  sint    f_ctr   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ectr  ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process def list markup           */
extern  sint    f_dl    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_dt    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_dd    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_edl   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process ordered list markup       */
extern  sint    f_ol    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_eol   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process unordered list markup     */
extern  sint    f_ul    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_eul   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process simple list markup        */
extern  sint    f_sl    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_esl   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process remark markup             */
extern  sint    f_rem   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_erem  ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process repeating lists           */
extern  sint    f_rl    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_erl   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process a list item               */
extern  sint    f_li    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_li_err( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat,
                                                     boolean end_mark ) ;
extern  sint    f_li_pop( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat,
                                                            sint type ) ;
extern  sint    f_rl_li ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process figure markup             */
extern  sint    f_fig   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_efig  ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                  /* Process field markup              */
extern  sint    f_fld   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_sfield( string name, struct f_prof_s * f_prof,
                                            struct f_fmat_s * f_fmat ) ;
extern  INF   * fld_new  ( struct f_prof_s * f_prof ) ;
extern  INF   * fld_fnd  ( string id, sint stem     ) ;
extern  sint    fld_drp  ( string id, sint stem     ) ;
extern  sint    fld_clr  ( INF  * field_ptr         ) ;
extern  sint    fld_stem ( string * result, string source, sint stem ) ;
extern  sint    fld_dump ( void                     ) ;


                                  /* Process table markup              */
extern  sint    f_table ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ti    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_etable( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                    /* Process attach item markup      */
extern  sint    f_att   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );


                                    /* Process example markup          */
extern  sint    f_xmp   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_exmp  ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                    /* Process index markup            */
extern  sint    f_i     ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_ei    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                    /* Process insert note markup      */
extern  sint    f_not    ( void ) ;
extern  sint    f_enot   ( void ) ;


                                    /* Process footnote markup         */
extern  sint    f_fn    ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_efn   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );



                                    /* Process alarm markup            */
extern  sint    f_alarm ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );


extern  sint    f_else  ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );
extern  sint    f_endif ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );





/* +-----------------------------------------------------------------+ */
/* |   FORMS SUBLEVEL     -----     FORMS                              */
/* +-----------------------------------------------------------------+ */


                                    /* Process text for display        */
extern  sint    f_render ( struct f_fmat_s * f_fmat, string buffin, hword type,
                                                     byte * flag_ptr ) ;

                                    /* Process end-of-record conditions*/
extern  sint    f_eor   ( struct f_prof_s * f_prof, struct f_fmat_s * f_fmat );

                                  /* Determines if need to parse       */
extern  byte   *f_resolv ( string form_id, hword flags ) ;

                                  /* Default values assigned           */
extern  byte   *f_profil ( string form_id, OCB * form_ocb, OCD * form_ocd,
                                          OCB * obj_ocb, hword flags ) ;

                                  /* Determines where to put form      */
extern  sint    f_getorg ( real * org_x, real * org_y, sint * last_seg ) ;
                                  /* matches qualifier lists           */
extern  sint    f_qual   ( string  qual, f_qualifiers m_quals ) ;

                                  /* Parser and processor init         */
extern  sint    f_fbeg   ( string form_id, hword flags, OCB ** form_ocb,
                                                   OCD ** form_ocd ) ;

                                  /* Parser and processor end          */
extern  sint    f_fend   ( string form_id, form_dat * formdata, OCB * ocb ) ;

                                  /* Add form to parsed list           */
extern  form_dat * frm_add  ( string form_id, hword flags ) ;

                                  /* Remove form from parsed list      */
extern  sint       frm_drp  ( string form_id, hword flags ) ;

                                  /* Get PTR from parsed list          */
extern  form_dat * frm_inta ( string form_id, hword flags ) ;

                                  /* Dump list of parsed forms         */
extern  sint       frm_dump ( void ) ;

                                                        /* $ENDEXTRACT */
