/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_pr_f()                                               |*/
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
/*|  :PURPOSE. PPPMAC printing routine--prints into a file only.      |*/
/*|            Initialization by #D_main() required.                  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns rcode from called routines.                    |*/
/*|                                                                   |*/
/*|  :CALLS.   f_printf()                                             |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Might not be portable--see inline comments in D_pr_s   |*/
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
sint D_pr_f(VARLIST1)
int       VARLIST2;
{
   sint  rc =  0  ;                    /* Rcode from called routines   */

   rc = fprintf(D_io,VARLIST1);

   return(rc);

}
