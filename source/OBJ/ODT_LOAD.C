/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_load                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    June  5, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   odt_type    hword    INPUT    Object type of the ODT   |
  |                                          that is to be loaded     |
  |                                                                   |
  |  :PURPOSE. To load ODT's from disk into memory                    |
  |                                                                   |
  |  :OUTPUT.  Returns the pointer to the ODT                         |
  |                0 if obj_type is invalid                           |
  |              - 1 if file not found                                |
  |                                                                   |
  |  :CALLS.   strcat (), open (), lseek (), read (), close (),       |
  |            os_mema ()                                             |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  Should use generic file descriptors but since the      |
  |            ODT's are later going to be stored as Objects, it      |
  |            was not necessary for this phase of implementation.    |
  |                                                                   |
  |            Shouldlook the object type up in the objecttable to    |
  |            get the correct object name to use as the generic file |
  |            name.                                                  |
  |                                                                   |
  |            If the ODT entry structure size changes then the LRECL |
  |            must be changed accordingly.                           |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "file.h"
#include "defs.h"
#include "str.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

#define I_O_ERROR  -1




ODT       *odt_load (  object_type  )

hword object_type ;



{

   odtrecord *odt = NIL;          /* Pointer to the ODT                */
   int  os_file_descriptor    = 0 ;
   sint rc      = 0 ,             /* intermediate function return code */
        size    = 0 ,             /* Size of file in bytes             */
        numread = 0 ;             /* Number of bytes read              */
   byte * fn    = NIL;            /* C-string containing filename      */
   string ending     = S_NULL,
          additional = S_NULL,
          filename   = S_NULL,
          result     = S_NULL,
          deflt      = S_NULL;










   filename = obj_name( object_type );

   s_init( &additional, " ( BIN LRECL 36 RECFM F" );
   s_init( &deflt,      "* * PSI ODT"             );

   s_init( &result,     "* * "                    );
   s_cat(  &result, result, filename, 0, 0        );
   s_init( &ending,     " ODT"                    );
   s_cat(  &result, result, ending, 0, 0          );

   rc = os_qfd( &result, result, deflt );
   if( rc == 0 )
   {

      s_cat( &result, result,   additional, 0, 0 );
      s_put( result,   '\0', s_len( result ) + 1 );



      os_file_descriptor = open ( s_buffer( result ), O_RDONLY) ;
      if ( os_file_descriptor != I_O_ERROR )
      {

         size = lseek ( os_file_descriptor, 0, 2 ) ;

         odt = ( ODT * ) os_mema (size) ;

         lseek ( os_file_descriptor, 0, 0 ) ;

         numread = read ( os_file_descriptor, (char *) odt, size ) ;

         close ( os_file_descriptor ) ;

         rc = odt_link( odt );


      }
      else
      {

         odt = (ODT *) -1;

      }
   }
   else
   {

      odt = NIL;

   }
   s_drop( & additional );
   s_drop( & deflt      );
   s_drop( & ending     );
   s_drop( & result     );


   return(odt);



}
