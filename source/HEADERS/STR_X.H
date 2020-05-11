/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    str_x.h                                                |*/
/*|  <LEVEL>   STR                                                    |*/
/*|  <AUTHOR>  Nestor Voronka, Kevin Walsh                            |*/
/*|  <DATE>    August 6, 1986                                         |*/
/*|  <PURPOSE> Contains stuff needed exclusively by external          |*/
/*|            invocations of string routines.  Note that             |*/
/*|            "str_comm.h" contains things used both internally      |*/
/*|            or externally.                                         |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/

#ifndef STR_X_H_INCLUDED
#define STR_X_H_INCLUDED

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The following parameter nomenclature is provided for quick       |*/
/*|  reference.  Refer to the STRING PROCESSING documention for       |*/
/*|  full documentation and explanations.  Keys of following:         |*/
/*|                                                                   |*/
/*|  S0,S0a,S0b:  Strings which will be cleared and modified by the   |*/
/*|               call;                                               |*/
/*|                                                                   |*/
/*|  S1,S2,S3:    Usually input strings, which are never altered by   |*/
/*|               called routine;                                     |*/
/*|                                                                   |*/
/*|  B:           A sint indicating the beginning of a range in a     |*/
/*|               string with regard to the operation;                |*/
/*|                                                                   |*/
/*|  L:           A sint indicating the length of a range (as above); |*/
/*|                                                                   |*/
/*|  N:           A sint used for other purposes (as indicated);      |*/
/*|                                                                   |*/
/*|  HWORD,                                                           |*/
/*|  SINT,TINT,                                                       |*/
/*|  BYTE,REAL:   Variables of the named TEIES types with misc uses;  |*/
/*|                                                                   |*/
/*|  CSTR:        A "standard" C null-terminated string;              |*/
/*|                                                                   |*/
/*|  RC:          The SINT return code of some variables that return  |*/
/*|               information in addition to or instead of a standard |*/
/*|               STR return code.                                    |*/
/*|                                                                   |*/
/*|  DT:          A "datetime" strucure parameter;                    |*/
/*|                                                                   |*/
/*|  MEM:         A pointer to memory (byte* or via s_buffer();       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Arguments denoted in lower case indicate a special data-type     |*/
/*|  in the context of the string function's use by another level.    |*/
/*|                                                                   |*/
/*|  ******:      Functions marked as such (instead of "extern") are  |*/
/*|               actually PPC macros and have less checking;         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

/* $EXTRACT $MAKER-PROTO */

/*+-------------------------------------------------------------------+*/
/*| Str Allocated Length:  RC = allocated length of S1                |*/
/*+-------------------------------------------------------------------+*/
/***** sint   s_allen {string S1}; */

/*+-------------------------------------------------------------------+*/
/*| Str Allocate:   Allocate a string S0 of N bytes.                  |*/
/*+-------------------------------------------------------------------+*/
extern sint s_alloc (string *S0, tint N);

/*+-------------------------------------------------------------------+*/
/*| ASCII to EBCDIC conversion:  Create S0 with the portion of S1     |*/
/*| starting at B of length L translated from ASCII to EBCDIC.        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_atoe (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Byte to String conversion:  Create S0 from given byte C.          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_btos (string *S0, byte C);

/*+-------------------------------------------------------------------+*/
/*| Str concatenate:   Create S0 with a copy of S1 suffixed with a    |*/
/*| copy of the portion of S2 starting at B of length L.              |*/
/*+-------------------------------------------------------------------+*/
extern sint s_cat (string *S0, string S1, string S2, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str concatenate from memory:  Suffix S0 with the portion of memory|*/
/*| starting at P for length L {bytes}.                               |*/
/*+-------------------------------------------------------------------+*/
extern sint s_catb (string *S0, byte *P, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str concatenate with info:  Suffix S0 with the portion of S1      |*/
/*| starting at B for length L including internal string data {Usage  |*/
/*| and allocation length fields}.                                    |*/
/*+-------------------------------------------------------------------+*/
extern sint s_catx (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str clip/cut:  First create S0a from the portion of S0b starting  |*/
/*| at B of length L; then remove the same portion of S0b.            |*/
/*|                                                                   |*/
/*| If S0a == S0b, then only the portion starting at B of length      |*/
/*| L is removed from both S0a and S0b.                               |*/
/*+-------------------------------------------------------------------+*/
extern sint s_clip (string *S0a, string *S0b, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str compair:  Compair S1 with the portion of S2 starting at P of  |*/
/*| length L using comparison scheme C.  Return rc to indicate results|*/
/*| and set WHERE to indicate where a difference was observed.        |*/
/*|                                                                   |*/
/*| RC 0 indicates both strings are equal;                            |*/
/*| RC 1 indicates that the first string was "bigger";                |*/
/*| RC 2 indicates that the second string was "bigger";               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint s_comp (string S1, string S2, sint B, sint L, byte C,
                    sint *WHERE);
/*+-------------------------------------------------------------------+*/
/*| Str context:  Unimplemented.                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_contxt(void);

/*+-------------------------------------------------------------------+*/
/*| Str copy:  Create S0 with a copy of the portion of S1 beginning   |*/
/*| at B of length L.                                                 |*/
/*+-------------------------------------------------------------------+*/
extern sint s_copy (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str validity check:  If S1 has certain specific characteristics   |*/
/*| indicating that it is corrupt, return TRUE.  If not, or if unable |*/
/*| to ascertain, return FALSE.                                       |*/
/*+-------------------------------------------------------------------+*/
extern bool s_corrup(string S1);

/*+-------------------------------------------------------------------+*/
/*| Char to String conversion:  Create S0 from given byte C.          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_ctos (string *S0, byte C);

/*+-------------------------------------------------------------------+*/
/*| Str trim:  Strip leading and/or trailing {as indicated by WHERE}  |*/
/*| occurances of the character WHAT from S0.                         |*/
/*+-------------------------------------------------------------------+*/
extern sint s_ctrim (string *S0, byte WHERE, byte WHAT);

/*+-------------------------------------------------------------------+*/
/*| Calculate allocation reduction {usually used internally by Str    |*/
/*| functions.  Returns new allocation length for a string according  |*/
/*| to the specified allocation slots given that the string currently |*/
/*| has allocated length HAS but is using {or will be using} length   |*/
/*| NEEDS for the data portion.                                       |*/
/*+-------------------------------------------------------------------+*/
extern sint s_cutall(sint HAS, sint NEEDS);

/*+-------------------------------------------------------------------+*/
/*| Deallocate Str:  Release the memory used by S0 and null the       |*/
/*| pointer used.                                                     |*/
/*+-------------------------------------------------------------------+*/
extern sint s_drop (string *S0);

/*+-------------------------------------------------------------------+*/
/*| Obsolete name--will be deleted--replaced by s_dttos{}             |*/
/*+-------------------------------------------------------------------+*/
extern sint s_dtform(string *S0, datetime *DATETIME, string S1);

/*+-------------------------------------------------------------------+*/
/*| Date/Time format:  Create S0 as a rendition of the DATETIME as    |*/
/*| specified by the template S1.  If DATETIME is NIL, the current    |*/
/*| DATETIME is used.  If S1 is S_NULL, then use the format indicated |*/
/*| in the TM_master structure.  If the TM_version is S_NULL, or the  |*/
/*| TM_master structure is NIL, a default version is used.  As the    |*/
/*| datetime is always presumed to be in GMT, the timezone difference |*/
/*| specified in the TM_master structure is used to recalculate the   |*/
/*| local time.  If the TM_master structure is NIL, then no           |*/
/*| recalculation is performed.                                       |*/
/*+-------------------------------------------------------------------+*/
extern sint s_dttos(string *S0, datetime *DATETIME, string S1);

/*+-------------------------------------------------------------------+*/
/*| Duplicate string:  Create S0 containing N copies of the portion of|*/
/*| S1 starting at B of length L.                                     |*/
/*+-------------------------------------------------------------------+*/
extern sint s_dup  (string *S0, string S1, sint B, sint L, sint N);

/*+-------------------------------------------------------------------+*/
/*| EBCDIC to ASCII conversion:  Create S0 with the portion of S1     |*/
/*| starting at B of length L translated from EBCDIC to ascii.        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_etoa (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Find first:  Returns the starting location in S1 where the portion|*/
/*| of S2 starting at B of length L was found.                        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_first (string S1, string S2, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Get date/time element as a string of numbers {usually called      |*/
/*| internally by string functions:  Creates S0 consisting of a       |*/
/*| specific portion of the given DT as indicated by C.               |*/
/*+-------------------------------------------------------------------+*/
extern sint s_gtdtnm(string *S0, datetime *DT, byte C);

/*+-------------------------------------------------------------------+*/
/*| Get date/time element as a string version {usually called         |*/
/*| internally by string functions:  Creates S0 consisting of a       |*/
/*| specific portion of the given DT as indicated by C; limits S0 to  |*/
/*| a maximum length of L.                                            |*/
/*+-------------------------------------------------------------------+*/
extern sint s_gtdtst(string *S0, datetime *DT, byte C, tint L);

/*+-------------------------------------------------------------------+*/
/*| Half-word to string conversion:  Create S0 consisting of the      |*/
/*| given halfword H.                                                 |*/
/*+-------------------------------------------------------------------+*/
extern sint s_htos (string *S0, hword H);

/*+-------------------------------------------------------------------+*/
/*| Str initialization:  Create S0 with the initial value specified   |*/
/*| by the ordinary C-type null-terminated string C.                  |*/
/*+-------------------------------------------------------------------+*/
extern sint s_init (string *S0, byte *C);

/*+-------------------------------------------------------------------+*/
/*| Insert string:  Create S0 from a S1 with the portion of S2        |*/
/*| beginning at B of length L inserted into S1 at position P.        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_ins  (string *S0, string S1, sint P,
                    string S2, sint beg, sint len);

/*+-------------------------------------------------------------------+*/
/*| Find last:   Returns the starting location in S1 where the portion|*/
/*| of S2 starting at B of length L was found searching right to left.|*/
/*+-------------------------------------------------------------------+*/
extern sint s_last (string S1, string S2, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Str Usage Length:  RC = usate length of S1                        |*/
/*+-------------------------------------------------------------------+*/
/***** sint   s_len   {S1};                                            */

/*+-------------------------------------------------------------------+*/
/*| Translate to lower case:  Translates all upper case characters    |*/
/*| to lower case in the portion of S0 starting at B of length L.     |*/
/*+-------------------------------------------------------------------+*/
extern sint s_lower (string S0, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Move string:  If N is S_IN, then copy the portion of S0 starting  |*/
/*| at B of length L into memory pointed to by P.  If N is S_OUT,     |*/
/*| create S0 from the data pointed to by P for length L.             |*/
/*+-------------------------------------------------------------------+*/
extern sint s_move (byte *p, string *S0, sint B, sint L, sint N);

/*+-------------------------------------------------------------------+*/
/*| Memory to printable  string: Creates S0 consisting of a copy of   |*/
/*| the memory indicated by P for length L.  Unprintable characters   |*/
/*| {refer to osk_cset.h} are translated into the character indicated |*/
/*| by C; if C is '\0' then '.' is used instead.                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_mtops (string *S0, byte *P, sint L, byte C);

/*+-------------------------------------------------------------------+*/
/*| Memory to hex string:  Creates S0 consisting of a copy of the     |*/
/*| memory indicated by P for length L.  The result string is always  |*/
/*| twice L.                                                          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_mtoxs (string *S0, byte *P, sint L);

/*+-------------------------------------------------------------------+*/
/*| Nice:  Alters the portion of S0 starting at B of length L so that |*/
/*| the first character and any subsequent character preceeded by a   |*/
/*| blank is upper case, and all other characters are lower case.     |*/
/*+-------------------------------------------------------------------+*/
extern sint s_nice (string *S0, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Find Nth:    Returns the starting location in S1 where the Nth    |*/
/*| occurance of the portion of S2 starting at B of length L was      |*/
/*| found.  If N is positive, search from left to right; if N is      |*/
/*| negative, search from right to left; if N is zero, don't search   |*/
/*| at all.                                                           |*/
/*+-------------------------------------------------------------------+*/
extern sint s_nth  (string S1, string S2, sint B, sint L, sint N);

/*+-------------------------------------------------------------------+*/
/*| Convert null-terminated to "EIES" string {misnamed}:  Create S0   |*/
/*| by copying the portion of C starting at the Bth letter for length |*/
/*| L.  If L > length of C or L is 0 then use the length of C.        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_ntoe (string *S0, byte *C, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Null out:  Set the usage length of S0 to zero {or nothing if S0   |*/
/*| is not allocated.                                                 |*/
/*+-------------------------------------------------------------------+*/
extern sint s_nulout(string *S0);

/*+-------------------------------------------------------------------+*/
/*| Calculate over-allocation size {usually used internally by Str    |*/
/*| functions.  Returns new allocation length for a string according  |*/
/*| to the specified allocation slots for a string that will be of    |*/
/*| usage length N.                                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint s_ovrall(sint N);

/*+-------------------------------------------------------------------+*/
/*| Pad string:  Creates S0 by padding or cutting S1.                 |*/
/*| DIR specifies that padding should be                              |*/
/*| done on the right {'R'}, the left {'L'}, or both {'B' or 'C'}.    |*/
/*| Character WHAT is used as the pad character.                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_pad  (string *S0, string S1, tint N, byte WHAT, byte DIR);

/*+-------------------------------------------------------------------+*/
/*| Parse string:  Parses S0a according to the template S1 upto an    |*/
/*| occurance of a character in the delimiter string S2.  The template|*/
/*| is used to determine if/what strings in the string vector S0v are |*/
/*| replaced by portions of the parsed string S0a, and might also     |*/
/*| create a parse overflow {spill} string S0b.                       |*/
/*+-------------------------------------------------------------------+*/
extern sint s_parse (string *S0a, string S1, string S2,
                     string S0v[], string *S0b);

/*+-------------------------------------------------------------------+*/
/*| Packed digits to hex string:  Creates S0 consisting of the        |*/
/*| expansion of the memory indicated by P of length L.  The location |*/
/*| must contain any nibble in: 0x012345679.  {This function would    |*/
/*| convert 0x0123456789 into a TEIES string as follows:              |*/
/*| "F0F1F2F3F4F5F6F7F8F9".                                           |*/
/*+-------------------------------------------------------------------+*/
extern sint s_pdtoxs(string *S0, byte *P, sint L);

/*+-------------------------------------------------------------------+*/
/*| Complex {relatively obsolete}:  If the high bit in the usage field|*/
/*| of the internal data of S1 is set, return TRUE, else FALSE.       |*/
/*+-------------------------------------------------------------------+*/
extern bool s_plex (string S1);

/*+-------------------------------------------------------------------+*/
/*| Prefix:  Create S0 by prefixing S1 with the portion of S2 starting|*/
/*| at B of length L.                                                 |*/
/*+-------------------------------------------------------------------+*/
extern sint s_pre(string *S0, string S1, string S2, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Replace:  Create S0 with a version of S1 in which the portion of  |*/
/*| S1 starting at B1 of length L1 has been replaced by the portion of|*/
/*| S2 starting at B2 of length L2.                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint s_rep (string *S0,string S1, sint B1, sint L1,
                               string S2, sint B2, sint L2);

/*+-------------------------------------------------------------------+*/
/*| Reverse:  Create S0 with a reversed copy of the portion of S1     |*/
/*| starting at B of length L.                                        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_rev (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Real to string:  Create S0 containing the string representation   |*/
/*| of the real number N.                                             |*/
/*+-------------------------------------------------------------------+*/
extern sint s_rtos (string *S0, real N);

/*+-------------------------------------------------------------------+*/
/*| String scan is not implemented.                                   |*/
/*+-------------------------------------------------------------------+*/
extern sint s_scan (void);

/*+-------------------------------------------------------------------+*/
/*| Simple {relatively obsolete}:  If the high bit in the usage field |*/
/*| of the internal data of S1 is not set, return TRUE, else FALSE.   |*/
/*+-------------------------------------------------------------------+*/
/***** bool   s_simp  {S1};                                            */

/*+-------------------------------------------------------------------+*/
/*| Signed integer to string convert:  Create S0 consisting of the    |*/
/*| string representation of the signed integer N.                    |*/
/*+-------------------------------------------------------------------+*/
extern sint s_sitos (string *S0, sint N);

/*+-------------------------------------------------------------------+*/
/*| String to byte:  Replace the byte pointed to by P with the Bth    |*/
/*| character of S1.                                                  |*/
/*+-------------------------------------------------------------------+*/
/***** byte   s_stob  {byte *P, string S1, sint B};                    */

/*+-------------------------------------------------------------------+*/
/*| String to byte:  Replace the byte C with the Bth character of S1. |*/
/*+-------------------------------------------------------------------+*/
/***** char   s_stoc  {byte C, string S1, sint B);

/*+-------------------------------------------------------------------+*/
/*| String to C-string:  Replace the data in the C-string pointed to  |*/
/*| by CSTR with the portion of S1 starting at B of length L.         |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stocs (byte **CSTR, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to date-time.  Sets DATETIME to the date-time provided in  |*/
/*| S1 as best as can be figgured.  S2 specifies the format of S1 in  |*/
/*| the standard date-time format notation.  B & L specify the range  |*/
/*| in S1 where the string is to be found.                            |*/
/*| If S2 is S_NULL, then the format indicated in the TM_master       |*/
/*| structure is used.  If the TM_version is S_NULL, or the           |*/
/*| TM_master structure is NIL, a default version is used.  As the    |*/
/*| datetime is always presumed to be in local timezone differenct,   |*/
/*| conversion to GMT is done.                                        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stodt(datetime *DATETIME, string S1, sint B, sint L,
   string S2);

/*+-------------------------------------------------------------------+*/
/*| String to half-word:  Replace the halfword indicated by pointer P |*/
/*| to the first half-word number found in the portion of S1 starting |*/
/*| at B of length L.  Search proceeds from right to left.            |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stoh (hword *P, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to real:   Replace the real indicated by pointer P with the|*/
/*| portion of S1 starting at B of length L in which a real number is |*/
/*| first found.                                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stor (real *P, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to signed-integer:  Replace the singed integer indicated by|*/
/*| pointer P with the first sint number found in the portion of S1   |*/
/*| starting at B of length L.  Search proceeds from right to left.   |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stosi (sint *P, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to string of hex:  Creates S0 consisting of a hex dump     |*/
/*| translation of the portion of S1 starting at B of length L.       |*/
/*| S0 will always become twice the lenght.  {So if S1="123" then     |*/
/*| S0 will be "F1F2F3"}.                                             |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stosx (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to teies-ingeger {unsigned}:  Replace the unsigned integer |*/
/*| pointed to by P with the first unsigned integer found in the      |*/
/*| portion of S1 starting at B of length L.                          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stoti (tint *P, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String to printable  string: Creates S0 consisting of a copy of   |*/
/*| the portion of S1 starting at B of length L.  Unprintable chars   |*/
/*| {refer to osk_cset.h} are translated into the character indicated |*/
/*| by C; if C is '\0' then '.' is used instead.                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_stops (string *S0,            sint B, sint L, byte C);

/*+-------------------------------------------------------------------+*/
/*| Str swap context:  Unimplmented.                                  |*/
/*+-------------------------------------------------------------------+*/
extern sint s_swapct(void);

/*+-------------------------------------------------------------------+*/
/*| String of hex to memory:  Converts the portion of S1 starting at  |*/
/*| B of length L from hex to characters and places it at memory      |*/
/*| memory pointed to by P.  The overall length must be positive and  |*/
/*| the string can contain only characters from this set:             |*/
/*| {0123456789abcdefABCDEF}                                          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_sxtom (byte **P, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| String of hex to string:  Creates S0 from the portion of S1       |*/
/*| stargint at B of length L from hex to regular string characters.  |*/
/*| The overall length must be positive and the string can contain    |*/
/*| characters from this set:  {0123456789abcdefABCDEF}               |*/
/*+-------------------------------------------------------------------+*/
extern sint s_sxtos (string *S0, string S1, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Teies integer to string:  Creates S0 from given TEIES integer     |*/
/*| {unsigned integer} N.                                             |*/
/*+-------------------------------------------------------------------+*/
extern sint s_titos (string *S0, tint N);

/*+-------------------------------------------------------------------+*/
/*| Translate:  Creates S0 from the translation of the portion of S1  |*/
/*| starting at B of length L according to S2 mapped to S3.           |*/
/*| Any character in the indication section is translated if it occurs|*/
/*| in S2; the character in the same position in S3 is used instead.  |*/
/*| If S2 is longer than S3, then pad character C is used to translate|*/
/*| characters not mapped to S3.                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_trans (string *S0, string S1, sint B, sint L,
                     string S2, string S3, byte C);

/*+-------------------------------------------------------------------+*/
/*| Trim blanks:  Shortens S0 if need by to strip blanks from it.     |*/
/*| If C is 'L' then leading blanks are stripped; if 'T', then        |*/
/*| trailing; if 'B', then both.                                      |*/
/*+-------------------------------------------------------------------+*/
extern sint s_trim (string *S0, byte C);

/*+-------------------------------------------------------------------+*/
/*| String Type:  If all of the portion of S1 starting at B of length |*/
/*| L is of the type specified by C, return 0; else return the        |*/
/*| position of the 1st character found not of the specified type     |*/
/*| relative to the beginning of the string {not the portion}.        |*/
/*|                                                                   |*/
/*| Valid values of C are: 'I' {integer}, 'R' {real}, 'A' {alpha-     |*/
/*| numeric, 'L' {letters/all alpha}, 'N' {all numeric},              |*/
/*| 'P' {printable}, 'S' {special--printable non-alphnumerics},       |*/
/*| and 'U' {un-numeric printables}.                                  |*/
/*+-------------------------------------------------------------------+*/
extern sint s_type (string S1, sint B, sint L, byte C);

/*+-------------------------------------------------------------------+*/
/*| Translate to upper case:  Translates all lower case characters    |*/
/*| to upper case in the portion of S0 starting at B of length L.     |*/
/*+-------------------------------------------------------------------+*/
extern sint s_upper (string S0, sint B, sint L);

/*+-------------------------------------------------------------------+*/
/*| Vector allocate:  Create string vector S0 of N elements.          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_valloc(string **S0, hword N);

/*+-------------------------------------------------------------------+*/
/*| Vector deallocate:  Deallocate string vector S0 and all strings   |*/
/*| contained.                                                        |*/
/*+-------------------------------------------------------------------+*/
extern sint s_vdrop (string **S0);

/*+-------------------------------------------------------------------+*/
/*| Vector contcatenate:  Under development.                          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_vcat ();

/*+-------------------------------------------------------------------+*/
/*| Verify string contents:  Locates invalid characters in the        |*/
/*| portion of the subject string S1 beginning at character B of      |*/
/*| length L.   The char_set string S2 specifies what characters may  |*/
/*| or may not appear in the subject string as per the parameter      |*/
/*| "PARAM" as per:                                                   |*/
/*|                                                                   |*/
/*|   S_ALLOW  -->  the char_set string specifies characters that     |*/
/*|                 are permitted in the subject string;              |*/
/*|                                                                   |*/
/*|   S_DISALLOW --> the char_set string specifies characters that    |*/
/*|                  are NOT permitted in the subject string;         |*/
/*|                                                                   |*/
/*| The RC of this function indicates the location of the first       |*/
/*| character {searching left-to-right} that is not permitted, or     |*/
/*| zero if the entire portion of S1 is valid, or a negative string   |*/
/*| level error code.                                                 |*/
/*+-------------------------------------------------------------------+*/
extern sint s_verify (string S1, string S2, sint B, sint L, sint PARAM);

/*+-------------------------------------------------------------------+*/
/*| Look up word in a template and return the matching word position. |*/
/*|                                                                   |*/
/*| Based on template type:                                           |*/
#define S_TEMPLATE_IS_WORD_STRING   1
#define S_TEMPLATE_IS_VECTOR        2
/*|                                                                   |*/
/*| the template either points to a string of words or a vector of    |*/
/*| strings.                                                          |*/
/*+-------------------------------------------------------------------+*/
extern sint s_word(      string   source,
                         void   * template,
                         byte     template_type,
                         sint     begin,
                         sint     length,
                         sint   * match_position                  );


#endif
