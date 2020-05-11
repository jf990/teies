#include "xmodem.h"

int putbyte( ch )
   byte ch;
{
  byte *dat = " ";
   *dat = 0x02;
   *(dat+1) = ch;
   return( Inter(CONWRIT, dat, 2));
}
