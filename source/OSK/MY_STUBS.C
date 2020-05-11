#define DEBUG
#define VM
#include <manifest.h>
/*
** This is a collection of a number of assorted routines which
** are provided to improve portability to CMS, like sleep() and
** ffs(), and a few other "typical" entry points which are
** here to allow programs to load okay, but which do not provide
** correct function, like open() and fstat();
*/
#include <bsdtime.h>

#define QUSER             "QUERY USERID"

sleep(num)
int num;
{
    char buf[50];
    sprintf(buf, "CP SLEEP %d SEC", num);
    system(buf);
    return(0);
}

/*
** Find the first set bit
** i.e. least signifigant 1 bit:
** 0 => 0
** 1 => 1
** 2 => 2
** 3 => 3
** 4 => 4
*/

int
ffs(mask)
unsigned int    mask;
{
    register i;

    if ( ! mask ) return 0;
    i = 1;
    while (! (mask & 1)) {
        i++;
        mask = mask >> 1;
    }
    return i;
}


gethostname(cp, len)
char *cp;
int  len;
{
    int rc;
    char buf[24];
    register char *pp = buf;

    rc = CPCMD(QUSER, strlen(QUSER), buf, sizeof(buf));
    if (rc < 0) return(-1);
/*
** Search for the third word (after 2nd space)
**
** Q USERID returns "name at node"
*/
    while (*pp != ' ') pp++;   /* skip over name */
    while (*pp == ' ') pp++;   /* skip over first set of blanks */
    while (*pp != ' ') pp++;   /* skip over at */
    while (*pp == ' ') pp++;   /* skip over second set of blanks */
    strncpy(cp,pp,len);
    return(0);
}

lseek(fd,offset,whence)
int fd;
unsigned long offset;
int whence;
{
   return(-1);
}

getpid()
{
    char buf[24];
    int rc,id;

    rc = CPCMD(QUSER, strlen(QUSER), buf, sizeof(buf));
    id = *((int *) buf);
    return(id);   /* pretend */
}

gettimeofday(now, tzp)
struct timeval *now;
struct timeval *tzp;
{
     static int last_sec = 0;
     static int last_usec = 0;

     now->tv_sec = time(0);
     if (last_sec != now->tv_sec)
       now->tv_usec = 0;
     else now->tv_usec = ++last_usec;
     last_sec = now->tv_sec;
     last_usec = now->tv_usec;
}

geteuid()
{
   return(0);
}

getegid()
{
   return(0);   /* pretend */
}

getgroups(gidsetlen,gidset)
int gidsetlen;
int *gidset;
{
   if (gidsetlen < 1) return(-1);
   *gidset = getegid();
   return(1);
}

#ifdef DEBUG
dump_bfr(buf,len)
unsigned char buf[];
int len;
{
 register int i;

 for(i=0;i<len;i++) {
  printf("%2.2x%c",buf[i],((i & 15) == 15) ? '\n' : ' ');
 }
 printf("\n");
}
#endif
