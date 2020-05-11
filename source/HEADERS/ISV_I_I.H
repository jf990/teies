/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     ISV_I_I H                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    ISV                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Actual internal header file, included by main internal |*/
/*|            header file ISV_I.H                                    |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   "isv_map" struct not used.                             |*/
/*|                                                                   |*/
/*| :MODS.     19_Apr_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized.                  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef ISV_I_I_H_INCLUDED
#define ISV_I_I_H_INCLUDED

#define ISV_PROOF_AT 0        /* Location of "ISV!" proof word         */
#define ISV_PROOF_S  4

#define ISV_LINK_AT  4        /* Location of 8 byte pointer to next ISV*/
#define ISV_LINK_S   4

#define ISV_N        12       /* Location of 4 byte max-isv number     */
#define ISV_N_S      4

#define ISV_LENGTH   16       /* 4 bytes describing length in D-words  */
#define ISV_LENGTH_S 4

#define ISV_TAGS     20       /* 4 bytes, displacement to sorted tags  */
#define ISV_TAGS_S   4

#define ISV_M        24       /* Location of 4 bytes telling # isv's   */
#define ISV_M_S      4

#define ISV_SOURCE   28       /* Location of 17 byte source FD         */
#define ISV_SOURCE_S 17

#define ISV_OBJECT   49       /* Location of 17 byte object FD         */
#define ISV_OBJECT_S 17

#define ISV_DATE     62       /* 6 bytes, date ISV object created      */
#define ISV_DATE_S   6

#define ISV_TIME     68       /* 4 bytes, time of ISV object creation  */
#define ISV_TIME_S   4

#define ISV_START    72       /* Start of ISV table                    */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Following structure is useless when ISV's are created by TSTR    |*/
/*|  MACRO because C aligns the structure elements differently.       |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
struct isv_map
{
   tint  next_one      ;     /* 8 byte pointer to next ISV table       */
   hword n             ;     /* 4 byte max-isv number                  */
   hword length        ;     /* 4 bytes describing lenght in D-words   */
   hword tags          ;     /* 4 bytes, displacement to sorted tags   */
   hword m             ;     /* 4 bytes giving # of (actual) isv's     */
   byte  source[17]  ;     /* 17 byte source file FD                 */
   byte  object[17]  ;     /* 17 byte object file FD                 */
   byte  c_date[6]   ;     /* 6 bytes, date ISV object created       */
   byte  c_time[4]   ;     /* 4 bytes, time ISV object created       */
   byte  *data         ;     /* 1st byte of data                       */
} ;

typedef struct isv_map isv_ptr;

#endif
