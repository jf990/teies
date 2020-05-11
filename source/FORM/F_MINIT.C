/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_minit.c                                              |
  |  <AUTHOR>  John L. Foster, Nestor Voronka                         |
  |  <DATE>    November 20, 1987                                      |
  |  <LEVEL>   Forms                                                  |
  |  <GLOBAL>  TM_master->f_cmds       Table of markups their argu-   |
  |                                    ment parsers and exe funcs     |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_ncmds      # of elements in above table   |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_pmfun      Table of PM's command keys and |
  |                                    their exe functions            |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_npmfun     # of elements in above table   |
  |                                                                   |
  |  <PARAM>   None.                                                  |
  |                                                                   |
  |  <PURPOSE> Initialize the Forms markup command table, Presenta-   |
  |            tion Manager's command key table.                      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_mema(), s_init ()                                   |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    May 17, 1989       JF        Added RL tag definitions. |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "formi.h"
#include "form_i.h"





sint f_minit ()

{

   sint   i                  =    0 ;             /* Counter variable  */
   sint   j                  =   -1 ;             /* Counter variable  */
   sint   k                  =   -1 ;             /* Counter variable  */
   hword  * key_set          =  NIL ;
   struct _f_exe_cmd *f_mcmds= NULL ;             /* array CMDS of cmd */
   struct exec_pfk *f_pmfun  = NULL ;             /* Array for PF Keys */
   struct f_qual_s *f_colors = NULL ;             /* Array for PF Keys */
   struct f_qual_s *f_just   = NULL ;             /* Array for PF Keys */
   struct f_qual_s *f_units  = NULL ;             /* Array for PF Keys */

   struct form_global * f_global = NIL;







TM_master->forms = os_mema( sizeof( struct form_global ) );
f_global = (struct form_global *) TM_master->forms;
os_memn(    f_global,       sizeof( struct form_global ) );

f_mcmds =  ( struct _f_exe_cmd * )
                       os_mema( sizeof( struct _f_exe_cmd ) * NUM_CMDS );
os_memn( f_mcmds, sizeof( struct _f_exe_cmd ) * NUM_CMDS );

f_pmfun =(struct exec_pfk *)os_mema(sizeof(struct exec_pfk)*NUM_PM_FUNS);
os_memn( f_pmfun, sizeof(struct exec_pfk) * NUM_PM_FUNS );

f_units =(struct f_qual_s *)os_mema(sizeof(struct f_qual_s)*NUM_UNITS  );
f_colors=(struct f_qual_s *)os_mema(sizeof(struct f_qual_s)*NUM_COLORS );
f_just  =(struct f_qual_s *)os_mema(sizeof(struct f_qual_s)*NUM_JUSTIFS);



/*+-------------------------------------------------------------------+
  | Initialize the parser tag and qualifier table.                    |
  +-------------------------------------------------------------------+*/


   s_init(&      (f_mcmds[++j].name),             "HP"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_hp ;
                  f_mcmds[j].exe_fun  = f_hp  ;
                  f_mcmds[j].end_fun  = f_ehp    ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 1);
   s_valloc   (         &( f_mcmds [j].quals.qual), 1);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 1;
   s_init (              ( f_mcmds [j].quals.qual + 1),"COLOR");
   f_mcmds[j].quals.id[1] = 1;

   s_init(&      (f_mcmds[++j].name),             "H"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_h ;
                  f_mcmds[j].exe_fun  = f_h    ;
                  f_mcmds[j].end_fun  = f_eh   ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 2);
   s_valloc   (         &( f_mcmds [j].quals.qual), 2);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 2;
   s_init (              ( f_mcmds [j].quals.qual + 1),"ID");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"STITLE");
   f_mcmds[j].quals.id[2] = 2;

   s_init(&      (f_mcmds[++j].name),             "P"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL ;
                  f_mcmds[j].exe_fun  = f_p  ;
                  f_mcmds[j].end_fun  = f_ep ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "SPACE"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_sp;
                  f_mcmds[j].exe_fun  = f_sp   ;
                  f_mcmds[j].end_fun  = NULL   ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "CENTER" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_ctr;
                  f_mcmds[j].exe_fun  = f_ctr ;
                  f_mcmds[j].end_fun  = f_ectr   ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 1);
   s_valloc   (         &( f_mcmds [j].quals.qual), 1);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 1;
   s_init (              ( f_mcmds [j].quals.qual + 1),"BREAK");
   f_mcmds[j].quals.id[1] = 1;

   s_init(&      (f_mcmds[++j].name),             "TOPM"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_top_m ;
                  f_mcmds[j].end_fun  = f_etop_m;
                  f_mcmds[j].quals.id = NULL    ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "BOTTOMM"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_bot_m ;
                  f_mcmds[j].end_fun  = f_ebot_m;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "FIELD" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_fld;
                  f_mcmds[j].exe_fun  = f_fld  ;
                  f_mcmds[j].end_fun  = NULL   ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 16);
   s_valloc   (         &( f_mcmds [j].quals.qual), 16);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 16;
   s_init (              ( f_mcmds [j].quals.qual + 1),"NAME");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"ID");
   f_mcmds[j].quals.id[2] = 2;
   s_init (              ( f_mcmds [j].quals.qual + 3),"VAR");
   f_mcmds[j].quals.id[3] = 3;
   s_init (              ( f_mcmds [j].quals.qual + 4),"INPUT");
   f_mcmds[j].quals.id[4] = 4;
   s_init (              ( f_mcmds [j].quals.qual + 5),"OUTPUT");
   f_mcmds[j].quals.id[5] = 5;
   s_init (              ( f_mcmds [j].quals.qual + 6),"LEN");
   f_mcmds[j].quals.id[6] = 6;
   s_init (              ( f_mcmds [j].quals.qual + 7),"JUSTIFY");
   f_mcmds[j].quals.id[7] = 7;
   s_init (              ( f_mcmds [j].quals.qual + 8),"DEFAULT");
   f_mcmds[j].quals.id[8] = 8;
   s_init (              ( f_mcmds [j].quals.qual + 9),"PAD");
   f_mcmds[j].quals.id[9] = 9;
   s_init (              ( f_mcmds [j].quals.qual + 10),"PROMPT");
   f_mcmds[j].quals.id[10] = 10;
   s_init (              ( f_mcmds [j].quals.qual + 11),"HELP");
   f_mcmds[j].quals.id[11] = 11;
   s_init (              ( f_mcmds [j].quals.qual + 12),"NOECHO");
   f_mcmds[j].quals.id[12] = 12;
   s_init (              ( f_mcmds [j].quals.qual + 13),"OBJECTTYPE");
   f_mcmds[j].quals.id[13] = 13;
   s_init (              ( f_mcmds [j].quals.qual + 14),"INDEX");
   f_mcmds[j].quals.id[14] = 14;
   s_init (              ( f_mcmds [j].quals.qual + 15),"REPLACE");
   f_mcmds[j].quals.id[15] = 15;
   s_init (              ( f_mcmds [j].quals.qual + 16),"OBJ");
   f_mcmds[j].quals.id[16] = 16;

   s_init(&      (f_mcmds[++j].name),             "ATTACH"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_att ;
                  f_mcmds[j].exe_fun  = f_att    ;
                  f_mcmds[j].end_fun  = NULL     ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 2);
   s_valloc   (         &( f_mcmds [j].quals.qual), 2);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 2;
   s_init (              ( f_mcmds [j].quals.qual + 1),"NAME");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"REFERENCE");
   f_mcmds[j].quals.id[2] = 2;

   s_init(&      (f_mcmds[++j].name),             "DL"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_dl  ;
                  f_mcmds[j].exe_fun  = f_dl     ;
                  f_mcmds[j].end_fun  = f_edl    ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 5);
   s_valloc   (         &( f_mcmds [j].quals.qual), 5);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 5;
   s_init (              ( f_mcmds [j].quals.qual + 1),"TSIZE");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"BREAK");
   f_mcmds[j].quals.id[2] = 2;
   s_init (              ( f_mcmds [j].quals.qual + 3),"COMPACT");
   f_mcmds[j].quals.id[3] = 3;
   s_init (              ( f_mcmds [j].quals.qual + 4),"HEADHI");
   f_mcmds[j].quals.id[4] = 4;
   s_init (              ( f_mcmds [j].quals.qual + 5),"TERMHI");
   f_mcmds[j].quals.id[5] = 5;

   s_init(&      (f_mcmds[++j].name),             "DT"     ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_dt    ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL    ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "DD"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_dd    ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "PF"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_pf ;
                  f_mcmds[j].exe_fun  = NULL    ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "SL"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_sl    ;
                  f_mcmds[j].end_fun  = f_esl   ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "OL"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_ol    ;
                  f_mcmds[j].end_fun  = f_eol   ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "UL"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_ul    ;
                  f_mcmds[j].end_fun  = f_eul   ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "LI"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_li    ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "XMP"   ) ;
                  f_mcmds[j].func_typ = FORM_XMP_0;
                  f_mcmds[j].arg_fun  = farg_xmp;
                  f_mcmds[j].exe_fun  = f_xmp   ;
                  f_mcmds[j].end_fun  = f_exmp  ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 3);
   s_valloc   (         &( f_mcmds [j].quals.qual), 3);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 3;
   s_init (              ( f_mcmds [j].quals.qual + 1),"PARSE");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"VAR");
   f_mcmds[j].quals.id[2] = 2;
   s_init (              ( f_mcmds [j].quals.qual + 3),"VARIABLE");
   f_mcmds[j].quals.id[3] = 2;

   s_init(&      (f_mcmds[++j].name),             "ALARM" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_alarm ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "IF" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_if ;
                  f_mcmds[j].exe_fun  = NULL    ;
                  f_mcmds[j].end_fun  = f_endif ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "ELSE" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_2;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_else  ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "ENDIF" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_2;
                  f_mcmds[j].arg_fun  = NULL    ;
                  f_mcmds[j].exe_fun  = f_endif ;
                  f_mcmds[j].end_fun  = NULL    ;
                  f_mcmds[j].quals.id = NULL ;
                  f_mcmds[j].quals.qual = S_NULL;

   s_init(&      (f_mcmds[++j].name),             "REMARK" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_3;
                  f_mcmds[j].arg_fun  = NULL      ;
                  f_mcmds[j].exe_fun  = f_rem     ;
                  f_mcmds[j].end_fun  = f_erem    ;
                  f_mcmds[j].quals.id = NULL      ;
                  f_mcmds[j].quals.qual = S_NULL  ;

   s_init(&      (f_mcmds[++j].name),             "RL" ) ;
                  f_mcmds[j].func_typ = FORM_XMP_1;
                  f_mcmds[j].arg_fun  = farg_rl;
                  f_mcmds[j].exe_fun  = f_rl   ;
                  f_mcmds[j].end_fun  = f_erl  ;
         f_mcmds[j].quals.id = (sint *) os_mema(sizeof (sint) * 3);
   s_valloc   (         &( f_mcmds [j].quals.qual), 3);
   s_smp_ul   ( (string) ( f_mcmds [j].quals.qual)   ) = 3;
   s_init (              ( f_mcmds [j].quals.qual + 1),"LIST");
   f_mcmds[j].quals.id[1] = 1;
   s_init (              ( f_mcmds [j].quals.qual + 2),"REFERENCE");
   f_mcmds[j].quals.id[2] = 2;
   s_init (              ( f_mcmds [j].quals.qual + 3),"NAME");
   f_mcmds[j].quals.id[3] = 3;



   f_global->f_cmds  = (byte *) f_mcmds ;
   f_global->f_ncmds =              ++j ;


/*+-------------------------------------------------------------------+
  | Define a function table of routines to call corresponding to the  |
  | available PM functions.  If a user requests one of these functions|
  | it will be handled by the PM and control will stay with the PM.   |
  +-------------------------------------------------------------------+*/

   f_pmfun[ PM_NEXT  ].exe_fun = p_next   ;
   f_pmfun[ PM_PREV  ].exe_fun = p_prev   ;
   f_pmfun[ PM_TOP   ].exe_fun = p_top    ;
   f_pmfun[ PM_BOT   ].exe_fun = p_bot    ;
   f_pmfun[ PM_HOME  ].exe_fun = p_home   ;
   f_pmfun[ PM_HPREV ].exe_fun = p_hprev  ;
   f_pmfun[ PM_HNEXT ].exe_fun = p_hnext  ;
   f_pmfun[ PM_LEFT  ].exe_fun = p_left   ;
   f_pmfun[ PM_RIGHT ].exe_fun = p_right  ;

   f_global->pm_fun     = (byte *) f_pmfun ;

   f_global->num_pm_fun = NUM_PM_FUNS ;


/*+-------------------------------------------------------------------+
  | Define the global default function key value set.                 |
  +-------------------------------------------------------------------+*/

   key_set = f_global->pf_key ;

   key_set[  0 ] =  FORM_ENTER    ;          /*   Enter  */
   key_set[  1 ] =  FORM_HELP     ;          /*   PF 1   */
   key_set[  2 ] =  FORM_CONFIRM  ;          /*   PF 2   */
   key_set[  3 ] =  FORM_QUIT     ;          /*   PF 3   */
   key_set[  4 ] =  FORM_PROCESS  ;          /*   PF 4   */
   key_set[  5 ] =  FORM_REPLY    ;          /*   PF 5   */
   key_set[  6 ] =  FORM_PRINT    ;          /*   PF 6   */
   key_set[  7 ] =  FORM_PAGE_UP  ;          /*   PF 8   */
   key_set[  8 ] =  FORM_PAGE_DOWN;          /*   PF 7   */
   key_set[  9 ] =  FORM_SCAN     ;          /*   PF 9   */
   key_set[ 12 ] =  FORM_HOMEBASE ;          /*   PF12   */
   key_set[ 16 ] =  FORM_TOP      ;          /*   pf16   */
   key_set[ 17 ] =  FORM_BOTTOM   ;          /*   pf17   */
   key_set[ 18 ] =  FORM_HOME     ;          /*   pf18   */
   key_set[ 19 ] =  FORM_HPREV    ;          /*   pf19   */
   key_set[ 20 ] =  FORM_HNEXT    ;          /*   pf20   */
   key_set[ 22 ] =  FORM_LEFT     ;          /*   pf22   */
   key_set[ 24 ] =  FORM_RIGHT    ;          /*   pf24   */
   key_set[ 25 ] =  FORM_CLEAR    ;          /*   Clear  */
   key_set[ 26 ] =  FORM_ESCAPE   ;          /*   PA 1   */
   key_set[ 27 ] =  FORM_EDIT_HELP;          /*   PA 2   */

   return(0);
}
