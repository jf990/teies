/***********************************************************************

:NAME.    VDI_X.H
:LEVEL.   VDI
:DATE.    June 11, 1987
:AUTHOR.  Kenneth Liang

:PURPOSE. Declares external VDI functions

:MODS.

:END.

 ***********************************************************************/



/* $EXTRACT */

/*********************************************************************

    VDI functions

**********************************************************************/

                                       /* -------------------------- */
extern sint   vini_vdi      ();        /* Initialize Virtual Device  */
                                       /* -------------------------- */
extern sint   v_opnwk       ();        /* Open virtual device and    */
                                       /* update active device table.*/
                                       /*   tint work_in[19]       */
                                       /*   sint *device             */
                                       /*   tinit work_out[66]     */
                                       /* -------------------------- */
extern sint   v_clswk       ();        /* Close virtual device and   */
                                       /* update active device table */
                                       /*   sint device              */
                                       /* -------------------------- */
extern sint   v_name        ();        /* Set "human" name field in  */
                                       /* device table for a         */
                                       /* requested device.          */
                                       /*   sint      device         */
                                       /*   string    name           */
                                       /* -------------------------- */
extern sint   v_clrwk       ();        /* Clear device display       */
                                       /* surface.                   */
                                       /*   sint device              */
                                       /* -------------------------- */
extern sint   v_updwk       ();        /* Process all pending updates*/
                                       /* to a device.               */
                                       /*   sint device              */
                                       /* -------------------------- */
extern sint   v_clrarea     ();        /* Clear a specified viewport */
                                       /* area.                      */
                                       /*   sint      device         */
                                       /*   real      xmin           */
                                       /*   real      xmax           */
                                       /*   real      ymin           */
                                       /*   real      ymax           */
                                       /* -------------------------- */
extern sint   v_gtext       ();        /* Output string starting at  */
                                       /* x, y. Current text align-  */
                                       /* ment and attributes apply. */
                                       /*   sint      device         */
                                       /*   NDCoord   x, y           */
                                       /*   char      *str           */
                                       /* -------------------------- */
extern sint   vst_color     ();        /* Set text index color.      */
                                       /*   sint device              */
                                       /*   sint index               */
                                       /* -------------------------- */
extern sint   vs_highl      ();        /* Set highlighting attribute */
                                       /* of a device to a new value */
                                       /*   sint device              */
                                       /*   hword highlight          */
                                       /* -------------------------- */
extern sint   vrq_loc       ();        /* Request input locator.     */
                                       /*   sint          device     */
                                       /*   Array of NDC  xy         */
                                       /*   byte          ink        */
                                       /*   byte          rb         */
                                       /*   byte          echo       */
                                       /*   Array of NDC  xy2        */
                                       /*   byte          *t         */
                                       /* -------------------------- */
extern sint   vsm_loc       ();        /* Sample input locator.      */
                                       /*   sint           device    */
                                       /*   Array of NDC   xy        */
                                       /* -------------------------- */
extern sint   vrq_chr       ();        /* Request input character.   */
                                       /* Returns only when character*/
                                       /* is available.              */
                                       /*   sint device              */
                                       /*   char *c                  */
                                       /* -------------------------- */
extern sint   vsm_chr       ();        /* Sample input character.    */
                                       /*   sint device              */
                                       /*   char *c                  */
                                       /* -------------------------- */
extern sint   vrq_str       ();        /* Request input string.      */
                                       /* Returns only when string   */
                                       /* is available.              */
                                       /*   sint           device    */
                                       /*   hword          max       */
                                       /*   byte           echo      */
                                       /*   Array of NDC   xy        */
                                       /*   string         *str      */
                                       /* -------------------------- */
extern sint   vrq_mstr      ();        /* Request multiple input     */
                                       /*   strings                  */
                                       /* Returns only when string   */
                                       /* is available.              */
                                       /*   sint           device    */
                                       /*   field         *field     */
                                       /* -------------------------- */
extern sint   vsm_str       ();        /* Sample input string.       */
                                       /*   sint           device    */
                                       /*   hword          max       */
                                       /*   byte           echo      */
                                       /*   Array of NDC   xy        */
                                       /*   string         *str      */
                                       /* -------------------------- */
extern sint   v_alarm       ();        /* Ring the bell of a device  */
                                       /*   sint           device    */
                                       /* -------------------------- */




/* $ENDEXTRACT */

