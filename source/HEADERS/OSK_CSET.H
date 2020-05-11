/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    osk_cset.h                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   osk                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  OSK_CSET    PPCvar   IN/OUT   Defined by this file and |*/
/*|                                          checked by this file so  |*/
/*|                                          that it can accidentally |*/
/*|                                          be #included more than   |*/
/*|                                          once without harm; also  |*/
/*|                                          used by other files      |*/
/*|                                          requiring osk_cset.h     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Contains an extern of the os_char[] table and        |*/
/*|            #def's to interrogate the table.                       |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    08_Aug_86   K.J.Walsh   Written                        |*/
/*|  :MODS.    21_Jan_86   K.J.Walsh   Added numeric value/char functs|*/
/*|  :MODS.    30_Jun_88   K.J.Walsh   Added OS_LETTER & OS_ISLETTER()|*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef OSK_CSET
#  define OSK_CSET 1

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| os_char[] : OS Character attributes table.                      |*/
/*|                                                                   |*/
/*| Defined in "teies.h" / used to see if a given character is a valid|*/
/*| part of a file descriptor or if it is probably printable  in the  |*/
/*| current operating system.                                         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
extern   byte     os_char[]   ;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| The following defines are used to intorragte (and set in teies.h) |*/
/*| the bitmaps in the table "os_char".                               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  OS_PRINTABLE   0x01           /* B'0000,0001' can be printed  */
#define  OS_FD_CHAR     0x02           /* B'0000,0010' can be in a FD  */
#define  OS_LOWER       0x04           /* B'0000,0100' lower case char */
#define  OS_UPPER       0x08           /* B'0000,1000' upper case char */
#define  OS_LETTER      0x0C           /* B'0000,1100' upper & lower   */
#define  OS_NUMERIC     0x10           /* B'0001,0000' 0,1,2,...9      */
#define  OS_ALPHANUM    0x1C           /* B'0001,1100' upper,lower,num */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Functions to facilitate use of the above:                         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define OS_CANPRINT(a_byte)   (os_char[(sint) (a_byte)] & OS_PRINTABLE)
#define OS_CANFD(a_byte)      (os_char[(sint) (a_byte)] & OS_FD_CHAR)
#define OS_ISLOWER(a_byte)    (os_char[(sint) (a_byte)] & OS_LOWER)
#define OS_ISUPPER(a_byte)    (os_char[(sint) (a_byte)] & OS_UPPER)
#define OS_ISLETTER(a_byte)   (os_char[(sint) (a_byte)] & OS_LETTER)
#define OS_ISNUMERIC(a_byte)  (os_char[(sint) (a_byte)] & OS_NUMERIC)
#define OS_ISALPHANUM(a_byte) (os_char[(sint) (a_byte)] & OS_ALPHANUM)

#define OS_ISNUM(a_byte)      OS_ISNUMERIC(a_byte)

#define OS_PRINT_AS(a_byte) (OS_CANPRINT(a_byte) ? (a_byte) : '.' )

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Case conversion fudge numbers:                                    |*/
/*|                                                                   |*/
/*| Always added:  (known lower char) + OS_UP_DIFF = (upper case char)|*/
/*|                (known upper char) + OS_LO_DIFF = (lower case char)|*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| Numeric converion fudge numbers:                                  |*/
/*|                                                                   |*/
/*| Always added:  (numeric value   ) + OS_NV_DIFF = (numeric    char)|*/
/*|                (known numeric ch) + OS_NC_DIFF = (numeric value  )|*/
/*|                                                                   |*/
/*| Note that these functions presume that a given value is valid in  |*/
/*| the context of these conversions!                                 |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#if ( '0' == 0xF0)                     /* If this is an EBCDIC system  */
#  define  OS_UP_DIF    (0x40)               /* 'a' +    0x40  = 'A'   */
#  define  OS_LO_DIF    ( - OS_UP_DIF )      /* 'A' + (- 0x40) = 'a'   */
#  define  OS_NV_DIF    (0xF0)               /* (1) +    0xF0  = '1'   */
#  define  OS_NC_DIF    ( - OS_NV_DIF )      /* '1' + (- 0xF0) = (1)   */
#endif

#if ( '0' == 0x30 )                    /* If this is an ASCII system   */
#  define  OS_LO_DIF    (0x20)               /* 'A' +    0x20) = 'a'   */
#  define  OS_UP_DIF    ( - OS_LO_DIF )      /* 'a' + ( -0x40) = 'A'   */
#  define  OS_NV_DIF    (0x48)               /* (1) +    0x48  = '1'   */
#  define  OS_NC_DIF    ( - OS_NV_DIF )      /* '1' + (- 0x48) = (1)   */
#endif

#if ( '0' != 0x30 && '0' != 0xF0 )     /* If not ASCII or EBCDIC system*/
#  define    OS_LO_DIF  0                    /* Prevent other errors   */
#  define    OS_UP_DIF  0                    /* from hiding true error */
   ACK System_is_not_ASCII_or_EBCDIC;        /* Die on purpose         */
#endif

                                       /* Case functions               */
#define OS_TOUPPER(x) (OS_ISLOWER(x) ? (x) + OS_UP_DIFF :  (x) )
#define OS_TOLOWER(x) (OS_ISUPPER(x) ? (x) + OS_LO_DIFF :  (x) )

                                       /* Numeric functions            */
#define OS_V2CHAR(x) ((x) + OS_NV_DIF)
#define OS_C2VAL(x)  ((x) + OS_NC_DIF)

                                       /* Some synonyms                */
#define OS_UP_DIFF   OS_UP_DIF
#define OS_LO_DIFF   OS_LO_DIF

#define OS_V2C(x)    OS_V2CHAR(x)
#define OS_C2V(x)    OS_C2VAL(x)

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| End of the beginning #if...                                       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#endif
