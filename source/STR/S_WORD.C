/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    s_word()                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 3, 1990                                        |
  |  <LEVEL>   STR                                                    |
  |  <PARAM>   source      string     INPUT  Source word to be checked|
  |  <PARAM>   template    void   *   INPUT  Single string of words or|
  |                                          vector of words to be    |
  |                                          matched against source.  |
  |  <PARAM>   template_type byte     INPUT  Used to identify the temp|
  |                                          late as a single string  |
  |                                          or a vector.             |
  |  <PARAM>   begin        sint      INPUT  Position in source to    |
  |                                          start search.            |
  |  <PARAM>   length       sint      INPUT  Amount of soruce after   |
  |                                          start to consider.       |
  |  <PARAM>   match_pos    sint *    OUTPUT Word # in template the   |
  |                                          match of source occured. |
  |  <PURPOSE> Operates similar to the REXX word() function.  THe     |
  |            source word is matched against a table of words in the |
  |            template.  If a match occurs, the word # in template   |
  |            is returned.  0 is returned for no match.  Return codes|
  |            < 0 are string level error codes.  Begin and Length are|
  |            standard string level offsets into the source to operate
  |            on only a substring of source.  The word number is re- |
  |            turned from the function as well as the last parameter |
  |            of the routine while points to a sint.  If it is a NIL |
  |            or -1 pointer it is not used.                          |
  |                                                                   |
  |            Special handling is done with the template identified by
  |            the byte template_type.  If it is a single string then |
  |            the word list is separated by blanks and word numbers  |
  |            are determined by the number of blanks.  If it is a    |
  |            string vector then each entry is a vector index (array)|
  |            and words may contain blanks in this case.             |
  |                                                                   |
  |            If source is a S_NULL string 0 is returned (no match)  |
  |            regardless of the template contents (even S_NULL).     |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"




sint s_word( source, template, template_type, begin, length, match_pos )

string       source;
void       * template;
byte         template_type;
sint         begin, length, * match_pos;



{

   sint        my_rc           =      0;
   string      word_template   = S_NULL,
             * vector_template =    NIL;

string input_remainder = S_NULL;
string parse_template = S_NULL;
hword word_count = 0;
string parse_delim = S_NULL;
string spill_string = S_NULL;
string try_this_string = S_NULL;
string *parse_vector = S_NULL;
sint s_comp_junk;
boolean done = FALSE;
string string2find = S_NULL;


#D_begin(  s_word, STR  );
#D_str(   'source', 'what to look-up in template.' );
#D_show(  'Template Type %d; Beg=%d; Len=%d; Store Result at x%x.', '\
                              template_type, begin, length, match_pos' );

switch( template_type ){

   case                         0 :
   case S_TEMPLATE_IS_WORD_STRING :{

      #D_dump('s_buffer(source)','s_len(source)','string to find')
      word_template = (string) template;
      #D_str( 'word_template', 'word string to check against.' );
      s_copy( &input_remainder, template, 0, 0);
      s_init( &parse_template, "@1");
      s_valloc( &parse_vector, 2);
      s_copy( &string2find, source, begin, length);

      do {
         s_parse( &input_remainder, parse_template, parse_delim
         , parse_vector, &spill_string);

         s_copy( &try_this_string, parse_vector (: 1 :), 0, 0);
         #D_dump('s_buffer( try_this_string)','s_len(try_this_string)'\
         ,'string to check')
         word_count++;

         if (s_comp(try_this_string, string2find, 0, 0, 'M'
         , &s_comp_junk) == 0){
            done = TRUE;
            my_rc = word_count;
            #D_msgs('found string on word %d','word_count')
            }

         s_copy( &input_remainder, spill_string, 0, 0);
         #D_dump('s_buffer(input_remainder)','s_len(input_remainder)'\
         ,'what is left of string')
         } while ((done == FALSE) && (s_len( input_remainder) > 0));

      s_drop( &input_remainder);
      s_drop( &parse_template);
      s_drop( &parse_delim);
      s_drop( &spill_string);
      s_drop( &try_this_string);
      s_drop( &string2find);

      s_vdrop( &parse_vector);
      break;}

   case S_TEMPLATE_IS_VECTOR      :

      vector_template = (string *) template;
      #D_strvec( 'vector_template', 'check against multiple strings' );
      #D_msgeds('vector template s_word is not supported yet',1)
      break;

   default                        :

      #D_msgeds('unknown template type %d','template_type')
      my_rc = S_BAD_PARAM ;
      break;

}

#D_retn('my_rc','%d',0,'my_rc==0','word not found','word found')


}
