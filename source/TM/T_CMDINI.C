/*+--------------------------------------------------------------------+
  |      <DOC>                                                         |
  |      <NAME>         t_cmdini.c                                     |
  |      <AUTHOR>       Anoop K. Srivastava                            |
  |      <DATE>         March 14 1988                                  |
  |      <LEVEL>        TM                                             |
  |      <GLOBAL>                                                      |
  |      <PARAM>        ptr2 - pointer to the root of the tree.        |
  |                                                                    |
  |      <PURPOSE>      To store all the legal command 'words' in a    |
  |                     balanced tree structure using tm_tree. The     |
  |                     recore for each key (or command word) has three|
  |                     fields viz.,                                   |
  |                               object type, object code and a pointer
  |                     to the function to be called.                  |
  |                     The object type field decides what type of     |
  |                     command it is i.e. single char- '-', '--', '+' |
  |                     '++' or '?' (type 1), action (type 2), object  |
  |                     (type 3), modifier (type 4), or single word    |
  |                     command (type 5).                              |
  |                     The object code is the code defined for the    |
  |                     command words in the obj_comm.h file           |
  |                     in case of single char commands the functions  |
  |                     to process the commands are called directly    |
  |                     using the pointers to the required functions   |
  |                     In all other cases the command processing is   |
  |                     done through t_jmpst(). So in these cases we   |
  |                     do not use the pointers to the function field  |
  |                     leaving it NIL.                                |
  |                                                                    |
  |      <OUTPUT>       sint                                           |
  |      <CALLS>        mem_add(), meminfo(), obj_grec(), mem_item().  |
  |      <ERRORS>       none.                                          |
  |      <LIMITS>       none.                                          |
  |      <MODS>                                                        |
  |              14_Jul_88  JF    Added mem_item() call.               |
  |              01_may_89  JF    Changed parse logic to put everything|
  |                               on one line, use s_parse(), and allow|
  |                               comments in the file.                |
  |      </DOC>                                                        |
  +--------------------------------------------------------------------*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "cmd.h"
#include "exc.h"

#define NODE_KEY_LENGTH     16




sint    t_cmdini(  tree_origin  )

struct  memnode  **tree_origin;


 {


   hword        command_type = 0,
                function_id  = 0;

   sint           rc   = 0;

   OCB          *ocb   = NIL;
   OCD          *ocd   = NIL;

   string       file          = S_NULL,
              buffer          = S_NULL,
              parse_delimiter = S_NULL,
              parse_template  = S_NULL,
              spill_str       = S_NULL,
            * str_vector      = S_NULL,
              result          = S_NULL;

   struct    memitem  * tree_root       = NIL;
   struct    memnode  * tree_branch     = NIL;
   struct    cmd      * command_entry   = NIL;









   s_init( & file,  "* * TEIES CMDDATA" );

   ocb = obj_opn( (OID *) file, ( OS_FILE | OBJ_READ ) );

   if( OCB_POINTER_OK( ocb ) )
   {

      tree_root = mem_item();
      ocd_new(  & ocd,             NIL  );
      s_valloc( & str_vector,      4    );
      s_init(   & parse_template,  "@1 @2 @3 @4" );
      s_init(   & parse_delimiter, ";*" );

      while(  obj_grec ( ocb, ocd, & buffer )   )
      {

         s_parse( & buffer, parse_template, parse_delimiter, str_vector,
                                                           & spill_str );
         if( s_len( (string) str_vector ) == 3 )
         {

            os_memn( &(tree_root->key), NODE_KEY_LENGTH );
            os_memc( s_buffer( str_vector[ 1 ] ),
                     & (tree_root->key),
                     s_len( str_vector[ 1 ] )            );


            command_entry  = (struct cmd *) os_mema( sizeof(struct cmd));
            os_memn( command_entry, sizeof( struct cmd ) );

            s_stoh( & command_type, str_vector[ 2 ], 0, 0 );
            command_entry->typeid[ 0 ]  = command_type;

            s_stoh( & function_id, str_vector[ 3 ], 0, 0 );
            command_entry->objcode[ 0 ] = function_id;

            tree_root->contents = (byte *) command_entry ;
            tree_branch = mem_add( tree_root, tree_branch );


         }
      }
      os_memd( tree_root );

      * tree_origin = tree_branch;

      obj_cls(   ocb             );
      ocd_drp( & ocd             );
      s_vdrop( & str_vector      );
      s_drop(  & parse_template  );
      s_drop(  & parse_delimiter );
      s_drop(  & spill_str       );
      s_drop(  & buffer          );

   }
   else
   {

      rc = TM_CANNOT_OPEN_COMMAND_DATA_FILE;
      except( rc, CONTROL_ERROR, file, S_NULL, S_NULL, S_NULL, S_NULL );

   }
   s_drop( & file );


   return(rc);
}

