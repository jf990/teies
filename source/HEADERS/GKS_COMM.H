/***********************************************************************

:NAME.   GKS_COMM.H
:LEVEL.  GKS
:DATE.   June 12, 1987
:AUTHOR. Kenneth Liang

:PURPOSE.

    This header file contains definitions of GKS macro's and data
    structures used by both GKS external and internal routines. It is
    included by both GKS.H and GKS_I.H, but only one copy of it is
    invoked because of the flag GKS_COMM.


:MODS.
:END.


************************************************************************/

#ifndef  GKS_COMM
#define  GKS_COMM  TRUE


#define  g_rectan(r) ( (r)->xmin <= (r)->xmax && (r)->ymin <= (r)->ymax )




/**********************************************************************

    This structure is used in linking general data structures in a list.
    Each element is identified by an integer value.

          +------------+   +------------+
    head->|id|data|next|-->|id|data|next|--> ...
          +------------+   +------------+
               |                |
               V                V
          +------------+   +------------+
          | data       |   | data       |
          | structure  |   | structture |
          +------------+   +------------+




***********************************************************************/

struct   list
{                                      /******************************/
    sint           id;                 /* Element identifier         */
    byte           *data;              /* Pointer to data structure  */
    struct list    *next;              /* Pointer to the next element*/
};                                     /******************************/



/**********************************************************************
    GKS states
***********************************************************************/

#define  GKCL      1                        /* GKS is closed            */
#define  GKOP      2                        /* GKS is open              */
#define  WSAC      3                        /* At least one workstation */
                                            /*    is active             */
#define  WSOP      4                        /* At least one workstation */
                                            /*    is open               */
#define  SGOP      5                        /* At least one segment is  */
                                            /*    open                  */


/**********************************************************************
    GKS input and output levels

    Input  level   :    A, B, C
    Output level   :    M, 0, 1, 2
***********************************************************************/

#define  GL_MA     0
#define  GL_MB     1
#define  GL_MC     2
#define  GL_0A     3
#define  GL_0B     4
#define  GL_0C     5
#define  GL_1A     6
#define  GL_1B     7
#define  GL_1C     8
#define  GL_2A     9
#define  GL_2B    10
#define  GL_2C    11


/**********************************************************************
    Maximum number of normalization transformations
***********************************************************************/

#define  G_MAX_TRANS    48

/**********************************************************************
    Text font and precision values
***********************************************************************/

#define  STRING    0
#define  CHAR      1
#define  STROKE    2


/**********************************************************************
    Text alignment values
***********************************************************************/

#define  TA_NORMAL    0
#define  TA_LEFT      1
#define  TA_CENTER    2
#define  TA_RIGHT     3
#define  TA_TOP       4
#define  TA_CAP       5
#define  TA_HALF      6
#define  TA_BASE      7
#define  TA_BOTTOM    8


/**********************************************************************
    Fill area interior style values
***********************************************************************/

#define  HOLLOW    0
#define  SOLID     1
#define  PATTERN   2
#define  HATCH     3



/**********************************************************************
    Clipping indicator values
***********************************************************************/

#define  NO_CLIP   0
#define  CLIP      1


/**********************************************************************
    Workstation state values
***********************************************************************/

#define  INACTIVE  0
#define  ACTIVE    1


/**********************************************************************
    Display surface empty values
***********************************************************************/

#define  NOTEMPTY  0
#define  EMPTY     1


/**********************************************************************
    New frame action necessary at update
***********************************************************************/



/**********************************************************************
    Workstation transformation update state values
***********************************************************************/

#define  NOTPENDING     0
#define  PENDING        1


/**********************************************************************
    Locator operating mode values
***********************************************************************/

#define  REQUEST        0
#define  SAMPLE         1
#define  EVENT          2


/**********************************************************************
    Echo switch values
***********************************************************************/

#define  NOECHO         0
#define  ECHO           1


/**********************************************************************
    Pick initial status values
***********************************************************************/

#define  NOPICK         0
#define  PICK           1



/**********************************************************************
    Workstation categories
***********************************************************************/

#define  OUTIN     1
#define  INPUT     2
#define  OUTPUT    3
#define  MI        4
#define  MO        5
#define  WISS      6





/**********************************************************************
    Segment visibility
***********************************************************************/

#define  VISIBLE   1
#define  INVISIBLE 0


/**********************************************************************
    Segment detectability
***********************************************************************/

#define  DETECTABLE     1
#define  UNDETECTABLE   0





/**********************************************************************
    General Drawing Primitive Index
***********************************************************************/

#define  BOX       0
#define  ARC       1
#define  PIE       2
#define  CIRCLE    3
#define  ELLIPSE   4


/*---------------------------+
|   Device coordinate units  |
+---------------------------*/

#define  METERS         0
#define  OTHER          1

/*---------------------------+
|   Color available values   |
+---------------------------*/

#define  COLOR          0
#define  MONOCHROME     1

/*------------------------------------------+
|   Dynamic modification capacity values    |
|   IRG - implict regeneration necessary    |
|   IMM - performed immediately             |
+------------------------------------------*/

#define  IRG            0
#define  IMM            1



                                  /***********************************/
struct   font_prec                /* Text font and precision pair    */
{                                 /* ------------------------------- */
    sint      font;               /* Text font                       */
    sint      prec;               /* Text precision                  */
};                                /***********************************/


struct  colors                    /***********************************/
{                                 /* Color (red,green,blue)          */
    real      red;                /*   intensities                   */
    real      green;              /*                                 */
    real      blue;               /*                                 */
};                                /***********************************/

                                  /***********************************/
struct   gdps                     /* Generalized Drawing Primitives  */
{                                 /* ------------------------------- */
    sint      gdpid;              /* GDP identifier                  */
    sint      numattrs;           /* Number of sets of atrributes used*/
    sint     *attrs;              /* list of sets of attritbues used */
    sint      maxcinx;            /* Maximum number of color indices */
    sint      numsegpri;          /* Number of segment priorities    */
};                                /*   supported                     */
                                  /***********************************/



/**********************************************************************
    Definitions for primitives
***********************************************************************/

#define  POLYLINE  0
#define  POLYMARK  1
#define  TEXT      2
#define  FILLAREA  3
#define  CELLARRAY 4
#define  FIELD     5




struct   prim
{                                 /***********************************/
    sint      type;               /* Primitive type                  */
    sint      pickid;             /* Pick identifier                 */
    sint      len;                /* Length of ifier                 */
    real     *x;                  /* Pick identifier                 */
    real     *y;                  /* Pick identifier                 */
    byte     *data;               /* Pointer to the data record of   */
                                  /*   primitive                     */
    byte     *attr;               /* Pointer to a bundle of attribute*/
    struct prim *next;            /* Pointer to next primitive       */
                                  /***********************************/
};



/**********************************************************************
    Segment State List
***********************************************************************/

#define  NUMSEGPRI      10

#define  SEGPRIO(p) ( (sint) ( (p) * (NUMSEGPRI-1) ) )


                                  /******************************/
typedef  real matrix[2][3];   /* 2D Transformation matrix   */
                                  /******************************/

struct   seg_sl                   /***********************************/
{                                 /*                                 */
    sint           id;            /* Segment name                    */
    area           frame;         /* Area the segment covers         */
    struct list   *wk;            /* Set of associated workstations  */
    matrix         mtx;           /* Segment transformation matrix   */
    sint           visib;         /* Segment visibility              */
    hword          highlight;     /* Segment highlighting            */
    sint           color;         /* Segment color index             */
    real           priority;      /* Segment priority                */
    sint           detect;        /* Segment detectability           */
    struct prim   *primitive;     /* Set of primitives associated    */
                                  /*   with this segment             */
    struct seg_sl *next;          /* Pointer to next segment SL      */
};                                /***********************************/







/*********************************************************************
    Worstation State List
 *********************************************************************/



/*--------------------------------------------------------------------
    For every logical input device of class LOCATOR
---------------------------------------------------------------------*/

struct   _locdev
{                                 /***********************************/
    sint           wkid;          /* Workstation    identifier       */
    sint           locid;         /* Locator device identifier       */
    sint           opmode;        /* Operating mode                  */
    sint           itransnum;     /* Initial transformation number   */
    position       ilocpos;       /* Initial locator position        */
    sint           petype;        /* Prompt and echo type            */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Echo area                       */
    tint          *locdrec;       /* Locator data record             */
    struct _locdev *next;         /* Pointer to next locator device  */
};                                /***********************************/



/*--------------------------------------------------------------------
    For every logical input device of class STROKE
---------------------------------------------------------------------*/

struct   _stkdev
{
    sint           wkid;          /* Workstation    identifier       */
    sint           stkid;         /* Stroke  device identifier       */
    sint           opmode;        /* Operating mode                  */
    sint           itransnum;     /* Initial transformation number   */
    position       ipntstk;       /* Initial points in stroke        */
    sint           inumpnts;      /* Initial number of points        */
    sint           petype;        /* Prompt and echo type            */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Pointer to echo area            */
    sint           bufsize;       /* Input buffer size               */
    tint          *stkdrec;       /* Stroke  data record             */
    struct _stkdev *next;         /* Pointer to next stroke  device  */
};                                /***********************************/



/*--------------------------------------------------------------------
    For every logical input device of class VALUATOR
---------------------------------------------------------------------*/

struct   _valdev
{
    sint           wkid;          /* Workstation    identifier       */
    sint           valid;         /* Valuator device identifier      */
    sint           opmode;        /* Operating mode                  */
    sint           ival;          /* Initial value                   */
    sint           petype;        /* Prompt and echo type            */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Default echo area               */
    tint          *valdrec;       /* Default valuator data record    */
    struct _valdev *next;         /* Pointer to next valuator device */
};                                /***********************************/




/*--------------------------------------------------------------------
    For every logical input device of class CHOICE
---------------------------------------------------------------------*/

struct   _chcdev
{
    sint           wkid;          /* Workstation    identifier       */
    sint           chcid;         /* Choice device identifier        */
    sint           opmode;        /* Operating mode                  */
    sint           ichcnum;       /* Initial choice number           */
    sint           petype;        /* Prompt and echo type            */
    sint           maxchcalt;     /* Maximum number of choice        */
                                  /*   alternatives                  */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Default echo area               */
    tint          *chcdrec;       /* Default choice   data record    */
    struct _chcdev *next;         /* Pointer to next choice   device */
};                                /***********************************/



/*--------------------------------------------------------------------
    For every logical input device of class PICK
---------------------------------------------------------------------*/

struct   _picdev
{
    sint           wkid;          /* Workstation    identifier       */
    sint           picid;         /* Pick   device identifier        */
    sint           opmode;        /* Operating mode                  */
    sint           istatus;       /* Initial status                  */
    sint           iseg;          /* Initial segment                 */
    sint           ipickid;       /* Initial pick identifier         */
    sint           petype;        /* Prompt and echo type            */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Echo area                       */
    tint          *picdrec;       /* Pick     data record            */
    struct _picdev *next;         /* Pointer to next pick     device */
};


/*--------------------------------------------------------------------
    For every logical input device of class STRING
---------------------------------------------------------------------*/

struct   _strdev
{
    sint           wkid;          /* Workstation    identifier       */
    sint           strid;         /* String device identifier        */
    sint           opmode;        /* Operating mode                  */
    string         istr;          /* Initial string                  */
    sint           petype;        /* Prompt and echo type            */
    sint           echosw;        /* Echo switch                     */
    area           echoarea;      /* Default echo area               */
    sint           bufsize;       /* Maximum input buffer size       */
    tint          *strdrec;       /* Default string   data record    */
    struct _strdev *next;         /* Pointer to next string   device */
};                                /***********************************/






                                  /***********************************/
                                  /* Workstation State List          */
struct    wk_sl                   /*                                 */
{                                 /* ------------------------------- */
    sint           state;         /* Workstation state               */
    struct list   *seg;           /* Set of stored segments for this */
    field         *inpfld;        /* List of input fields            */
    sint           surfempty;     /* Display suface empty            */
    sint           newframe;      /* New frame action necessary at   */
    struct _locdev *locator;      /* Set of locator input device     */
                                  /*   associated                    */
    struct list    *pick;         /* Set of pick input device        */
                                  /*   associated                    */
    struct list    *strng;        /* Set of string input device      */
                                  /*   associated                    */
    struct list    *stroke;       /* Set of stroke input device      */
                                  /*   associated                    */
    struct list    *valuator;     /* Set of valuator input devic     */
                                  /*   associated                    */
    struct list    *choice;       /* Set of choice input device      */
                                  /*   associated                    */
};                                /***********************************/


struct   wk_dt                    /***********************************/
{                                 /*                                 */
    sint           dc_units;      /* Device coordinates units        */
    real           dc_dss[2];   /* Maximum display surface size in */
                                  /*   device coordinates            */
    real           ru_dss[2];   /* Maximum display surface size in */
                                  /*   raster units                  */
                                  /* ------------------------------- */
    sint           numltypes;     /* Number of available line types  */
    sint          *linetypes;     /* List of available line types    */
    sint           numlwidths;    /* Number of available linewidths  */
    real           nomlwidth;     /* Nominal linewidth               */
    real           minlwidth;     /* Minimum linewidth               */
    real           maxlwidth;     /* Maximum linewidth               */
    sint           nummtypes;     /* Number of available marker types*/
    sint          *marktypes;     /* List of available marker types  */
    sint           nummksizes;    /* Number of available marker sizes*/
    real           nommksize;     /* Nominal marker size             */
    real           minmksize;     /* Minimum marker size             */
    real           maxmksize;     /* Maximum marker size             */
    sint           numtfontprec;  /* Number of text font and         */
                                  /*   precision pairs               */
    struct font_prec *tfontprec;  /* List of text font and precision */
                                  /*   pairs                         */
    sint           numcheight;    /* Number of available character   */
                                  /*   heights                       */
    real           mincheight;    /* Minimum character height        */
    sint           numfaistyle;   /* Number of available fill area   */
                                  /*   interior style                */
    sint          *faistyle;      /* List of available fill area     */
                                  /*   interior style                */
    sint           numhatstyles;  /* Number of available hatch styles*/
    sint          *hatchstyles;   /* List of available hatch styles  */
    sint           numcolorint;   /* Number of available color       */
                                  /*   intensities                   */
    sint           coloravail;    /* Color available                 */
    sint           numpcolrinx;   /* Number of predifined color      */
                                  /*   indicex (symbols)             */
    struct colors *color;         /* Color ( red, green, blue )      */
                                  /*   intensities                   */
                                  /* ------------------------------- */
    sint           numgdp;        /* Number of available Generalized */
                                  /*   Drawing Primitives            */
    struct gdps   *gdp;           /* List of available Generalized   */
                                  /*   Drawing Primitives            */
                                  /* ------------------------------- */
    sint           segtrans;      /* Segment transformation          */
    sint           visibility;    /* Visibility                      */
    sint           highlight;     /* Highlighting                    */
    sint           segprio;       /* Segment priority                */
    sint           addprim;       /* Adding primitives to open       */
                                  /*   segment overlapping segment   */
                                  /* ------------------------------- */
    struct list   *locator;       /* For every logical input device  */
                                  /*    of class LOCATOR             */
    struct list   *stroke;        /* For every logical input device  */
                                  /*    of class STROKE              */
    struct list   *valuator;      /* For every logical input device  */
                                  /*    of class VALUATOR            */
    struct list   *choice;        /* For every logical input device  */
                                  /*    of class CHOICE              */
    struct list   *pick;          /* For every logical input device  */
                                  /*    of class PICK                */
    struct list   *strng;         /* For every logical input device  */
                                  /*    of class STRING              */
};                                /***********************************/




/*********************************************************************

    Workstation List

**********************************************************************/


struct   wk_lst                   /***********************************/
{                                 /*                                 */
    sint           id;            /* Workstation identifier          */
    sint           type;          /* Workstation type                */
    sint           category;      /* Workstation category            */
    sint           devid;         /* Identifier of associated device */
    tint          *dev;           /* Pointer to device entry in VDI  */
    struct wk_sl  *wksl;          /* Pointer to workstation SL       */
    struct wk_dt  *wkdt;          /* Pointer to workstation DT       */
    struct wk_lst *next;          /* Pointer next workstation entry  */
};                                /***********************************/







                                   /******************************/
struct    gks_dt                   /*    GKS Description Table   */
{                                  /* -------------------------- */
     sint           gkslvl;        /* Level of GKS               */
     sint           numwktypes;    /* Number of available        */
                                   /*   workstation types        */
     sint           *avwktypes;    /* List of available types of */
                                   /*   workstations.            */
     sint           maxopnwk;      /* Maximum number of simu-    */
                                   /*   taneous open workstations*/
     sint           maxactwk;      /* Maximum number of simutan- */
                                   /*   eous  active workstations*/
     sint           maxntnum;      /* Maximum normalization      */
};                                 /*   transformation number    */
                                   /******************************/




                                  /***********************************/
struct    gks_sl                  /* <<<  GKS State List  >>>        */
{                                 /* ------------------------------- */
    struct wk_lst *opnwk;         /* Set of open workstations        */
    struct list   *actwk;         /* Set of active workstations      */
    hword          highlight;     /* Current highlighting value      */
    sint           pl_index;      /* Current polyline index          */
    sint           linetype;      /* Current linetype;               */
    real           linewidth;     /* Current linewidth scale factor  */
    sint           pl_color;      /* Current polyline color index    */
    sint           pm_index;      /* Current poly marker index       */
    sint           markertype;    /* Current marker type             */
    sint           marker_ssf;    /* Current marker size scale factor*/
    sint           pm_color;      /* Current polymarker color index  */
    sint           t_index;       /* Current text index              */
    sint           t_fontprec;    /* Current text font and precision */
    sint           c_expfac;      /* Current character expansion factor*/
    real           c_spacing;     /* Current character spacing       */
    sint           t_color;       /* Current text color index        */
    real           c_height;      /* Current character height        */
    struct cupvec  {              /* Current character up vector     */
              real x,y;           /*                                 */
              }    c_upvec;       /*                                 */
    sint           t_path;        /* Current text path               */
    struct talign  {              /* Current text alignment          */
              sint horiz, vert;   /*                                 */
              }    t_align;       /*                                 */
    sint           fa_index;      /* Current fill area index         */
    sint           fai_style;     /* Current fill area interior style*/
    sint           fa_style;      /* Current fill area style index   */
    sint           fa_color;      /* Current fill area color index   */
    sint           pickid;        /* Current pick identifier         */
    wvtrans       *curtrans;      /* Pointer to current normalization*/
    wvtrans       *trans;         /* List of normalization           */
                                  /*   transformations ordered by    */
                                  /*   viewport input priority       */
    sint           clip_ind;      /* Clipping indicator              */
    struct seg_sl *opnseg;        /* Pointer to open segment         */
    struct seg_sl **seg;          /* Pointer to an array of set of   */
                                  /*   segments in use               */
                                  /*---------------------------------*/
                                  /* Bundle entries are added        */
                                  /* dynamically                     */
};                                /***********************************/




extern   sint                opstate;
extern   struct    gks_dt   *gksdt;
extern   struct    gks_sl   *gkssl;


#endif
