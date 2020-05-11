/**********************************************************************

IBM3270.H                        October 24, 1986                   JF

                                                             Level: VDI


         Used for device dependant definitions to use the 3270
         class display devices.

:MODS.

    06/15/87  Ken Liang      Change SA from 0x00 to 0x28

    06/22/87  Ken Liang      Add NONDISPLAY




**********************************************************************/

#ifndef  IBM3270
#define  IBM3270   TRUE



#define  NON_GRAF       0x04

               /*  CCW Code values  */

#define  CCW_WRITE      0x29
#define  CCW_READ       0x2A
#define  CCW_WSF        0x29


               /*  CCW Write commands  */

#define  ERASE_WRITE    0x80    /*                                     */
#define  ERASE_WRITEA   0xC0    /*                                     */
#define  WRITE_SF       0x20    /*                                     */
#define  WRITE_RT       0x00    /*                                     */


               /*  CCW Read commands   */


#define  READ_MODIFIED  0x80    /*                                     */
#define  READ_ANY       0x00    /*                                     */
#define  READ_EF        0x10    /*                                     */


     /* 3270 I/O Commands (must be first character in CCW Buffer) */


#define  EW_CMD         0xF5 /* Erase/Write                            */
#define  RM_CMD         0xF6 /* Read Modified                          */
#define   W_CMD         0xF1 /* Write                                  */
#define  RB_CMD         0xF2 /* Read buffer                            */
#define  EA_CMD         0x6F /* Erase all unprotected fields           */
#define  RA_CMD         0x6E /* Read all modified fields               */
#define  WF_CMD         0xF3 /* Write Structured Field                 */


     /*  3270 display buffer Orders  */


#define  SBA            0x11    /* Start Buffer Address                */
#define  SF             0x1D    /* Start field                         */
#define  SFE            0x29    /* Start field extended                */
#define  MF             0x2C    /* Modify Field                        */
#define  SA             0x28    /* Set attribute                       */
#define  IC             0x13    /* Insert Cursor                       */
#define  PT             0x05    /* Program Tab                         */
#define  RA             0x3C    /* Repeat to Address                   */
#define  EUA            0x12    /* Erase Unprotected to Address        */
#define  GE             0x08    /* Graphic Escape                      */


     /*  3270 display buffer field order masks */


#define  DEFAULT        0x40    /* Default field attribute    0x40|0x00*/
#define  PROTECTED      0x60    /* Protected field            0x40|0x20*/
#define  NUMERIC        0x50    /* Number only field          0x40|0x10*/
#define  PEN_DETECT     0x4C    /* Light Pen detectable field 0x40|0x0C*/
#define  MDT            0x41    /* Modified data tag indicator0x40|0x01*/
#define  INTENSE3270    0x48    /* Intensity detect           0x40|0x08*/
#define  NONDISPLAY     0x4C    /* Non-display                0x40|0x0C*/
#define  RESET          0xFF    /* Reset field attribute mask          */
                                /* ----------------------------------- */
                                /* NOTE: 0x40 has to be added to field */
                                /*   attribute in SNA 3270 data stream */


      /*  3270 Write Control Character Masks  */


#define  P_FORMAT       0x30    /*                                     */
#define  PRINTER        0x08    /*                                     */
#define  ALARM          0x04    /*                                     */
#define  KEYBOARD       0x02    /*                                     */
#define  RESET_MDT      0x01    /*                                     */

#define  WCC            0xC0    /* For terminal I/O, start with this   */
                                /* mask value                          */



         /*  Highlighting Property     */

#define      DEFAULT3270               0x00
#define      NORMAL3270                0xF0
#define      BLINK3270                 0xF1
#define      REVERSE3270               0xF2
#define      UNDERLINE3270             0xF4



         /* Extended field attribute type   */

#define      ALLCHARATTR               0x00 /* All character attributes   */
#define      FATTRIBUTE                0xC0 /* 3270 Field attribute       */
#define      FVALIDATION               0xC1 /* Field validation           */
#define      FOUTLINE                  0xC2 /* Field outlining            */
#define      HIGHLIGHT3270             0x41 /* Extended highlighting      */
#define      COLOR3270                 0x42 /* Color                      */
#define      CHARACTER_SET             0x43 /* Character set              */
/* other                                       Reserved                   */



         /*  3279 style color attribute values */

#define      BLUE                      0xF1
#define      RED                       0xF2
#define      PINK                      0xF3
#define      GREEN                     0xF4
#define      TURQUOISE                 0xF5
#define      YELLOW                    0xF6
#define      WHITE                     0xF7



         /* Attention Identifier Key values  */

#define      NO_AID                    0x60
#define      ENTER                     0x7D
#define      PF1                       0xF1
#define      PF2                       0xF2
#define      PF3                       0xF3
#define      PF4                       0xF4
#define      PF5                       0xF5
#define      PF6                       0xF6
#define      PF7                       0xF7
#define      PF8                       0xF8
#define      PF9                       0xF9
#define      PF10                      0x7A
#define      PF11                      0x7B
#define      PF12                      0x7C
#define      PF13                      0xC1
#define      PF14                      0xC2
#define      PF15                      0xC3
#define      PF16                      0xC4
#define      PF17                      0xC5
#define      PF18                      0xC6
#define      PF19                      0xC7
#define      PF20                      0xC8
#define      PF21                      0xC9
#define      PF22                      0x4A
#define      PF23                      0x4B
#define      PF24                      0x4C
#define      CLEAR                     0x6D
#define      PA1                       0x6C
#define      PA2                       0x6E
#define      PA3                       0x6B





        /**************************************************/
        /* Values defined in fields CQYSECT returned from */
        /* from CONSOLE QUERY.                            */
        /* Refer to Virtual Machine/System Product        */
        /* CMS Console Facility Release 4, page 29-32     */
        /* SC24-5333-0                                    */
        /**************************************************/

                                       /* ------------------------------- */
                                       /* Values defined in CQYATTR,      */
                                       /* device attribute flags          */
                                       /* ------------------------------- */
#define  CQYDARMT       0x04           /* Device is a remote 3270         */
#define  CQYDADSP       0x02           /* Device is a display             */
#define  CQYDAVCN       0x01           /* Device is a virtual console     */
                                       /* ------------------------------- */

                                       /* ------------------------------- */
                                       /* Values defined in CQYSTAT,      */
                                       /* device status flag 1            */
                                       /* ------------------------------- */
#define  CQYDATTN       0x80           /* Attention pending               */
#define  CQYDDISC       0x40           /* Device is disconnected          */
                                       /* ------------------------------- */


                                       /* ------------------------------- */
                                       /* Values defined in CQYDRFL, flags*/
                                       /* ------------------------------- */
#define  CQYDQREC       0x80           /* Extended color supported        */
#define  CQYDQREH       0x40           /* Extended highlighting supported */
#define  CQYDQRPS       0x20           /* PSS supported                   */
#define  CQYDQR14       0x01           /* 14-bit addressing supported     */
                                       /* ------------------------------- */

                                       /* ------------------------------- */
                                       /* Values defined in CQYPFLG,      */
                                       /* path flag                       */
                                       /* ------------------------------- */
#define  CQYDPLIO       0x80           /* Path that did the last I/O      */
                                       /* ------------------------------- */

                                       /* ------------------------------- */
                                       /* Values defined in CQYPSLCC,     */
                                       /* logout pending/condition codes  */
                                       /* ------------------------------- */
#define  CQYPLOG        0x04           /* Logout pending                  */
#define  CQYPCC         0x03           /* Defferd condition code          */
                                       /* ------------------------------- */

                                       /* ------------------------------- */
                                       /* Values defined in CQYPUST,      */
                                       /* unit status byte                */
                                       /* ------------------------------- */
#define  CQYPATTN       0x80           /* Attention                       */
#define  CQYPSTMD       0x40           /* Status modifier                 */
#define  CQYPCUE        0x20           /* Control unit end                */
#define  CQYPBUSY       0x10           /* Busy                            */
#define  CQYPCHEN       0x08           /* Channel end                     */
#define  CQYPDVEN       0x04           /* Device end                      */
#define  CQYPUNCK       0x02           /* Unit check                      */
#define  CQYPUNEX       0x01           /* Unit exception                  */
                                       /* ------------------------------- */

                                       /* ------------------------------- */
                                       /* Values defined in CQYPCST,      */
                                       /* chaanel status byte             */
                                       /* ------------------------------- */
#define  CQYPPCI        0x80           /* program-controlled interruption */
#define  CQYPICL        0x40           /* Incorrect length                */
#define  CQYPPGCK       0x20           /* Program check                   */
#define  CQYPPRCK       0x10           /* Protection check                */
#define  CQYPCDCK       0x08           /* Channel data check              */
#define  CQYPCCCK       0x04           /* Channel control check           */
#define  CQYPICCK       0x02           /* Interface control check         */
#define  CQYPCHCK       0x01           /* Chaining check                  */
                                       /* ------------------------------- */



/*-------------------------------------------------------------------+
|   Bit Mask for 32-bit flags used to describe device specific       |
|   features important to the operation of the VDI.                  |
|                                                                    |
|        NAME         | BIT MASK   | MEANING                         |
+-------------------------------------------------------------------*/
#define  EXT_COLOR      0x0002    /* More than 2 colors allowed      */
#define  HIGHLIGHTING   0x0004    /* Highlighting supported          */
#define  EXT_HIGHL      0x0008    /* Extended highlighting supported */







        /* 3270 window and partition definitions */


#define      RESET_PART                0x00
#define      READ_PART                 0x01
#define      MODIFY_PART               0x0A
#define      SET_WINDOW                0x0B
#define      CREATE_PART               0x0C
#define      KILL_PART                 0x0D
#define      ACTIVATE_PART             0x0E



#endif

