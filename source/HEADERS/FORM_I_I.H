/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    form_i_i.h  ---  HEADER FILE                           |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    March 21, 1988                                         |
  |  <LEVEL>   FORMS                                                  |
  |  <PURPOSE> Contains the declaration required only by internal     |
  |              forms functions.                                     |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    NV  Mar 21 88  Restructure forms header files to       |
  |                           internal/external/common structure.     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

/* +-----------------------------------------------------------------+ */
/* |   These constants are for internal use to those routines calling| */
/* |      f_render for use in parsing xmp values.                    | */
/* +-----------------------------------------------------------------+ */

#define F_RENDER_CR    1
#define F_RENDER_NCR   2

/* +-----------------------------------------------------------------+ */
/* |   These macros are used to access individual items from various | */
/* |      structures used by the Forms Processor.                    | */
/* +-----------------------------------------------------------------+ */

#define PROFILE(element)    (f_prof->element)
#define  FORMAT(element)    (f_fmat->element)

/* +-----------------------------------------------------------------+ */
/* |   This structure is the CMD structure for the function keys     | */
/* | that are recognized by the Presentation Manager.  Use the GKS   | */
/* | Key Value as an index to this array to invoke an internal PM    | */
/* | function (NOT user programmable.)                               | */
/* |                                                                 | */
/* | This table is defined in f_minit() and is located off           | */
/* | TM_master->f_pmfun                                              | */
/* +-----------------------------------------------------------------+ */

struct exec_pfk
{
   sint    ((*exe_fun)()) ;       /* Function to execute               */
} ;

#define NUM_PM_FUNS      10

#define PM_NEXT           1
#define PM_PREV           2
#define PM_TOP            3
#define PM_BOT            4
#define PM_HOME           5
#define PM_HPREV          6
#define PM_HNEXT          7
#define PM_LEFT           8
#define PM_RIGHT          9

#define NUM_FUNKEYS      99       /* 0     - Enter
                                     1..24 - PF Keys
                                     25    - CLEAR
                                     26    - PA1
                                     27    - PA2
                                     28    - PA3                       */

#define  F_MIN_FUNCTION_KEY_VALUE    0
#define  F_MAX_FUNCTION_KEY_VALUE   99

#define  F_NUM_REDEFINABLE_FUNKEYS  12

/* +-----------------------------------------------------------------+ */
/* |   This structure contains the information required by PM for    | */
/* |      proper handling of windows (topmat, botmat, body).         | */
/* +-----------------------------------------------------------------+ */

struct WIND_DAT
{
   sint trans_nm ;                /* Transformation number of window   */

   sint frst_seg ;                /* First segment in this window      */
   sint last_seg ;                /* Last  segment in this window      */

   real win_wid  ;                /* Width  of Window            (WC)  */
   real win_len  ;                /* Length of Window            (WC)  */

   real w_min_x  ;                /* Minimum X of this window     (WC) */
   real w_max_x  ;                /* Maximum X of this window     (WC) */

   real w_max_y  ;                /* Maximum Y of this window    (WC)  */
   real w_min_y  ;                /* Minimum Y of this window    (WC)  */

   real v_max_x  ;                /* Maximum X of this viewport  (NDC) */
   real v_min_x  ;                /* Minimum X of this viewport  (NDC) */
   real v_max_y  ;                /* Maximum Y of this viewport  (NDC) */
   real v_min_y  ;                /* Minimum Y of this viewport  (NDC) */
} ;

typedef struct WIND_DAT  wind_dat ;



/* +-----------------------------------------------------------------+ */
/* |   This structure is the Presentation Managers profile and       | */
/* |      contains all the info that the PM needs.                   | */
/* +-----------------------------------------------------------------+ */

struct p_prof_s
{
   sint wk_id        ;            /* Current Workstation ID            */

   real scrn_wid     ;            /* Width  of screen (or page)   (WC) */
   real scrn_len     ;            /* Length of screen (or page)   (WC) */

   real w_min_x      ;            /* Minimum X of the form        (WC) */
   real w_max_x      ;            /* Maximum X of the form        (WC) */

   real w_max_y      ;            /* Maximum Y of the form       (WC)  */
   real w_min_y      ;            /* Minimum Y of the form       (WC)  */

   real w_cur_x      ;            /* Current X position in the form    */
   real w_cur_y      ;            /* Current Y position in the form    */

   wind_dat *top_mat ;            /* Window information on Top Matter  */
   wind_dat *body    ;            /* Window information on text        */
   wind_dat *bot_mat ;            /* Window information on Bottom Matr */

   boolean leave     ;            /* Indicates whether to leave form   */
   boolean alarm     ;            /* Indicates whether to alarm        */
   string  help_dat  ;            /* Info needed to process HELP reuest*/

   hword  pf_key [NUM_FUNKEYS]; /* Values of PF keys, if 0 then get  */
                                  /* it out of the global table in     */
                                  /* TM_master->forms->pf_key          */

   string pf_val [ F_NUM_REDEFINABLE_FUNKEYS ];
                                  /* Actual user redefined function key*/
                                  /* names.  This table only has names */
                                  /* that are not the standard ones for*/
                                  /* index reference. EG. 3rd string is*/
                                  /* FORM_USER_DEFINED_3.              */
} ;



/* +-----------------------------------------------------------------+ */
/* |  This structure stores information required to process lists.   | */
/* |    It will be stacked with the TM stack routines.               | */
/* +-----------------------------------------------------------------+ */

struct      f_lst_s
{
   byte    type     ;             /* Type of list (see defined values) */

#define    ANY_LIST          0x00 /* Any of the following list types   */
#define    ORD_LIST          0x01 /*         <ol>                      */
#define    ORDERED_LIST      0x01 /*         <ol>                      */
#define    SIMP_LIST         0x02 /*         <sl>                      */
#define    SIMPLE_LIST       0x02 /*         <sl>                      */
#define    UNORD_LIST        0x03 /*         <ul>                      */
#define    UNORDERED_LIST    0x03 /*         <ul>                      */
#define    REPEATING_LIST    0x04 /*         <rl>                      */
#define    DEFINITION_LIST   0x05 /*         <dl>                      */

   byte    level    ;             /* Level of nesting                  */
   tint    seqno    ;             /* Sequence # of <li> for ORD_LIST   */
} ;

#define LVL_IND             5.0   /* Amount to indent per level of list*/
#define LIST_LEVEL_INDENT   5.0   /* Amount to indent per level of list*/
#define LIST_PREFIX_SIZE    4.0   /* Width of prefix for list items    */
#define PRE_SIZ             4.0   /* Width of prefix for list items    */
                                  /*    must be < LVL_IND              */
#define MAX_LIST_LEVELS 20        /* Maximum levels of nesting lists   */



/* +-----------------------------------------------------------------+ */
/* | This structure stores information needed to render text in the  | */
/* |   GKS world with all the proper attributes, formatting etc.     | */
/* +-----------------------------------------------------------------+ */

struct   f_fmat_s
{
   OCB            *ocb      ;     /* Related object's OCB              */

   string          line_bfr ;     /* Line buffer for f_render          */

   real            org_x    ;     /* X origin of form (WC)             */
   real            org_y    ;     /* Y origin of form (WC)             */
   real            cur_x    ;     /* Current X of form (WC)            */
   real            cur_y    ;     /* Current Y of form (WC)            */
   sint            last_seg ;     /* Last segment assigned             */

   byte            ftype    ;     /* TRUE -> rendering a field         */
   byte            echo     ;     /* is field value to be echoed ?     */

   real            lef_mar  ;     /* Current Left margin               */
   real            rig_mar  ;     /* Current Right margin              */
   real            max_rmar ;     /* Greatest right margin             */
   tint            spacing  ;     /* Current spacing setting           */
   byte            just     ;     /* Current justification             */

   sint           *hi_lite  ;     /* Phrase highlighting stack         */
   sint           *color_f  ;     /* Foreground color stack            */
   sint           *color_b  ;     /* Background color stack            */
   sint           *font     ;     /* Font in use                       */
   sint           *if_stack ;     /* If stack                          */
#define MAX_ATT_STACK    20       /* Maximum number of elements that   */
                                  /*   can be pushed on the attribute  */
                                  /*   stacks above                    */

   sint            rl_item  ;     /* repeating list counter            */

   tint            fig_cnt  ;     /* Figure reference counter          */
   tint            scrn_cnt ;     /* Estimated number of screens       */
   byte            head_lev ;     /* Heading level                     */

   byte            ol_level ;     /* Levels of ordered   lists         */
   byte            sl_level ;     /* Levels of simple    lists         */
   byte            ul_level ;     /* Levels of unordered lists         */
   byte           *list_stk ;     /* Pointer to List info stack        */

   INF            *fst_inf  ;     /* Base Ptr to list of fields        */
   INF            *cur_fld  ;     /* Pointer to current field          */

   tint     status          ;     /* State of affairs for f_render     */
#define START      ((tint) 0x80000000)  /*  Before any text is found   */
#define FORMING    ((tint) 0x00000001)  /*  Default status             */
#define BOT_MAT    ((tint) 0x00000002)  /*  Now within bot_mat tags    */
#define TOP_MAT    ((tint) 0x00000004)  /*  Now within top_mat tags    */
#define FIGURE     ((tint) 0x00000008)  /*  Now within fig tags        */
#define HEADING    ((tint) 0x00000010)  /*  Now within heading tags    */
#define F_CENTER   ((tint) 0x00000020)  /*  Now within center tags     */
#define UPPERCASE  ((tint) 0x00000040)  /*  f_render in uppercase only */
} ;

/* +-----------------------------------------------------------------+ */
/* | This structure stores information needed to figure out where to | */
/* |   place the form in the world.                                  | */
/* +-----------------------------------------------------------------+ */

struct f_gks_s
{
   sint last_seg ;
   real bot_y    ;
} ;

/* +-----------------------------------------------------------------+ */
/* | This structure stores information for a particular f_render     | */
/* |   line                                                          | */
/* +-----------------------------------------------------------------+ */

struct f_cent_s
{
   string  text;
   string  field_txt;
   sint    length;
   sint   *flag_ptr;
   sint    echo;
   sint    hl;
   sint    field_type;
} ;

/* +-----------------------------------------------------------------+ */
/* | This structure stores information needed by the parser and also | */
/* |   values that are to be communicated to the f_*** routines.     | */
/* +-----------------------------------------------------------------+ */

struct  f_prof_s                 /* Markup Qualifiers Stored           */
{
                  /* Object Level Interface */

   string   mark_up;              /* Unparsed mark up & qualifier      */

   string   open ;                /* open tag for mark ups             */
   string   cls  ;                /* close tag for mark ups            */
   string   end  ;                /* end markup prefix                 */
   string   bvar ;                /* beginning bvariable delimeter     */
   string   evar ;                /* ending variable delimeter         */

                /* FIELD qualifiers */

   string   io_otype;             /* field - objecttype                */
   string   io_id;                /* field - id                        */
   string   io_name;              /* field - name                      */
   boolean  io_var;               /* field - variable                  */
   string   io_index;             /* field - index                     */
   string   io_prompt;            /* field - prompt                    */
   byte     io_io;                /* field - what kind of field is it  */
   byte     io_echo;              /* field - echo/noecho               */
   string   io_help;              /* field - help reference            */
   sint     io_len;               /* field - len                       */
   byte     io_pad;               /* field - pad                       */
   byte     io_just;              /* field - justify                   */
   string   io_def;               /* field - default                   */

                /* ATTACH qualifiers */

   string   att_name;             /* attach - name                     */
   string   att_ref;              /* attach - reference                */

                /* CENTER qualifiers */

   string   ctr_brk;              /* center - break                    */

                /* H qualifiers */

   sint     h_level;              /* h - (a number)                    */
   string   h_id;                 /* h - id                            */
   string   h_title;              /* h - title                         */

                /* HP qualifiers */

   sint     hp_level;             /* hp - (a number)                   */
   sint     hp_color;             /* hp - color                        */

                /* DL qualifiers */

   string   dl_tsize;             /* dl - tsize                        */
   sint     dl_brk;               /* dl - break                        */
   sint     dl_comp;              /* dl - compact                      */
   string   dl_hdhi;              /* dl - headhi                       */
   string   dl_trmhi;             /* dl - termhi                       */

                /* FIG qualifiers */

   string   fig_id;               /* fig - id                          */
   string   fig_plc;              /* fig - place                       */
   sint     fig_wdth;             /* fig - width                       */
   byte     fig_frm;              /* fig - frame                       */
   string   fig_frmc;             /* frame string when fig_frm string  */
   sint     fig_dpth;             /* fig - depth                       */

                 /* SPACE qualifiers */

   sint     sp_amt;               /* space - amount                    */
   string   sp_units;             /* space - units                     */

                 /* Repeating list qualifiers */

   string   rl_list;              /* rl - list                         */
   string   rl_ref;               /* rl - reference                    */
   string   rl_name;              /* rl - name                         */

                  /* XMP qualifiers */

   boolean  all_prse;             /* flag to set parsing on/off        */
   boolean  var_prse;             /* are variables to be parsed        */
   boolean  mkp_prse;             /* are markups   to be parsed        */

                  /* INDEX qualifiers */

   sint     ind_lev;              /* the index level                   */

                  /* FOOTNOTE qualifiers */

   boolean  fn_on;                /* footnote on                       */

                  /* NOTE qualifiers */

   boolean  note_on;              /* note on                           */

   };

/* +-----------------------------------------------------------------+ */
/* | This structure stores information for all the qualifiers for    | */
/* |   a given markup, it contains a string vector containing the    | */
/* |   names of the markups and an array containing the corresponding| */
/* |   id's for the markups allowing for multiple spellings of a     | */
/* |   qualifier to have the same result                             | */
/* +-----------------------------------------------------------------+ */

#define    F_BOOL_Q        1      /*    Boolean qualifier              */
#define    F_STR_Q         2      /*    String  qualifier              */
#define    F_NUM_Q         3      /*    Numeric qualifier              */
#define    F_COLOR_Q       4      /*    Color   qualifier              */
#define    F_UNIT_Q        5      /*    Unit    qualifier              */

#define    NUM_UNITS      11
#define    NUM_COLORS      7
#define    NUM_JUSTIFS     4

struct f_qual_s
{
   string_vector   qual ;         /* Contains qualifier name           */
   sint           *id  ;          /* Type of qualifier                 */
} ;
typedef struct f_qual_s f_qualifiers;

/* +-----------------------------------------------------------------+ */
/* | This structure stores information about all the markups that    | */
/* |   can be processed.  Name, pts to parsing, start and end        | */
/* |   routines, number of qualifiers and array of qualifiers.       | */
/* +-----------------------------------------------------------------+ */

#define FORM_XMP_0    0
#define FORM_XMP_1    1
#define FORM_XMP_2    2
#define FORM_XMP_3    3

#define NUM_CMDS     34

struct _f_exe_cmd
{
   string           name       ;  /* function name                     */
   f_qualifiers     quals      ;  /* Array with qualifiers             */
   sint           func_typ     ;
   sint         ((*arg_fun)()) ;  /* pointer to arg parser for fun     */
   sint         ((*exe_fun)()) ;  /* pointer to execution for fun      */
   sint         ((*end_fun)()) ;  /* pointer to end routine to fun     */
} ;

