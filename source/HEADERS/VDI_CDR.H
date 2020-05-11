/*********************************************************************

:NAME.    VDI_CDR.H
:LEVEL.   VDI internal
:DATE.    September, 14, 1987
:AUTHOR.  Kenneth Liang

:PURPOSE. Defines constants, macros, and declarations used in Virtual
          Device Driver for IBM 3270 console.

:MODS.

:END.

 ***********************************************************************/








/**********************************************************************

    VDI internal macros.

**********************************************************************/

#define  CUR_LOC( buf ) ( (*((buf)+1)&0x3F) * 64 + (*((buf)+2)&0x3F) )



/**********************************************************************

    Write mode

**********************************************************************/

#define  EW        0    /* Erase/Write                */
#define  EWA       1    /* Erase/Write/Alternate      */
#define  W         2    /* Write                      */








/* $EXTRACT */



/*********************************************************************

    VDI Driver routines for IBM 3270 terminal

**********************************************************************/

                                       /* -------------------------- */
extern sint   vd_init       ();        /* Initialize 3270 terminal.  */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   sint      * out          */
                                       /* -------------------------- */
extern sint   vd_clrwk      ();        /* Clear display surface of   */
                                       /* 3270 terminal.             */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vd_clrarea    ();        /* Clear a specified display  */
                                       /* area.        .             */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   area      * view         */
                                       /* -------------------------- */
extern sint   vd_flush      ();        /* Process all pending updates*/
                                       /* to device.                 */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vd_prtscr     ();        /* Print the display surface  */
                                       /* of the device to a file.   */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   string      GFD;         */
                                       /* -------------------------- */
extern sint   vd_write      ();        /* Output a text string to    */
                                       /* 3270 terminal display      */
                                       /* surface.                   */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdrq_loc      ();        /* Request input locator.     */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdsm_loc      ();        /* Sample input locator.      */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdrq_chr      ();        /* Request input character.   */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdsm_chr      ();        /* Sample input character.    */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdrq_str      ();        /* Request input string.      */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdrq_mstr     ();        /* Request multiple input     */
                                       /*   strings.                 */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vdsm_str      ();        /* Sample input string.       */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
extern sint   vds_field     ();        /* Set one input field on     */
                                       /* screen                     */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * buf          */
                                       /*   hword     addr           */
                                       /*   sint      max            */
                                       /*   sint      echo           */
                                       /*   hword     highlight      */
                                       /* -------------------------- */
extern sint   vd_alarm      ();        /* Ring the bell of a device  */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   byte      * arg_ptr      */
                                       /* -------------------------- */
                                  /***********************************/
                                  /* Internal routines for IBM 3270  */
                                  /* console device driver           */
                                  /***********************************/
                                       /* -------------------------- */
extern sint   vd_xytol      ();        /* Convert NDC to linear      */
                                       /* buffer address.            */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   NDCoord   *x_ndc, *y_ndc */
                                       /*   hword       l            */
                                       /* -------------------------- */
extern sint   vd_ltoxy      ();        /* Convert linear buffer      */
                                       /* address to NDC.            */
                                       /*   VDIdevice * dev_ptr      */
                                       /*   hword       l            */
                                       /*   NDCoord   *x_ndc, *y_ndc */
                                       /* -------------------------- */


/* $ENDEXTRACT */
