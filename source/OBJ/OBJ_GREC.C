/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                                          OBJ_GREC.c
<level>                                         OBJ
<AUTHOR>                                        John L. Foster
<DATE>                                          July 15, 1988

<Param>                  ocb  : a pointer to object control block

                         ocd  : when in the object is the data to come
                                out of.

                         line : a pointer to a buffer

<OUTPUT>                 line : fills in the buffer pointed to by
                                line with the contents of object
                                component descriptor.

                                Returns the string pointer to the line
                                that is the last argument.

                                Returns S_NULL to indicate end of file.

<PURPOSE>
        this routine given a pointer to an object control block and a
        pointer to a buffer it fills in the buffer and returns that
        string pointed to by line.  If the OCD is NIL then the object
        is assumed to be a OS file and each line is returned one at a
        time.  Else the OCD is assumed to point to an Index or Array
        segment, where we will return the next available BSE of text.
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




string obj_grec( ocb, ocd, line )

OCB            *ocb;
OCD                 *ocd;
string                   *line;


{


sint    rc            =     0;
boolean allocated_ocd = FALSE;
string  return_string = S_NUL,
        holder        = S_NUL;




  if( OCB_POINTER_OK( ocb ) )
  {

     if( ocd == NIL )
     {

        ocd = ocd_new( & ocd, NIL );
        allocated_ocd = TRUE;

     }
     if( ! (ocb->mode & OS_FILE) )
     {

        ODT    *odt_entry =    NIL;


        odt_entry = odt_find( ocb, ocd );
        if( ODT_POINTER_OK( odt_entry ) )
        {

           if( odt_type( odt_entry ) == OC_NDX )
           {

              string  prefix    = S_NULL;


              s_init(  & (ocd->key),                          "1234" );
              os_memc( & (ocb->status), s_buffer( ocd->key ), 4      );
              s_copy(  & holder, ocd->name,                     0, 0 );
              s_init(  & prefix, ".TEXT"                             );
              s_cat(   & (ocd->name), ocd->name, prefix,        0, 0 );
              s_drop(  & prefix                                      );
              ocb->status ++ ;

           }
           else if ( odt_type( odt_entry ) == OC_ARY )
           {

              ocd->bias = ocb->status;
              ocb->status ++ ;

           }
           else
           {

              rc = INVALID_OCD_TYPE;

           }
        }
        else
        {

           rc = INVALID_OCD_TYPE;

        }
     }
     if( rc == 0 )
     {


        rc = obj_get( ocb, ocd, line );

        if( rc != 0 )
        {

           return_string = S_NULL;

        }
        else
        {

           return_string = *line;

        }
     }
     else
     {


     }
     if( allocated_ocd )
     {

        ocd_drp( & ocd );

     }
     if( s_len( holder ) > 0 )
     {

        s_copy( &(ocd->name), holder, 0, 0 );
        s_drop( &holder                    );

     }
  }
  else
  {

     return_string = S_NULL;

  }




   return(return_string);
}

