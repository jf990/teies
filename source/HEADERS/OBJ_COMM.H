/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ_COMM H                                             |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    June  8, 1988                                          |
  |  <LEVEL>   Object                                                 |
  |  <PURPOSE> This header file contains information that is to be    |
  |            common to both the internal and external OBJ headers.  |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

struct      x_oid_x                       /* o o o o o o o o o o o o o */
{                                         /*                           */
            byte one;                     /*                           */
            byte two;                     /*    D e f i n e what an    */
            byte three;                   /*       OID is.             */
            byte four;                    /*                           */
            byte five;                    /*                           */
            byte six;                     /*                           */
            byte seven;                   /*                           */
            byte eight;                   /*                           */
};                                        /*                           */
                                          /*                           */
typedef     struct x_oid_x    OID;        /* o o o o o o o o o o o o o */

#define     OID_IS_NULL( oidptr )        ( (*oidptr).one   == 0  && \
                                           (*oidptr).two   == 0  && \
                                           (*oidptr).three == 0  && \
                                           (*oidptr).four  == 0  && \
                                           (*oidptr).five  == 0  && \
                                           (*oidptr).six   == 0  && \
                                           (*oidptr).seven == 0  && \
                                           (*oidptr).eight == 0     )

#define     OID_IS_PSI( oidptr )         ( (*oidptr).one   == 0x88 && \
                                           (*oidptr).two   == 0x99 && \
                                           (*oidptr).three == 0xAA && \
                                           (*oidptr).four  == 0xBB && \
                                           (*oidptr).five  == 0xCC && \
                                           (*oidptr).six   == 0xDD && \
                                           (*oidptr).seven == 0xEE && \
                                           (*oidptr).eight == 0xFF    )

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*                                                             */
/*   Structure definition for the Object Definition Table      */
/*                                                             */
/*                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define MAX_COMPONENT_NAME_LENGTH 16

extern struct ODT_RECORD     /* Structure for an element of an ODT     */
{
   byte      name [ MAX_COMPONENT_NAME_LENGTH + S_OVERHEAD ] ;
                             /* Object Component Descriptor's name     */
   byte      level;          /* Level of nesting                       */
   hword     offset;         /* Offset from the beginning of segment   */
   hword     offnxt;         /* ODT record number of next nested level */
   hword     offprv;         /* ODT record number of prev nested level */
   hword     type;           /* Type of Component (#defined below)     */
   PRIVS     privs;          /* A matrix containing priviledge masks   */

} ;

typedef       struct  ODT_RECORD    odtrecord;
typedef       struct  ODT_RECORD    ODT;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*   Structure definition for the Object Control Block         */
/*                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern struct __OCB
{

   OID         oid       ;        /* Object Identifier                 */
   hword       mode      ;        /* Mode that object was open for     */
   hword       lfd       ;        /* Logical File Descriptor for obj   */
   tint        goid      ;        /* Group Object Identifier for obj   */
struct dms_cb *dmscb     ;        /* Pointer to DMS control block, and */
                                  /*  if OS_FILE then it is the (sint) */
                                  /*  file descriptor.                 */
   hword       type      ;        /* Object type.  If an OS_FILE, the  */
                                  /*  type will be one of the following*/
                                  /*                                   */
#define OS_FILE_FIXED_FORMAT                  'F'
#define OS_FILE_VARIABLE_FORMAT               'V'
                                  /*                                   */
   hword       isvref    ;        /* ISV reference to name of object   */
   datetime    d_t       ;        /* Date/Time object was opened       */
   byte      * actions   ;        /* Pointer to Actions upon object    */
   odtrecord * odt       ;        /* Ptr to Object Definition Table    */
   string      lastdbp   ;        /* Last Database path used           */
   sint        status    ;        /* Status of the object              */
   tint        flags     ;        /* Status flags indicating state     */

                              /* = = = = = = = = = = = = = = = = = = = *\
                               | Obj open and close take the ocb->mode |
                               | bit map for various condition flags   |
                               | which are defined as follows:         |
                              \* = = = = = = = = = = = = = = = = = = = */
#define     OBJ_REMOTE       1   /*     00000000 00000001              */
#define     OBJ_COMMIT       2   /*     00000000 00000010              */
#define     OBJ_UPDATE       2   /*     00000000 00000010              */
#define     OBJ_READ         4   /*     00000000 00000100              */
#define     OBJ_ABANDON      8   /*     00000000 00001000              */
#define     OBJ_CANCEL       8   /*     00000000 00001000              */
#define     OBJ_FLUSH       16   /*     00000000 00010000              */
#define     OBJ_REFRESH     16   /*     00000000 00010000              */
#define     OBJ_DELETE      32   /*     00000000 00100000              */
#define     OBJ_PENDING_SYNC 64  /*     00000000 01000000              */
#define     OBJ_SYNC_PENDING 64  /*     00000000 01000000              */
#define     OBJ_NOT_OBJECT 128   /*     00000000 10000000              */
#define     OS_FILE       0x80   /*                                    */
#define     LOCAL_OS_FILE 0x80   /*                                    */
#define     OBJ_RAW       0x100  /*     00000001 00000000              */
#define     OBJ_BINARY_MODE 512  /*     00000010 00000000 For OS Files */
#define     OBJ_CACHE_PENDING 1024 /*   00000100 00000000 For OBJ_REP  */
#define     OBJ_CACHE_UPDATE  2048 /*   00001000 00000000 For OBJ_REP  */



 struct __OCB *nextocb   ;        /* Pointer to next OCB               */
 struct __OCB *prevocb   ;        /* Pointer to previous OCB           */
   string      gfd       ;        /* TEIES Str pointer to a local OS   */
   string      last_ocd  ;        /* Last OCD processed for dbpath     */
   string      last_key  ;        /* Last key used in a Ndx search     */
   byte      * last_seg  ;        /* Last segment used by DMS calls    */
   sint        last_seg_len;      /* Length of that last segment       */
   ODT       * last_odt_entry;    /* Last OCD table pointer used.      */
   string      server    ;        /* Server who owns this object.      */
   sint        open_count;        /* Counter maintained by _opn & _cls */
   hword       buf_len   ;        /* Length read in or to write        */
   byte        location  ;        /* Distributed object flag (see below*/

#define          REMOTE_OBJECT   99
#define          LOCAL_OBJECT    98
#define          FOREIGN_OBJECT  97

   byte        test_data ;        /* For test routines.                */

} ;

typedef struct __OCB OCB;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*   Structure definition for an Object Component Descriptor   */
/*                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern struct __OCD
{
   string name       ;       /* Name of the Object component           */
   sint   bias       ;       /* Signed offset to be used in ndx search */
   string key        ;       /* Key to the indx segment containing cmp */
   sint   num_keys   ;       /* Number of keys for this component      */
} ;

typedef struct __OCD ocdrecord;
typedef struct __OCD OCD;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*    These constants are for use with the type field of an ODT    */
/*    Each constant is used as a HALFWORD in the object level.     */
/*                                                                 */
/*    Primitive data types have been moved to DATAPRIM.H to be     */
/*    used by both OBJECT and VARIABLE levels.                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

                                  /*  - - - - - Segment Types - - - - -*/
#define       OC_SEG     257      /* Repeating Segment                 */
#define       OC_NDX     258      /* Index Segment                     */
#define       OC_ARY     259      /* Array Segment                     */
#define       OC_HSEG    260      /* Hierarchial Segment               */
#define       OC_HHSG    261      /* Root-Hierarchial Segment          */
#define       OC_RSEG    261      /* Root-Hierarchial Segment          */
#define       OC_ROOT    261      /* Root-Hierarchial Segment          */
                                  /* This OCD type is to be used when
                                     the level beneath it contains
                                     both Data and Segments.  The
                                     data must come first.             */
#define       OC_MAX     261      /* Boundry for range checking        */


/*+------------------------------------------------------------------+*/
/*|  Structures required for the ODT_Make function.                  |*/
/*|   ** Remove the array definition and make it a TM_master ptr **  |*/
/*+------------------------------------------------------------------+*/

#define ODT_TYPES 15                  /* Total # of ODT types minus 1 */

extern struct ODT_T_R
{
   string name;
   sint   type;
   sint   length;
};

extern struct ODT_T_R odt_t_r [ODT_TYPES+1];


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*    A table is needed to map Object names with their respective  */
/*    types and ISV keys. Initialized in OBJ_TINI() and created    */
/*    in OBJ_TABLE H.                                              */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*    These constants are for use in the type field of ODT 0 to    */
/*    indicate of what type of object the ODT defines. Also used as*/
/*    the index key into the Action/Object Table - defined in      */
/*    OBJ_ACT.H                                                    */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define NUMBER_OF_OBJECTS 12      /* DO not count PSI objects      */
#define TOTAL_OBJECTS     27      /* Count PSI Objects             */


#define         ANY_OBJ    0      /* When the object is not known  */
                                  /*   but the Action is           */
#define        USER_OBJ    1      /* User    Directory Object      */
#define       GROUP_OBJ    2      /* Group   Directory Object      */
#define        GRUP_OBJ    2      /* Group   Directory Object      */
#define      NETDIR_OBJ    3      /* Network Directory Object      */
#define     NETWORK_OBJ    3      /* Network Directory Object      */
#define        NTDR_OBJ    3      /* Network Directory Object      */
#define  CONFERENCE_OBJ    4      /* Conference Root               */
#define      CONFER_OBJ    4      /* Conference Root               */
#define        CONF_OBJ    4      /* Conference Root               */
#define          CR_OBJ    4      /* Conference Root               */
#define     COMMENT_OBJ    5      /* Conference Comment            */
#define     CONFCOM_OBJ    5      /* Conference Comment            */
#define          CC_OBJ    5      /* Conference Comment            */
#define     MAILMES_OBJ    6      /* Mail Message                  */
#define        MAIL_OBJ    6      /* Mail Message                  */
#define         NTF_OBJ    7      /* Notification                  */
#define       NOTIF_OBJ    7      /* Notification                  */
#define NOTIFICATION_OBJ   7      /* Notification                  */
#define    ACTIVITY_OBJ    8      /* Activities                    */
#define         ACT_OBJ    8      /* Activities                    */
#define        MEMB_OBJ    9      /* Membership List Object        */
#define  MEMBERSHIP_OBJ    9      /* Membership List Object        */
#define        LIST_OBJ   10      /* List processing object        */
#define      TICKET_OBJ   11      /* Ticket attachment links       */
#define        TICK_OBJ   11      /* Ticket attachment links       */
#define    CONTENTS_OBJ   12      /* Item contents (the TEXT)      */
#define END_USER_OBJECT_RANGE 12  /*                               */
/*   = = = = = = = = = = = = Object 13(D) through 34(X) Reserved   */
#define START_SYSTEM_INDEX_RANGE 35 /* Lowest possible system index*/
#define         SYS_OBJ   35      /* System Parts                  */
#define         SGI_OBJ   36      /* System Group Index            */
#define         PSI_OBJ   37      /* Primary System Index          */
#define       SOIDI_OBJ   38      /* System OID Index              */
#define       SNTFI_OBJ   39      /* System NoTiFication Index     */
#define         SKI_OBJ   40      /* System Keyword Index          */
#define         SAI_OBJ   41      /* System Author Index           */
#define        SDTI_OBJ   42      /* System Date/Time Index        */
#define         CRI_OBJ   43      /* Conference Root Index         */
#define       SODTI_OBJ   44      /* Object Definition Table       */
#define         ARI_OBJ   45      /* Action Resolution Index       */
#define         SHI_OBJ   46      /* System Help Index             */
#define         SNI_OBJ   47      /* System Name Index             */
#define       SERRI_OBJ   48      /* System Error Index Object     */
#define         SFI_OBJ   49      /* System Forms Index            */
#define       SDELI_OBJ   50      /* System Deletion Index         */
#define         SMI_OBJ   51      /* System Mail ID Index          */
#define     CCINDEX_OBJ   52      /* Conference comment index for  */
#define COMMENT_INDEX_OBJECT 52   /*   conference roots.           */
#define END_SYSTEM_INDEX_RANGE 52 /* Limit of System Indexes       */

#define         MAX_OBJ   52      /* Range limit                   */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/*    Define constants for the Actions available on each object:       */
/*Also used as the index key into the Action/Object Table - defined in */
/*    OBJ_ACT.H                                                        */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define NUMBER_OF_ACTIONS  15


#define           ANY_ACT   0     /* When the Action is known but  */
                                  /*    the Object is not.         */
#define          VIEW_ACT   1     /*   vw                          */
#define          SCAN_ACT   2     /*   sc                          */
#define        CREATE_ACT   3     /*   cr                          */
#define       COMPOSE_ACT   3     /*   cr                          */
#define        MODIFY_ACT   4     /*   md                          */
#define        REMOVE_ACT   5     /*   rm                          */
#define        REVIEW_ACT   6     /*   rv                          */
#define          FIND_ACT   7     /*   fn                          */
#define          JOIN_ACT   8     /*   jn                          */
#define         ENTER_ACT   8     /*   jn                          */
#define       RECEIVE_ACT   9     /*   rc                          */
#define            DO_ACT  10     /*   do                          */
#define           GET_ACT  11     /*   gt                          */
#define      ORGANIZE_ACT  12     /*   or                          */
#define          LINK_ACT  13     /*   lk                          */
#define      ABSTRACT_ACT  14     /*   ab                          */
#define         EXIST_ACT  15     /*   ex                          */

#define           MAX_ACT  15     /*  Upper limit of the Actions   */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*   Object - Action Modifiers for use in the user interface.  */
/*                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ANY_MOD    1
#define NEW_MOD    1
#define OLD_MOD    2
#define MARKED_MOD 3
#define UNDONE_MOD 4
#define DONE_MOD   5

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*   Structure definition for an Object/Action Matrix Table.   */
/*   Global to the system:                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


extern struct _act_obj
       {

          sint (*(linked_code))();

       }  AOM[ NUMBER_OF_ACTIONS ][ NUMBER_OF_OBJECTS ];

typedef struct _act_obj OBJ_ACT;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                             */
/*   Structure definition for an Object/Action Matrix Table.   */
/*   Local for each Object, for tailoring:                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


extern   struct _oamt_
         {

              sint      action;      /* Action code number (see ISV.h) */
              byte      role[6];   /* Privilege/role bit mask        */
              byte      code_type;   /* Type of action code:           */

#define                 NOT_IMPLEMENTED             0
#define                 COMPILED_AND_LINKED         1
#define                 INTERNAL_INTERPRETED        2
#define                 EXTERNAL_INTERPRETED        3
#define                 COMPILED_NOT_LINKED         4

              byte      code_path[20];/*TEIES str with the location  */
                                        /*  of where the code is       */
              byte      form_path[20];/*TEIES str with the location  */
                                        /*  of where the Form source is*/
         };                             /*                             */
                                        /* NOTE: the action code # is  */
typedef  struct _oamt_ OAMT;            /*  the same number as the ISV */
                                        /*  code for that string.      */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                     *
 *  This macro will return a TRUE if oid - a is identical to oid - b   *
 *                     and a FALSE otherwise                           *
 *                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define oid_cmp(a,b)    ( ((a).one    == (b).one  )  &&\
                          ((a).eight  == (b).eight)  &&\
                          ((a).seven  == (b).seven)  &&\
                          ((a).six    == (b).six  )  &&\
                          ((a).five   == (b).five )  &&\
                          ((a).four   == (b).four )  &&\
                          ((a).three  == (b).three)  &&\
                          ((a).two    == (b).two  )  )

#define oid_pcmp(a,b)   ( ((a)->one   == (b)->one  ) &&\
                          ((a)->eight == (b)->eight) &&\
                          ((a)->seven == (b)->seven) &&\
                          ((a)->six   == (b)->six  ) &&\
                          ((a)->five  == (b)->five ) &&\
                          ((a)->four  == (b)->four ) &&\
                          ((a)->three == (b)->three) &&\
                          ((a)->two   == (b)->two  ) )


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macro returns the pointer to the Object Definition    *
 * Table given the pointer to the Object Control Block.                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define ocb_odt(ocb_entry)     ((ocb_entry)->odt)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macros return the fields of an ODT record when passed *
 * the pointer to the record in 'odt_entry'.                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define odt_name(odt_entry)             ((string)(odt_entry)->name)
#define odt_lvl(odt_entry)              ((odt_entry)->level)
#define odt_level(odt_entry)            ((odt_entry)->level)
#define odt_off(odt_entry)              ((odt_entry)->offset)
#define odt_offset(odt_entry)           ((odt_entry)->offset)
#define odt_nxto(odt_entry)             ((odt_entry)->offnxt)
#define odt_next_offset(odt_entry)      ((odt_entry)->offnxt)
#define odt_prvo(odt_entry)             ((odt_entry)->offprv)
#define odt_previous_offset(odt_entry)  ((odt_entry)->offprv)
#define odt_type(odt_entry)             ((odt_entry)->type)
#define odt_priv(odt_entry)             ( &((odt_entry)->privs))
#define odt_lktyp(odt_entry)        ((hword *)&((odt_entry)->privs.five))
#define odt_link_type(odt_entry)    ((hword *)&((odt_entry)->privs.five))
#define odt_inhr(odt_entry)         (*((ODT **) &((odt_entry)->privs)) )
#define odt_inheritance(odt_entry)  (*((ODT **) &((odt_entry)->privs)) )


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macro will return the record number of the ODT record *
 * if it is passed the pointer to the ODT table in 'odt' and the       *
 * pointer to the ODT record in 'odt_entry'.                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define ocd_num(odt,odt_entry)   ((odt_entry)-(odt))

#define OCB_POINTER_OK(ocbptr) \
                      ((ocbptr != NIL) && (ocbptr != (OCB  *) -1 ))
#define OCD_POINTER_OK(ocdptr) \
                      ((ocdptr != NIL) && (ocdptr != (OCD  *) -1 ))
#define ODT_POINTER_OK(odtptr) \
                      ((odtptr != NIL) && (odtptr != (ODT  *) -1 ))
#define OID_POINTER_OK(oidptr) \
                      ((oidptr != NIL) && (oidptr != (OID  *) -1 ))
#define DATA_POINTER_OK( data) \
                      ((data   != NIL) && (data   != (byte *) -1 ))


#define obj_sflag   ( ocb, flag )     ( (ocb)->mode  |=  flag )
#define obj_set_flag( ocb, flag )     ( (ocb)->mode  |=  flag )

#define obj_reset_flag( ocb, flag )   ( (ocb)->mode  &= ~flag )
#define obj_rflag     ( ocb, flag )   ( (ocb)->mode  &= ~flag )


#define obj_tmode   ( ocb, bits )     ( (ocb)->mode &   bits )
#define obj_test_mode(ocb, bits )     ( (ocb)->mode &   bits )

#define obj_smode   ( ocb, bits )     ( (ocb)->mode |=  bits )
#define obj_set_mode( ocb, bits )     ( (ocb)->mode |=  bits )

#define obj_reset_mode( ocb, bits )   ( (ocb)->mode &= ~bits )
#define obj_rmode     ( ocb, bits )   ( (ocb)->mode &= ~bits )

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*    These constants are for use with the DMS interface and       */
/*    OBJ_REP(), OBJ_OPN(), OBJ_CLS() for the DMS interface.       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define     SEG_HOLD        'H'
#define     SEG_UPDATE      'U'

#define     OBJ_LEVEL_ZERO   ((hword) 0)

                              /* = = = = = = = = = = = = = = = = = = = *\
                               | Obj open and close take the ocb->flags|
                               | bit map for various condition flags   |
                               | which are defined as follows:         |
                              \* = = = = = = = = = = = = = = = = = = = */
#define     OBJ_REMOTE       1   /*     00000000 00000001              */
#define     OBJ_COMMIT       2   /*     00000000 00000010              */
#define     OBJ_UPDATE       2   /*     00000000 00000010              */
#define     OBJ_READ         4   /*     00000000 00000100              */
#define     OBJ_ABANDON      8   /*     00000000 00001000              */
#define     OBJ_CANCEL       8   /*     00000000 00001000              */
#define     OBJ_FLUSH       16   /*     00000000 00010000              */
#define     OBJ_DELETE      32   /*     00000000 00100000              */
#define     OBJ_UNUSED_64   64   /*     00000000 01000000              */
#define     OBJ_NOT_OBJECT 128   /*     00000000 10000000              */
#define     OS_FILE       0x80   /*     00000000 10000000              */

#define     PUBLIC_CONF   1
#define     GROUP_CONF    2
#define     PRIVATE_CONF  3
#define     SPECIAL_CONF  4

#define     PUBLIC_GROUP  1
#define     CLOSED_GROUP  3

#define     OCB_NOT_FOUND  ((OCB *) -1 )
#define     OCB_TABLE_FULL ((OCB *) -1 )
#define     OCB_NOT_OPEN   ((OCB *) -1 )




#define     OBJ_MINIMUM_LFD_SIZE          99
#define     OBJ_MAXIMUM_LFD_SIZE     2000000
#define     MASTERSERVER_LFD_SIZE       4900
#define     USER_LFD_SIZE                250

#define     OBJ_INTERACTIVE             0x41
#define     OBJ_OID_PROVIDED            0x42
#define     OBJ_MODIFIER_PROVIDED       0x43
#define     OBJ_OCB_PROVIDED            0x44

#define     OCD_ONLY          0
#define     STAY_IN_LEVEL     1
#define     SEGMENT_ONLY      2
#define     COMPONENT_ONLY    3
#define     ANY_COMPONENT     4
