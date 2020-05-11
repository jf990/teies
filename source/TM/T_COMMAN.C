/*+--------------------------------------------------------------------+
  |      <DOC>                                                         |
  |      <NAME>         t_command()                                    |
  |      <AUTHOR>       Anoop K. Srivastava                            |
  |      <DATE>         march 10, 1988                                 |
  |      <LEVEL>        TM                                             |
  |      <GLOBAL>       none.                                          |
  |                                                                    |
  |      <PARAM>        cmdline  string  INPUT                         |
  |      <PARAM>        cmd_root node *  INPUT                         |
  |      <PARAM>        level    hword   INPUT                         |
  |                                                                    |
  |      <PURPOSE>      To process the commandline typed in by the     |
  |                     user. This routine checks for single/double    |
  |                     char commands (e.g. +, -, ++ etc.), single     |
  |                     word commands and commands of the format       |
  |                     (action modifier object parameter). In the     |
  |                     last case, the action modifier and object      |
  |                     may be typed-in in any order but parameters    |
  |                     MUST be typed in only after action object and  |
  |                     modifier. If same word has two types (e.g. say |
  |                     action and object then the order of priority   |
  |                     is action, object modifier, with action having |
  |                     the highest priority.                          |
  |                                                                    |
  |      <OUTPUT>       sint                 result code               |
  |                                                                    |
  |      <CALLS>                  s_parse(),mem_fnd(), t_jmpst()       |
  |                     mem_item(), s_valloc(), s_vdrop()              |
  |      <ERRORS>                                                      |
  |      <LIMITS>       does not handle commands of type #, #-# etc.   |
  |      <MODS>                                                        |
  |      Feb 8, 1989    JF      Added code to determine if a ?word,    |
  |                             ??word or ???message was entered.  In  |
  |                             this case the entry is not searched    |
  |                             in the command table.                  |
  |      Sep 2, 1989    JF      Added result code for numeric command  |
  |                             line input.                            |
  |      </DOC>                                                        |
  +--------------------------------------------------------------------*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "tm_cmd.h"
#include "tm.h"
#include "debug.h"



sint t_command (  cmdline, cmd_root, level  )


string            cmdline;
struct memnode   *cmd_root;
hword             level;

{


      string     target      = S_NULL,
                 original_ln = S_NULL,
                 template    = S_NULL,
                 deli_str    = S_NULL,
                 spill_str   = S_NULL,
                 os_command  = S_NULL,
                 blank       = S_NULL,
                 result      = S_NULL,
                 *param      = S_NULL,
                 *str_vec    = S_NULL,
                  holder     = S_NULL,
                 parameters  = S_NULL;
      byte       *temp       = NIL;
      hword      count1      = 0,
                 rc          = 0,
                 number      = 0,
                 leave_value = 0,
                 obj[5]       ;
      sint       j           = 0,
                 i           = 0,
                 flag        = 0,
                 junk        = 0,
                 length_of_key=0,
                 word_count  = 0,
                 count       = 0;
      boolean    error       = 0 ;
      OCB       *ocb         = NIL;

      struct     memitem * tmid = NIL;
      COMMAND  * new         = NIL;







if( s_len( cmdline ) < 1 )
{

   leave_value = 0;

}
else
{

s_copy( & original_ln, cmdline, 0, 0 );

                             /*************************************/
holder      = cmdline    ;   /* Small Tweek:  Make sure we save a */
cmdline     = original_ln;   /* copy of the original command line */
original_ln = holder     ;   /* and only muck with a local copy.  */
holder      = S_NULL     ;   /*************************************/

if ( s_char_s( cmdline, 0 ) == '?' )
{

   t_help( cmdline );
   rc = HELP_WAS_REQUESTED;

}
else
{

   s_init ( & template, " @~.. @n." );
   s_init ( & deli_str, ";"         );
   s_init ( & blank,    " "         );
   s_upper(   cmdline,  0, 0        );

   s_valloc( & str_vec, 20 );
   s_valloc( & param,   20 );
   os_memn(   obj, 5 * sizeof( hword ) );
   tmid = mem_item();
   tmid->contents = (byte *) os_mema( sizeof(  COMMAND  ) );

   while ( s_len( cmdline ) && ! leave_value )
   {

      leave_value = 0;

      s_parse ( & cmdline, template, deli_str, str_vec, & spill_str );


      word_count = s_len( (string) str_vec );

      if( word_count == 1 )
      {
            /*+--------------------------------------------------------+
              | First check if a single word entered is a number, if so|
              | return it as a reactive choice.  Otherwise decipher the|
              | command input.                                         |
              +--------------------------------------------------------+*/

         s_stoh( & number, str_vec[ 1 ], 0, 0 );


         if( number > 0 && number < 99 )
         {

            leave_value = number + TM_REACTIVE_MENU_SELECTION ;
            break;

         }
      }
      os_memn( &(tmid->key), MEM_MAXIMUM_KEY_LENGTH );

      if( s_len( str_vec[ 1 ] ) > MEM_MAXIMUM_KEY_LENGTH )
      {

            length_of_key = MEM_MAXIMUM_KEY_LENGTH;

      }
      else
      {

         length_of_key = s_len( str_vec[ 1 ] );

      }
      os_memc( s_buffer(str_vec[1]), &(tmid->key), length_of_key );

      rc = mem_fnd(tmid, cmd_root, 0);
      if (rc == 0 )
      {

         new  = ( COMMAND * ) tmid->contents;

         if (new->typeid[0] == SINGLE_WORD_COMMAND )
         {


            switch(  new->objcode[ 0 ]  )
            {

               case COMMAND_QUIT :

                  rc = t_s_off( 1 );
                  if( rc != SIGNOFF_ABANDON_REQUESTED )
                  {

                     t_shutdn(    );
   exit(0);

                  }
                  break;

               case COMMAND_CANCEL  :

                  leave_value = USER_REQUESTED_QUIT ;
                  break;

               case COMMAND_PROCESS :

                  leave_value = USER_REQUESTED_PROCESS ;
                  break;

               case COMMAND_HOMEBASE:

                  leave_value = USER_REQUESTED_ABANDON ;
                  break;

               default              :

                  leave_value = 0;
                  break;

            }
         }
         else if (new->typeid[0] == SYSTEM_COMMAND )
         {

            i = 2;
            while (  s_len (str_vec[i] )  )
            {

               s_cat( & os_command, os_command, str_vec[i], 0, 0);
               s_cat( & os_command, os_command, blank,        0, 0);
               i++;

            }
            rc = os_call( os_command          );
            s_drop( & os_command );

         }
         else
         {

            obj[ new->typeid[0] ] = new->objcode[0]   ;
            j = 0;
            while ( new->typeid[j])
            {

               if (new->objcode[j] == COMMAND_QUALIFIER )
               {
                    flag = 1 ;
               }
               else
               {
                    flag = 0 ;
               }
               j ++ ;

            }
            count  = 2;
            count1 = 1;

            while(  s_len( str_vec[ count ] )  )
            {

               os_memn( &(tmid->key), MEM_MAXIMUM_KEY_LENGTH );
               if( s_len( str_vec[ count ] ) > MEM_MAXIMUM_KEY_LENGTH )
               {

                  length_of_key = MEM_MAXIMUM_KEY_LENGTH;

               }
               else
               {

                  length_of_key = s_len( str_vec[ count ] );

               }
               os_memc( s_buffer( str_vec[count]), &(tmid->key),
                                                         length_of_key );

               tmid->contents = (byte * ) os_mema( sizeof( COMMAND ));
               rc = mem_fnd(tmid, cmd_root, 0 );
               if (  ( rc == 0 ) && ( count <= 3 )  )
               {

                  new = ( COMMAND * ) tmid->contents;

                  j = 0;
                  while (  obj[ new->typeid[ j ] ] > 0  )
                  {

                     j++;

                  }
                  if ( new->typeid[j] )
                  {
                     obj[new->typeid[j]] = new->objcode[j];
                  }
                  else
                  {

                     except( Word_Not_Defined, USER_ERROR, str_vec[count],
                                        S_NULL, S_NULL, S_NULL, S_NULL );
                     leave_value = Word_Not_Defined ;
                     break;

                  }
               }
               else if ( count >= 3)
               {

                  while ( s_len ( str_vec[ count ] ) )
                  {

                     s_copy( &(param[count1]), str_vec[count],
                                                                  0, 0 );
                     s_cat( & parameters, parameters, str_vec[ count ],
                                                                  0, 0 );
                     s_smp_ul( (string) &(param[0]) ) = (hword) count1;
                     count1 ++ ;
                     count  ++ ;

                  }

                  }
                  else if ( flag )
                  {

                     j = 1;
                     while (s_len(str_vec[j] ))
                     {

                        s_copy( &param[j], str_vec[j], 0, 0);
                        j ++ ;

                     }
                     for (  j=0; j<=5; j++)
                     {

                        obj[j] = 0;

                     }
                  }
                  else
                  {

                     error = TRUE;
                     leave_value = Noun_Not_Defined;
                     except ( Noun_Not_Defined, USER_ERROR,
                                                        str_vec[count],
                                        S_NULL, S_NULL, S_NULL, S_NULL );
                     break;

                  }
                  count += 1;

               }
               if ( ! error )
               {


                  leave_value = t_jmpst( obj[3], obj[2], obj[4],
                                             parameters, ocb, & result );

               }
            }
         }
         else
         {

            leave_value =  Verb_Not_Defined ;
            except (  Verb_Not_Defined, USER_ERROR, str_vec[1],
                                        S_NULL, S_NULL, S_NULL, S_NULL );

         }
      }


      s_vdrop( & str_vec    );
      s_vdrop( & param      );
      s_drop ( & template   );
      s_drop ( & deli_str   );
      s_drop ( & blank      );
      s_drop ( & parameters );
      s_drop ( & spill_str  );

   }
   s_drop ( & cmdline  );

}

   return(leave_value);
}
