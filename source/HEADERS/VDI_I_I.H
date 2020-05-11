/***********************************************************************

:NAME.    VDI_I_I.H
:LEVEL.   VDI
:DATE.    June 11, 1987
:AUTHOR.  Kenneth Liang

:PURPOSE. Declares external VDI functions

:MODS.

:END.

 ***********************************************************************/



/* $EXTRACT */

/**********************************************************************

    VDI dispatcher

**********************************************************************/

extern   sint vdi           ();        /* VDI dispatcher called by   */
                                       /* vdi functions to access    */
                                       /* driver routines of a       */
                                       /* particular device.         */
                                       /*   sint device              */
                                       /*   sint function            */
                                       /*   byte *arg_ptr            */
                                       /* Returns 0 if operation is  */
                                       /* successful.                */


/*********************************************************************

    Internal VDI functions

**********************************************************************/

                                       /* -------------------------- */
extern        VDIdevice    *v_intab(); /* Check if device is in table*/
                                       /*    sint   device           */
                                       /* returns 0 if device is not */
                                       /* in vditab, -1 if no dev's, */
                                       /* or ptr to device entry.    */
                                       /* -------------------------- */
extern sint   v_dc          ();        /* Convert Normalized Device  */
                                       /* Coordinates to Device      */
                                       /* Coordinates.               */
                                       /*   VDIdevice *dev_ptr       */
                                       /*   NDCoord   x_ndc, y_ndc   */
                                       /*   DCoord    x_dc,  y_dc    */
                                       /* -------------------------- */
extern sint   v_ndc         ();        /* Convert Device Coordinates */
                                       /* into Normalized Device     */
                                       /* Coordinates.               */
                                       /*   VDIdevice *dev_ptr       */
                                       /*   DCoord    x_dc,  y_dc    */
                                       /*   NDCoord   x_ndc, y_ndc   */
                                       /* -------------------------- */
extern sint   v_dump        ();        /* Display VDI device table   */
                                       /*   - no arguments -         */
                                       /* -------------------------- */



/* $ENDEXTRACT */

