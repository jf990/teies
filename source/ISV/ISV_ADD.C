/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_add()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV byte* IN/OUT   Root ISV pointer         |*/
/*|                                                                   |*/
/*|  :PARAM.   isv_id      string   INPUT    String containing ISV    |*/
/*|                                          file descriptor in       |*/
/*|                                          teies standard file      |*/
/*|                                          descriptor format        |*/
/*|                                                                   |*/
/*|  :PURPOSE. Push down another ISV entry on ISV stack.              |*/
/*|                                                                   |*/
/*|  :OUTPUT.  zero or a standard ISV error code                      |*/
/*|                                                                   |*/
/*|  :CALLS.   os_qfd(), os_mema(), os_memd()                         |*/
/*|                                                                   |*/
/*|  :ERRORS.  ISV_FNF, ISV_IFC                                       |*/
/*|                                                                   |*/
/*|  :LIMITS.  Phase 1 version -- fetchs ISV's from CMS files         |*/
/*|                                                                   |*/
/*|  :MODS.    05_Apr_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42;             |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include <stdio.h>                  /* Required!                       */


sint isv_add(isv_id)

string   isv_id   ;           /* name of ISV                           */

{
   byte     *cstr       =  0      ; /*  C style version of the string  */
   sint     count       =  0      ; /* A counter that we use all over  */
   char     ch          =  0      ; /* Also used by SKIP()             */
   byte     *boot       =  0      ; /* To pull in ISV part cont'n len  */
   byte     *addr       =  0      ; /* Address of memory for ISV       */
   byte     *ptr        =  0      ; /* Pointer we move around it it    */
   sint     rc          =  0      ; /* RC from stuff                   */
   tint     *size       =  0      ; /* size of ISV extraced from ISV   */
   tint     realsize    =  0      ; /* *size is in D-words, hafta calc */
   string   blank       =  S_NULL ; /* A single space, need in file gen*/
   string   info        =  S_NULL ; /* mem man info from isv           */
   string   names_at    =  S_NULL ; /* location of name - list         */
   string   name_count  =  S_NULL ; /* total number of sets in isv     */
   string   defalt      =  S_NULL ; /* default file name.              */
   tint     *kludge     =  0      ; /* see descriptive paragraph below */
   FILE     *isv_file             ; /* I/O pointer                     */
   byte     *proof_str  = "ISV!"  ; /* Proof string--must start ISV$OBJ*/


   rc = s_init(&defalt,"* * * ISV$OBJ"); /* Qualify the file           */
   rc = os_qfd(&isv_id,isv_id,defalt);
   rc = s_drop(&defalt);

   cstr = os_mema( s_len(isv_id));
   rc = s_stocs(&cstr,isv_id,0,0);

   isv_file = fopen(cstr,"r");           /* Attempt to open here      */
   if ( isv_file == NULL)
   {
      os_memd(cstr);
   return(ISV_FNF);
   }

/*+-------------------------------------------------------------------+*/
/*|  hafta find out length of ISV.  Allocate just enough to find out, |*/
/*|  then do so.                                                      |*/
/*+-------------------------------------------------------------------+*/
   boot = os_mema(ISV_LENGTH+ISV_LENGTH_S);   /* allocate              */
   ptr = boot;                                /* copy pointer          */

   for( count=0; count < ISV_LENGTH+ISV_LENGTH_S; count++)
   {
      *ptr = getc(isv_file);                  /* bring in characters   */
      if ( EOF == (int) *ptr )
      {
         fclose(isv_file);
   return(ISV_IFC);
      }
      ptr++;                                  /* set for next          */
   }

/*+-------------------------------------------------------------------+*/
/*|  Find the "ISV!" prefix in the file, which indicates that it was  |*/
/*|  (probably) created correctly by TSTR MACRO and is not, for       |*/
/*|  instance, C source code.                                         |*/
/*+-------------------------------------------------------------------+*/
   for ( count = 0 ; count < ISV_PROOF_S ; count++ )
   {
      if ( *(boot+count) != *(proof_str+count) )
      {
         os_memd(cstr);
   return(ISV_INVALID_FORMAT);
      }
   }


   size = (tint*)(boot + ISV_LENGTH);/* home in on value in actual ISV */
   realsize = *size * 8;            /* we need BYTES but it's in Dwords*/


   os_memd(boot);                   /* don't need that anymore        */
   if ( *size < 1 )
   {
      os_memd(cstr);
   return(ISV_INVALID_FORMAT);
   }


/*+-------------------------------------------------------------------+*/
/*|  The ISV length computed when the ISV was generated is now known. |*/
/*|  Allocate memory and bring in entire ISV                          |*/
/*+-------------------------------------------------------------------+*/
   addr = os_mema(realsize);        /* allocate enought for whole ISV  */
   ptr = addr;

   isv_file = freopen(cstr,"r",isv_file);/* reopen the ISV file        */
   os_memd(cstr);
   if ( isv_file == NIL )
   {
   return(ISV_FNF);
   }

   for ( count = 0; count < realsize; count++)
   {
      *ptr = getc(isv_file);        /* bring one in                    */
      if ( EOF == (int) *ptr )
      {
   return(ISV_IFC);
      }
      ptr++;
   }

/*+-------------------------------------------------------------------+*/
/*|  Set ISV seek path pointers                                       |*/
/*+-------------------------------------------------------------------+*/

   ptr = ISV_OBJECT + addr;         /* Locate isv object FD field      */

   for ( count = 0 ; count < ISV_OBJECT_S ; count++)
   {
      *ptr = cstr[count:);
      ptr++;
   }

   if (  TM_master->ISV == 0 )              /* Is this the 1st ISV loaded?     */
   {
                                    /* yup                             */
      TM_master->ISV = addr;
   }
   else
   {                               /* nope -- push ISV's              */
/*+-------------------------------------------------------------------+*/
/*|CLUDGE - It seems that there is no portable way that I can - KLUDGE|*/
/*|CLUDGE - stick in the address of the preceding ISV, with   - KLUDGE|*/
/*|CLUDGE - the possible exception of using a structure that  - KLUDGE|*/
/*|CLUDGE - would also be unstable because C likes to align   - KLUDGE|*/
/*|CLUDGE - things in structures (and the data for the struct - KLUDGE|*/
/*|CLUDGE - is created by a by TSTR MACRO, which does no such - KLUDGE|*/
/*|CLUDGE - thing.                                            - KLUDGE|*/
/*+-------------------------------------------------------------------+*/
      /* point to link info in NEW one   */
      kludge = (tint*) (addr + ISV_LINK_AT);
      /* put pointer of OLD one into NEW */
      *kludge =  (tint) TM_master->ISV;


      TM_master->ISV = addr;       /* set TM pointer to the NEW one   */
   }
   fclose(isv_file);

   return(0);

}
