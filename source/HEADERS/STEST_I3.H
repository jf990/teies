/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STEST_I3 H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STEST                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   (Refer to STEST.C)                                     |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Does extern's of CLIB functions needed by STR testbed; |*/
/*|            also extern's of globals defined in STEST.C, the       |*/
/*|            testbed main file.                                     |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     ??_???_??   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized.                  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STEST_I3_H_INCLUDED

#   define STEST_I3_H_INCLUDED

   extern fprintf () ;
   extern strlen  () ;
   extern strcpy  () ;
   extern strcmp  () ;
   extern char *malloc  () ;

   /*+----------------------------------------------------------------+*/
   /*|                                                                |*/
   /*|  stest() global variables                                      |*/
   /*|                                                                |*/
   /*+----------------------------------------------------------------+*/
   extern string   corrupt ;
   extern string   complex ;
   extern string   *negstrptr;
   extern string   *nilstrptr;

   extern byte    *nilbyteptr  ;
   extern byte    *negbyteptr  ;

   extern int      errors     ;
   extern int      tests      ;
   extern char     *s         ;

#   ifdef OSK_H_INCLUDED
      extern datetime *testdt;
#   endif

#ifndef DIFFICULT_TEST
#   define DIFFICULT_TEST 0
#endif

#endif
