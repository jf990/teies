/*********************************************************************


:NAME.    GKS_X.H
:LEVEL.   GKS
:DATE.    June 23, 187
:AUTHOR.  Kenneth Liang

:PURPOSE.

     This header file contains the declarations of GKS routines called
     by GKS user modules.

:LIMITS.
    There are some restrictions on the use of multiple input fields.
    G_field{}, g_redraw{}, and g_rdwseg{} display input fields on the
    screen and also add a new field to the list of input fields
    associated with the specified workstation. The list of fields is
    removed ONLY when grq_mstr{} is called.

    CAUTION: Repeated use of g_redraw AND G_rdwseg without calling
             grq_mstr may cause unwanted input field{s} on the screen.

<MODS>       March 8, 1989     JF       Added prototyping.
</DOC>
*********************************************************************/





/* $EXTRACT     $MAKER-PROTO                                           */
                             /******************************************/
                             /*   <<<    GKS CONTROL FUNCTIONS   >>>   */
                             /******************************************/

extern sint gini_gks( sint numtypes, string * wknames, sint * wktypes );
                                    /* ------------------------------- */
                                    /* Initialize the GKS.             */
                                    /*   sint    numwkst               */
                                    /*   string  wknames[], wktypes[]  */
                                    /* ------------------------------- */

extern    sint    gopn_gks( tint err_file, sint seg_size )     ;

                                    /* Open the GKS system for use     */
                                    /* Arg is error log pointer        */
                                    /* ------------------------------- */

extern    sint    gcls_gks( void )     ;

                                    /* Close GKS and input/display sys */
                                    /*   No args                       */
                                    /* ------------------------------- */
                             /******************************************/
                             /*   <<<    WORKSTATION FUNCTIONS   >>>   */
                             /******************************************/

extern    sint    gopn_wk( sint work_id, sint conn_id, sint type )      ;

                                    /* ------------------------------- */
                                    /* Open a workstation              */
                                    /*   sint   work_id                */
                                    /*   string device                 */
                                    /*   sint   wk_type                */
                                    /* ------------------------------- */

extern    sint    gcls_wk( sint work_id )      ;

                                    /* Close a workstation             */
                                    /*   sint   work_id                */
                                    /* ------------------------------- */

extern    sint    gact_wk( sint work_id )      ;

                                    /* Activate a workstation, making  */
                                    /* it the current device for output*/
                                    /*   sint   work_id                */
                                    /* ------------------------------- */

extern    sint    gdct_wk( sint work_id )      ;

                                    /* Deactivate a workstation        */
                                    /*   sint   work_id                */
                                    /* ------------------------------- */

extern    sint    gclear_wk( sint work_id, byte flag )    ;

                                    /* Clear  workstation              */
                                    /*   sint wk_id, flag              */
                                    /* ------------------------------- */

extern    sint    gupdate_wk( sint work_id, byte flag )   ;

                                    /* Update workstation              */
                                    /*   sint wk_id, flag              */
                                    /* ------------------------------- */
                             /******************************************/
                             /*   <<< TRANSFORMATION FUNCTIONS   >>>   */
                             /******************************************/

extern    sint    gclr_vi( sint workid, sint transnum )      ;

                                    /* ------------------------------- */
                                    /* Clear viewport area of a        */
                                    /*   workstation.                  */
                                    /*   sint workid, transnum         */
                                    /* ------------------------------- */

extern sint gs_wind( sint transnum,
                     real xmin, real xmax, real ymin, real ymax
                   );

                                    /* Set the world window            */
                                    /*   sint trans                    */
                                    /*   real xmin, xmax, ymin, ymax   */
                                    /* ------------------------------- */

extern sint gs_view( sint transnum,
                     real xmin, real xmax, real ymin, real ymax
                   );

                                    /* Set the world viewport          */
                                    /*   sint trans                    */
                                    /*   real xmin, xmax, ymin, ymax   */
                                    /* ------------------------------- */

extern    sint    g_seltrn( sint transnum )     ;

                                    /* Select the current transformatio*/
                                    /* number                          */
                                    /*   sint  trans                   */
                                    /* ------------------------------- */

extern    sint    gs_clip( byte clip_ind )      ;

                                    /* Set clipping indicator          */
                                    /*   byte clip_ind                 */
                                    /* ------------------------------- */
                             /******************************************/
                             /* WORKSTATION TRANSFORMATION FUNCTIONS   */
                             /******************************************/

extern sint gswk_wind( sint work_id,
                       real xmax, real xmin, real ymax, real ymin
                     );
                                    /* ------------------------------- */
                                    /* Set workstation  window         */
                                    /*   sint wkid                     */
                                    /*   real xmin, xmax, ymin, ymax   */
                                    /* ------------------------------- */

extern    sint    gswk_view( sint work_id,
                             real xmax, real xmin, real ymax, real ymin
                           );
                                    /* Set workstation viewport        */
                                    /*   sint wkid                     */
                                    /*   real xmin, xmax, ymin, ymax   */
                                    /* ------------------------------- */

extern    sint    gqmax_disp( sint   workid,
                              sint * device_unit,
                              real * max_xdc,
                              real * max_ydc,
                              real * max_xras,
                              real * max_yras
                            );
                                    /* Find out the size of a worksta- */
                                    /* tion's display surface.         */
                                    /*   sint workid                   */
                                    /*   sint *device_unit             */
                                    /*   real *max_xdc, *max_ydc       */
                                    /*   real *max_xras, *max_yras     */
                                    /* ------------------------------- */
                             /******************************************/
                             /* TEXT OUTPUT PRIMITIVES                 */
                             /******************************************/

extern    sint    g_ttext( real xw, real yw, string tstr )      ;

                                    /* ------------------------------- */
                                    /* TEIES string display function   */
                                    /* Args are X and Y coords of char */
                                    /* start location {real} and the   */
                                    /* ptr {TEIES}                     */
                                    /* ------------------------------- */

extern    sint    g_text( real xw, real yw, byte * cstr )       ;

                                    /* C string display function       */
                                    /* Args are same as G_TTEXT except */
                                    /* string pointer is to a C string.*/
                                    /* ------------------------------- */

extern    sint    gst_color( sint index )    ;

                                    /* Set text color index            */
                                    /*   sint index                    */
                                    /* ------------------------------- */
                             /******************************************/
                             /* SEGMENT FUNCTIONS                      */
                             /******************************************/

extern   sint      gcreat_seg( sint segname );

                                    /* ------------------------------- */
                                    /* Open a segment                  */
                                    /*      sint segname               */
                                    /* ------------------------------- */

extern   sint      gcls_seg( void );

                                    /* Finish making a segment         */
                                    /* ------------------------------- */

extern   sint      gdelet_seg( sint segname );

                                    /* Delete the segment entirely,    */
                                    /* including removing its display  */
                                    /* from any workstations on which  */
                                    /* it is visible                   */
                                    /*      sint segname               */
                                    /* ------------------------------- */

extern   sint      g_redraw  ( sint workid );

                                    /* Redraw all segments stored in   */
                                    /* a workstation SL                */
                                    /*      sint workid                */
                                    /* ------------------------------- */

extern   sint      g_rdwseg  ( sint workid, sint segid );

                                    /* Redraw a segment stored in      */
                                    /* a workstation SL                */
                                    /*      sint workid                */
                                    /*      sint segid                 */
                                    /* ------------------------------- */
                             /******************************************/
                             /* SEGMENT ATTRIBUTES                     */
                             /******************************************/

extern   sint      gs_svisi( sint segname, sint visib );

                                    /* ------------------------------- */
                                    /* Visibility is set to 1 visible*/
                                    /* or 0 invisible. Default visibl*/
                                    /*      sint segname               */
                                    /*      sint visibility            */
                                    /* ------------------------------- */

extern   sint      gs_shigh( sint segname, sint highlight );

                                    /* Highlighting is set to a new    */
                                    /* value                           */
                                    /*      sint segname               */
                                    /*      sint highlighting          */
                                    /* ------------------------------- */

extern   sint      gs_scolor( sint segname, sint color );

                                    /* Segment color is set to a new   */
                                    /* value                           */
                                    /*      sint segname               */
                                    /*      sint color {index}         */
                                    /* ------------------------------- */

extern   sint      gs_sprio( sint segname, sint priority );

                                    /* The segment's priority is set to*/
                                    /* a number between 0 and 1.       */
                                    /*      sint segname               */
                                    /*      sint priority              */
                                    /* ------------------------------- */

extern   sint      gs_stran( sint segname, matrix * transmtx );

                                    /* Sets the segment transformation */
                                    /* matrix to the given value.      */
                                    /*      sint segname               */
                                    /*      matrix transmtx            */
                                    /* ------------------------------- */

extern   sint      gs_sdetec( sint segname, sint detect );

                                    /* Detectability is set to 0       */
                                    /* undetectable or 1 detectable    */
                                    /*      sint segname               */
                                    /*      sint detectability         */
                                    /* ------------------------------- */
                             /******************************************/
                             /* FIELD FUNCTIONS                        */
                             /******************************************/

extern   sint      g_field( real   xw,
                            real   yw,
                            sint   type,
                            sint   max,
                            string tstr,
                            sint   echo,
                            sint   highlight,
                            byte   * flag
                          );
                                    /* ------------------------------- */
                                    /* Set an input or output field    */
                                    /*   real    xw, yw                */
                                    /*   sint    type                  */
                                    /*   sint    max                   */
                                    /*   string  tstr                  */
                                    /*   sint    echo                  */
                                    /*   hword   highlight             */
                                    /* ------------------------------- */

extern   sint      grq_mstr( sint workid, sint * key );

                                    /* Request strings from multiple   */
                                    /* input fileds.                   */
                                    /*   sint    workid                */
                                    /*   sint    key {1-24 PFs,0 ENTER}*/
                                    /* ------------------------------- */

                             /******************************************/
                             /* INPUT INQUIRY FUNCTIONS                */
                             /******************************************/
                                    /* ------------------------------- */
                                    /* ------------------------------- */
                             /******************************************/
                             /* LOCATOR FUNCTIONS                      */
                             /******************************************/

extern    sint    g_loc_ini( sint       workid,
                             sint       locid,
                             position * ipos,
                             sint       transnum,
                             sint       type,
                             area     * echoarea,
                             tint     * datarec
                           );
                                    /* ------------------------------- */
                                    /* Initializes locator input device*/
                                    /*   sint    workid                */
                                    /*   sint    locid                 */
                                    /*   position ipos                 */
                                    /*   sint    transnum              */
                                    /*   sint    type                  */
                                    /*   area    echoarea              */
                                    /*   tint    datarec               */
                                    /* ------------------------------- */

extern    sint    gsloc_mode( sint workid,
                              sint locid,
                              sint opmode,
                              sint echo
                            );
                                    /* Sets operating-mode entry for a */
                                    /* locator device                  */
                                    /*   sint    workid                */
                                    /*   sint    locid                 */
                                    /*   sint    opmode                */
                                    /*   sint    echo                  */
                                    /* ------------------------------- */

extern    sint    grq_loc ( sint       workid,
                            sint       locid,
                            sint     * status,
                            sint     * transnum,
                            position * wcpos
                          );
                                    /* Requests the current position of*/
                                    /* a locator device connected to a */
                                    /* specified workstation.          */
                                    /*   sint    workid                */
                                    /*   sint    locid                 */
                                    /*   sint   *status                */
                                    /*   sint   *transnum              */
                                    /*   position *locpos              */
                                    /* ------------------------------- */

extern sint gsview_pri( sint trans, sint reftrans, sint relprio );

                                    /* Set viewport input priority     */
                                    /*   sint    trans                 */
                                    /*   sint    reftrans              */
                                    /*   sint    relprio               */
                                    /* ------------------------------- */
                             /******************************************/
                             /* STRING  FUNCTIONS                      */
                             /******************************************/

extern    sint    g_str_ini( sint   workid,
                             sint   strid,
                             string istr,
                             sint   type,
                             area * echoarea,
                             tint * datarec
                           );
                                    /* ------------------------------- */
                                    /* Initializes string  input device*/
                                    /*   sint    workid                */
                                    /*   sint    strid                 */
                                    /*   string  istr                  */
                                    /*   sint    type                  */
                                    /*   area    echoarea              */
                                    /*   tint    datarec               */
                                    /* ------------------------------- */

extern    sint    gsstr_mode( sint workid,
                              sint strid,
                              sint opmode,
                              sint echo
                            );
                                    /* Sets operating-mode entry for a */
                                    /* string device                   */
                                    /*   sint    workid                */
                                    /*   sint    strid                 */
                                    /*   sint    opmode                */
                                    /*   sint    echo                  */
                                    /* ------------------------------- */

extern    sint    grq_str ( sint workid,
                            sint strid,
                            sint * status,
                            string tstr
                          );
                                    /* Requests a character-string     */
                                    /* input from a string device con- */
                                    /* nected to specified workstation.*/
                                    /*   sint    workid                */
                                    /*   sint    locid                 */
                                    /*   sint   *status                */
                                    /*   string  str                   */
                                    /* ------------------------------- */
                             /******************************************/
                             /*                                        */
                             /******************************************/

extern    sint    gs_highl( hword highlight ) ;

                                    /* ------------------------------- */
                                    /* Set current highlighting value  */
                                    /*   hword   highlight             */
                                    /* ------------------------------- */

extern    sint    g_alarm( sint work_id ) ;

                                    /* Make a workstation to beep      */
                                    /*   sint    workid                */
                                    /* ------------------------------- */
extern    sint    g_dump( void ) ;
                                    /* Display GKS state list, GKS     */
                                    /*   description table, workstation*/
                                    /*   state lists, workstation      */
                                    /*   description tables            */
                                    /* ------------------------------- */



/* $ENDEXTRACT */
