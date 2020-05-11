/***********************************************************************

:NAME.   GV_COMM.H
:LEVEL.  GKS & VDI
:DATE.   Aug. 19, 1987
:AUTHOR. Kenneth Liang

:PURPOSE.

    This header file contains definitions of GKS macro's and data
    structures used by both GKS and VDI.


:MODS.
:END.


************************************************************************/

#ifndef   GV_COMM
#define  GV_COMM   TRUE


#define  ROUND(v) ( ((v)-(sint)(v)) < 0.5 ? (sint)(v) : (1+(sint)(v)) )



/**********************************************************************
    Highlighting
***********************************************************************/

#define  NORMVIDEO 0x0000
#define  REVVIDEO  0x0001
#define  UNDERLINE 0x0002
#define  BLINKING  0x0004
#define  INTENSIFY 0x0008




typedef  struct _point {
              real x;
              real y;
         } position;

typedef  struct _area {
              real xmin, ymin, xmax, ymax;
         } area;

typedef  struct wvtrans {
              sint transnum;
              real xwmin, ywmin, xwmax, ywmax;
              real xvmin, yvmin, xvmax, yvmax;
              struct wvtrans *next;
         } wvtrans;


/**********************************************************************
    Values of deferral mode
***********************************************************************/

#define  ASAP      0
#define  BNIG      1
#define  BNIL      2
#define  ASTI      3




/**********************************************************************
    Field definition
***********************************************************************/

#define  FINPUT    0
#define  FOUTPUT   1
#define  V_FLD_MOD_MASK 0x04



typedef  struct _field {
              sint      type;          /* Field type: FINPUT or FOUTPUT   */
              sint      max;           /* Maximum length of input string  */
              sint      echo;          /* Echo flag for input string      */
              position  pos;           /* Starting position of echo area  */
              string    str;           /* Pointer to a TEIES string       */
              hword     highlight;     /* Highlighting value of the field */
              byte     *flag;          /* Pointer to flag for field       */
                                       /* modification                    */
              struct _field *next;     /* Pointer to next field           */
         } field;


/* $EXTRACT */


                                       /* ------------------------------- */
extern   sint      gv_wvx();           /* Transform a point from window   */
                                       /*   to viewport                   */
                                       /*   wvtrans   trans               */
                                       /*   real xw,yw,*xv,*yv            */
                                       /* ------------------------------- */
extern   sint      gv_vwx();           /* Transform a point from viewport */
                                       /*   to window                     */
                                       /*   wvtrans   trans               */
                                       /*   real xv,yv,*xw,*yw            */
                                       /* ------------------------------- */


/* $ENDEXTRACT */



#endif

