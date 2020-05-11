/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_parse ()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  S_NULL                                                 |*/
/*|                                                                   |*/
/*|  :PARAM.   target      *string  IN/OUT   Str to be parsed         |*/
/*|  :PARAM.   template_i   string  INPUT    Template for parsing     |*/
/*|  :PARAM.   deli_str_i   string  INPUT    Contains delimiter chars |*/
/*|  :PARAM.   str_vec[]  string  IN/OUT   An array of strings      |*/
/*|  :PARAM.   spill_str   *string  OUTPUT   Leftover of parse        |*/
/*|                                                                   |*/
/*|  :PURPOSE. To take the target string and find the first occurence |*/
/*|            of any delimiter character in it.  The section before  |*/
/*|            the delimiter is to be parsed and all after the deli-  |*/
/*|            miter goes into the target string.  Then the string    |*/
/*|            vector is assigned strings according to the template.  |*/
/*|            The remains of the target after the parse are returned |*/
/*|            in the spill_str.                                      |*/
/*|                                                                   |*/
/*|            Template elements:                                     |*/
/*|              @n @#n @$n @x.n @(n) @"n @'ccn @%ccn @! @@ @?cllcttc |*/
/*|              @-cllc @~cllcttc @+cllc @=                           |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns standard STR rc or zero                        |*/
/*|                                                                   |*/
/*|  :CALLS.   s_char_s,s_len,s_type,s_copy,s_clip,s_first,s_cat,     |*/
/*|            s_trim,s_drop,s_init,s_cat                             |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    13 Jan 88   Steve Muccione   Preliminary version.      |*/
/*|            1  Feb 88   Steve Muccione   Made change to work       |*/
/*|                                         with s_type.              |*/
/*|            10 Feb 88   Steve Muccione   Updated for trail blanks, |*/
/*|                                         added ", ' modifier.      |*/
/*|            12 Feb 88   Steve Muccione   Added % modifier.         |*/
/*|            17 Feb 88   Steve Muccione   Added @(n), n shorthand   |*/
/*|            22 Feb 88   Steve Muccione   Fixed problem with nul    |*/
/*|                                         delimiter.                |*/
/*|             7 Mar 88   Steve Muccione   Rewrote ",',%,#,$ funcs.  |*/
/*|            19 May 88   Steve Muccione   Added conditional literal |*/
/*|                                         @?cLLLcTTTc               |*/
/*|            23 May 88   Steve Muccione   Added conditional exit    |*/
/*|                                         literal @-cLLLc           |*/
/*|            27 May 88   Steve Muccione   Fixed problem with null   |*/
/*|                                         extractions.              |*/
/*|            10 Jun 88   Steve Muccione   Added repeating template  |*/
/*|                                         @~cllcttc                 |*/
/*|            13 Jun 88   Steve Muccione   Updated for new error cds |*/
/*|            15 Jun 88   Steve Muccione   Added non-destructive     |*/
/*|                                         single chr lits @+cllc    |*/
/*|            28 Jun 88   Steve Muccione   Added @=n                 |*/
/*|             5 Jul 88   Steve Muccione   Fixed @+ to work with     |*/
/*|                                         @",@'                     |*/
/*|            25 Apr 89   Steve Muccione   Added not (^) modifier to |*/
/*|                                         ~ and ?, ie @^~, @^?      |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"
#include "str.h"
#include "osk.h"

#define WHITE_SPACE(var) ((var) == ' ')
#define ON_TOKEN(str,ptr) ((s_char_s((str),((ptr)-1)) == '@') && \
                           (s_char_s((str),(ptr)) != '@'))
#define ON_DOUBLE_AT(str,ptr) ((s_char_s((str),((ptr)-1)) == '@') && \
                           (s_char_s((str),(ptr)) == '@'))
#define NOT_NULL(str) (s_len((str)) != 0)

sint s_parse (target,template_i,deli_str_i,str_vec,spill_str)

string *target;
string template_i;
string deli_str_i;
string str_vec[];
string *spill_str;

/* start of s_parse */

{

string   func_delim  =  S_NULL;        /* ties string with '@'         */
string   garbage     =  S_NULL;        /* garbage string used w/clip   */
string   tmp_literal =  S_NULL;        /* stores extracted literal     */
string   extracted   =  S_NULL;        /* resultes of a pass           */
string   BLANK       =  S_NULL;        /* ties string with ' '         */
string   template    =  S_NULL;        /* working version of template  */
string   deli_str    =  S_NULL;        /* working version of deli_str  */
string   start_delim =  S_NULL;        /* start delim for '"' option   */
string   end_delim   =  S_NULL;        /* ending delim for '"' option  */
string   strv_delim  =  S_NULL;        /* a string that holds ')'      */
string   not_operator=  S_NULL;        /* a string that holds ')'      */
sint     ex_num      =  0;             /* specific amount to copy      */
sint     arg_num     =  0;             /* which str_vec to copy to     */
sint     literal_start= 0;             /* start of literal in target   */
sint     type_except =  0;             /* result of s_type             */
sint     first_delim =  0;             /* loc of delim in template     */
sint     tmp_loop    =  0;             /* temporary loop variable      */
sint     rc          =  0;             /* return code                  */
sint     s_parse_rc  =  0;             /* return code for s_parse      */
sint     res         =  0;             /* all purpose result variable  */
sint     current     =  0;             /* current default arg_num val  */
sint     num_lits    =  0;             /* number of @+cllc literals    */
byte     type        =  0;             /* paramater for s_type         */
boolean  first_pass  =  TRUE;          /* flag for first token parse   */
boolean  trim_end    =  TRUE;          /* trim trailing spaces or not  */
boolean  do_extract  =  TRUE;          /* set when extract completed   */
boolean  negate      =  TRUE;          /* negate operator flag         */



rc=s_init (&BLANK," ");
rc=s_init (&func_delim,"@");
rc=s_init (&strv_delim,")");
rc=s_init (&not_operator,"^");
rc=s_copy (&template,template_i,1,0);
rc=s_copy (&deli_str,deli_str_i,1,0);
first_pass=TRUE;
s_parse_rc = 0;

/*---------------------------------------------------------------------*/
/* Replace @(##) template sequences with ## string vectors             */
/*---------------------------------------------------------------------*/
for (first_delim=1;(first_delim<s_len(template)-1);first_delim++)
{
   if (ON_DOUBLE_AT (template,first_delim))
   {
     first_delim++;
   } else if (
              (s_char_s(template,first_delim-1) == '@') &&
              (s_char_s(template,first_delim  ) == '(')
             )
   {
      rc=s_clip (&tmp_literal,&template,first_delim,
          s_first(strv_delim,template,first_delim,0)-first_delim+1);
      rc=s_clip (&garbage,&tmp_literal,1,2);
      literal_start=s_type (tmp_literal,0,0,'N');
      rc=s_clip (&garbage,&tmp_literal,literal_start,0);
      rc=s_stosi(&arg_num,tmp_literal,0,0);
      if (s_len((string) str_vec) >= arg_num)
      {
         rc=s_clip (&garbage,&template,first_delim,0);
         rc=s_cat (&template,template,*(str_vec+arg_num),0,0);
         rc=s_cat (&template,template,garbage,0,0);
      }
   }
}
/*---------------------------------------------------------------------*/
/* Set the number of string vectors in use to zero                     */
/*---------------------------------------------------------------------*/
s_smp_ul((string) str_vec)=(hword) 0;

/*---------------------------------------------------------------------*/
/* Find the earliest occurance of a deliminator in target and clip     */
/*   into spill string (will be exchanged with target before exit)     */
/*---------------------------------------------------------------------*/
first_delim = -1;
s_nulout (spill_str);
while NOT_NULL (deli_str)
{
   rc=s_clip (&tmp_literal,&deli_str,1,1);
   res=s_first(tmp_literal,*target,0,0);
   if (
       ((res != 0) && (res < first_delim)) ||
       ((res != 0) && (first_delim == -1))
      )
   {
      first_delim = res;
   }
}
if (first_delim != -1)
{
   s_clip(spill_str,target,first_delim,0);
   s_clip(&garbage,spill_str,1,1);
}
rc=s_trim (&template,'L');
rc=s_trim (target,'L');

/* begining of loop */

while (NOT_NULL (template) && NOT_NULL (*target))
{
/*---------------------------------------------------------------------*/
/* Check if template starts with a blank if so then trim all blanks    */
/*   template and if they exist in target                              */
/*   else if template starts with a token (ie. no literal) then        */
/*   trim target up to first blank                                     */
/*---------------------------------------------------------------------*/
   if (WHITE_SPACE(s_char_s(template,0)))
   {
      rc=s_trim (&template,'L');
      rc=s_trim (target,'L');
   }
   else
   {
      if (
          ON_TOKEN (template,1) && (first_pass != TRUE)
         )
      {
         rc=s_trim (target,'L');
      }
   }
/*---------------------------------------------------------------------*/
/* Check if template starts with a literal (ie. not a token) if so then*/
/*   extract literal and kill target upto and including literal        */
/*   else just kill token identifier (ie. '@')                         */
/*---------------------------------------------------------------------*/
   trim_end=FALSE;
   if (!ON_TOKEN(template,1))
   {
      rc=s_nulout (&tmp_literal);
      for (first_delim=1;
           (
              (first_delim<s_len(template))
               && (!ON_TOKEN(template,first_delim))
           );
           first_delim++
          )
      {
         if (ON_DOUBLE_AT(template,first_delim))
         {
            first_delim++;
         }
         rc=s_cat (&tmp_literal,tmp_literal,template,first_delim,1);
      }
      rc=s_cat (&tmp_literal,tmp_literal,template,first_delim,1);
      rc=s_clip (&garbage,&template,1,first_delim);
      if (s_char_s (tmp_literal,s_len(tmp_literal)-1) == '@')
      {
         rc=s_clip (&garbage,&tmp_literal,s_len(tmp_literal),1);
      }
      if WHITE_SPACE (s_char_s(tmp_literal,s_len(tmp_literal)-1))
      {
         s_trim (&tmp_literal,'T');
         trim_end=TRUE;
      }
      if (s_first(tmp_literal,*target,1,0)>0)
      {
         rc=s_clip (&garbage,target,1,
                 s_first(tmp_literal,*target,1,0)+s_len(tmp_literal)-1);
      }
      if (trim_end)
      {
         rc=s_trim (target,'L');
      }
   }
   else
   {
      rc=s_clip (&garbage,&template,0,1);
   }
   if (trim_end)
   {
      rc=s_trim (target,'L');
   }
   first_pass=FALSE;
   do_extract=TRUE;
   arg_num=-1;
   ex_num=-1;
/*---------------------------------------------------------------------*/
/* If template starts with a number then extract it and assign to      */
/*   arg_num                                                           */
/*---------------------------------------------------------------------*/
   if (OS_ISNUM (s_char_s(template,0)) && NOT_NULL (template))
   {
      literal_start=s_type (template,0,0,'N');
      if (literal_start > 0)
      {
         --literal_start ;
      }
      rc=s_clip (&tmp_literal,&template,1,literal_start);
      s_stosi(&arg_num,tmp_literal,0,0);
   } else if ((s_char_s(template,0) == 'n') && (arg_num < 1))
   {
      arg_num=(++current);
      rc=s_clip (&tmp_literal,&template,1,1);
   }
/*---------------------------------------------------------------------*/
/* Processing for template tokens.                                     */
/*                                                                     */
/* Type modifiers:                                                     */
/*   Default type is 'P' (all printable characters)                    */
/*   # make type 'N' numerics                                          */
/*   ! make type 'L' alphabetics and numerics                          */
/*   $ make type 'U' alphabetics only                                  */
/*   . then out arg_num is actuall the pre-defined amount to copy      */
/*      from the target so assign it to ex_num and make arg_num        */
/*      undefined                                                      */
/*                                                                     */
/* Extraction modifiers:                                               */
/*   Notation: c-character delimiter  ll-literal  tt-template          */
/*   'cc      -extract the next cc quoted string or unquoted word      */
/*   "        -return the next " or ' quoted string or unquoted word   */
/*   %cc      -return the next cc quoted string anywhere in the target */
/*   ~cllcttc -execute tt template until ll literal is encountered     */
/*   ?cllcttc -if ll literal is encountered then execute tt template   */
/*   -cllc    -if ll literal is encountered then halt parsing          */
/*   +cllc    -one shot non destructive multiple single character      */
/*             literals                                                */
/*                                                                     */
/* Miscellaneous tokens:                                               */
/*   =#       -set the variable extraction location to # (ie. n=#)     */
/*                                                                     */
/*                                                                     */
/*---------------------------------------------------------------------*/
   if (s_char_s (template,0) == '^')
   {
      negate = TRUE;
      s_clip (&garbage,&template,1,1);
   } else
   {
      negate = FALSE;
   }
/*---------------------------------------------------------------------*/
/* type='P';  (all printable characters) default unless overridden     */
/*   by #, $ or !.                                                     */
/*---------------------------------------------------------------------*/
   type='P';
   switch (s_char_s(template,0))
   {
      case '.' :
         ex_num=arg_num;
         arg_num=-1;
         s_clip (&garbage,&template,1,1);
         break;
      case '#' :
         type='N';
         s_clip (&garbage,&template,1,1);
         break;
      case '!' :
         type='L';
         s_clip (&garbage,&template,1,1);
         break;
      case '$' :
         type='U';
         s_clip (&garbage,&template,1,1);
         break;
      case '\'' :
         do_extract=FALSE;
         s_nulout (&start_delim);
         s_nulout (&end_delim);
         s_trim (target,'L');
         s_ctos (&start_delim,s_char_s(template,1));
         s_ctos (&end_delim  ,s_char_s(template,2));
         s_clip (&garbage,&template,0,3);
         if (s_char_s(*target,0) == s_char_s(start_delim,0))
         {
            rc=s_clip (&garbage,target,1,1);
            rc=s_clip (&extracted,target,1,
                       s_first (end_delim,*target,2,0));
            if (s_char_s(extracted,s_len(extracted)-1) ==
                s_char_s(end_delim,0))
            {
               rc=s_clip(&garbage,&extracted,-1,1);
            }
         }
         else
         {
            do_extract = TRUE;
         }
         break;
      case '\"' :
         do_extract=FALSE;
         s_nulout (&start_delim);
         s_nulout (&end_delim);
         s_trim (target,'L');
         if (s_char_s(*target,0) == '\'')
         {
            s_ctos (&start_delim,'\'');
            s_ctos (&end_delim  ,'\'');
         }
         else
         {
            s_ctos (&start_delim,'\"');
            s_ctos (&end_delim  ,'\"');
         }
         s_clip (&garbage,&template,0,1);
         if (s_char_s(*target,0) == s_char_s(start_delim,0))
         {
            rc=s_clip (&garbage,target,1,1);
            rc=s_clip (&extracted,target,1,
                       s_first (end_delim,*target,2,0));
            if (s_char_s(extracted,s_len(extracted)-1) ==
                s_char_s(end_delim,0))
            {
               rc=s_clip(&garbage,&extracted,-1,1);
            }
         }
         else
         {
            do_extract = TRUE;
         }
         break;
      case '%' :
         s_nulout (&start_delim);
         s_nulout (&end_delim);
         s_trim (target,'L');
         s_ctos (&start_delim,s_char_s(template,1));
         s_ctos (&end_delim  ,s_char_s(template,2));
         s_clip (&garbage,&template,0,3);
         if (s_first (start_delim,*target,1,0) != 0)
         {
            rc=s_clip (&garbage,target,1,
                       s_first (start_delim,*target,1,0));
            rc=s_clip (&extracted,target,1,
                       s_first (end_delim,*target,2,0));
            if (s_char_s(extracted,s_len(extracted)-1) ==
                s_char_s(end_delim,0))
            {
               rc=s_clip(&garbage,&extracted,-1,1);
            }
         }
         else
         {
            rc=s_nulout (target);
         }
         do_extract=FALSE;
         break;
      case '~':
         s_nulout (&start_delim);
         s_trim (target,'L');
         s_ctos (&start_delim,s_char_s(template,1));
         s_nulout (&tmp_literal);
         s_trim (target,'L');
         rc=s_copy (&tmp_literal,template,3,
                    s_first (start_delim,template,3,0)-2);
         if (s_char_s(tmp_literal,s_len(tmp_literal)-1) ==
             s_char_s(start_delim,0))
         {
            rc=s_clip (&garbage,&tmp_literal,-1,1);
         }
         s_trim (&tmp_literal,'B');
         if (NOT_NULL(tmp_literal))
         {
            if (s_first (tmp_literal,*target,0,0) == 1)
            {
               if (!negate)
               {
                  rc=s_clip (&garbage,&template,1,
                     s_first (start_delim,template,3+s_len(tmp_literal)+1,
                     0));
                  rc=s_clip (&garbage,target,1,s_len(tmp_literal));
               } else
               {
                  rc=s_clip (&garbage,target,1,s_len(tmp_literal));
                  rc=s_pre (&template,template,not_operator,0,0);
                  rc=s_pre (&template,template,func_delim  ,0,0);
                  rc=s_copy (&garbage ,template,
                       6+s_len(tmp_literal),s_first (start_delim,
                       template,6+s_len(tmp_literal),0)-(6+
                        s_len(tmp_literal)));
                  rc=s_pre (&template,template,garbage,0,0);
               }
            } else
            {
               if (!negate)
               {
                  rc=s_pre (&template,template,func_delim,0,0);
                  rc=s_copy (&tmp_literal,template,
                       3+s_len(tmp_literal)+2,s_first (start_delim,
                       template,3+s_len(tmp_literal)+2,0)-(3+
                        s_len(tmp_literal)+2));
                  rc=s_pre (&template,template,tmp_literal,1,0);
               } else
               {
                  rc=s_clip (&garbage,&template,1,
                     s_first (start_delim,template,3+s_len(tmp_literal)+1,
                     0));
               }
            }
         } else
         {
               rc=s_pre (&template,template,func_delim,0,0);
               rc=s_copy (&tmp_literal,template,5,
                           s_first (start_delim,template,5,0)-(5));
               rc=s_pre (&template,template,tmp_literal,1,0);
         }
         break;
      case '?':
         s_nulout (&start_delim);
         s_trim (target,'L');
         s_ctos (&start_delim,s_char_s(template,1));
         s_clip (&garbage,&template,0,2);
         s_nulout (&tmp_literal);
         s_trim (target,'L');
         if (s_first (start_delim,template,0,0) != 0)
         {
            rc=s_clip (&tmp_literal,&template,0,
                       s_first (start_delim,template,0,0));
            if (s_char_s(tmp_literal,s_len(tmp_literal)-1) ==
                s_char_s(start_delim,0))
            {
               rc=s_clip (&garbage,&tmp_literal,-1,1);
            }
            s_trim (&tmp_literal,'B');
         }
         if (NOT_NULL(tmp_literal))
         {
            if ((s_first (tmp_literal,*target,0,0) == 1) ^ negate)
            {
               if (!negate)
               {
                  rc=s_clip (&garbage,target,0,s_len(tmp_literal));
               }
               rc=s_clip (&garbage,&template,
                    s_first (start_delim,template,0,0),1);
            } else
            {
               if (negate)
               {
                  rc=s_clip (&garbage,target,0,s_len(tmp_literal));
               }
               rc=s_clip (&garbage,&template,0,
                    s_first (start_delim,template,0,0));
            }
         } else
         {
            rc=s_clip (&garbage,&template,0,
                       s_first (start_delim,template,0,0));
         }
         break;
      case '-':
         s_nulout (&start_delim);
         s_trim (target,'L');
         s_ctos (&start_delim,s_char_s(template,1));
         s_clip (&garbage,&template,0,2);
         s_nulout (&tmp_literal);
         rc=s_clip (&tmp_literal,&template,0,
                    s_first (start_delim,template,0,0));
         if (s_char_s(tmp_literal,s_len(tmp_literal)-1) ==
             s_char_s(start_delim,0))
         {
            rc=s_clip (&garbage,&tmp_literal,-1,1);
         }
         s_trim (&tmp_literal,'B');
         if (NOT_NULL(tmp_literal))
         {
            if (s_first (tmp_literal,*target,0,0) == 1)
            {
               s_nulout (&template);
               rc=s_clip (&garbage,target,1,s_len(tmp_literal));
               s_parse_rc = S_PARSE_CE;
            }
         }
         break;
      case '+':
         s_ctos (&start_delim,s_char_s(template,1));
         rc=s_clip (&garbage,&template,0,
                    s_first (start_delim,template,3,0));
         break;
      case '=':
         s_clip (&garbage,&template,1,1);
         if (OS_ISNUM (s_char_s(template,0)) && NOT_NULL (template))
         {
            literal_start=s_type (template,0,0,'N');
            if (literal_start > 0)
            {
               --literal_start ;
            }
            rc=s_clip (&tmp_literal,&template,1,literal_start);
            s_stosi(&arg_num,tmp_literal,0,0);
            current= (--arg_num);
            arg_num = -1;
         }
         break;
   }

/*---------------------------------------------------------------------*/
/* If template is a number than it must be the real arg_num so         */
/*   read it and assign it                                             */
/*                                                                     */
/*---------------------------------------------------------------------*/
   if (OS_ISNUM (s_char_s(template,0)) && NOT_NULL (template))
   {
      literal_start=s_type (template,0,0,'N');
      if (literal_start > 0)
      {
         --literal_start ;
      }
      rc=s_clip (&tmp_literal,&template,1,literal_start);
      s_stosi(&arg_num,tmp_literal,0,0);
   } else if ((s_char_s(template,0) == 'n') && (arg_num < 1))
   {
      arg_num=(++current);
      rc=s_clip (&tmp_literal,&template,1,1);
   }
/*---------------------------------------------------------------------*/
/* If arg_num is defined than if we are to copy an exact amount do it  */
/*   else find the first occurance of our token deliminater            */
/*   (everything before it must be out literal so copy it into         */
/*   tmp_literal)  if tmp_literal is null than make it ' '.            */
/*   Find the location of the first occurance of the literal in target */
/*   Find the location of the character where our specified type       */
/*     ends                                                            */
/*   extract from target into extracted and copy it into the           */
/*     (arg_num)th string vector                                       */
/*                                                                     */
/*                                                                     */
/*---------------------------------------------------------------------*/
   if (arg_num != -1)
   {
      if ((ex_num != -1) && do_extract)
      {
         rc=s_clip (&extracted,target,1,ex_num);
      } else if (do_extract)
      {
         rc=s_nulout (&tmp_literal);
         first_delim=0;
         if (ON_TOKEN(template,1) && (s_char_s(template,1) == '+'))
         {
            s_ctos (&start_delim,s_char_s(template,2));
            num_lits =  s_first (start_delim,template,4,0) - 4;
            literal_start = s_first (BLANK,*target,0,0);
            if (literal_start == 0)
            {
               literal_start = s_len (*target);
            }
            s_copy (&tmp_literal,BLANK,0,0);
            for (tmp_loop = 1; tmp_loop <= num_lits; tmp_loop++)
            {
               s_ctos (&start_delim,s_char_s(template,tmp_loop+2));
               res = s_first (start_delim,*target,0,0);
               if ((res != 0) && (res <= literal_start))
               {
                  literal_start = res;
                  s_copy (&tmp_literal,start_delim,0,0);
               }
            }
         } else
         if (ON_TOKEN(template,1) &&
              (
                (s_char_s(template,1) == '?') ||
                (s_char_s(template,1) == '-') ||
                (s_char_s(template,1) == '~') ||
                (
                  (s_char_s(template,1) == '^') &&
                  (
                    (s_char_s(template,2) == '~')
                  )
                )
              )
            )
         {
            s_nulout (&start_delim);
            s_ctos (&start_delim,s_char_s(template,2));
            rc=s_copy (&tmp_literal,template,4,
                       s_first (start_delim,template,4,0)-3);
            if (s_char_s (start_delim,0) ==
                s_char_s (tmp_literal,s_len(tmp_literal)-1))
            {
               s_clip (&garbage,&tmp_literal,-1,1);
            }
            if (
                  (s_first(tmp_literal,*target,0,0) == 0 ) ||
                  (
                     (NOT_NULL(tmp_literal)                 ) &&
                     (s_first(BLANK,*target,0,0)       != 0 ) &&
                     (
                        s_first(BLANK,*target,0,0) <
                        s_first(tmp_literal,*target,0,0)
                     )
                  )
               )
            {
               s_nulout (&tmp_literal);
            }
         } else
         {
           for (first_delim=1;
                (
                   (first_delim < s_len(template))
                    && (!ON_TOKEN(template,first_delim))
                );
                first_delim++
               )
          {
              if (ON_DOUBLE_AT(template,first_delim))
              {
                 first_delim++;
              }
              rc=s_cat (&tmp_literal,tmp_literal,template,first_delim,1);
           }
         }
         trim_end=FALSE;
         if NOT_NULL (template)
         {
            if (!(ON_TOKEN(template,0) && (s_char_s(template,1) == '?')))
            {
              rc=s_cat (&tmp_literal,tmp_literal,template,first_delim,1);
              if ((s_char_s (tmp_literal,s_len(tmp_literal)-1) == '@')
                    && (first_delim < s_len(template)))
              {
                 rc=s_clip (&garbage,&tmp_literal,s_len(tmp_literal),1);
              }
            }
            if WHITE_SPACE (s_char_s(tmp_literal,s_len(tmp_literal)-1))
            {
               trim_end=TRUE;
            }
            s_trim (&tmp_literal,'B');
         }
         if (!NOT_NULL(tmp_literal))
         {
            rc=s_init (&tmp_literal," ");
         }
         literal_start=s_first(tmp_literal,*target,1,0);
         if (literal_start == 1)
         {
            do_extract = FALSE;
         }
         if (literal_start > 1)
         {
            literal_start--;
         } else if (literal_start < 1)
         {
            literal_start=s_len(*target);
         }
         if (NOT_NULL(*target))
         {
            type_except=s_type (*target,0,0,type);
         } else type_except = 0;
         if (type_except == S_NIL_STR)
         {
            type_except = 0;
         }
         if (type_except == 1)
         {
            do_extract = FALSE;
         }
         if (type_except != 0)
         {
            type_except--;
         } else
         {
            type_except = literal_start;
         }
         if (type_except < literal_start)
         {
            literal_start=type_except;
         }
         if (do_extract)
         {
            rc=s_clip (&extracted,target,1,literal_start);
            if (trim_end)
            {
               s_trim (&extracted,'T');
            }
         } else s_nulout (&extracted);
      }
/*---------------------------------------------------------------------*/
/* Drop null string for benefit of #D_strvec which expects the last    */
/*   string to be S_NULL                                               */
/*---------------------------------------------------------------------*/
      if (!NOT_NULL (extracted))
      {
         rc=s_nulout (&extracted);
      }
      if (s_allen((string) str_vec) >= arg_num)
      {
         current = arg_num;
         rc=s_copy (str_vec+arg_num,extracted,1,0);
         if (
             (s_len((string) str_vec) < arg_num) &&
             (NOT_NULL (extracted))
            )
         {
            s_smp_ul ((string) str_vec) = (hword) arg_num;
         }
      }
   }
}
if ((s_parse_rc == 0) && (NOT_NULL (template)))
{
   s_parse_rc = S_PARSE_IP;
}
/*---------------------------------------------------------------------*/
/* Switch spill_str and target to what they should be on return        */
/* and de-allocate all internal strings                                */
/*---------------------------------------------------------------------*/
rc=s_copy (&tmp_literal,*spill_str,1,0);
s_copy (spill_str,*target,1,0);
rc=s_copy (target,tmp_literal,1,0);
rc=s_drop (&func_delim);
rc=s_drop (&garbage);
rc=s_drop (&tmp_literal);
rc=s_drop (&extracted);
rc=s_drop (&BLANK);
rc=s_drop (&template);
rc=s_drop (&deli_str);
rc=s_drop (&start_delim);
rc=s_drop (&end_delim);
rc=s_drop (&strv_delim);
rc=s_drop (&not_operator);


   return(s_parse_rc);
}
