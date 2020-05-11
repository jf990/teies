/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    form.h      ---  HEADER FILE                           |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    March 21, 1988                                         |
  |  <LEVEL>   FORMS                                                  |
  |  <PURPOSE> To provide external declarations for forms.            |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
  |  <MODS>    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |                                                                   |
  |  <MODS>    JF  Mar 01 89  Prototyping in place.                   |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/

/* $EXTRACT $MAKER-PROTO */



extern  sint  form     ( string form_id, OCB * obj_ocb, hword flags ) ;

                                  /* Call to the forms processor       */

/*
                string  form_id  ; Form ID of form to parse and render
                OCB    *obj_ocb  ; OCB of obj referenced by object fields
                hword   flags    ; Flags for forms control {defined in
                                      "form_com.h"}
                        NO_FLAGS        No flags
                        STATE_FORM      This is a state   form
                        HELP_FORM       This is a help    form
                        EXCEPT_FORM     This is an except form
                        OTHER_FORM      This is none of the above
                        DO_NOT_DISPLAY  Do not call PM just parse
                        SOUND_ALARM     Sound alarm when form is shown

                sint    key_hit  ; Command key {PF/ENTER/PA} that caused
                                      PM to end.  If error - S_NULL    */



extern sint   f_close  ( string form_id, hword flags ) ;

                                  /* Take a form out of the world and  */
                                  /*   deallocate all space associated */
                                  /*   with the form                   */
/*
                string  form_id  ; Form ID of object to deallocate
                hword   flags    ; Flag indicating type of form
                        STATE_FORM     This is a state   form
                        HELP_FORM      This is a help    form
                        EXCEPT_FORM    This is an except form
                        OTHER_FORM     This is none of the above
                sint    rc       ; Always 0.                           */



extern sint   f_minit  ( void ) ;
                                  /* Initializes parser lookup tables  */
                                  /*   and global vars for forms       */
                                  /*   WARNING: Call only once !!!!!   */
/*
                sint    rc       ; Always 0.                           */



extern sint   f_mend   ( void ) ;
                                  /* Deallocates parser lookup tables  */
                                  /*   and global vars for forms       */
                                  /*                                   */
/*
                sint    rc       ; Always 0.                           */



extern sint   f_rstfld ( string form_id, sint flags, string field_id ) ;

                                  /* Resets a field to the contents    */
                                  /*    derived at parsing time        */
/*
                string  form_id  ; Form ID of form to update
                hword   flags    ; Flag indicating type of form
                string  fld_id   ; Id of field to update
                sint    rc       ; -1 if error, and 0 otherwise.       */


extern sint   f_rstinf ( string form_id, hword flags ) ;

                                  /* Resets all  input fields to not   */
                                  /*    modified.                      */
/*
                string  form_id  ; Form ID of form to update
                hword   flags    ; Flag indicating type of form
                sint    rc       ; always 0                            */


extern sint   f_setfld ( string form_id, string field_id,
                                            string new_field_contents ) ;
                                  /* Updates the output field with a   */
                                  /*    a new value                    */
/*
                string  form_id  ; Form ID of form to update
                hword   flags    ; Flag indicating type of form
                string  fld_id   ; Id of field to update
                string  new_val  ; New value for field
                sint    rc       ; -1 if error, and 0 otherwise.       */


extern sint   f_updfld ( string form_id, hword form_flags  );

                                  /* Updates the source of all input   */
                                  /* fields the user modified on a form*/
                                  /* Returns error code if a field is  */
                                  /* bad or the form requested does not*/
                                  /* exist.                            */


extern boolean f_fmod( string form_id, hword form_flags, string field_id );

                                  /* Determine if a field has been     */
                                  /*    modified.                      */
/*
                string  form_id  ; Form ID of form to update
                hword   flags    ; Flag indicating type of form
                string  fld_id   ; Id of field to see if changed
                boolean rc       ; TRUE if changes, else FALSE         */

extern sint  f_curpos ( string form_id, sint flags, string fld_name,
                                               real pos_x, real pos_y ) ;
                                  /* Positions cursor at this field    */
                                  /*   WARNING: Will only work on the  */
                                  /*   next form displayed.            */
/*
                string  form_id  ; Form ID of form to update
                hword   flags    ; Flag indicating type of form
                string  fld_name ; Position cursor at this field
                sint    rc       ; -1 if error, and 0 otherwise.       */



extern form_dat *frm_inta ( string form_id, hword flags ) ;

                                  /* Returns a form_dat pointer to the */
                                  /*   requested form                  */
/*
                string    form_id ; Form ID of form to find
                hword     flags   ; Flag indicating type of form
                form_dat *ptr     ; Pointer to form's data structure   */

/* $ENDEXTRACT */
