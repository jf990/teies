struct btm_str {
   tint bias;  /* where element zero in the bit string starts */
   tint number_bits; /* the real number of bits in the bitstream.
                        The size allocated will be the size of this
                        structure plus this number bits div 8 */
   };


/* internal functions manipulate this structure so we can have a
   common base function */
struct btm_adr {
   byte *address;
   byte bit_mask;
   };

extern struct btm_sys {
   byte *bit_count_array;
   } btmsys;


/* $extract $maker-proto */

hword               btm_and( struct btm_str **target
                    , struct btm_str *btmstra, struct btm_str *btmstrb);
#define BTM_AND_OK 0 /* add completed ok */
#define BTM_AND_ARG 1 /* btmstra or btmstrb are not proper); */
#define BTM_AND_SIZE 2 /* size of a != b != target */
#define BTM_AND_BIAS 3 /* biases do not match */

/* to a bitwise and of the two structures and put it into the third
structure.  If '*target' is NULL, then allocate and fill it.  It will
be ok for 'target' to be 'btmstra' or 'btmstrb' */

hword               btm_beg( void);  /* initialize btm system structs */


hword               btm_clr( struct btm_str *btmstr);

hword               btm_cls( struct btm_str *btmstr);
#define BTM_CLS_OK   0 /* no apparent errors */
#define BTM_CLS_NULL 1 /* arg was a null ptr */

hword               btm_com (struct btm_str **target
                    , struct btm_str *btmstra
                    , struct btm_str *btmstrb
                    , hword operation); /* combine 2 arrays via op */


#define BTM_COM_OP_AND 1
#define BTM_COM_OP_OR  2
#define BTM_COM_OP_XOR 3

#define BTM_COM_OK 0 /* add completed ok */
#define BTM_COM_ARG 1 /* btmstra or btmstrb are not proper); */
#define BTM_COM_SIZE 2 /* size of a != b != target */
#define BTM_COM_BIAS 3 /* biases do not match */
#define BTM_COM_NOP  4 /* operation is not valid */


lint                btm_diff( struct btm_str *a, struct btm_str *b
                    , tint starting_bit);
#define BTM_DIFF_BAD_ARG -1 /* one arg is not valid range */
#define BTM_DIFF_END     -2 /* reached the end of one array */
#define BTM_DIFF_BEGIN   -3 /* before range of one array */

hword               btm_dis( struct btm_str *btmstr, byte *message);


hword               btm_end(void);


hword               btm_fill( struct btm_str *btmstr);
                    /* fill the array with set bits */


lint                btm_find( struct btm_str *btmstr, lint start
                    , hword direction, hword search4);
#define BTM_FIND_END -1 /* reached the end */
#define BTM_FIND_ARG -2 /* invalid arguements */

#define BTM_FIND_FORWARD 3   /* search in forward direction */
#define BTM_FIND_REVERSE 4   /* search in reverse direction */

#define BTM_FIND_SET 5       /* find a set bit */
#define BTM_FIND_RESET 6     /* find a non set bit */

/* searching for bits tested, searching for zeros has not */



/* btm_next is to be phased out, it has been replace by btm_find */
tint                btm_next( struct btm_str *btmstr, tint start);

struct btm_str     *btm_opn( tint number_of_bits);


tint                btm_qbs( struct btm_str *btmstr);
                    /* "Query Bits Set", counts how many bits
                    are set, excluding those below bottom range */


hword               btm_qry( struct btm_str *btmstr, lint which_one);
/* notes: for out of bit field queries: zero will be returned if
the requested bit is greater than the range.  one will be returned
if the requested bit is lower than the range */


/* will return the current high and low margins of the btmstr.
NULL addresses in 'low' or 'high' will result in no updating */
hword               btm_range( struct btm_str *btmstr, lint *low
                    , lint *high);
#define BTM_RANGE_OK 0
#define BTM_RANGE_ARG 1 /* btmstr is not valid */


hword               btm_res( struct btm_str *btmstr, lint which_one );

hword               btm_set( struct btm_str *btmstr, lint which_one );
#define BTM_SET_OK 0
#define BTM_SET_TOO_HIGH 1
#define BTM_SET_TOO_LOW  2
#define BTM_SET_ARG 3

sint                btm_shift( struct btm_str *btmstr, hword direction
                    , lint howfar);

#define BTM_SHIFT_ARG -1 /* bad arguements */
/* if it worked, it will return the number of bits shifted */


#define BTM_SHIFT_LEFT  1 /* move the bits left */
#define BTM_SHIFT_RIGHT 2 /* move the bits right */
#define BTM_SHIFT_FORCE 4 /* override normal checks and do it */

/*  left and right refer to the direction the bits move.  To shift left
looses the high end bits, and to shift right looses the low end bits.
If 'howfar' is zero, a shift left will shift until a zero is encountered
in one of the bytes.  A shift right will shift until a one is found
in one of the bytes.  If the force flag is present and 'howfar' is
divisible by 8 {implementation restriction}, the exact number will
be shifted ignoring the bit contents.
*/

tint                btm_size(struct btm_str *btmstr);
                    /* returns number of bytes required to store
                    this structure.  Zero indicates passed null ptr */

/* $ci2e-internal */

lint                btmefind( struct btm_str *btmstr, lint start
                    , hword direction, hword search4);
                    /* this is the routine that does the real
                    work for btm_find, uses 'btm_find' arguements */

hword               btmqbit( struct btm_str *btmstr, lint which_bit
                    , struct btm_adr *btmadr);

#define BTMQBIT_OK 0
#define BTMQBIT_NULL 1

/* $endextract */
#define bits2byte(x) (( x + 7) / 8)
/* btm_str layout:

+-------------------------------+
|      |             |          |
| bias | number_bits | the bits |
|      |             |          |
+-------------------------------+

Bit layout:

12345678  9abcdef0
Byte 0    Byte 1

Bit position 12 determined: 12 / 8 = 1;  12 & 0x7 = 2
Therefore bit 12 is in byte 1 (starting from zero), bit number 2.


Notes: there is no bit zero.  Zero is a marker indicating that one
wishes to start from the beginning or end, depending on other arguements

Maximum bit value is 31 bits long.  Negative values are used to
represent error codes.
$ci2e-external */












