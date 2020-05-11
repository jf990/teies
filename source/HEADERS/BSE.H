/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *    <Doc>                                                          *
 *    :name.           BSE.h                                         *
 *    :level.          OBJ                                           *
 *    :Author.         Nestor Voronka                                *
 *    :Date.           Wednesday, August 6, 1986                     *
 *                                                                   *
 *                                                                   *
 *    :Purpose.                                                      *
 *                                                                   *
 *    This file contains the structure BSEMAP which is the           *
 *    structure used whenever accessing a Basic Storage Element.     *
 *    Also defined are the bit sequences for the BSE header tracking *
 *    used when manipulating BSE's.                                  *
 *    <Doc>                                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct _bse_
{
   hword type   ;                 /* Describes data contained in BSE   */
   hword len    ;                 /* Length of data field              */
   byte  begin  ;                 /* Data Field                        */
} ;
typedef struct _bse_ BSE;



struct _bseheader_
{
   tint encrypt       ;           /* Encryption key or 0 for none      */
   byte typemap [8] ;           /* Bit map of data types used        */
   hword mxrcln       ;           /* Maximum record length             */
   hword spare        ;           /* Extra 16 bits in case we need them*/
} ;
typedef struct _bseheader_ BSEHEADER;



#define BSE_MAX_KEY_SIZE        4



#define B_RFIRST  0x1000          /* Type = 0001 0000 0000 0000        */
#define B_RCONT   0x0000          /* Type = 0000 0000 0000 0000        */
#define B_RLAST   0x2000          /* Type = 0010 0000 0000 0000        */
#define B_RONLY   0x3000          /* Type = 0011 0000 0000 0000        */

#define B_SFIRST  0x4000          /* Type = 0100 0000 0000 0000        */
#define B_SCONT   0x0000          /* Type = 0000 0000 0000 0000        */
#define B_SLAST   0x8000          /* Type = 1000 0000 0000 0000        */
#define B_SONLY   0xC000          /* Type = 1100 0000 0000 0000        */





#define B_END       0x0000      /* Type = 0000 0000 0000 0000        */
#define B_BOOLEAN   0x0001      /* Type = 0000 0000 0000 0011        */
#define B_INTEGER   0x0002      /* Type = 0000 0000 0000 0011        */
#define B_BITSTR    0x0003      /* Type = 0000 0000 0000 0011        */
#define B_OCTETSTR  0x0004      /* Type = 0000 0000 0000 0011        */
#define B_NULL      0x0005      /* Type = 0000 0000 0000 0101        */
#define B_SEQUENCE  0x0010      /* Type = 0000 0000 0001 0011        */
#define B_SET       0x0011      /* Type = 0000 0000 0001 0011        */
#define B_NUMBERIC  0x0012      /* Type = 0000 0000 0001 0011        */
#define B_PRINTABLE 0x0013      /* Type = 0000 0000 0001 0011        */
#define B_T61       0x0014      /* Type = 0000 0000 0001 0011        */
#define B_VIDEOTEXT 0x0015      /* Type = 0000 0000 0001 0101        */
#define B_IA5       0x0016      /* Type = 0000 0000 0001 0101        */
#define B_UTCTIME   0x0017      /* Type = 0000 0000 0001 0101        */
#define B_GENTIME   0x0018      /* Type = 0000 0000 0001 0101        */
#define B_MIDI      0x003D      /* Type = 0000 0000 0011 1101        */
#define B_GML       0x003E      /* Type = 0000 0000 0011 1110        */
#define B_BHEAD     0x003F      /* Type = 0000 0000 0011 1111        */




