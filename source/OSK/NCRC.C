#include <stdio.h>
#define VOID  int
unsigned short int crc_table[256];

main()

{
VOID gen_tab();
 FILE *fp;


unsigned short int gen_crc();
unsigned short int length,crc;               /* crc = 0*9001 */
static char testmsg[] = "this is a test message ";

fp=fopen("crc t","w");
gen_tab();                          /*   fill up the table */
print_tab(fp);

length = sizeof(testmsg)-1;
fprintf(fp,"LENGTH TESTMSG = %d \n",length);

fprintf(fp,"BEFORE ENTER = [%s] \n",testmsg);
crc = gen_crc(length,testmsg,fp);

fprintf(fp,"\n\n\n TESTMSG [%s] \n CRC = %04x\n\n",testmsg,crc);
return(0);
}


VOID gen_tab()

{
short int temp;
union { unsigned short int i;

       struct {
           unsigned   :8;
           unsigned i8:1; unsigned i7:1; unsigned i6:1;
           unsigned i5:1; unsigned i4:1; unsigned i3:1;
           unsigned i2:1; unsigned i1:1;
        }bit;
 }iUn;

union {
       unsigned short int start;

       struct {
           unsigned b16:1;
           unsigned b15:1; unsigned b14:1; unsigned b13:1;
           unsigned b12:1; unsigned b11:1; unsigned b10:1;
           unsigned b9 :1;
           unsigned b8 :1; unsigned b7 :1; unsigned b6 :1;
           unsigned b5 :1; unsigned b4 :1; unsigned b3 :1;
           unsigned b2 :1; unsigned b1 :1;
       }startbit;
 }startUn;

for (iUn.i = 0; iUn.i <255; iUn.i++) {

/*printf("I=%x:%d\n",iUn.i,iUn.i); */
/*printf("I=%x: bit1=%d  bit8=%d\n",iUn.i,iUn.bit.i1,iUn.bit.i8);
*/
        startUn.start = 0;

temp = (iUn.bit.i7 ^ iUn.bit.i6 ^ iUn.bit.i5 ^ iUn.bit.i4 ^ \
        iUn.bit.i3 ^ iUn.bit.i2 ^ iUn.bit.i1);

        startUn.startbit.b16 =(iUn.bit.i8 ^ temp);
        startUn.startbit.b15 =(temp);
        startUn.startbit.b14 =(iUn.bit.i8 ^ iUn.bit.i7);
        startUn.startbit.b13 =(iUn.bit.i7 ^ iUn.bit.i6);
        startUn.startbit.b12 =(iUn.bit.i6 ^ iUn.bit.i5);
        startUn.startbit.b11 =(iUn.bit.i5 ^ iUn.bit.i4);
        startUn.startbit.b10 =(iUn.bit.i4 ^ iUn.bit.i3);
        startUn.startbit.b9  =(iUn.bit.i3 ^ iUn.bit.i2);
        startUn.startbit.b8  =(iUn.bit.i2 ^ iUn.bit.i1);
        startUn.startbit.b7  =(iUn.bit.i1);
        startUn.startbit.b1  =(iUn.bit.i8 ^ temp);

      printf("TEMP = %x    start=%x\n",temp, startUn.start);

        crc_table[iUn.i]=startUn.start;
     }
   }
/*********************************************/

VOID print_tab(fp)
FILE *fp;
{
  int i;

    for ( i=0; i<256;i++){
      if ( ! ( i % 8))

        fprintf(fp,"\n %02x  -  %04x",i,crc_table[i]);

      else
        fprintf(fp,"  %04x",crc_table[i]);
       }
    fprintf(fp," \n");
 }
/************************************************/

unsigned short gen_crc(length,textptr,fp)
unsigned short int length;
char *textptr;
FILE *fp;

{
  short int i,index;
  unsigned short crc;

  crc=0;            /* crc starts at zero at every message */

   fprintf(fp," ENTERED AS ? [%s] \n",textptr);
     for( i=0; i<length; i++,textptr++)
       {
          index = ( ( crc ^ *textptr ) & 0x00FF);
          crc= ( ( crc >>8) & 0x00FF)^crc_table[index];
       }
    fprintf(fp," CRC IS? %04x \n",crc);
     return(crc);
}

/**************************************************************/

