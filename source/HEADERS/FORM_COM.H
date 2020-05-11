/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    form_com.h  ---  HEADER FILE                           |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    March 21, 1988                                         |
  |  <LEVEL>   FORMS                                                  |
  |  <PURPOSE> Contains declarations neccessary for routines both     |
  |               internal and external to forms                      |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/




/* +-----------------------------------------------------------------+ */
/* |  These macros are used to make fast test, sets, and clears of   | */
/* |    bit mapped fields                                            | */
/* +-----------------------------------------------------------------+ */

#define SET_MASK(var,mask)    ((var) |= ( mask))
#define CLR_MASK(var,mask)    ((var) &= (~mask))
#define CHK_MASK(var,mask)   (((var) &  ( mask)) != 0)



/* +-----------------------------------------------------------------+ */
/* |   This structure is a segment in a doubly linked list of forms  | */
/* |      that have been parsed and are still in the world.          | */
/* +-----------------------------------------------------------------+ */

struct FORM_DAT
{
   string           form_id ;     /* Form Identification               */
   hword            flags   ;     /* Flags for this form               */
   byte            *p_prof  ;     /* Presentation Manager's profile    */
   struct inf      *fst_inf ;     /* First   inf in list               */
   struct FORM_DAT *prev    ;     /* Previous form_dat structure       */
   struct FORM_DAT *next    ;     /* Next     form_dat structure       */
} ;

typedef struct FORM_DAT form_dat ;

#define FORM_NOT_ACTIVE   ((form_dat *)-1)


/* +-----------------------------------------------------------------+ */
/* |  This structure describes a segment in a doubly linked list of  | */
/* |    field for the current form                                   | */
/* +-----------------------------------------------------------------+ */

struct inf
{
   OCB        *ocb      ;         /* OCB of related object             */
   OCD        *ocd      ;         /* OCD of field in the object        */

   string      id       ;         /* Field identifier                  */
   real        pos_x    ;         /* X-coordinate of field (WC)        */
   real        pos_y    ;         /* Y-coordinate of field (WC)        */
   sint        seg_num  ;         /* Segment number of field           */
   sint        stem     ;         /* Variable stem number              */

   string      prompt   ;         /* Prompt characters                 */
   string      contents ;         /* Current contents of field         */
   string      defalt   ;         /* Default value of field            */
   string      init_val ;         /* Initial value of field            */
   string      prv_cont ;         /* Previous value of field           */

   byte        just     ;         /* Justification for field           */
#define        N_JUST      0x00   /*   No     justification            */
#define        L_JUST      0x01   /*   Left   justification            */
#define        R_JUST      0x02   /*   Right  justification            */
#define        F_JUST      0x03   /*   Fill   justification            */
#define        C_JUST      0x04   /*   Center justification            */
   byte        pad      ;         /* Pad character for field           */
   byte        ftype    ;         /* Type of field                     */
#define        FLD_NONE    0x00   /*    Not a field - normal text      */
#define        FLD_INPUT   0x01   /*    Input   field                  */
#define        FLD_OUTPUT  0x02   /*    Output  field                  */
#define        FLD_REPLACE 0x03   /*    Replace field                  */
   byte        echo     ;         /*    Echo/noecho characters         */
   hword       len      ;         /* Length of field                   */

   string      help_id  ;         /* Form ID of help form              */

   sint        type     ;         /* Type of field                     */
   string      min      ;         /*                                   */
   string      max      ;         /*                                   */
   string      other    ;         /* Depends on type of field          */

   byte        flags    ;         /* Interface data and other flags    */
#define        NO_L_DIS    0x01   /*   Do not display on line device   */
#define        NO_P_DIS    0x02   /*   Do not display on page device   */
#define        F_MOD       0x04   /*   User has modified field         */
#define        F_ERR       0x08   /*   Input error in field            */
   struct inf *next     ;         /* Pointer to next element           */
   struct inf *prev     ;         /* Pointer to prev element           */
} ;

typedef  struct  inf  INF ;

/* +-----------------------------------------------------------------+ */
/* |  The following structures are defined for interface from the TM | */
/* |    global process structure and each process for global and     | */
/* |    local forms processing {f_minit{}, f_profile{}}              | */
/* +-----------------------------------------------------------------+ */

struct form_process {                      /* For each TM process  */

                    byte     *f_prof;      /*Form profile structure*/
                    byte     *f_profdf;    /*Form default values   */
                    byte     *f_fmat;      /*Form formatter setting*/
                    byte     *p_prof;      /*Form presentation mgr */
                    string    f_setfld;    /* Current field to keep*/

                    } ;

struct form_global  {                        /* For the entire Form lvl*/

                    byte        *f_table;    /*Forms already parsed.  */
                    byte        *f_world;    /*Parts of GKS World used*/
                    byte        *f_cmds;     /*Form markup commands   */
                    sint         f_ncmds;    /*Forms total markups    */
                    byte        *f_units;    /*Form units             */
                    sint         f_nunits;   /*Number of units        */
                    byte        *f_colors;   /*Form colors            */
                    sint         f_ncolors;  /* number of colors      */
                    byte        *f_just;     /*Form justification     */
                    sint         f_njust;    /* number of justifies   */
                    byte        *pm_fun;     /*Form PM commands       */
                    sint         num_pm_fun; /*Forms total PM commands*/
                                             /* Table of PF functions */
                    hword        pf_key[ 99 ];

                    } ;

/* +-----------------------------------------------------------------+ */
/* |  Macro to return the stem of a field for <RL>                   | */
/* +-----------------------------------------------------------------+ */

#define fld_qstem(inf)   ((inf)->stem)

/* +-----------------------------------------------------------------+ */
/* |  These are values that are used with the flags parameter        | */
/* |    whenever you reference forms.                                | */
/* +-----------------------------------------------------------------+ */

#define  NO_FLAGS        ((hword) 0x0000)

#define  DO_NOT_DISPLAY  ((hword) 0x0001)

#define  SOUND_ALARM     ((hword) 0x0002)

#define  STATE_FORM      ((hword) 0x0004)
#define  HELP_FORM       ((hword) 0x0008)
#define  EXCEPT_FORM     ((hword) 0x0010)
#define  OTHER_FORM      ((hword) 0x0020)
