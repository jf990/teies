/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_save                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    June  4, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   *odt      odtrecord  INPUT    Pointer to the ODT that  |
  |                                          is to be save on disk    |
  |             name     string     INPUT    Filename of the file in  |
  |                                          which the ODT is to be   |
  |                                          stored                   |
  |                                                                   |
  |  :PURPOSE. To save a ODT on disk in a file call 'fname'           |
  |                                                                   |
  |  :OUTPUT.  Returns   0     No errors.                             |
  |                     -1     OPEN failed.                           |
  |                                                                   |
  |  :CALLS.   strcat (), open (), write (), close ()                 |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  If the ODT entry structure size changes then the LRECL |
  |            must be changed accordingly.                           |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "file.h"
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

#define I_O_ERROR -1




sint odt_save (odt, name)

odtrecord *odt        ;
string     name       ;


{
   sint  fpout    = 0 ;           /* File pointer                      */
   sint  numbytes = 0 ;           /* Number of bytes that ODT takes up */
   sint  rc       = 0 ;           /* Number of bytes written to disk   */
   string fname   = S_NULL,
          file_name=S_NULL,
          defalt  = S_NULL,
          extra   = S_NULL,
          newname = S_NULL,
          odt_ext = S_NULL;


   s_copy( &newname, name, 0, 0 );

   s_init( &fname, "A * "   );
   s_init( &defalt,"A * * ODT");
   s_init( &odt_ext, " ODT" );
   s_init( &extra, " ( bin lrecl 36 recfm f") ;
   s_trim( &newname, 'B'                   );
   s_cat(  &fname, fname, newname, 0, 0    );
   s_cat(  &fname, fname, odt_ext, 0, 0    );
   s_drop( &newname );
   s_drop( &odt_ext );
   s_drop( &defalt  );

   os_qfd( &file_name, fname, S_NULL    );
   s_cat(  &file_name, file_name, extra, 0, 0 );
   s_drop( &extra   );

   s_put( file_name, 0, s_len( file_name )+1 );

   fpout = open (s_buffer(file_name), O_CREAT | O_WRONLY | O_TRUNC) ;
   s_drop( &file_name );
   s_drop( &fname     );

   if ( fpout == I_O_ERROR )
   {

      rc = ODT_CANNOT_BE_SAVED;

   }
   else
   {

      numbytes = (odt->offset + 1) * sizeof (odtrecord) ;

      rc = write (fpout, (byte *)odt, numbytes) ;



      close (fpout) ;

   }

   return(rc);
}
