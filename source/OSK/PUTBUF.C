#include "xmodem.h"

int putbuf( buf, size)
   byte *buf;
   int  size;
{
   return( Inter(CONWRIT, buf, size) );
}
