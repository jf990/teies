/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_pr_s()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.  D_pr_d      *sint()    OUTPUT Dummy print function ptr |*/
/*|                                                                   |*/
/*|  :PARAM.   a           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   b           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   c           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   d           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   e           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   f           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   g           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   h           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   i           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   j           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   k           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   l           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   m           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   n           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   o           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   p           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   q           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   r           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   s           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   t           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   u           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   v           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   w           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   x           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   y           (anything) INPUT  (see documentation)      |*/
/*|  :PARAM.   z           (anything) INPUT  (see documentation)      |*/
/*|                                                                   |*/
/*|  :PURPOSE. PPPMAC routine for printing to stdout.                 |*/
/*|            Requires initialization by #D_main().                  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns rcode from called routines.                    |*/
/*|                                                                   |*/
/*|  :CALLS.   fprintf()                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Might not be portable:                                 |*/
/*|                                                                   |*/
/*|  The following routine, I am ashamed to admit, is a major tweak   |*/
/*|  that takes advantage of the various loopholes and weaknesses of  |*/
/*|  C.  The idea is to be able to prints to the screen and into file |*/
/*|  without having my source crowded with dual function calls (which |*/
/*|  render the source impossible to maintain, of course.             |*/
/*|                                                                   |*/
/*|  In theory, I can call the following function with and also call  |*/
/*|  the print functions with an arbritray number of params because   |*/
/*|  they determine how many paramaters are expected from the format  |*/
/*|  argument.  Because this routine passes pointers, which are       |*/
/*|  the same as integers in this installation, I can use pointers    |*/
/*|  and get by.  I do not think it would work if we wanted to        |*/
/*|  print a float, or on a system where the pointer and integer      |*/
/*|  size differ.  In the latter two cases, a grunt will have to      |*/
/*|  duplicate the "printf()" calls, then change the first of each    |*/
/*|  pair to printf and the second of each of fprintf...              |*/
/*|                                                                   |*/
/*|  :MODS.    11_Nov_86   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Moved from debugi.h to STR     |*/
/*|                                    txtlib.                        |*/
/*|  :MODS.    28_Sep_86   K.J.Walsh   D_pr split into several D_pr_  |*/
/*|                                    functions.                     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"
extern fprintf();
#define VARLIST1 a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
#define VARLIST2 *a,*b,*c,*d,*e,*f,*g,*h,*i,*j,*k,*l,*m,*n,*o,*p,*q,\
                  *r,*s,*t,*u,*v,*w,*x,*y,*z
sint D_pr_s(VARLIST1)
int       VARLIST2;
{
   sint  rc =  0  ;                    /* Rcode from called routines   */

   rc = fprintf(stdout,VARLIST1);

   return(rc);

}
