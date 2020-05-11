#ifndef DMS_I_H_INCLUDED /* $ci2e-redefine */
#define DMS_I_H_INCLUDED /* $ci2e-redefine */
#define PRIV_LEN 6  /* eventually move to defs.h */
#define DMS_MAX_KEY_LEN 32 /* this is the same as SGI_KEY_LEN in sgi.h*/

/* $ci2e-struct */
struct dms_cb {    /* main control block for subsystem */
   hword lfd;
   tint goid;
   struct stl_cb *stlcb;

   /* memory buffer that contains variable length data segments.
      sgb routines are used to access variables here to avoid dup code.
      Allocation is done via bmm routines */
   struct virbh *buf_hdr;
   byte *buf_adr;

   /* current pointer */
   tint cur_blk;
   hword cur_seg;
   hword cur_pos;

   /* new pointer */
   tint new_blk;
   hword new_seg;
   hword new_pos;

   /* top pointer */
   tint top_blk;
   hword top_seg; /* does this do anything? It should always be 1 */

   hword path_new;  /* use DMSDLAN and DMSDLAO */
   hword open_mode; /* use DMSOEU or DMSOER */

   struct dms_dla *dla, *dlb;
   hword len_dla, len_dlb;    /* last valid element, start at one */
                              /* value zero indicates empty */

   hword fnd_dla;             /* level marker from dmsfnd */

   byte epm[ PRIV_LEN ];    /* external priv mask */
   byte ipm[ PRIV_LEN ];    /* internal priv mask */

   /* keys */
   byte *key;        /* a confusing array of pointers to strings */
   hword num_key;             /* number currently valid keys */

   /* intermedary segment address for sgj routines */
   tint sgjblk;
   hword sgjseg;

   /* lookaside for movement routines */
   hword seg_stat;
   hword seg_type;       /* sgb segment type values */
   hword seg_ref;
   byte la_key [ 32 ]; /* look aside key */

   /* status indicators */
   hword status;  /* boolean flags in here */
#define DMS_CB_READ_ONLY 1 /* do not send update info here */


   hword key_len; /* length of key */
   hword pack_stat;
/* element pack_stat in dms_cb */
#define DMS_PACK_FLAG_READY 1 /* path/data have not been packed */
#define DMS_PACK_FLAG_NOTR  0 /* do a dmsmnd to get next data */


   hword new_st; /* new segment type, to match the new set */

   /* the following element will contain "DMS CONTROL BLOK" if the
   cb is open, otherwise it will contain "DMS CLS CNTL BLK" if it is
   closed.  If neither, then it is not a valid cb */
   byte ident[16];

   /* usually false, if true will allow sgh and sgi segments to
   be modified */
   boolean allow_all_segment_access;



   tint transaction_ring_goid; /* used to link dms updates */

   /* dummy variables here for future expansion without having to
      recompile all of dms */
   hword dhword1, dhword2;
   tint dtint1, dtint2;
   byte *dbyte1, *dbyte2;

   };

/* the following 2 defines are for dms_cb element 'path_new' */
#define DMSDLAN 1  /* is new, must run dms_fnd again */
#define DMSDLAO 2  /* is old, can use DLA contents */


/* the following 2 defines are for dms_cb element 'open_mode' */
#define DMSOEU 1
#define DMSOER 2

/* number of dms_dla elements defined in dmslms ( ) */
struct dms_dla {   /* data lookaside arrays */
   tint blknum;
   hword segnum;
   hword pos;
                   /* sgj elements stored in array index 'n-1' */
   tint sgjblk;    /* used as lookaside for sgj_str */
   hword sgjseg;

   hword seg_stat;   /* indicates what dms tree movements were done */
   hword seg_type;   /* what kinda segment we got here */
   hword seg_ref;    /* hierarchical or repeating seg value */
   hword key_len;    /* length of key */
   byte  key [32]; /* storage for key being used */

   /* dummy variables here for future expansion without having to
      recompile all of dms */
   hword dhword1,dhword2;
   tint dtint1, dtint2;
   byte *dbyte1, *dbyte2;

   };

/* used to define 'what2do' movements */
#define DMSWATD 1 /* move downward from root of database entry */
#define DMSWATF 2 /* move forward through type */
#define DMSWATU 3 /* move upward toward root of database entry */
#define DMSWATE 4 /* hit end of the level */
#define DMSWATB 5 /* hit bottom when going down */


struct dmsr_hdr {   /* used in goid table blocks */
   hword ident;};


/* the following are to decide between DLA and DLB */
#define DMSIDLA 1   /* use DLA */
#define DMSIDLB 2   /* use DLB */

/* what info is in which seg numbers */
#define DMSBDP  1  /* database pointer */
#define DMSBOP  2  /* object pointer, for the future */
#define DMSBDLA 3  /* database lookaside a */
#define DMSBDLB 4  /* database lookaside b */
#define DMSBDAT 5  /* external data at current position */
#define DMSBPM  6  /* priv mask for data */
#define DMSBID  7  /* internal data at current position */
#define DMSBNDP 8  /* new database path that is being built */

extern struct dms_sys {
   boolean dms_running;
   boolean journal_active;
   boolean comment_journal;
   void *journal_file; /* its really a FILE * */


   } dmssys;


struct dms_wcb { /* wander or walk, depending on what you wanna do */
   hword lfd;
   struct dms_cb *dmscb;
   tint curblk;
   hword curseg;
   struct dms_wcb *prev_cb;
   struct virbh *blkhdr;
   struct sgb_res *sgbres;
   void *curadr;
   hword seg_type;
   hword seg_len;
   hword seg_ref; /* if hier, is seg ref, if index, 0=first, 1=other*/
   byte cur_key [ DMS_MAX_KEY_LEN ];
   hword (*user_function)();
#define DMS_WCB_CONTINUE 0 /* keep processing as normal */
#define DMS_WCB_RETURN   1 /* return from this level */
#define DMS_WCB_EXIT     2 /* terminate all processing recursively */
#define DMS_WCB_GOTO     3 /* user exit has specified where to go */

   tint *max_wander; /* if reaches zero, will abort all calls */
   hword safety_version; /* if not equal to the #def vsn, abort */

   tint usert0;
   tint usert1;
   tint usert2;
   tint usert3;
   tint usert4;

   void *userv0;
   void *userv1;
   void *userv2;
   void *userv3;
   void *userv4;
   hword key_len, path_len;

   };


/* subsystem prefix designations:
   dms_ : external interface
   dmsb : buffer management
   dmsc : close routines
   dmsd : display/diagnose
   dmsj : journaling functions
   dmsl : lookaside routines
   dmsm : movement routines
   dmso : open routines
   dmsr : root index routines
   dmst : test routines
   dmsv : remoVe from a given point on down

exceptions: dmsrplt



*/

/* $ci2e-end-struct */
#define DMS_ERROR_OUT_OF_SPACE 22099




/* $extract $maker-proto, last routine number used is 26 */


/* mainly external interface routines */

                    /* to enable easier conversions, dms_beg parms
                    temporarily removed from declaration.  They are
                    "hword lfd, hword flags".  'lfd' is who to open
                    flags is how.  If 'lfd' is zero, then initialize
                    system without opening any lfds.

                    For now: use dms_beg(0,0); to initialize system.

                    */

hword               dms_backup(hword lfd_from, hword lfd_to,hword flags);



hword               dms_beg (hword lfd, hword options);
                             /* "BEGin"
                                initialize subsystem */

#define DMS_BEG_READ_ONLY 1  /* open the file read only */
#define DMS_BEG_NOCLEAN   2  /* do not invoke auto cleanup */
#define DMS_BEG_VERIFY    4  /* calls DIC integrity check */
#define DMS_BEG_MEMORY    8  /* exists only in memory (can't be big) */
#define DMS_BEG_JOURNAL  16  /* activate journal logging */



#define DMS_BEG_OK 22130 /* ran ok */
#define DMS_BEG_NOT_EXIST 22131 /* database file does not exist */
#define DMS_BEG_CRASH     22132 /* database crashed
                                   force option not present */

tint                dms_ce  (hword lfd, hword hdrsize);
                             /* "Create Entry"
                                gets next available tint goid, allocates
                                and returns it.  hdrsize is the #
                                of elements in the root segment */

hword               dms_cls (struct dms_cb *dmscb, hword how);
                             /* "CLoSe" dms entry */
#define DMSCLSC 1   /* close commit updates */
#define DMSCLSB 2   /* close backout updates */
#define DMSCLSR 3   /* close read only <actually ignored> */

/* valid values to 'how' in dms_cls */
#define DMS_CLOSE_COMMIT   DMSCLSC
#define DMS_CLOSE_BACKOUT  DMSCLSB
#define DMS_CLOSE_READONLY DMSCLSR

/* dms_cls return codes */
#define DMS_CLOSE_OK          22110
#define DMS_CLOSE_BAD_DMSCB   22111
#define DMS_CLOSE_STL_PROBLEM 22112


/* this routine is to be phased out, use dms_qns instead */
lint                dms_cns(struct dms_cb *dmscb);
                    /* count number of segments */


hword               dms_compact( hword lfd, tint goid);
                    /* compact a goid to save space */
#define DMS_COMPACT_OK    22210
#define DMS_COMPACT_OPEN  22211  /* cannot compact an open goid */
#define DMS_COMPACT_ALLOC 22212  /* goid not allocated */


hword               dms_copy(struct dms_cb *dmscbfrom
                    , struct dms_cb *dmscbto, hword flags);
                    /* copy from dmscbfrom into same place in
                    dmscbto, from the current database path on
                    down.  Flags ignored at present time */
/* return codes */
#define DMS_COPY_OK           22010 /* it did it without problems */
#define DMS_COPY_READ_ONLY    22011 /* dmscbto is read/only */
#define DMS_COPY_FROM_INVALID 22012 /* dmscbfrom is not valid/open */
#define DMS_COPY_TO_INVALID   22013 /* dmscbto is not valid/open */
#define DMS_COPY_ERROR        22014 /* internal dms error */


struct dms_cb      *dms_cov( struct dms_cb * dmscb);
                   /*   copy old view */

hword               dms_dar(struct dms_cb *dmscb, tint curblk
                    , hword curseg, boolean at_top);
                    /* if at_top == true, then curblk and curseg
                       are ignored.  This routine prints out all
                       segments recursively from the topblk */
#define DMS_DAR_OK 22140


hword               dms_de(hword lfd,tint goid);
                            /* "Delete Entry", the whole thing */

#define DMSDELD 22020 /* operation done */
#define DMSDELN 22021 /* lfd,goid pair does not exist */
#define DMSDELO 22022 /* someone has the hword lfd,tint goid open */

#define DMS_DE_OK                  DMSDELD
#define DMS_DELETE_OK              DMSDELD
#define DMS_DELETE_ENTRY_DONE      DMSDELD /* to be phased out */
#define DMS_DELETE_ENTRY_NOT_EXIST DMSDELN
#define DMS_DELETE_ENTRY_OPEN      DMSDELO
#define DMS_DELETE_ENTRY_ERROR 22023 /* internal dms error */


hword               dms_dov ( hword lfd , tint goid);
                   /*           "Delete Old View", note: not
                                external usage */

hword               dms_dump( hword lfd, byte *file2dumpto);
                    /* dumps out contents of dms */

#define DMS_DUMP_OK 22150


                    /* arguement will be 'hword lfd' to signify
                    which lfd will be closed.  A zero means
                    shut down everything. */

hword               dms_end ();
                    /* "END"    shut down subsystem */
#define DMS_END_OK 22160


hword               dms_find(struct dms_cb *dmscb);
                                /* "Find" */

/* followin are return codes for dms_find         */
#define DMSFINDD 22030  /* no error, data found       */
#define DMSFINDN 22031  /* no error, no data found    */
#define DMSFINDE 22032  /* error in coding occured    */

#define DMS_FIND_OK         DMSFINDD
#define DMS_FIND_DATA_FOUND DMSFINDD
#define DMS_FIND_NO_DATA_FOUND DMSFINDN
#define DMS_FIND_ERROR_ENCOUNTERED DMSFINDE
#define DMS_FIND_BAD_DMSCB 22033
#define DMS_FIND_BAD_PATH 22034 /* path conflicts with str found */


hword               dms_fmt(hword lfd, tint numblks);
                            /*  "Format", hword lfd, number_of_blocks */

#define DMS_FMT_OK 22120 /* no problem noticed */
#define DMS_FMT_INTERNAL_ERROR 22129

hword               dms_jc(byte *comment);
                    /* put a comment into the journal log, C string */
#define DMS_JC_OK 22210


hword               dms_jnl(hword command, byte *filename
                    , hword lfd, hword options);
#define DMS_JNL_CMD_DUMP  500 /* dump data out */
#define DMS_JNL_CMD_INPUT 501 /* read data from file */
#define DMS_JNL_CMD_STOP  502 /* stop dump data out, keep file open */
#define DMS_JNL_CMD_CLOSE 503 /* close and stop journaling */
#define DMS_JNL_CMD_START 504 /* start journaling again */

#define DMS_JNL_OPT_DUMPALL 1 /* dump all data out first */
#define DMS_JNL_OPT_COMMENT 2 /* dump short fmt and comment printable */
#define DMS_JNL_OPT_APPEND  4 /* append to file, don't clear first */
#define DMS_JNL_OPT_CLEAR   8 /* clear dbms file before entering data */

#define DMS_JNL_OK 22200


sint                dms_link (struct dms_cb *dmscb1
                    , struct dms_cb *dmscb2); /* link 2+ updates */
/* this will link 2 or more updates together.  Once linked, the dms
will not commit the updates till all the linked dmscb's have been
closed.  This is to prevent inconsistencies between goid's if the
system crashes during an obj link operation */

#define DMS_LINK_OK 22250
#define DMS_LINK_DMSCB 22251 /* one or both dmscb's invalid */
#define DMS_LINK_LOCKED 22252 /* one or both dmscb's already linked */
                              /* note: this can be used in the future
                                 to implement semaphoring */

tint                dms_obj (hword lfd,tint command);
                            /*  Store/Fetch object root integer */

struct dms_cb      *dms_opn (hword lfd,tint goid,hword open_mode);
                   /*  "OPeN" Open database item with given mode */
#define DMSOPNU 1 /* open entry in update mode */
#define DMSOPNR 2 /* open entry in read only mode */
#define DMSOPNF 3 /* Force entry to be opened in update;
                     will execute a close backout first if the
                     entry is already open */

#define DMS_OPEN_UPDATE DMSOPNU
#define DMS_OPEN_READ_ONLY DMSOPNR
#define DMS_OPEN_FORCE_WRITE DMSOPNF
#define DMS_OPEN_DELETE 4  /* open for deleting */
#define DMS_OPEN_NEW    5  /* to copy in a big new goid */


hword               dms_pak(struct dms_cb *dmscb
                    ,byte **buffer,hword *buf_len, byte *ocb);
                    /* "Pack" segments */

#define DMS_PAK_OK    22040
#define DMS_PACK_OK   22040
#define DMS_PACK_DONE 22040
#define DMS_PACK_NOT_DONE 22041
#define DMS_PACK_ERROR 22042
#define DMS_PACK_EMPTY 22043

/* possible errors are: bad or closed dmscb, dms full, buffer invalid,
   buffer len invalid */



hword               dms_pg  (hword lfd, tint goid);
                    /*  "Print contents of Goid" */
#define DMS_PRINT_GOID_OK 22050
#define DMS_PRINT_GOID_NOT_ALLOCATED 22051
#define DMS_PRINT_GOID_CODE_ERROR 22052


lint                dms_pos (struct dms_cb *dmscb);
                    /* find the position of something relative
                    to the beginning of that level */
#define DMS_POS_BAD_DMSCB -1 /* invalid dmscb */


byte               *dms_qd  (struct dms_cb *dmscb); /*
                                "Query Data"
                                Return a ptr to the data */

hword               dms_qdl (struct dms_cb *dmscb);
                            /* NOTE: to be phased out.
                                "Query Data Length" */

byte               *dms_qdp (struct dms_cb *dmscb);
                             /* "Query Database Path" */

tint                dms_qfb( hword lfd); /* query # free blocks */


hword               dms_qgs (hword lfd,tint goid);
                    /*"Query Goid Status" */

#define Dms_Query_Goid_Status dms_qgs

/* note: these are bitwise status codes, not normal return codes */
#define DMSQGSA  0 /* available, no locks and allocated */
#define DMSQGSE  1 /* coding error */
#define DMSQGSU  2 /* goid not used */
#define DMSQGSL  4 /* locked out */
#define DMSQGSW  8 /* writable lock is set */
#define DMSQGSN 16 /* primary hword lfd not initialized or exist */

#define DMS_QUERY_GOID_STATUS_AVAILABLE         DMSQGSA
#define DMS_QUERY_GOID_STATUS_ERROR_ENCOUNTERED DMSQGSE
#define DMS_QUERY_GOID_STATUS_UNUSED_GOID       DMSQGSU
#define DMS_QUERY_GOID_STATUS_LOCKED_GOID       DMSQGSL
#define DMS_QUERY_GOID_STATUS_WRITE_LOCK_SET    DMSQGSW
#define DMS_QUERY_GOID_STATUS_NOT_INITIALIZED   DMSQGSN
#define DMS_QUERY_GOID_STATUS_LINK_LOCK         32
#define DMS_QUERY_GOID_STATUS_LFD_READ_ONLY     64
#define DMS_QUERY_GOID_STATUS_LFD_NOT_OPEN      128


byte               *dms_qik(struct dms_cb *dmscb,hword keynum);
                             /* "Query Individual Key", act ret string */

byte               *dms_qk  (struct dms_cb *dmscb);
                             /* "Query Key"
                                Returns ptr to start of key str array */

hword               dms_qkn (struct dms_cb *dmscb);
                             /* "Query Key Number"; # element in array*/

byte               *dms_qla (hword lfd);
                             /* "Query Lfd Assignment" */

hword               dms_qld (struct dms_cb *dmscb);
                            /*  "Query Length of Data" */

sint                dms_qnb( hword lfd, tint goid);
                    /* query number blocks used by a goid */




#define dms_qns DMS_query_number_segments

lint                dms_qns( struct dms_cb *dmscb);
                    /* "Query Number of Segments at current point" */
                    /* routine does not really do anything right now */
#define DMS_QNS_BAD_DMSCB -1 /* dmscb not valid */


hword               dms_qpl (struct dms_cb *dmscb);
                            /*  "Query database Path Length" */

hword               dms_qvg(hword lfd,tint goid);
                           /* "Query Valid Goid", checks lfd/goid
                              combination for possible errors or
                              "impossible" values */

#define DMS_Query_Valid_Goid dms_qvg

#define DMS_QVGL 22071 /* lfd is not valid */
#define DMS_QVGG 22072 /* goid is not valid */
#define DMS_QVGA 22073 /* goid is not allocated */
#define DMS_QVGO 22070 /* lfd and goid appear valid */

#define DMS_QUERY_VALID_GOID_LFD_BAD             DMS_QVGL
#define DMS_QUERY_VALID_GOID_GOID_BAD            DMS_QVGG
#define DMS_QUERY_VALID_GOID_GOID_NOT_ALLOCATED  DMS_QVGA
#define DMS_QUERY_VALID_GOID_OK                  DMS_QVGO


hword               dms_rmv (struct dms_cb *dmscb);
                           /*   "ReMoVe" from pointed to on down */
#define DMS_RMV_OK 22080
#define DMS_RMV_EMPTY 22081  /* its a bit iffy */
#define DMS_RMV_ERROR 22082  /* errors, not sure why yet */
#define DMS_RMV_NULL_PATH 22083 /* not allowed to remove null path */
#define DMS_RMV_TOP_ERROR 22084 /* internal dms error, should never
                               happen. <ha!> */
#define DMS_RMV_READ_ONLY 22085 /* dmscb opened read only */


hword               dms_rpl ( struct dms_cb *dmscb );
                             /* "Replace" */

/* following are return codes for dms_rpl                         */
#define DMSRPLD 22090 /* no error, data stored                        */
#define DMSRPLN 22091 /* dms limit encountered, data not stored       */
#define DMSRPLE 22092 /* dms logic error encountered, data not stored */
#define DMSRPLR 22093 /* dms entry was opened read only               */

#define DMS_RPL_OK                    DMSRPLD
#define DMS_REPLACE_OK                DMSRPLD
#define DMS_REPLACE_DATA_STORED       DMSRPLD
#define DMS_REPLACE_NOT_STORED        DMSRPLN
#define DMS_REPLACE_ERROR_ENCOUNTERED DMSRPLE
#define DMS_REPLACE_READ_ONLY         DMSRPLR
#define DMS_REPLACE_LFD_FULL          22094
#define DMS_REPLACE_BAD_DMSCB         22095
#define DMS_REPLACE_BAD_PATH          22096


hword               dms_sd  (struct dms_cb *dmscb,byte *start,lint len) ;
                             /* "Set Data" */
#define DMS_SD_OK 22170
#define DMS_SD_NOT_DONE 22171

hword               dms_sdp (struct dms_cb *dmscb,byte *start,sint len) ;
                             /* "Set Database Path" */
#define DMS_SDP_OK 22180
#define DMS_SDP_NOT_DONE 22181

hword               dms_sla (hword lfd,byte *new_name);
                            /* "Set Lfd Assignment */
#define DMS_SLA_OK 22190


hword               dms_sync( struct dms_cb *dmscb);
                    /* commit current view, keep dmscb open */
#define DMS_SYNC_OK 22260
#define DMS_SYNC_DMSCB 22261 /* invalid dmscb */
#define DMS_SYNC_LINK  22262 /* unable to sync, other members of
                                link chain are not closed */

hword               dms_unpk(struct dms_cb *dmscb
                    ,byte *buffer,hword buflen, byte *ocb);
                             /* "Unpack" buffer block */
#define DMS_UNPACK_OK           22100
#define DMS_UNPACK_READ_ONLY    22101
#define DMS_UNPACK_REPL_ERROR   22102
#define DMS_UNPACK_BAD_ARG      22103
#define DMS_UNPACK_LFD_FULL     22104


byte               *dms_vsn (void);
                           /*   Return version number as a C string */

/* $ci2e-internal */



hword               dmsfnd  (struct dms_cb *dmscb);
                          /*    Internal find function */


/* buffer management routines */

hword               dmsbbdp (struct dms_cb *dmscb);
                         /*    "Buffer Build Database Path",
                               Build db path from DLA */

byte               *dmsbgpp (struct dms_cb *dmscb);
                         /* ret actual byte in dp */

hword               dmsbmdp (struct dms_cb *dmscb);
                         /*     "Make Database Path" from DLA */

hword               dmsbsd  (struct dms_cb *dmscb
                    , struct sgbstr *segadr);
                           /*  "Buffer, Set Data" */

hword               dmsbsdp (struct dms_cb *dmscb
                    , struct sgbstr *segadr);
                           /*   "Buffer Set Database Path */

/* close routines */
hword               dmsclsb (struct dms_cb *dmscb);

hword               dmsclsc (struct dms_cb *dmscb);

hword               dmsclsr (struct dms_cb *dmscb);


/* display/debug routines */

hword               dmsdacb(struct dms_cb *dmscb);
                    /* "Display All given dms C B" */


hword               dmsdblk (hword lfd,tint blknum);

/* $ci2e-external */

hword               dmsdbuf(byte *buf_adr);
                          /*    Like dmsdblk, but arg is buffer */

/* $ci2e-internal */
hword               dmsdcb  (struct dms_cb *dmscb);
                              /*  "Display Control Block" */

hword               dmsddl  (struct dms_cb *dmscb, hword ddl);
                              /*  display database lookaside array */

hword               dmsdlfd ( hword lfd , hword flags); /* <hword lfd,
flags);
                    display the contents of an hword lfd */
#define DMSDLFDR 1 /* root block display */
#define DMSDLFDG 2 /* goid table entries */
#define DMSDLFDS 4 /* STL chain entries */
#define DMSDLFDD 8 /* general Data blocks */

hword               dmsdlg  (hword lfd,tint goid);
                             /* "Display blocks with Lfd Goid" */

/* external buffering routines:  These routines were written
   so that I can use my own debugging without having to mess
   with the standardized debuggin macros */

hword               dmsebeg ( void); /* < ); */

tint                dmsece  (hword lfd, hword hdrsize);
                            /*  "Create Entry" */

hword               dmsecls (struct dms_cb *dmscb, hword how);
                            /*  "CLoSe entry */

hword               dmsefmt (hword lfd, tint numblks);

tint                dmseobj (hword lfd,tint command);

hword               dmseqnb( hword lfd, tint blknum, hword segnum
                    , tint *block_table, tint *block_table_len);

hword               dmserpl (struct dms_cb *dmscb);


hword               dmsewander( struct dms_wcb *dmswcb, tint curblk
                    , hword curseg);

/* journaling routines */

                    /* journal dump routine a */
hword               dmsjda( hword opcode, hword lfd, tint goid
                    , tint option, tint rc);
#define DMSJDA_OPEN 1
#define DMSJDA_CLOSE 2
#define DMSJDA_DE    3
#define DMSJDA_CE    4
#define DMSJDA_FMT   5
#define DMSJDA_COPY  6
#define DMSJDA_OBJ   7
#define DMSJDA_QVG   8

hword               dmsjdb( hword opcode, struct dms_cb *dmscb
                    , tint rc);

#define DMSJDB_FIND 9
#define DMSJDB_REPL 10
#define DMSJDB_REMV 11
#define DMSJDB_QNS  12
#define DMSJDB_POS  13

hword               dmsjdc( hword opcode, struct dms_cb *dmscb
                    , byte * pack_buffer, tint rc);
#define DMSJDC_PACK 14
#define DMSJDC_UNPK 15

hword               dmsjddat( byte *initial, void *start
                    , hword length);



hword               dmsjdmp(hword lfd, byte *file2use, hword options);
#define DMSJDMP_APPEND  1 /* append file on open */
#define DMSJDMP_COMMENT 2 /* comment the file, use short format */


hword               dmsjinp(hword lfd, byte *file2use, hword options);
#define DMSJINP_CLEAR 1 /* clear dbms before use */



/* lookaside array management routines */
hword               dmslce  (struct dms_cb *dmscb, hword dlasel);
                            /*  clear DL entry */

struct dms_dla     *dmslge  (struct dms_cb *dmscb
                   ,hword  dlasel, hword dlapos);
                         /*     get dl ptr given dl num and dlapos */

hword               dmslia  (struct dms_cb *dmscb);
                           /*   "Initialize dlA" */

hword               dmslms  (void ); /*max size of DLA, DLB */

hword               dmsludl (struct dms_cb *dmscb, hword dlasel);
                          /*    update at end, DLA or DLB */

/* movement routines */
#define DMS_MOVE_FATAL 64 /* fatal error in move operation */

hword               dmsmd   (struct dms_cb *dmscb);
                             /* "Move Down" */

hword               dmsmf   (struct dms_cb *dmscb);
                             /* "Move Forward" */

hword               dmsmnd  (struct dms_cb *dmscb);
                             /* "Move to Next Data" element */
#define DMSMND_OK 0    /* data element returned */
#define DMSMND_ERROR 1 /* coding error or limit encountered */
#define DMSMND_END   2 /* reached level set by orig dms_sdp */

hword               dmsmrf  (struct dms_cb *dmscb);
                             /* "Move Recursively Forward" */

hword               dmsmu   (struct dms_cb *dmscb);
                             /* "Move Up" the tree */


/* open routines */

struct dms_cb      *dmsoen  (hword lfd, tint goid);
                   /* open entry as "new", has fewer view checks */


struct dms_cb      *dmsoeu  (hword lfd, tint goid, hword open_mode);

struct dms_cb      *dmsoer  (hword lfd,tint goid);


/* "root" routines */

tint                dmsrae  (hword lfd);
                            /* "Root, Allocate Entry"
                               allocate a tint goid and return it */

hword               dmsrce  (hword lfd, tint goid, hword hdr_size);
                            /* Create Entry, puts in a specific
                            goid, warning: vir_root->gt_low may
                            not be set correctly */

hword               dmsrde  (hword lfd,tint goid);
                           /*  "Root, delete entry"
                               free the tint goid entry */

tint                dmsrgn  (hword lfd, tint goid);
                               /* "Root, Get Next", given hword lfd and
                               tint goid, will return next allocated goid
                               or zero if error or at end */

tint                dmsrgng (void); /*
                               "Get Number Goid" pointers per block */

tint                dmsrgr  (hword lfd,tint goid);
                           /*  get root block given tint goid */

hword               dmsrit  (hword lfd);
                          /*&  initialize tint goid table */

hword               dmsrrr(hword lfd,tint goid,tint rootblk);
                           /*  Replace Root Block */

/* remoVe routines */
hword               dmsva(struct dms_cb *dmscb); /* "remoVe All" */

#define DMSVA_OK 0 /* no error */
#define DMSVA_ERROR 1 /* general purpose error */

hword               dmsvnt(struct dms_cb *dmscb); /* "remoVe New Tre    e" */


hword               dmsvr(struct dms_cb *dmscb, tint curblk
                    , hword curseg);
                    /* remoVe recursively */


/* wander routines */
hword               dmswbdp(struct dms_wcb *dmswcb, byte *path_adr);

hword               dmswrbdp(struct dms_wcb *dmswcb, byte *path_adr);
                    /* recursive version */

hword               dmswqnb(struct dms_wcb *dmswcb);



/* heX routines */

hword               dmsxdo(byte *source, byte *destination, hword len);
                    /* dump src -> destination modified pdump fashion */


hword               dmsxsdp(struct dms_cb *dmscb
                    ,byte *path, lint length);
                    /* if length is -1, then find length by
                    a null terminated string */


/* exceptions to subprefix conventions */


hword               dmsrplt(struct dms_cb *dmscb
                    ,tint rpl_blk, hword rpl_seg)  ;
                          /*   "RePLace Tree", part of replace */

/* general special routines */
byte               *x2c( byte * address,hword how_many);

/* $endextract */

/* $ci2e-external */




#endif /* initial ifndef */
