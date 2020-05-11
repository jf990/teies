#include <stdio.h>

extern COMIO();
extern COMINTR();
extern COMSET();
extern COMCLO();
extern COMREAD();
extern COMWRIT();
extern COM2ASC();
extern COM2EBC();
int comextrn;
FILE *errf;

main()
{
   int rc = 0;

/* char *respref = 0x40115D7F11000000000009;
   char *normal =  0xC3115D7F114040130008;
   char *invite =  0xC3115D7F110005910008;
   char prefix =  "0xC3115D7F110005F1F2F3F4F4F3F2F1";
   char prefix(::) = {\0xC3,\0x11,\0x5d};

*/

   char *prefix, *prefix1;
   int prefixlen = 15;
   int i = 0;
   int strl = 1000;
   char *str;

   errf = fopen("com err","w");
/*
   str = "HELLO"; strl = 5;
   rc = Inter(COM2ASC, str , strl);
   dump(str,strl,errf);

   rc = Inter(COM2EBC, str , strl);
   dump(str,strl,errf);
*/
   rc = Inter(COMSET);
   fprintf(errf,"SET:rc=%x\n",rc);

   str = malloc( strl );
/*
   sleep(10);
   rc = Inter(COMWRIT, prefix, &prefixlen);
   fprintf(errf,"write:rc=%x   len = %d\n",rc, prefixlen);
   rc = Inter(COMWRIT, prefix, &prefixlen);
   fprintf(errf,"write:rc=%x   len = %d\n",rc, prefixlen);
   rc = Inter(COMWRIT, prefix, &prefixlen);
   rc = Inter(COMWRIT, prefix, &prefixlen);
   fprintf(errf,"write:rc=%x\n",rc);
*/

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





for(i=0;i<2 ;i++){

dump(prefix,prefixlen,errf);
   rc = Inter(COMWRIT, prefix, &prefixlen);
   fprintf(errf,"write:rc=%x   len = %d\n",rc, prefixlen);
   rc = Inter(COMREAD, str, &strl);
   fprintf(errf,"REAd:rc=%x\n",rc);
   if (rc == 0)
      dump(str, strl, errf);
}
   rc = Inter(COMCLO);
   fprintf(errf,"CLO:rc=%x\n",rc);

   exit(rc);
}
 dump(ptr, len, outfile)
   char *ptr;
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
