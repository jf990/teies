/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_atoe()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   source      string   INPUT    string to convert from   |*/
/*|  :PARAM.   target      string*  OUTPUT   target string pointer    |*/
/*|  :PARAM.   beg         sint     INPUT    starting position copy   |*/
/*|  :PARAM.   len         sint     INPUT    copy length              |*/
/*|                                                                   |*/
/*|  :PURPOSE. Creates a string 'target' after conveting the source   |*/
/*|            string from ASCII to EBCDIC.                           |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint string error code or zero                 |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_LEN                                              |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    21_Aug_86   N.Voronka   Authored.                      |*/
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

   byte ae[] = {
         0x00,0x01,0x02,0x03,0x37,0x2D,0x2E,0x2F,
         0x16,0x05,0x15,0x0B,0x0C,0x0D,0x0E,0x0F,
         0x10,0x11,0x12,0x13,0x3C,0x3D,0x32,0x26,
         0x18,0x19,0x3F,0x27,0x1C,0x1D,0x1E,0x1F,
         0x40,0x5A,0x7F,0x7B,0x5B,0x6C,0x50,0x7D, /*    ! " # $ % & '  */
         0x4D,0x5D,0x5C,0x4E,0x6B,0x60,0x4B,0x61, /*  ( ) * + , - . /  */
         0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7, /*  0 1 2 3 4 5 6 7  */
         0xF8,0xF9,0x7A,0x5E,0x4C,0x7E,0x6E,0x6F, /*  8 9 : ; < = > ?  */
         0x7C,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7, /*  @ A B C D E F G  */
         0xC8,0xC9,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6, /*  H I J K L M N O  */
         0xD7,0xD8,0xD9,0xE2,0xE3,0xE4,0xE5,0xE6, /*  P Q R S T U V W  */
         0xE7,0xE8,0xE9,0xAD,0xE0,0xBD,0x5F,0x6D, /*  X Y Z   \   ^ _  */
         0x79,0x81,0x82,0x83,0x84,0x85,0x86,0x87, /*  ` a b c d e f g  */
         0x88,0x89,0x91,0x92,0x93,0x94,0x95,0x96, /*  h i j k l m n o  */
         0x97,0x98,0x99,0xA2,0xA3,0xA4,0xA5,0xA6, /*  p q r s t u v w  */
         0xA7,0xA8,0xA9,0xC0,0x6A,0xD0,0xA1,0x07, /*  x y z { | } ~    */
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, } ;


sint s_atoe (target, source, beg, len)
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
      s_nulout(target) ;
   return(0);
   }

   if (beg < 0)                   /* If beginning is negative compute  */
      beg = lens + beg + 1 ;      /*   the positive value              */

   if (len > lens - beg + 1 )     /* If length past end indicated cut  */
      len =  lens - beg + 1 ;


   if (beg <= 0)                  /* Check for too negative beginning  */
   {
      s_nulout(target) ;
   return(0);
   }

   if (beg > lens)                /* If beginning is past end          */
   {
      s_nulout(target) ;
   return(0);
   }

   rc = s_alloc (target, len) ;        /* Allocate space for target    */

   if (s_simp(source))                 /* If S I M P L E               */
   {
      for (count = 0   ; count <= len - 1; count++)  /*  C O N V E R T */
         s_char_s(*target, count) = ae [s_char_s(source, count+beg-1)];
      s_smp_ul(*target)= len ;         /* Set length used in target    */
   }

   else                                /* Else C O M P L E X           */
   {
   return(S_BAD_SIZE);
   }

   return(0);
}
