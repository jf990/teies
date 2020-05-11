/* This file is program generated from file "DBV_I.H" */
/* using REXX routine CI2E */
#define DBV_I_H_DEFINED 1

struct dbv_cb {
   struct dbv_str **array;   /* hash array */
   struct dbv_cb *next;      /* next lower control block, NIL term */
   hword level;              /* current level in use (for push/pop) */
   struct dbv_str *top;      /* place to put next dbv_str element */
   void *end;                /* where the end of the cb is */
   byte ident [ 16 ];
                  /*    123456789abcdef0 */
#define DBV_CB_ID_ON   "DBV_IDENT_ON_CB "
#define DBV_CB_ID_OFF  "DBV_IDENT_OFF_CB" */

   };

struct dbv_str {
   struct dbv_str *next;
   hword label_len;
   hword data_len;
   hword level;
   hword type;
   };

#define DBV_TYPE_CONSTANT 1 /* it is a constant size declaraction */
#define DBV_TYPE_STRING 2   /* needs an indirect call to free mem */
#define DBV_TYPE_LOWEST_UPT 100 /* lowest user programable type */




extern struct dbv_sys {
   struct dbv_cb *top;
   hword array_size;
   hword table_size;
   } dbvsys;


/* last count  5 $extract $maker-proto */

hword                        dbv_add(void *label, hword label_type
                             , hword data_type, hword flags
                             , void *initial_value, hword size);

#define DBV_ADD_LABEL_C 99
#define DBV_ADD_LABEL_T 98
#define DBV_ADD_TYPE_CS 1 /* C string */
#define DBV_ADD_TYPE_TS 2 /* teies string */
#define DBV_ADD_TYPE_STR 3 /* structure, fixed size */
/* types 100-200 are user programable types */

#define DBV_ADD_HISTORY   1 /* scan history and fetch if it exists */
#define DBV_ADD_NOHISTORY 0 /* just add if not already there */

/* return codes */
#define DBV_ADD_OK 13040
#define DBV_ADD_DUP 13041




hword                        dbv_beg(hword how);

#define DBV_BEG_NORMAL 1 /* use standalone system */
#define DBV_BEG_TM     2 /* use tm level */

/* return codes */
#define DBV_BEG_OK 13010
#define DBV_BEG_HOW_UNKNOWN 13011



hword                        dbv_end(void);
#define DBV_END_OK 13050





void                        *dbv_get(void *label, hword label_type
                             , hword flags);
                            /* if found, will point to data */

#define DBV_GET_C_STRING 12
#define DBV_GET_T_STRING 13
#define DBV_GET_HISTORY 1      /* will search history chain */
#define DBV_GET_NOHISTORY 0
#define DBV_GET_COPY2PRESENT 2 /* copy to present level */



hword                        dbv_pop(void);
#define DBV_POP_OK 13030
#define DBV_POP_FIRST_ONE 13031 /* can't pop first level */


hword                        dbv_push(void);
#define DBV_PUSH_OK 13020


hword                        dbv_stat(void *label, hword label_type
                             , hword what2check);
#define DBV_STAT_C_STRING 1
#define DBV_STAT_T_STRING 2

#define DBV_STAT_LEVEL 100
#define DBV_STAT_LEN   101
#define DBV_STAT_TYPE  102
#define DBV_STAT_EXIST 103





