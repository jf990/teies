#ifndef SGB_I_H_INCLUDED
#define SGB_I_H_INCLUDED
/* block level segment routines */
/* all these assume you have a in memory buffer and a segment
number inside the block.  From there we have all the basic
operations, get, put, add, delete, get free space  */

/* structure of reserved hwords */
/* $ci2e-struct */
struct sgb_res{
   hword ident;       /* for database dump routines, set in sgb_int */
   hword blksize;     /* size of block in bytes */
   hword linkcount;   /* # links to this block */
   hword sla_size;    /* # elements in Segment Look-Aside table */
   hword free;        /* the start of "free" space in buffer */
   tint goid;         /* goid of item */
   };

struct sgbstr{
    hword length;     /* holds length of data alone */
    hword type;       /* used by DMS to identify segment types */
    /* actual data goes here */
    };

struct sgb_sr{     /* reference to another segment */
   hword segnum;   /* valid segment numbers are 1..4095
                      top 4 bits reserved for future expansion */
   tint blknum;
   };


/* $ci2e-end-struct */
/* routine names $EXTRACT $maker-proto */

hword sgb_ass(byte *block, hword size);
                   /*   allocate a seg given size, returns #, zero err
                        Note: size is a hword                         */

hword sgb_cpy( struct sgbstr * seg1 , struct sgbstr * seg2);
                   /*   copy from seg1 to seg2 */

hword sgb_dis(byte *block);       /*  display all segments */

hword sgb_dre(byte *block);
                        /* "Display Reserved Elements" */

hword sgb_ds(byte *block,hword seg); /* delete segment */

struct sgbstr *sgb_fs(byte *block, hword seg);
                   /*   returns a pointer to segment */

byte *sgb_gsd(struct sgbstr *seg);
                    /*  ret ptr to data itself */

hword sgb_gsl(struct sgbstr *seg);
                    /*  returns ONLY the length of segment */

hword sgb_gss(struct sgbstr *seg);
                    /*  returns the length of the segment PLUS
                        the size of the struct info */


byte *sgb_qd(byte *block, hword segnum); /* get start addr of seg */

hword sgb_ql(byte *block, hword segnum); /* get length of data */

hword sgb_qt(byte *block, hword segnum); /* get segment type */

/* $ci2e-internal */
byte *sgb_fls( byte * block , hword thresh);
                   /*   finds lowest offset over given threshold
                        returns NIL if it is the highest segment */

hword sgb_gas( struct sgb_res * block);
                   /*   returns # elements in offset array */
hword sgb_gfb( byte * block);
                   /*   returns # of free bytes in block */

struct sgb_sr *sgb_gfp( byte * block , hword segnum , hword field);
                   /*          get a ptr to a seg reference */

hword sgb_glc( struct sgb_res * block);
                    /*  returns link count on block */

hword sgb_gdns(void);
                    /*  "Get Default Number of Segments" */

hword *sgb_gop( byte * block , hword segnum);
                    /*  ret address of offset pointer */

hword sgb_ns(struct sgb_res *bufadr);
                    /*  "Normalize Segment" structures */
/* $ci2e-external */

hword sgb_qst(struct sgbstr *seg);
                    /*  "Query Segment Type" */
hword sgb_sst(struct sgbstr *seg, hword type);
                    /*  "Set Segment Type" */
#define SGBSTE 0 /* error */
#define SGBSTU 1 /* unknown */
#define SGBSTH 2 /* hierarchical */
#define SGBSTR 3 /* repeating */
#define SGBSTI 4 /* index */
#define SGBSTB 5 /* basic, no formating */
#define SGBSTD 5 /* data segment */
#define SGBSTJ 6 /* aux index segment structure */
#define SGBSTK 7 /* key for the index */
#define SGBSTPP 8 /* dms_pak path */
#define SGBSTPD 9 /* dms_pak data */
#define SGBSTPDP 10 /* dms_pak delete path */


hword sgb_int(byte *block, hword count,hword blksize);
                   /*   init an offset table of size count to zero */

hword sgb_plc(byte *block,hword value);
                   /*   sets the link count */

tint  sgb_qfb( byte * block , hword segnum , hword field);
                   /*   gets the block # in a seg reference */

hword sgb_qfs( byte * block , hword segnum , hword field);
                   /*   get the seg # in a seg reference */

hword sgb_sfb( byte * block , hword segnum , hword field , tint value);
                   /*   set the block # in a seg reference */

hword sgb_sfs( byte * block , hword segnum , hword field , hword value);
                   /*   set segment # in a seg reference */

hword sgb_vbi( byte * buffer);
                   /*   "Verify Buffer Integrity" */

hword sgb_ws(byte *block, hword segnum, struct sgbstr *new_seg);
                   /*   write segment at addr to
                        following block at segment num */

struct sgbstr *sgb_x( byte * addr , hword length);
                   /*   temp function to translate straight to segs */

/* $ci2e-internal */
hword cfill( unsigned char * addr , long int len , unsigned char
insert);          /* */

/* $ci2e-external */
/* $ENDEXTRACT */

/* here's the layout for the disk block:

--------------------------------------------------------------
| Reserved material |  SLA | segment data areas | free space |
--------------------------------------------------------------

Reserved material: contains material that is unique to each
disk block.  This is in struct sgb_res.

SLA: This is an array which contains the actual position of
the segment in the disk block.  Segment number 1 will be the
first element in the array.  The half-word in that element
contains the offset into the block (in bytes) to the starting
point of the individual segment.  A value of ZERO indicates
that the segment is not used.  The value of FFFF is in the
future it will mean that the segment is prived, the value of
FFFE indicates that the it is not known if the user has access
to this segment.  Please note that the last two values are
to be used only when transfering buffers between machines.

Segment data areas: To accomodate brain damaged machines, each
segment begins on a boundary divisible by 2.  At present there
is only the length of the segment, stored as a half-word.
After that is the data itself.

Free space: basically unused space.  It is not in any format.

*/





#endif /* initial #ifndef */
