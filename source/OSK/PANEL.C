#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 1
#include <pnldattr.h>
#include <pnls.h>
#include <pnlf.h>
#endif

#define CONSTANT 0
#define NAME 1
#define ADDRESS 2

#if 1
static void *sptr;
static char buffer(:80:);
#endif

main()
{
#if 1
   system("CP SET PF1 IMMED \x03");
   system("CP Q PF1");
   setup();
   printf("Out of setup\n");
   while (gets(buffer) != NULL)
   {
      printf("Buf:%s:\n",buffer);
      setcursor(NAME);
      putdata();
      sread(sptr);
      getdata();
      puts(buffer);
   }
   cleanup();
   return(0);
}
/*  *********************  */
static setup()
{
   sptr = scrt(NULL);
   saddf(sptr, fcrt(CONSTANT,1,1,1,10,DA_HIGHLIGHT,UA_CONSTANT,"NAME:"));
   saddf(sptr, fcrt(NAME,1,12,1,16,DA_NORMAL,UA_EDIT,NULL));
  saddf(sptr,fcrt(CONSTANT,2,1,1,10,DA_HIGHLIGHT,UA_CONSTANT,"ADDRESS:"));
   saddf(sptr, fcrt(ADDRESS,2,12,3,16,DA_NORMAL,UA_EDIT, NULL));
}
/*  ***********************  */
static setcursor(int name)
{
   void *fptr;

   fptr = sfindfname(sptr, name);
   schgcrow(sptr, frow(fptr));
   schgccol(sptr, fcol(fptr));
}
/*   *********************   */
static putdata()
{
   sputf(sptr, NAME, strtok(buffer, ":"));
   sputf(sptr, ADDRESS, strtok(NULL, ":"));
}
/*   ***********************  */
static getdata()
{
   sprintf(buffer, "%s:%s", sgetf(sptr, NAME), sgetf(sptr, ADDRESS));
}
/* ****************************  */
static cleanup()
{
   void *fptr;

   while((fptr = sfindfnext(sptr,NULL)) != NULL)
   {
      printf("Hello\n");
      ffree(sdelf(sptr,fptr));
   }
   sfree(sptr);
}
#endif
