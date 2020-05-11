/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_etoa()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   source      string   INPUT    string to be converted   |*/
/*|  :PARAM.   target      string*  OUTUT    pointer to target string |*/
/*|  :PARAM.   beg         sint     INPUT    position to start copy   |*/
/*|  :PARAM.   len         sint     INPUT    length to copy           |*/
/*|                                                                   |*/
/*|  :PURPOSE. Creates a string after conveting the source string     |*/
/*|            from EBCDIC to ASCII                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint string error code or zero                 |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_RC                                               |*/
/*|                                                                   |*/
/*|  :LIMITS.  Cannot yet process complex strings.                    |*/
/*|                                                                   |*/
/*|  :MODS.    20_Aug_86   N.Voronka   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"


extern  sint  s_alloc   () ;
extern  sint  s_nulout  () ;

   byte ea[] = {
         0x00,0x01,0x02,0x03,0x00,0x09,0x00,0x7F,
         0x00,0x00,0x00,0x0B,0x0C,0x0D,0x0E,0x0F,
         0x10,0x11,0x12,0x13,0x00,0x00,0x08,0x00,
         0x18,0x19,0x00,0x00,0x1C,0x1D,0x1E,0x1F,
         0x00,0x00,0x00,0x00,0x00,0x0A,0x17,0x1B,
         0x00,0x00,0x00,0x00,0x00,0x05,0x06,0x07,
         0x00,0x00,0x16,0x00,0x00,0x00,0x00,0x04,
         0x00,0x00,0x00,0x00,0x14,0x15,0x00,0x1A,
         0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x2E,0x3C,0x4D,0x2B,0x00, /*        . < ( +    */
         0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*  &                */
         0x00,0x00,0x21,0x24,0x2A,0x29,0x3B,0x5E, /*      ! $ * ) ; ^  */
         0x2D,0x2F,0x00,0x00,0x00,0x00,0x00,0x00, /*  - /              */
         0x00,0x00,0x7C,0x2C,0x25,0x5F,0x3E,0x3F, /*      | , % _ > ?  */
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x60,0x3A,0x23,0x40,0x27,0x3D,0x22, /*    ` : # @ ' = "  */
         0x00,0x61,0x62,0x63,0x64,0x65,0x66,0x67, /*    a b c d e f g  */
         0x68,0x69,0x00,0x00,0x00,0x00,0x00,0x00, /*  h i              */
         0x00,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70, /*    j k l m n o p  */
         0x71,0x72,0x00,0x00,0x00,0x00,0x00,0x00, /*  q r              */
         0x00,0x7E,0x73,0x74,0x75,0x76,0x77,0x78, /*    ~ s t u v w x  */
         0x79,0x7A,0x00,0x00,0x00,0x00,0x00,0x00, /*  y z              */
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*                   */
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*                   */
         0x7B,0x41,0x42,0x43,0x44,0x45,0x46,0x47, /*  { A B C D E F G  */
         0x48,0x49,0x00,0x00,0x00,0x00,0x00,0x00, /*  H I              */
         0x7D,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50, /*  } J K L M N O P  */
         0x51,0x52,0x00,0x00,0x00,0x00,0x00,0x00, /*  Q R              */
         0x5c,0x00,0x53,0x54,0x55,0x56,0x57,0x58, /*  \   S T U V W X  */
         0x59,0x5A,0x00,0x00,0x00,0x00,0x00,0x00, /*  Y Z              */
         0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37, /*  0 1 2 3 4 5 6 7  */
         0x38,0x39,0x00,0x00,0x00,0x00,0x00,0x00, /*  8 9              */
         };
sint s_etoa (target, source, beg, len)
string            source ; /* TEIES String to copy from                */
string           *target ; /* TEIES String to copy to                  */
sint              beg    ; /* Position to copy from                    */
sint              len    ; /* How many characters to copy              */
{
   sint           count  ; /* Counter for copying loop                 */
   sint           lens   ; /* Used length of source                    */
   sint           rc     ; /* Return code from s_alloc                 */




   if (len < 0)                   /* Check for negative length         */
   {
      s_nulout(target);
   return(S_BAD_LEN);
   }

   if (beg == 0)                  /* Make sure beginning is 1          */
      beg = 1 ;

   lens = s_len (source) ;        /* Get used length of source         */
   if (len == 0)                  /* If len=0 search entire string     */
      len = lens ;

   if (len == 0)                  /* If source is empty no action      */
   {
      s_nulout (target) ;
   return(0);
   }

   if (beg < 0)                   /* If beginning is negative compute  */
      beg = lens + beg + 1 ;      /*   the positive value              */

   if (len > lens - beg + 1 )     /* If length past end indicated cut  */
      len =  lens - beg + 1 ;


   if (beg <= 0)                  /* Check for too negative beginning  */
   {
      s_nulout (target) ;
   return(0);
   }

   if (beg > lens)                /* If beginning is past end          */
   {
      s_nulout (target) ;
   return(0);
   }

   rc = s_alloc (target, len) ;       /* Allocate space for target    */

   if (S_BAD_RC(rc))                   /* If s_alloc failed            */
   {
   return(rc);
   }

   if (s_simp(source))                 /* If S I M P L E               */
   {
      for (count = 0   ; count <= len - 1; count++)  /*  C O N V E R T */
         s_char_s(*target, count) = ea [s_char_s(source, count+beg-1)];
      s_smp_ul(*target) = len ;        /* Set length used in target    */
   }
   else                                /* Else C O M P L E X           */
   {
   return(S_BAD_SIZE);
   }

   return(0);
}
