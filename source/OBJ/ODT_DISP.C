/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_disp                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka, John L. Foster                         |
  |                                                                   |
  |  :DATE.    June  4, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   *odt_entry  odtrecord INPUT   A pointer to the first   |
  |                                          element in the ODT       |
  |                                          table                    |
  |                                                                   |
  |  :PURPOSE. This routine upon being passed a pointer to an ODT     |
  |            table will display all the information that the ODT    |
  |            contains in a table.                                   |
  |                                                                   |
  |  :OUTPUT.  Always returns 0.                                      |
  |                                                                   |
  |  :CALLS.   printf ()                                              |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  Must be changed to use GKS eventually.                 |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "stdio.h"
#include "file.h"
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"





sint odt_disp (odt_entry)

odtrecord  *odt_entry;



{


   FILE  *outfile     =    NIL;
   sint   count       =      0,
          count2      =      0,
          numelems    =      0 ;
   ODT   *holder      =    NIL;
   string object_name = S_NULL,
          asterik     = S_NULL,
          out_file_nm = S_NULL,
          out_file_df = S_NULL;






   s_init( & out_file_df, "A * * ODTT"                      );
   s_init( & asterik,     " *"                              );
   s_copy( & object_name, obj_name( odt_entry->type ), 0, 0 );
   s_init( & out_file_nm, "* * "                            );
   s_cat(  & out_file_nm, out_file_nm, object_name,    0, 0 );
   s_cat(  & out_file_nm, out_file_nm, asterik,        0, 0 );
   os_qfd( & out_file_nm, out_file_nm, out_file_df          );
   s_put(    out_file_nm, NIL,  s_len( out_file_nm ) + 1    );

   outfile = fopen( s_buffer( out_file_nm ), "w" );
   if( outfile == (FILE *) 0xFFFFFFFF || outfile == NIL )
   {

      outfile = stdout;

   }
   s_pad(   &object_name, object_name, 5, ' ', 'R' );



   fprintf(outfile, "\n\n" );
   fprintf(outfile, "OCD             Level Offset OffNxt OffPrv Type Privs\n" );
   fprintf(outfile, \
"------------------------------------------------------------------\n" );

   holder = odt_entry;
   numelems = odt_entry->offset ; /* Get number of elements in table   */

   for (count = 0 ; count <= numelems ; count++)
   {                              /* Go through all the entries        */
                                  /* Display the name (TEIES string)   */
      for (count2 = 1 ; count2 <= 12  ; count2++)
      {
         if (count2 <= odt_entry->name[1])
            fprintf ( outfile, "%c",   odt_entry->name[count2+3]) ;
         else
            fprintf ( outfile, " " ) ;
      }
      fprintf ( outfile, "   " ) ;

      fprintf(outfile, "%6d ",    odt_entry->level  );
      fprintf(outfile, "%6d ",    odt_entry->offset );
      fprintf(outfile, "%6d ",    odt_entry->offnxt );
      fprintf(outfile, "%6d ",    odt_entry->offprv );

      if (count != 0)             /* Display ODT record type           */
      {                           /* If record is entry and not header */
         switch (odt_entry->type)
         {
         case OC_BYTE:
            fprintf ( outfile, "BYTE ") ;
            break ;
         case OC_INT :
            fprintf ( outfile, "INT  ") ;
            break ;
         case OC_REAL:
            fprintf ( outfile, "REAL ") ;
            break ;
         case OC_STR :
            fprintf ( outfile, "STR  ") ;
            break ;
         case OC_D_T  :
            fprintf ( outfile, "D/T  ") ;
            break ;
         case OC_PRIV:
            fprintf ( outfile, "PRIV ") ;
            break ;
         case OC_OID :
            fprintf ( outfile, "OID  ") ;
            break ;
         case OC_INHR:
            fprintf ( outfile, "INHRT") ;
            break ;
         case OC_HWRD:
            fprintf ( outfile, "HWRD ") ;
            break ;
         case OC_ISV:
            fprintf ( outfile, "ISV  ") ;
            break ;
         case OC_SEG :
            fprintf ( outfile, "SEG  ") ;
            break ;
         case OC_NDX :
            fprintf ( outfile, "NDX  ") ;
            break ;
         case OC_ARY :
            fprintf ( outfile, "ARY  ") ;
            break ;
         case OC_HSEG:
            fprintf ( outfile, "HSEG ") ;
            break ;
         case OC_HHSG:
            fprintf ( outfile, "HHSG ") ;
            break ;
         default     :
            fprintf ( outfile, "%04d ",odt_entry->type) ;
            break ;
         }
      }
      else                        /* Otherwise entry is the header and */
      {                           /*    display the type of ODT        */

         s_copy( &object_name, obj_name( odt_entry->type ), 0, 5 );
         s_pad(  &object_name, object_name, 5, ' ', 'R'          );
         s_put(   object_name, '\0', s_len( object_name ) + 1    );
         fprintf(outfile, "%s", s_buffer( object_name ));
         s_drop( &object_name               );

      }
                                  /* Display the 6 priviledge bytes    */

         fprintf ( outfile, "x%02X",odt_entry->privs.one  );
         fprintf ( outfile, "x%02X",odt_entry->privs.two  );
         fprintf ( outfile, "x%02X",odt_entry->privs.three);
         fprintf ( outfile, "x%02X",odt_entry->privs.four );
         fprintf ( outfile, "x%02X",odt_entry->privs.five );
         fprintf ( outfile, "x%02X",odt_entry->privs.six  );

      fprintf(outfile, "\n");
      if (count == 0)
      {
         fprintf ( outfile, \
"------------------------------------------------------------------\n" );
      }
      if (count != numelems)
         odt_entry++;
   }
   fprintf(outfile, "\n\n" );
   fprintf(outfile, "Summary:\n\n" );
   fprintf(outfile, "Object Type: %d;  ISV Reference: %d;",holder->type,holder->
      offnxt );
   fprintf(outfile, "  Total R/S Elements: %d\n", holder->level );
   fprintf(outfile, "Total OCD's: %d;  Total ODT Links:  %d",holder->offset,
      holder->offprv );
   fprintf(outfile, "\n" );


   s_drop( &out_file_df );
   s_drop( &asterik     );
   s_drop( &object_name );
   s_drop( &out_file_nm );

   if( outfile != stdout )
   {

      fclose( outfile );

   }


   return(0);
}
