#include "defs.h"
#include "xmodem.h"
#include "ibm3270.h"
    FILE *errfile;
    struct rdbufstr *inpdat;
    int conextrn;
main()
{
   int datlen, qlen, rc = 0;
   char *datptr, *datptr2, *qptr;
   sint dev = CONDEV;
   char *prefix, *prefix1;
   sint prefixlen = 15;

   FILE *infile, *outfile;
   int numread = 0,
       times = 0;

   byte *str, *tmpstr;
   char ch;

   conextrn = NULL;

   errfile = fopen("con err a","w");
#if 0
   outfile = fopen("con out b","w");
   infile = fopen("con c b", "r");

   rc = Inter(CONSET);
   fprintf(errfile,"rc:%d, from set", rc);
#endif
   datptr = "abcdefghi#&##~~~~~";
   datlen = strlen(datptr) + 5;
   dump(datptr, datlen, errfile);

   rc = con_tran(CON_EBC2ASC, datptr, datlen);
   fprintf(errfile,"From con_tran:%d\n", rc);
   dump(datptr, datlen, errfile);

   rc = con_pfx(&datptr, &datlen);
   fprintf(errfile,"From con_pfx:%d\n", rc);
   dump(datptr, datlen, errfile);

   rc = con_upfx(&datptr, &datlen);
   fprintf(errfile,"From con_upfx:%d\n", rc);
   dump(datptr, datlen, errfile);

   rc = con_tran(CON_ASC2EBC, datptr, datlen);
   fprintf(errfile,"From con_tran:%d\n", rc);
   dump(datptr, datlen, errfile);



exit(0);





   qlen = 80;
   qptr = malloc( qlen );
   rc = os_conn(CON_SET, dev, qptr, &qlen);
   fprintf(errfile,"From set rc:%d, qlen:%d\n", rc, qlen);
   dump(qptr, qlen, errfile);
   free(qptr);

   rc = os_conn(CON_CLEAR, dev);
   fprintf(errfile,"rc:%d, from clear\n", rc);

   prefix = malloc(prefixlen);
   prefix1 = malloc(prefixlen);
  *(prefix+0) = 0xC3;
  *(prefix+1) = 0x11;
  *(prefix+2) = 0x5d;
  *(prefix+3) = 0x7f;
  *(prefix+4) = 0x11;
  *(prefix+5) = 0x00;
  *(prefix+6) = 0x05;

  *(prefix1+0) = 0xF1;
  *(prefix1+1) = 0xF2;
  *(prefix1+2) = 0xf3;
  *(prefix1+3) = 0xf4;
  *(prefix1+4) = 0xf5;
  *(prefix1+5) = 0xf6;
  *(prefix1+6) = 0xf7;
  *(prefix1+7) = 0xf8;

   strncpy((prefix+7), prefix1, 8);
   qlen = 96 + 16;
   qptr = malloc(qlen);
   rc = os_conn(CON_WRITE, dev, prefix, &prefixlen);
   fprintf(errfile,"rc:%d, from read\n", rc);
   dump(prefix, prefixlen, errfile);

   rc = os_conn(CON_READ, dev, qptr, &qlen);
   fprintf(errfile,"rc:%d, from read\n", rc);
   fprintf(errfile,"Qlen= %d,  Str:%s:\n",qlen,qptr);
   if ( rc == 0)
      dump(qptr, qlen, errfile);
   free(qptr);

#if 0
   rc = Inter(CONCLEA);
   fprintf(errfile,"rc:%x, from clear", rc);

   datlen = 20;
   datptr = malloc(datlen);
   str = malloc( 20 );
   tmpstr = str;

/*
   *(str++) = 0x03;
*/
   *(str++) = 0x07;
   *(str++) = SBA;
   *(str++) = 0x00;
   *(str++) = 0x40;
   *(str++) = SF;
   *(str++) = NONDISPLAY;
/*
*(str++) = 'X';
*(str++) = 'Y';
*(str++) = 'Z';
   *(str++) = IC;
  */
sleep(2);

   inpdat = malloc(sizeof(struct rdbufstr));
   inpdat->bufptr = inpdat->rdptr = 0;
   inpdat->data = malloc( BUFSIZE );
   inpdat->rawdata = malloc( RAWSIZE );
str = tmpstr;
/*
*(str++) = 0x03;
*(str++) = 'A';
*(str++) = 'B';
*(str++) = 'c';
*(str++) = 'd';
rc = Inter(CONWRIT, tmpstr, str - tmpstr);
   fprintf(errfile,"rc:%x, from write", rc);
sleep(2);
*/

rc = Inter(CONREAD, datptr, &datlen);
   fprintf(errfile,"rc:%x, from read", rc);
   dump(datptr, datlen, errfile);

sleep(5); datlen = 20;
rc = Inter(CONREAD, datptr, &datlen);
   fprintf(errfile,"rc:%x, from read", rc);

dump(datptr, datlen, errfile);

/*
   xget(infile, 0);
xput(outfile, 0);
*/
fclose(infile);
fclose(outfile);
#endif

   rc = os_conn(CON_CLOSE);
   fprintf(errfile, "rc:%x from close\n",rc);

   fclose(errfile);

exit(0);
}
 dump(ptr, len, outfile)
   byte *ptr;
   int  len;
   FILE *outfile;
{
   int i;

  fprintf(outfile,"{%d}",len);
   for (i=0;i < len; i++)
   {
     fprintf(outfile,"<%x>%c",ptr(:i:), ptr(:i:));
   }
  fprintf(outfile, "\n");
}
