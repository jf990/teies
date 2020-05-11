/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     s_dtform()                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    str                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.   *tstr        string   IN/OUT   target string of operation*/
/*| :PARAM.   *dt          datetime INPUT    pointer to date/time stru|*/
/*| :PARAM.    ffrmat      string   INPUT    desired format template  |*/
/*|                                                                   |*/
/*| :PURPOSE.  converts the date and time structure pointed to by     |*/
/*|            'dt', using the format specified in 'ffrmat' into      |*/
/*|            a string                                               |*/
/*|                                                                   |*/
/*| :OUTPUT.   returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*| :CALLS.    s_cat(), s_parse(), s_init(), s_pad(),                 |*/
/*|            s_drop(), s_ctos(), s_gtdtnm(), s_gtdtst(),            |*/
/*|            s_nulout(), s_stoti(), s_copy                          |*/
/*|                                                                   |*/
/*| :ERRORS.   none known                                             |*/
/*|                                                                   |*/
/*| :LIMITS.   Always truncates on left regardless of requested       |*/
/*|            justification when truncation is to be done.           |*/
/*|                                                                   |*/
/*|            Processing of unknown formats usually does not         |*/
/*|            result in an error rc, ie, "@#$3m" works.  Sort of.    |*/
/*|                                                                   |*/
/*| :MODS.     15_May_87   N.R.Voronka Written.                       |*/
/*| :MODS.     25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    changed checking of rcodes to  |*/
/*|                                    D_checkrc() macro.             |*/
/*| :MODS.     05_Feb_88   K.J.Walsh   Replaced s_up() with OS_TOUPPER|*/
/*| :MODS.     10_Jun_88   A.K.Mehta   Rewritten using s_parse()      |*/
/*| :MODS.     08_Jul_88   A.K.Mehta   Updated to use s_pad()         |*/
/*|                                                                   |*/
/*| :MODS.     25_Aug_88   K.J.Walsh   Added use of 'ourform', added  |*/
/*|                                    comments & organized debugging.|*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#define   STR_VLEN   15

sint s_dtform (tstr, dt, ffrmat)
string   *tstr   ;                /* Target string (pointer of)        */
datetime *dt     ;                /* Pointer to date-time structure    */
string    ffrmat ;                /* String containing format of dt    */

/*  start of s_dtform  */

{
   sint   ulenf  = 0 ;            /* Used length of format             */
   sint   posf   = 0 ;            /* Current position in format        */
   sint   ulenf1 = 0 ;            /* Used length of str_vec[] string */
   sint   posf1  = 0 ;            /* Current position in str_vec str   */
   byte   chr    = 0 ;            /* Character extracted from 'ffrmat' */
   byte   pad    = 0 ;            /* Character with which to pad       */
   byte   dir    = 0 ;            /* Direction to justify              */
   tint   fieldw = 0 ;            /* Individual field width            */
   sint   numlen = 0 ;            /* Length of field width number      */
   sint   rc     = 0 ;            /* Return code from string functions */
   sint   mainrc = 0 ;            /* Return code for this function     */
   sint   timezn = 0 ;            /* Time zone in sint for comparison  */
   sint   numout = 0 ;            /* Number from the datetime routine  */
   string isvstr    = S_NULL ;    /* s_gtdtst returns an ISV pointer   */
   string tmpin     = S_NULL ;    /* Contains the ffrmatters output    */
   string padstr    = S_NULL ;    /* Contains the pad character        */
   string deli_str  = S_NULL;     /* Working version of deli_str       */
   string spill_str = S_NULL;     /* Working version of spill_str      */
   string template  = S_NULL;     /* Working version of template       */
   string ourform   = S_NULL;     /* Our version of the format         */
   string *str_vec  = S_NULL;     /* An array of strings               */
   byte   def_dir   = '?'   ;     /* Default padding direction         */

   datetime  *tempdt= NIL  ;
   datetime  diffdt        ;      /* (Will be nulled out later         */

   /*+----------------------------------------------------------------+*/
   /*|                                                                |*/
   /*+----------------------------------------------------------------+*/

   rc =s_nulout(tstr) ;       /*  nullout the target string            */

    /*+---------------------------------------------------------------+*/
    /*|  If format is S_NULL, get TM structures.  If its also NUL,    |*/
    /*|  then we pick a default.                                      |*/
    /*+---------------------------------------------------------------+*/
   ulenf = s_len (ffrmat) ;       /* get used length of format string  */
   if ( ulenf == 0 )
   {
      if ( TM_master != NIL )
      {
         if ( TM_master->ucb != NIL )
         {
            ulenf = s_len(TM_master -> ucb -> dtform);
         }
      }
      if ( ulenf == 0 )                      /* Gotta user our own     */
      {
         s_init(&ourform,"@#d @$3m @#2y @#2i:@#2n:@#2s");
      }
      else
      {
         s_copy(&ourform,TM_master->ucb->dtform,0,0);
      }
   }
   else
   {
      s_copy(&ourform,ffrmat,0,0);
   }

    /*+---------------------------------------------------------------+*/
    /*| Make temporary copy of given DateTime or find current DateTime|*/
    /*| if not supplied.                                              |*/
    /*+---------------------------------------------------------------+*/
    tempdt = (datetime*) os_mema(sizeof(datetime));
    if ( dt == NIL )
    {
      os_dttm(tempdt);
    }
    else
    {
      os_memc(  dt,  tempdt, sizeof ( datetime )  );
    }

    /*+---------------------------------------------------------------+*/
    /*|   correction depending upon time zone                         |*/
    /*+---------------------------------------------------------------+*/

    if ( TM_master != NIL )
    {
       if ( TM_master->ucb != NIL )
       {
          timezn =  TM_master -> ucb -> time_zone ;
       }
       else
       {
          timezn = 0;
       }
    }
    else
    {
       timezn = 0;
    }

    os_memn(&diffdt, sizeof(datetime));

    if ( timezn < 0 )
    {
        diffdt.hour = (byte) ( 0 - timezn);
        rc = os_dtsub (&tempdt, &diffdt );
    }
    else if ( timezn > 0 )
    {
        diffdt.hour = (byte) timezn;
        rc = os_dtadd (&tempdt, &diffdt );
    }

   /*   Now tempdt = tempdt + diffdt         */

   /*+----------------------------------------------------------------+*/
   /*|             INITIALIZE STRING VECTOR                           |*/
   /*+----------------------------------------------------------------+*/
   s_valloc(&str_vec, STR_VLEN); /* allocates memory for string vector */

    rc = s_init ( &template,"@~..@n@@.");

    /* This template is equivalent to "@1@@@2@@@3@@@4..."             */

    s_parse(&ourform, template,  deli_str,   str_vec,  &spill_str);

    /* Here we don't check rc from s_parse because return codes from  */
    /* s_parse indicate stage of parsing.                             */

    ulenf = s_len ((string) str_vec); /* get used length of str vector */

    /*+---------------------------------------------------------------+*/
    /*| For each string of string vector (returned by s_parse () )    |*/
    /*| we check used length  and convert into resulting string  by   |*/
    /*| checking each character.                                      |*/
    /*+---------------------------------------------------------------+*/
    for ( posf = 1 ; posf <= ulenf ; posf++ )
    {

     /* check for null string returned by s_parse()                    */
     if ( (str_vec[posf]) != S_NULL )
     {
       ulenf1 = s_len ( (string) str_vec[posf] );
       posf1 = 0 ;
       chr = s_char_s ( str_vec[posf],posf1); /* get first character */
       if ( chr == '#' || chr == '$' )
       {
             /*+------------------------------------------------------+*/
             /*|   checks first character of the string               |*/
             /*|    if '#', resulting string will be numeric          |*/
             /*|    if '$', resulting string will be string           |*/
             /*+------------------------------------------------------+*/
             numlen   =  0 ;
             fieldw   = 0  ;

             if ( chr == '$')  /* decides  pad to be used              */
             {
                 pad = ' ';            /* Pad strings with blanks      */
                 def_dir = 'R';        /* Pad strings on right side    */
             }
             else
             {
                 pad = '0';            /* Pad numerics with zeros      */
                 def_dir = 'L';        /* Pad numerics on left side    */
             }

             /*+------------------------------------------------------+*/
             /*|   JUSTIFY ( optional ) left , right , center         |*/
             /*+------------------------------------------------------+*/
             chr = OS_TOUPPER (s_char_s(str_vec[posf],posf1+1));
             if ( chr == 'L')
             {
                       dir = 'R';
                       posf1++;
             }
             else if ( chr == 'R')
             {
                       dir = 'L';
                       posf1++;
             }
             else if ( chr == 'B')
             {
                       dir = 'C';
                       posf1++;
             }
             else
                       dir = '0';/* case when justify is not requested */


             while(s_char_s (str_vec[posf], numlen + posf1 + 1) >= '0'
              &&  s_char_s (str_vec[ posf], numlen + posf1 + 1) <= '9')
             /* get number of characters to be converted into a number */
                       numlen++;

             if (numlen != 0)
             {
               /* convert numeric string into an integer               */
               rc=s_stoti (&fieldw, str_vec[posf], posf1 + 2, numlen );
             }

             chr=OS_TOUPPER(s_char_s(str_vec [posf],posf1+1+numlen));

             posf1 = posf1 + numlen + 2 ;


             if (pad == '0')
             {
               /*+----------------------------------------------------+*/
               /*|    Get the numeric version of format 'chr'         |*/
               /*+----------------------------------------------------+*/
                rc = s_gtdtnm ( &tmpin , tempdt , chr );
             }
             else
             {
                /*+---------------------------------------------------+*/
                /*|   Get the string version of format 'chr'          |*/
                /*+---------------------------------------------------+*/
                rc = s_gtdtst ( &isvstr , tempdt , chr , 0 );

                /* copy str returned by s_gtdtst into anthor string  */
                rc = s_copy(&tmpin,isvstr,0,0);
                isvstr = S_NUL ;  /* Protect the ISV table             */
             }

             if (fieldw == 0)/* when field width not specified    */
             {
                fieldw = s_len (tmpin);
             }
             else if (dir != '0' )
             /*  when  direction is specified in the format            */
             {
                s_pad(&tmpin , tmpin, fieldw,pad, dir);
             }
             else if (s_len (tmpin) < fieldw )
             {
             /* when direction not specified and fielwwidth is greater */
             /* than length returned by s_gtdtnm or s_gtdtst           */

                s_pad(&tmpin , tmpin, fieldw, pad, def_dir);
             }
             rc = s_cat ( tstr,*tstr,tmpin ,1,fieldw) ;
       }
       /*+------------------------------------------------------------+*/
       /*|   If we get here it is because either :                    |*/
       /*|   1: There is remaining string .                           |*/
       /*|   OR                                                       |*/
       /*|   2: First character in the string is not '#' or  '$'.     |*/
       /*|   cat the string to the resulting string                   |*/
       /*+------------------------------------------------------------+*/
       if ( posf1 < ulenf1)
       {
             rc = s_cat ( tstr, *tstr, str_vec[posf] , posf1+1, 0);
       }
     }
     mainrc = 0;
   }
    rc = s_drop ( &padstr) ;
    rc = s_drop ( &tmpin) ;
    rc = s_drop ( &template) ;
    rc = s_drop ( &ourform ) ;
    rc = s_vdrop ( &str_vec ) ;
    os_memd ( tempdt );

   return(mainrc);
    }
