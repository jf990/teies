/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    os_qfd()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   OSK                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   results     string*  IN/OUT   String returned to caller|*/
/*|  :PARAM.   given       string   INPUT    FD / partial FD for      |*/
/*|                                          qualification            |*/
/*|  :PARAM.   default     string   INPUT    FD qualification default |*/
/*|                                                                   |*/
/*|  :PURPOSE.                                                        |*/
/*+-------------------------------------------------------------------+*/
/*|                 Given a fragment of a file descriptor, it builds  |*/
/*|       remainder based on a given default.  What the result looks  |*/
/*|       like is OS dependant, reflecting the local identifiers.     |*/
/*|                                                                   |*/
/*|       An identifier will be considered in four parts:             |*/
/*|       1. The "disk" or mode on which the file resides, "FM";      |*/
/*|       2. The path to follow to reach the file (unfortunately,     |*/
/*|          a necessary system dependancy), called "FP";             |*/
/*|       3. The file name or (FN);                                   |*/
/*|       4. The file type (FT)                                       |*/
/*|                                                                   |*/
/*|       A file descriptor is passed as "FM FP FN FT", mimicking a   |*/
/*|       PC-DOS file descriptor, which is considred the most general |*/
/*|       case to use as a guide line.  Following are examples of     |*/
/*|       file descriptors (FD's) for some local machines, for use    |*/
/*|       as examples for future implementation:                      |*/
/*|                                                                   |*/
/*|       PC-DOS:   FM:FP\FN.FT, such as "C:\PTEIES\PROGRAM.C"        |*/
/*|                 The ":", "\", and "." are known dependancies that |*/
/*|                 os_qfd() would add;                               |*/
/*|                                                                   |*/
/*|       UNIX:     FP/FN.FT, such as "/usr/kjw/PROGRAM.C"            |*/
/*|                 The path "/usr/kjw/" is the presumable default,   |*/
/*|                 possibly provided by os_qfd(); the problem of case|*/
/*|                 dependancy is not addressed;  note that the FM    |*/
/*|                 is discarded;                                     |*/
/*|                                                                   |*/
/*|       P.E.                                                        |*/
/*|       OS 3230:  FM:FN.FT/FP, such as "SCRT:PROGRAM.C/998"         |*/
/*|                 The ":", ".", and "/" are known dependancies that |*/
/*|                 os_qfd() would add, the file mode "SCRT" and      |*/
/*|                 usernumber "998" would have to be obtained in     |*/
/*|                 accordance to the implemention standards;         |*/
/*|                 note that the FM is not 1 character;              |*/
/*|                                                                   |*/
/*|      UNISYS                                                       |*/
/*|      VS/9:      $FP.FT.FM, such as "$JW037703.PROGRAM.C"          |*/
/*|                 The "$" and "."'s are known dependancies that     |*/
/*|                 os_qfd() would add, the userid "JW037703" would   |*/
/*|                 have to be added in accordance with local         |*/
/*|                 standards;  note that the FP is discarded;        |*/
/*|                                                                   |*/
/*|      IBM VM:    FN FT FM, such as "PROGRAM C *"                   |*/
/*|                 The blanks are known dependancies added by        |*/
/*|                 os_qfd();  note that the file path is discarded;  |*/
/*|                                                                   |*/
/*|      Although the implementation will probably require some       |*/
/*|      adjustments to be made in calls to os_qfd(), and, of course, |*/
/*|      os_qfd() itself, it is believed by the author and his        |*/
/*|      co-conspiritors that this convention will require the least  |*/
/*|      amount of bending and hammering to transport.                |*/
/*|                                                                   |*/
/*|      The major difficulty is that the concept of file path (FP)   |*/
/*|      and usually file mode (FM) exist on foreign operating        |*/
/*|      systems, but the implementation details are almost always    |*/
/*|      incompatable.                                                |*/
/*|                                                                   |*/
/*|      The use of "*" as a wild card is percieved as an advantage   |*/
/*|      because most systems (1) use the same as a wild card, and/or |*/
/*|      (2) do not permit "*" as part of a file name.                |*/
/*|                                                                   |*/
/*|      Althought os_qfd() does not expand the FP if one is not given|*/
/*|      it maybe useful on some operating systems to complete the    |*/
/*|      expansion.                                                   |*/
/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or passed return codes                    |*/
/*|                                                                   |*/
/*|  :CALLS.   s_parse, s_len, s_valloc, s_nulout, s_drop, s_comp,    |*/
/*|            s_copy                                                 |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    27_Apr_87   K.J.Walsh   Written.                       |*/
/*|            21_Jun_88   S.Muccione  Re-Written to use s_parse      |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


sint os_qfd ( results, given_i, defalt_i)

string   *results ;                    /* What we give 'em             */
string   given_i  ;                    /* What they gave us            */
string   defalt_i;                    /* The likely difference between*/

{
   string  *given_vec  = NIL;
   string  *defalt_vec = NIL;
   string  template  = S_NULL;
   string  spill_str = S_NULL;
   string  delim_str = S_NULL;
   string  given     = S_NULL;
   string  defalt    = S_NULL;
   string  wild_card = S_NULL;
   string  blank     = S_NULL;
   sint    cntr      = 0;
   sint    r         = 0;


   s_valloc (&given_vec,4);
   s_valloc (&defalt_vec,4);

   s_init (&template," @n @n @n @n ");
   s_init (&wild_card,"*");
   s_init (&blank," ");

   s_copy (&given,given_i,0,0);
   s_copy (&defalt,defalt_i,0,0);

   s_parse (&given,template,delim_str,given_vec,&spill_str);
   s_parse (&defalt,template,delim_str,defalt_vec,&spill_str);

/*+-------------------------------------------------------------------+*/
/*| Here's the tricky part.  For ease of coding, we want to make      |*/
/*| sure that all the entries in the str_vec's are null that are not  |*/
/*| used.  s_parse doesn't do this so we must.                        |*/
/*+-------------------------------------------------------------------+*/
   for (cntr = s_len ((string) given_vec) + 1; cntr <= 4; cntr++)
   {
      s_nulout ( &( given_vec[cntr] ) );
   }
   s_smp_ul ((string) given_vec) = 4;

   for (cntr = s_len ((string) defalt_vec) + 1; cntr <= 4; cntr++)
   {
      s_nulout ( &( defalt_vec[cntr] ) );
   }
   s_smp_ul ((string) defalt_vec) = 4;


/*+-------------------------------------------------------------------+*/
/*| Now we check the entries in given_vec to se if they are blank     |*/
/*| or match the wild card, in which case we simply copy the          |*/
/*| corresponding entry from defalt_vec to given_vec.                |*/
/*+-------------------------------------------------------------------+*/

   for (cntr = 1; cntr < 5; cntr++)
   {
      if (
          ( s_len ( given_vec[cntr] ) == 0) ||
          ( given_vec[cntr] == S_NULL )     ||
          ( s_comp ( wild_card, given_vec[cntr], 0, 0, 'M', &r) == 0)
         )
      {
         s_copy ( &( given_vec[cntr] ), defalt_vec [cntr], 0, 0);
      }
   }

/*+-------------------------------------------------------------------+*/
/*| Here begins the machine dependant part,                           |*/
/*|   just concat what you want in what order and wallah.             |*/
/*+-------------------------------------------------------------------+*/

   s_nulout (results);
   s_copy   (results, given_vec[3]          , 0, 0);
   s_cat    (results, *results, blank         , 0, 0);
   s_cat    (results, *results, given_vec[4], 0, 0);
   s_cat    (results, *results, blank         , 0, 0);
   s_cat    (results, *results, given_vec[1], 0, 0);

/*+-------------------------------------------------------------------+*/
/*| All Done!                                                         |*/
/*+-------------------------------------------------------------------+*/

   s_drop (&template);
   s_drop (&wild_card);
   s_drop (&spill_str);
   s_drop (&given);
   s_drop (&defalt);
   s_drop (&blank);
   s_drop (&delim_str);
   s_vdrop(&given_vec);
   s_vdrop(&defalt_vec);


   return(0);

}
