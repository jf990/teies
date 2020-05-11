/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STR_COMM H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Contains definitions for use by both STR level         |*/
/*|            and functions using STR level.  It is included by both |*/
/*|            STR.H and STR_I.H for this reason.                     |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     16_Mar_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized / Trimmed MODS.   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STR_COMM_H_INCLUDED  /* Have I already been included?          */
#define STR_COMM_H_INCLUDED  /* No--mark for next time & continue...   */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Declarations of type string variables must include initialization |*/
/*| to S_NULL.                                                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define S_NULL   ((string) NULL)       /* Null string pointer          */
#define S_NUL    S_NULL                /* Synonym                      */
#define S_NIL    S_NULL                /* Synonym                      */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  S_MAXLEN was temporarily defined as 4092.  In was eventually to  |*/
/*|  be replaced with a macro that calculates                         |*/
/*|  S_MAXLEN = ((Block Size) - (Block Overhead) - (String Overhead)) |*/
/*|  Note that in this implementation the String Overhead is 4 bytes. |*/
/*|                                                                   |*/
/*|  This has been increased to 32767 - 4 (overhead) as DMS and OS    |*/
/*|  levels can process the extended simple strings.                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define S_MAXLEN   32763     /* Max length of a simple string          */
#define S_OVERHEAD 4         /* Overhead for a string above data reqmts*/

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The s_keep() macro (which pretends to be a function) is used to  |*/
/*|  control automatic overallocation/deallocation.  Nether is done   |*/
/*|  to strings updated by function X is X is called as shown:        |*/
/*|      rc = s_keep(X);                                              |*/
/*|  The exernal user should not touch s_keepsw except to define  it  |*/
/*|  in main().  The internal programmer promises to respect the      |*/
/*|  setting of it.  s_rcode is provided for use with s_keep() and    |*/
/*|  should likewise be left alone.                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_keep(FUN)     (s_keepsw = TRUE , s_rcode = FUN , \
                         s_keepsw = FALSE, s_rcode)
extern sint s_keepsw ;
extern sint s_rcode  ;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The following are used with complex string structures.           |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define S_PTRLEN    4        /* Length of embedded pointer             */

/* Macro returns TRUE if string is simple or null; returns FALSE if    */
/* complex or corrupt                                                  */

#define s_simp(tstr) \
    ( (bool) (tstr == S_NULL ) || \
    (s_smp_al(tstr) < S_MAXLEN ))

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Error codes returned by string functions.                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  S_BAD_PARM         -16001           /* Invalid parameter      */
#define  S_BAD_PARAM        S_BAD_PARM       /* Synonym                */

#define  S_BAD_LEN          -16002           /* Neg value of L given   */

#define  S_BAD_STR          -16003           /* Corrupt string located */

#define  S_BAD_MEM          -16004           /* Memory allocation error*/

#define  S_BAD_NUM          -16005           /* Can't B converted      */
                                             /* to numeric             */

#define  S_BAD_CONV         -16006           /* Num conversion too big */

#define  S_BAD_SIZE         -16007           /* Resulting STR too long */

#define  S_BAD_HLEN         -16008           /* LEN must be even for   */
                                             /* hex conversion         */

#define  S_NUL_STR          -16009           /* Null STR found (s_type)*/
#define  S_NIL_STR          S_NUL_STR        /* Synonym                */
#define  S_NULL_STR         S_NUL_STR        /* Synonym                */

#define  S_PARSE_EXIT       -16010           /* s_parse() conditional  */
                                             /* exit (as per template  */
#define  S_PARSE_CE         S_PARSE_EXIT     /* Synonym                */

#define  S_PARSE_INCOM      -16011           /* s_parse() incomplete   */
                                             /* parse (less than asked */
#define  S_PARSE_IP         S_PARSE_INCOM    /* Synonym                */

#define  S_OSK_ERROR        -16012           /* Unexpected results from*/
                                             /* a call to OSK function */
                                             /*(optimized version only)*/

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Some string functions return non-zero RC's that can indicate     |*/
/*|  success.  This macro traps all exceptions.                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  S_BAD_RC(rc)   ( rc < 0 )     /* Less than zero means error   */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The following characters identify parts of string structures...  |*/
/*|  this stuff really belongs in str_i_i.h, but is put here to       |*/
/*|  support s_len/s_allen/s_stoc macros below.                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_smp_ul(tst)     ((tst)->lused )       /* ...the usage length */
#define s_smp_al(tst)     ((tst)->lalloc)  /* ...the allocation length */
#define s_char_s(tstr,c)  (*(&((tstr)->begin)+c)) /* ... a character   */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Until the implementation of complex strings, the following       |*/
/*|  functions can be replaced with simple #define macros.  Note that |*/
/*|  functions that ordinarily return a STR RC always return zero.    |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_len(str)         ((sint) (str == S_NUL ? 0 : s_smp_ul(str)) )
#define s_allen(str)       ((sint) (str == S_NUL ? 0 : s_smp_al(str)) )
#define s_stoc(out,in,n)   ( out = \
                           (n == 0 ? s_char_s(in,0) : s_char_s(in,n-1)))
#define s_stob(out,in,n)   ( *(out) = \
                           (n == 0 ? s_char_s(in,0) : s_char_s(in,n-1)))

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  This function returns a pointer to the data area of a simple     |*/
/*|  string, for use when a string is being used as a buffer.         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_buffer(str)      (&((str)->begin))

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  This function returns a pointer to a string vector element.      |*/
/*|  Thus "s_vxtract(vector,1)" is a valid argument to any string     |*/
/*|  function, passing the first string in the vector as a valid      |*/
/*|  string.                                                          |*/
/*|                                                                   |*/
/*|  Note that 0 or 1 specifies the first vector.                     |*/
/*|                                                                   |*/
/*|  No range checking is performed.                                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_vxtract(vector,element)   (element == 0            ? \
                                    ((string *) vector[0]) : \
                                    ((string *) vector[(element-1)]))

#define s_vextract(vector,element)  s_vxtract(vector,element)
#define s_velement(vector,element)  s_vxtract(vector,element)
#define s_vtos(vector,element)      s_vxtract(vector,element)
#define s_svtos(vector,element)     s_vxtract(vector,element)

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  s_vlen() returns the number of strings in use in a given vector. |*/
/*|                                                                   |*/
/*|  s_vallen() returns the number of allocated strings in a given    |*/
/*|             vector.                                               |*/
/*+-------------------------------------------------------------------+*/

#define s_vlen(   vector )          s_len(   (string) vector )
#define s_vallen( vector )          s_allen( (string) vector )


/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Nestor want's to use s_char_s(), which is not a good idea.  The  |*/
/*|  following function is almost identical in purpose.               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_put(str,char,n)  ( s_char_s(str,n-1) = char)

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The s_move() function can copy to or from memory and a string.   |*/
/*|                                                                   |*/
/*|  S_IN means "copy the String INto memory".                        |*/
/*|                                                                   |*/
/*|  S_IN means "copy the String OUT of memory, making a string if    |*/
/*|  necessary"                                                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  S_IN  5                       /* Asks s_move() to copy to mem */
#define  S_OUT 21                      /* Asks s_move() to cpy FROM mem*/

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The s_verify() function can be given a character-set string      |*/
/*|  which is used to see if the characters are permitted in the      |*/
/*|  subject string (S_ALLOW) or not permitted (S_DISALLOW).          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  S_ALLOW        1
#define  S_DISALLOW     0

#define  S_ALLOWED      S_ALLOW        /* Synonym                      */
#define  S_NOTALLOWED   S_DISALLOW     /* Synonym                      */
#define  S_DISALLOWED   S_DISALLOW     /* Synonym                      */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| This matches the #ifndef STR_COMM_H at the beginning.             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#endif

