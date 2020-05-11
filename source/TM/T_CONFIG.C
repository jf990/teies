/*--------------------------------------------------------------------*
 |  <DOC>                                                             |
 |  <NAME>    t_config.c                                              |
 |  <AUTHOR>  John L. Foster / Patrick Taylor / Steve Muccione        |
 |  <DATE>    April 21, 1988                                          |
 |  <LEVEL>   TM                                                      |
 |  <GLOBAL>  TM_master                                               |
 |                                                                    |
 |  <PARAM>   filename    String   INPUT  This is the TEIES generic   |
 |                                        file descriptor of the file |
 |                                        to open for the configura-  |
 |                                        tion.                       |
 |                                                                    |
 |  <PURPOSE> Read the TEIES configuration file and set parameters    |
 |            as requested.  See specification document pg. 194 and   |
 |            example 105.                                            |
 |                                                                    |
 | format of lines in configuration file:                             |
 |                                                                    |
 |   KEYWORD <value if required,> <QKEY = value, QKEY = value, ... >  |
 |                                                                    |
 | keywords & qualifiers currently supported:               sys_init  |
 |                                                             field  |
 | -----------------------------------------                ----------|
 |   DATABASE "file"   {Up to 16 entries allowed}           lfd_name  |
 |     ACCESS  = xx     xx is in { MR, ER, MW, EW, RW }     access    |
 |     BLOCKS  = n      MIN_BLK_SIZE <= n <= MAX_BLK_SIZE,  blk_size  |
 |                      n is a multiple of MIN_BLK_SIZE               |
 |     BUFFERS = n      MIN_BUF <= n <= MAX_BUF             buffers   |
 |     HASH    = n      MIN_HASH <= n <= MAX_HASH           hash_size |
 |     SIZE    = n      MIN_SIZE <= n <= MAX_SIZE           # of blks |
 |   DATE "format"                                          date_fmt  |
 |     TIMEZONE = n     Timezone from GMT from -12 to +12.  time_zone |
 |                        (Default is 0)                              |
 |   EDITOR name        name is a single unquoted word      editor    |
 |     PROFILE=name     name of a profile to use with the editor.     |
 |   ERRORLOG "file"                                        error     |
 |   LANGUAGE "keyword"                                     ISV_name  |
 |   MEMORY                                                           |
 |     LOC  = n         n is a hex address                  mem_start |
 |     SIZE = n         n is a multiple of MIN_MEM_SIZE     mem_size  |
 |   PRINTER name       name is a single unquoted word      printer   |
 |     CLASS = x        x is a single character             prt_class |
 |   MASTERSERVER "network_id"  or *                        MS        |
 |                                                                    |
 |  <OUTPUT>  sint                                                    |
 |                                                                    |
 |  <CALLS>   fgets(), fopen(), os_mema(), os_memd(), os_qfd(),       |
 |            s_comp(), s_copy(), s_drop(), s_init(), s_just(),       |
 |            s_len(), s_parse(), s_put(), s_smp_al(), s_smp_ul(),    |
 |            s_stob(), s_stosi(), s_sxtos(), s_upper(), strlen(),    |
 |            fclose().                                               |
 |  <LIMITS>  All errors are handled by this routine. If there is an  |
 |            erroneous or ambiguous setting in the configuration     |
 |            source, the default value (the one hardcoded) is taken. |
 |                                                                    |
 |            This routine does not use the TEIES DMS. Instead it     |
 |            uses the CRTL for direct file manipulation since        |
 |            the TEIES DMS has not yet been started.                 |
 |                                                                    |
 |  <MODS>                                                            |
 |            21 Apr 88   Patrick D. Taylor  s_parse enhanced version.|
 |            30 Jun 88   Steve Muccione     Improved s_parse version.|
 |            30 Jul 88   JF                 Added TIMEZONE to DATE   |
 |            16 Jan 89   JF                 Added fclose()           |
 |            31 Jul 89   JF                 Added editor profile.    |
 |  </DOC>                                                            |
 *--------------------------------------------------------------------*/
#include "stdio.h"
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"


extern int   strlen();



#define TABLE_LEN       7
#define ACC_TABLE_LEN   5
#define MAX_KEY_LEN     8
#define MAX_QKEY_LEN    8
#define MIN_BLK_SIZE    512
#define MAX_BLK_SIZE    32768
#define MIN_MEM_SIZE    512
#define MIN_MEM_BLOCK   2
#define MIN_BUF         10
#define MAX_BUF         200
#define MIN_HASH        5
#define MAX_HASH        100
#define MIN_SIZE        OBJ_MINIMUM_LFD_SIZE
#define MAX_SIZE        OBJ_MAXIMUM_LFD_SIZE
#define MIN_TZ         -12
#define MAX_TZ          12
#define LINE_LENGTH     85

#define DATABASE       1
#define   DAT_ACC      101
#define   DAT_BUF      102
#define   DAT_HSH      103
#define   DAT_SIZ      104
#define   DAT_BLK      105
#define   DAT_NAM      106
#define DATE           2
#define   DTE_FMT      201
#define   DTE_TMZ      202
#define EDITOR         3
#define   EDT_NAM      301
#define   EDT_PROF     302
#define ERRORLOG       4
#define   ERR_NAM      401
#define LANGUAGE       5
#define   ISV_NAM      501
#define MEMORY         6
#define   MEM_LOC      601
#define   MEM_SIZ      602
#define PRINTER        7
#define   PRI_NAM      701
#define   PRI_CLA      702
#define MASTERSERVER   8
#define   MS_NAME      801

#define TAB_ENTRIES    8


struct table_def
                 {

                    sint     kw      ;
                    string   name    ;
                    string * qual    ;
                    sint   * qual_num;

                 } ;

typedef struct table_def ENTRY;




sint   t_config( filename )

string           filename;

{


   sint     val          = 0,
            rc           = 0,
            i1           = 0,
            i2           = 0,
            i3           = 0,
            i4           = 0,
            keyword      = 0,
            lfd_entry    = 0,
            qualifier    = 0;

   ENTRY  * table        = NIL,
          * table_top    = NIL;

   string   template     = S_NULL,
            delim        = S_NULL,
            spill        = S_NULL,
            target       = S_NULL,
            garbage      = S_NULL,
          * str_vec      =    NIL;

   boolean  end_of_file  = FALSE;
   FILE   * file_pointer =   NIL;
   byte   * line         =   NIL;








/*------------------------------------------------------------------*
 | Now initialize the sys_init struct to default values in case the |
 | file is not found or some of the cards are not in the found file |
 *------------------------------------------------------------------*/

TM_master->sys_init->lfd_name              = S_NULL;
TM_master->sys_init->access                = S_NULL;
TM_master->sys_init->_lfd_name             =    NIL;
TM_master->sys_init->_lfd_access_mode      =    NIL;
TM_master->sys_init->_lfd_block_size       =    NIL;
TM_master->sys_init->_lfd_number_of_blocks =    NIL;
TM_master->sys_init->ISV_name       = S_NULL;
TM_master->sys_init->error          = S_NULL;
TM_master->sys_init->editor         = S_NULL;
TM_master->sys_init->editor_profile = S_NULL;
TM_master->sys_init->printer        = S_NULL;
TM_master->sys_init->MS             = S_NULL;
TM_master->sys_init->datefmt        = S_NULL;

s_init( & ( TM_master->sys_init->lfd_name      ), "A * TEIES $DB"      );
s_init( & ( TM_master->sys_init->access        ), "RW"                 );

s_valloc( & ( TM_master->sys_init->_lfd_name ),       TM_MAX_NUMBER_OF_LFDS );
s_init(   & ( TM_master->sys_init->_lfd_name[ 1 ] ), "A * TEIES $DB" );
s_smp_ul( (string) TM_master->sys_init->_lfd_name )       = 1;
s_valloc( & ( TM_master->sys_init->_lfd_access_mode), TM_MAX_NUMBER_OF_LFDS );
s_init(   & ( TM_master->sys_init->_lfd_access_mode[ 1 ] ), "RW" );
s_smp_ul( (string) TM_master->sys_init->_lfd_access_mode) = 1;
TM_master->sys_init->_lfd_block_size       =
            (sint *) os_mema( sizeof( sint )   * TM_MAX_NUMBER_OF_LFDS );
TM_master->sys_init->_lfd_number_of_blocks =
            (sint *) os_mema( sizeof( sint )   * TM_MAX_NUMBER_OF_LFDS );

s_init( & ( TM_master->sys_init->ISV_name      ), "* * ENGLISH ISV$OBJ");
s_init( & ( TM_master->sys_init->error         ), "* * TEIES ERRLOG"   );
s_init( & ( TM_master->sys_init->editor        ), "XEDIT"              );
s_init( & ( TM_master->sys_init->editor_profile), "TEIES_SP"           );
s_init( & ( TM_master->sys_init->printer       ), "00E"                );
s_init( & ( TM_master->sys_init->MS            ), "TEIESMS "           );
s_init( & ( TM_master->sys_init->datefmt       ), "@#m/@#d/@#y"        );

TM_master->sys_init->blk_size  =   4096;
TM_master->sys_init->buffers   =     50;
TM_master->sys_init->hash_size =     11;
TM_master->sys_init->db_size   =    250;
TM_master->sys_init->mem_start =    NIL;
TM_master->sys_init->mem_size  = 512000;
TM_master->sys_init->prt_class =    'A';
TM_master->sys_init->time_zone =     -5;

/*-------------------------------------------*
 | prepare configuration file for processing |
 *-------------------------------------------*/

if( filename != (string) -1 )
{

   string default_fd = S_NULL,
          local_file = S_NULL;

   s_init( & default_fd, "* * TEIES SYS" );
   rc  =  os_qfd( &local_file, filename, default_fd );
   s_put( local_file, 0, s_len( local_file )+1 );
   file_pointer = fopen( s_buffer( local_file ), "r"  );
   s_drop ( & default_fd );
   s_drop ( & local_file );
}

if( file_pointer != NIL )
{                               /**************************************/
                                /* Do only if we have a file to parse */
                                /**************************************/

   line      =  os_mema ( sizeof( byte ) * (LINE_LENGTH+1) );
   table     = (ENTRY *) os_mema ( sizeof (ENTRY) * (TAB_ENTRIES+1) );
   os_memn (table, sizeof (ENTRY) * (TAB_ENTRIES+1) );
   table_top = table;

   s_init     ( &(table->name) , "DATABASE");
     table    ->kw       = DATABASE;
                           s_valloc ( &(table->qual),  8);
     table    ->qual_num = (sint *) os_mema  ( sizeof (sint) * (8 + 1));
     s_init   ( &((table->qual)[1]) , "ACCESS"  );
        (table->qual_num)[1] = DAT_ACC;
     s_init   ( &((table->qual)[2]) , "BLOCKS"  );
        (table->qual_num)[2] = DAT_BLK;
     s_init   ( &((table->qual)[3]) , "BUFFERS" );
        (table->qual_num)[3] = DAT_BUF;
     s_init   ( &((table->qual)[4]) , "HASH"    );
        (table->qual_num)[4] = DAT_HSH;
     s_init   ( &((table->qual)[5]) , "SIZE"    );
        (table->qual_num)[5] = DAT_SIZ;
     s_init   ( &((table->qual)[6]) , "NAME"    );
        (table->qual_num)[6] = DAT_NAM;
     s_init   ( &((table->qual)[7]) , "BLOCK"  );
        (table->qual_num)[7] = DAT_BLK;
     s_smp_ul ( (string) (table->qual) ) = 7;
     table++;

   s_init     ( &(table->name) , "DATE");
     table    ->kw       = DATE;
                           s_valloc ( &(table->qual),  3);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (3 + 1));
     s_init   ( &((table->qual)[1]) , "FORMAT"  );
        (table->qual_num)[1] = DTE_FMT;
     s_init   ( &((table->qual)[2]) , "TIMEZONE");
        (table->qual_num)[2] = DTE_TMZ;
     s_smp_ul ( (string) (table->qual) ) = 2;
     table++;

   s_init     ( &(table->name) , "EDITOR");
     table    ->kw       = EDITOR;
                           s_valloc ( &(table->qual),  2);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (2 + 1));
     s_init   ( &((table->qual)[1]) , "NAME"  );
        (table->qual_num)[1] = EDT_NAM;
     s_init   ( &((table->qual)[1]) , "PROFILE" );
        (table->qual_num)[1] = EDT_PROF;
     s_smp_ul ( (string) (table->qual) ) = 2;
     table++;

   s_init     ( &(table->name) , "ERRORLOG");
     table    ->kw       = ERRORLOG;
                           s_valloc ( &(table->qual),  2);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (2 + 1));
     s_init   ( &((table->qual)[1]) , "NAME"  );
        (table->qual_num)[1] = ERR_NAM;
     s_smp_ul ( (string) (table->qual) ) = 1;
     table++;

   s_init     ( &(table->name) , "LANGUAGE");
     table    ->kw       = LANGUAGE;
                           s_valloc ( &(table->qual),  2);
     table    ->qual_num = (sint *) os_mema  ( sizeof (sint) * (2 + 1));
     s_init   ( &((table->qual)[1]) , "NAME"  );
        (table->qual_num)[1] = ISV_NAM;
     s_smp_ul ( (string) (table->qual) ) = 1;
     table++;

   s_init     ( &(table->name) , "MEMORY");
     table    ->kw       = MEMORY;
                           s_valloc ( &(table->qual),  4);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (4 + 1));
     s_init   ( &((table->qual)[1]) , "LOC"     );
        (table->qual_num)[1] = MEM_LOC;
     s_init   ( &((table->qual)[2]) , "LOCATION");
        (table->qual_num)[2] = MEM_LOC;
     s_init   ( &((table->qual)[3]) , "SIZE"    );
        (table->qual_num)[3] = MEM_SIZ;
     s_smp_ul ( (string) (table->qual) ) = 3;
     table++;

   s_init     ( &(table->name) , "PRINTER");
     table    ->kw       = PRINTER;
                           s_valloc ( &(table->qual),  3);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (3 + 1));
     s_init   ( &((table->qual)[1]) , "NAME"    );
        (table->qual_num)[1] = PRI_NAM;
     s_init   ( &((table->qual)[2]) , "CLASS"   );
        (table->qual_num)[2] = PRI_CLA;
     s_smp_ul ( (string) (table->qual) ) = 2;
     table++;

   s_init     ( &(table->name) , "MASTERSERVER" );
     table    ->kw       = MASTERSERVER;
                           s_valloc ( &(table->qual),  2);
     table    ->qual_num = (sint *)os_mema  ( sizeof (sint) * (2 + 1));
     s_init   ( &((table->qual)[1]) , "NAME"  );
        (table->qual_num)[1] = MS_NAME;
     s_smp_ul ( (string) (table->qual) ) = 1;


/*------------------------------------------------*
 | Parse Table is initialized, now parse the file |
 *------------------------------------------------*/


   table = table_top;
   s_init (  & template, "@n @~.. @\"n@?,=,@\"n, .");
   s_init (  & delim   , ";" );
   s_valloc (& (str_vec), 20 );

   while ( ! end_of_file )
   {

      fgets ( line, LINE_LENGTH, file_pointer );
      if ( strlen (line) != NIL )
      {
         s_init  ( & target, line             );
         s_smp_ul(   target  ) = s_len( target ) -1 ;   /* Get rid of the
                                                           CR at end of
                                                           the line.   */
         s_parse ( &target, template, delim, str_vec, &spill);

         if ( s_len ((string) str_vec) > 0 )
         {
            for ( i1=1; i1 <= s_len ((string) str_vec); i1++ )
            {
               s_upper (str_vec [i1], 0, 0);
            }
            for ( i1=0; i1 < TAB_ENTRIES; i1++)
            {

               if ( s_comp ( str_vec[1],(table+i1)->name,
                             0, s_len(str_vec[1]), 'M', &i4) == 0)
               {
                  keyword = (table+i1)->kw;
                  for (i2=2; i2 <= s_len ((string) str_vec); i2++)
                  {
                     qualifier=0;
                     for ( i3=1;
                           i3 <= s_len ((string)((table+i1)->qual));
                           i3++ )
                     {
                        if (s_comp (str_vec [i2],
                                    ((table+i1)->qual)[i3],0,0,'M',&i4)
                            == 0)
                        {
                           qualifier = ((table+i1)->qual_num)[i3];
                           break;
                        }
                     }
                     switch (keyword)
                     {

                        case DATABASE:

                           switch (qualifier)
                           {

                              case DAT_ACC:

                                 if( lfd_entry == 1 )
                                 {

                                    s_copy(&(TM_master->sys_init->access)
                                        ,str_vec[i2+1],0,0);

                                 }
                                 s_copy(
                   &(TM_master->sys_init->_lfd_access_mode[lfd_entry]),
                                         str_vec[i2+1],0,0);
                                 i2++;
                                 break;

                              case DAT_BLK:

                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      ((val % MIN_BLK_SIZE) == 0) &&
                                      (MIN_BLK_SIZE <= val) &&
                                      (val <= MAX_BLK_SIZE) )
                                 {
                                    if( lfd_entry == 1 )
                                    {

                                       TM_master->sys_init->blk_size=val;

                                    }
                                    TM_master->sys_init->_lfd_block_size[
                                              lfd_entry - 1 ] = val;
                                 }
                                 i2++;
                                 break;

                              case DAT_BUF:

                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      (MIN_BUF <= val) &&
                                      (val <= MAX_BUF) )
                                 {
                                    TM_master->sys_init->buffers=val;
                                 }
                                 i2++;
                                 break;

                              case DAT_HSH:

                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      (MIN_HASH <= val) &&
                                      (val <= MAX_HASH) )
                                 {
                                   TM_master->sys_init->hash_size=val;
                                 }
                                 i2++;
                                 break;

                              case DAT_SIZ:

                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      (MIN_SIZE <= val) &&
                                      (val <= MAX_SIZE) )
                                 {
                                    if( lfd_entry == 1 )
                                    {

                                       TM_master->sys_init->db_size=val;

                                    }
                                    TM_master->sys_init->_lfd_number_of_blocks
                                           [ lfd_entry - 1 ] = val;

                                 }
                                 i2++;
                                 break;

                              case DAT_NAM:

                                 i2++;

                              default:

                                 lfd_entry ++ ;
                                 if( lfd_entry > TM_MAX_NUMBER_OF_LFDS )
                                 {

                                    lfd_entry = TM_MAX_NUMBER_OF_LFDS;

                                 }
                                 if( lfd_entry == 1 )
                                 {

                                    s_copy (
                                      &(TM_master->sys_init->lfd_name),
                                      str_vec[i2], 0, 0);

                                 }
                                 s_copy (
                                &(TM_master->sys_init->_lfd_name[lfd_entry]),
                                                    str_vec[i2], 0, 0);
                                 if( lfd_entry != 1 )
                                 {

            s_smp_ul( (string) TM_master->sys_init->_lfd_name )      ++ ;
            s_smp_ul( (string) TM_master->sys_init->_lfd_access_mode)++ ;

                                 }
                                 break;
                           }
                           break;
                        case DATE:
                           switch (qualifier)
                           {
                              case DTE_TMZ:
                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      (MIN_TZ <= val) &&
                                      (val <= MAX_TZ) )
                                 {
                                   TM_master->sys_init->time_zone=val;
                                 }
                                 i2++;
                                 break;
                              case DTE_FMT:
                                 i2++;
                              default:
                                 s_copy (
                                   &(TM_master->sys_init->datefmt),
                                   str_vec[i2], 0, 0);
                                 break;
                           }
                           break;
                        case EDITOR:
                           switch (qualifier)
                           {
                              case EDT_PROF :
                                 s_copy (
                                   &(TM_master->sys_init->editor_profile),
                                   str_vec[i2 + 1], 0, 0);
                                 i2 ++ ;
                                 break;
                              case EDT_NAM:
                                 i2++;
                              default:
                                 s_copy (
                                   &(TM_master->sys_init->editor),
                                   str_vec[i2], 0, 0);
                                 break;
                           }
                           break;
                        case ERRORLOG:
                           switch (qualifier)
                           {
                              case ERR_NAM:
                                 i2++;
                              default:
                                 s_copy (
                                   &(TM_master->sys_init->error),
                                   str_vec[i2], 0, 0);
                                 break;
                           }
                           break;
                        case LANGUAGE:
                           switch (qualifier)
                           {
                              case ISV_NAM:
                                 i2++;
                              default:

                              {

                                 string  isv_gfd_pre  = S_NULL,
                                         isv_gfd_post = S_NULL;

                                 s_init( & isv_gfd_pre,  "* * "     );
                                 s_init( & isv_gfd_post, " ISV$OBJ" );

                                 s_cat( & isv_gfd_pre, isv_gfd_pre,
                                          str_vec[ i2 ], 0, 0     );
                                 s_cat( & isv_gfd_pre, isv_gfd_pre,
                                          isv_gfd_post, 0, 0        );
                                 s_copy (
                                    & (TM_master->sys_init->ISV_name),
                                          isv_gfd_pre,   0, 0
                                        );
                                 s_drop( & isv_gfd_pre  );
                                 s_drop( & isv_gfd_post );
                                 i2 ++ ;
                                 break;
                              }
                           }
                           break;
                        case MEMORY:
                           switch (qualifier)
                           {
                              case MEM_SIZ:
                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) &&
                                      ((val % MIN_MEM_BLOCK) == 0) &&
                                      (MIN_MEM_SIZE <= val) )
                                 {
                                    TM_master->sys_init->mem_size=val;
                                 }
                                 i2++;
                                 break;
                              case MEM_LOC:
                                 rc = s_stosi (&val,str_vec[i2+1],0,0);
                                 if ( (rc <= 0) )
                                 {

                                    TM_master->sys_init->mem_start =
                                              (byte *) val;

                                 }
                                 i2++;
                                 break;
                           }
                           break;
                        case PRINTER:
                           switch (qualifier)
                           {
                              case PRI_CLA:
                                 if (s_len(str_vec[i2+1]) == 1)
                                 {
                                    s_stob (
                                      &(TM_master->sys_init->prt_class),
                                      str_vec[i2], 1);
                                 }
                                 i2++;
                                 break;
                              case PRI_NAM:
                                 i2++;
                              default:
                                 s_copy (
                                   &(TM_master->sys_init->printer),
                                   str_vec[i2], 0, 0);
                                 break;
                           }
                           break;
                        case MASTERSERVER:
                           switch (qualifier)
                           {
                              case MS_NAME:
                                 i2++;
                              default:
                                 s_copy (
                                   &      (TM_master->sys_init->MS),
                                           str_vec[i2], 0, 0        ) ;
                                 break;
                           }
                           break;
                     }
                  }
                  break;
               }
            }
         }
      }
      else
      {
         end_of_file = TRUE;
      }
   }
   fclose( file_pointer );



   for ( i1=0; i1 < TAB_ENTRIES; i1++ )
   {
      s_vdrop ( & ( table->qual     ));
      os_memd (     table->qual_num  );
      s_drop  ( & ( table->name     ));
      table++;
   }
   os_memd (   table_top );
   s_vdrop ( & (str_vec) );
   s_drop (  & target    );
   s_drop (  & spill     );
   s_drop (  & garbage   );
   s_drop (  & delim     );
   s_drop (  & template  );
   os_memd (   line      );

}




   return(0);

}
