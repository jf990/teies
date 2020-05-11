/********************************************************************
    <DOC>
    <Name>               OCD_DBP.C
    <LEVEL>              OBJ
    <AUTHOR>             John L. Foster
    <DATE>               September 01, 1987
    <PARAM>
            *ocb         pointer to the Object's Control Block

            *ocd         pointer to the object component descriptor
                         of the object field we need to compute a DMS
                         segment path to.

            *db_path     A TEIES string where we will place the resulting
                         data base path. The USER LENGTH of this string
                         is the length of the DMS path.

    <Global>

    <PURPOSE>
             Convert the ocd string into a data base path and place
             the result into the TEIES string pointed to by data.
             To use the results as a DMS path take the used length of the
             string and pass the DMS the pointer to the string's data
             field (with the S_BUFFER function).

             The local variable LEVEL tracks the number of segments
             on the same level as the target segment before the target
             segment.

             The local variable OCD_COUNTER counts how many OCD's from
             the beginning of the ODT table we are currently at.
    <CALLS>
             odt_find(), s_init(), s_comp(), s_move(), s_first(),
             odt_lvl(), odt_off(), s_drop(), os_mema(), os_memd(),
             os_memc().
    <ERRORS> 20023 : OCD not found in ODT specified.
             20024 :
             20025 : Root component not found in the ODT
    <LIMITS>
             Currently only handles 1 key in the DB Path. The Index Seg
             section should be updated to handle multiple keys.

             Does not handle Repeating Segments correctly as the segment
             number is not placed in the DMS path correctly. It is stored
             in the OCD bias field.

             Since DBP calls odt_find() to first verify the OCD does
             exist for the object in question, some of the logic assumes
             you have to find all the fields. Some of the logic tries
             to recover if it cannot resolve the DMS path. If odt_find()
             says it is there, then DBP *MUST* return a valid DMS PATH.
    <MODS>
   June 22 '88   Anoop Srivastava       Made changes to work with the
                                        inheritance type object component

   28_Nov_88     JF                     Added defined constants for segmt
                                        types, corrected s_put() macro
                                        usage in accordance with C 3.0

   07_Jan_89     JF                     Corrected Phantom segment detec-
                                        tion when only one data componet
                                        and one segment type are in the
                                        same segment.
   </DOC>
 ********************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"




#define INDEX_SEGMENT     ( (byte) 0x80 )
#define PHANTOM_SEGMENT   ( (byte) 0x01 )
#define END_OF_DBPATH     ( (byte) 0x00 )



sint   ocd_dbp ( ocb, ocd, db_path )


OCB        *ocb    ;            /* ptr, to the obj control block       */
OCD        *ocd    ;            /* ptr, to the ocd string and indx key */
string     *db_path;            /* TEIES str where we put the DB Path  */

{



sint       level        =  1,   /* Track the field level we are at     */
           key_len      =  0;
byte       rs_level     =  0;   /* Track the number of root seg OCD's  */
ODT       *odt          =  NIL, /* Pointer to the Object Def Table     */
          *odt_current  =  NIL; /* Pointer to where we are in the table*/
string     work_str     =S_NUL, /* Work string for intermediate fields */
           period       =S_NUL; /* Defines the OCD field separator     */
sint       odt_counter  =  1,   /* Current OCD record number we are at */
           path_length  =  0,   /* Total bytes written into Path so far*/
           last_dot     =  0,   /* Position of last . in OCD string    */
           prev_dot     =  0,   /* Position of last . in previous str  */
           rc           =  0,   /* Error detection and return code     */
           i            =  0,   /* Intermediate counter & place holder */
           k            =  0,   /*                                     */
           monitor      =  0;   /* Watch Dog loop counter              */
boolean    found        =FALSE, /* Flag to detect component in ODT     */
           last         =FALSE, /* Flag to detect last ODT in search   */
           phantom      =FALSE, /* True if there is a phantom segment  */
           inhr         =FALSE; /* flag to detect inheritance          */









   odt = ocb->odt;

   odt_current = odt + 1;
   s_alloc( db_path, 132 );

   s_init(  &period, "." );


  do
  {

     inhr = FALSE;
     last_dot = s_first( period, ocd->name, prev_dot, 0 ) - 1;
     if( last_dot < 0 )
     {

        last_dot = 0;

     }
     s_copy(  &work_str, ocd->name, 0, last_dot );

     while( !found )
     {

       if( odt_lvl( odt+odt_counter ) == 0 )
       {

          ++rs_level;
          if( odt_comp( odt+odt_counter, work_str )  )
          {

             found = TRUE;

          }
          else
          {

             odt_counter = odt_nxto( odt + odt_counter );

          }
       }
       else
       {

         odt_counter = 1;

       }

       if( (! found && odt_counter == 1) || (odt_counter > odt_off(odt)))
       {

   return(OCD_NOT_IN_ODT);

       }
     }

     if ( odt_type( odt + odt_counter) ==  OC_INHR )
     {
                                               /* check if the root    */
          inhr = TRUE;                         /* object  comp is of   */
          odt = odt_inhr( odt + odt_counter ); /* inheritance type     */
          odt_counter = 0;                     /* if so set inhr true  */
          rs_level  -= 1;                      /* decrement rs_level   */
          prev_dot = last_dot + 2;

     }

   }
   while ( inhr );

                                        /******************************/
                                        /* We found the root segment. */
                                        /* Now go through each OCD,   */
                                        /* find it in the ODT, and    */
                                        /* fill in the DBP accordingly*/
                                        /* Remember, once we find a   */
                                        /* non-segment data type we   */
                                        /* are done.                  */
                                        /*                            */
                                        /******************************/
   path_length++;
   s_put( *db_path, rs_level, 1 );


if( last_dot > 1 )                      /* = = = = = = = = = = = = = = */
{                                       /*                             */
                                        /* Do the following ONLY if    */
   found = FALSE;                       /* there are more components in*/
   do                                   /* the OCD Name.               */
   {                                    /*                             */
                                        /* = = = = = = = = = = = = = = */
      prev_dot = last_dot + 2;
      last_dot = s_first(period, ocd->name, prev_dot,s_len(ocd->name))-1;
      if( last_dot < 1  )
      {

         last = TRUE;
         s_copy( &work_str, ocd->name, prev_dot,
                 s_len(ocd->name) - prev_dot + 1 );

      }
      else
      {

         s_copy( &work_str, ocd->name, prev_dot, last_dot-prev_dot+1 );

      }
      odt_counter++;
      monitor =   0;

      do
      {

        odt_current = odt+odt_counter;


        if( odt_comp( odt_current, work_str ) )
        {

           found = TRUE;
           if(  odt_current->type >= OC_SEG )
           {
              i = odt_prvo( odt_current );
              while( odt_current != odt+i )        /******************/
              {                                    /*               */
                                                   /*               */
                 if( odt_type( odt+i ) < OC_SEG )  /*Now test to see*/
                 {                                 /*if there is a  */
                                                   /*phantom segment*/
                                                   /*at this level. */
                    s_put( (*db_path), ((byte)++level),\
                                       (++path_length)        );
                    phantom = TRUE;
                    break;                         /*If so we must  */
                                                   /*account for it */
                 }                                 /*as segment 01  */
                 i = odt_prvo( odt+i );            /*at this level  */
                                                   /*and increment  */
              }                                    /*our level count*/

              switch(  odt_current->type )
              {

                 case OC_HSEG :  /* ==> Hierarchal Segment <== */
                 case OC_RSEG :  /* ==> In Root    Segment <== */
                 case OC_SEG  :  /* ==> Segment -- Segment <== */
                 {

                    if( odt_type(odt_current) == 0 )
                    {

                       if( level > 127 )
                       {

                        level = 127;

                       }
                       s_put( (*db_path), ((byte)level),\
                                                   (++path_length));

                    }
                    break;

                 }
                 case OC_NDX :   /* ==> Indexed Segments  <== */
                 {

                    s_put( (*db_path), ((byte)level),\
                                                  (++path_length) );

                    if( ! ( ocd->bias == 0      &&
                            ocd->key  == S_NULL &&
                            last      == TRUE     ) )
                    {

                       ++path_length;

                       if( ocd->key == (string) -1 )
                       {

                          ocd->key = S_NULL;
                          s_alloc( & (ocd->key), 32 );
                          for( k=1; k<33; k++ )
                          {

                             s_put( ocd->key, (byte) 0xFF, k );

                          }
                          s_smp_ul( ocd->key ) = 32;
                          key_len = 32;
                          s_put(*db_path,(byte)(128+32), path_length );

                       }
                       else if( ( key_len = s_len( ocd->key )) == 0 )
                       {

                          s_put(*db_path,(byte)(128+32), path_length );
                          s_alloc( &(ocd->key), 32 );
                          os_memn( s_buffer(ocd->key), 32 );
                          s_smp_ul( ocd->key ) = 32;
                          key_len = 32;

                       }
                       else
                       {

                          s_put( *db_path, (byte)(128+key_len),
                                 path_length                    );

                       }
                       os_memc( &(ocd->bias),
                             s_buffer(*db_path)+path_length,  4 );
                       path_length += 4;

                       os_memc(s_buffer(ocd->key),
                            s_buffer(*db_path)+path_length,
                            key_len);
                       path_length += key_len;

                       s_copy( &(ocb->last_key), ocd->key, 0, 0 );

                    }
                    break;
                 }
                 case OC_ARY :           /* ==> Array Segment <== */
                 {

                    hword   temp_offset = 0;


                    ++path_length;
                    s_put( (*db_path), ((byte)level), path_length );
                    ++path_length;
                    s_put( (*db_path), INDEX_SEGMENT, path_length );
                    temp_offset = (hword) ocd->bias;
                    os_memc( &(temp_offset),
                             s_buffer(*db_path)+(++path_length),
                             2  );
                    ++path_length;
                    break;

                 }
                 default :
                 {

                    ++path_length;
                    s_put( (*db_path), ((byte)level), path_length );
                    break;

                 }
              }
              odt_current = odt + ( ++odt_counter );
              level = 1;
                                                  /****************/
           }                                      /*              */
           else                                   /*We found a non*/
           {                                      /*segment type, */
                                                  /*so indicate we*/
              last    = TRUE ;                    /*are done, then*/
              phantom = FALSE;                    /*check to see  */
                                                  /*if we add the */
                                                  /*extra segment.*/
                                                  /****************/
              i = odt_previous_offset( odt_current );
              while( (odt_current != ( odt + i )) &&
                     (odt_next_offset( odt + i )  !=
                      odt_previous_offset( odt +i ) ))
              {

                 if( odt_type( odt+i ) >= OC_SEG )
                 {

                    phantom = TRUE;

                 }
                 i = odt_previous_offset( odt+i );

              }
              if( odt_next_offset(     odt + i )    ==
                  odt_previous_offset( odt + i ) )
              {

                 if( odt_type( odt + odt_next_offset(odt+i)) >= OC_SEG )
                 {

                    phantom = TRUE;

                 }
              }
              if( phantom == TRUE )
              {

                 ++path_length;
                 s_put( (*db_path), PHANTOM_SEGMENT, path_length );

              }
           }                                           /*--------------*/
        }                                              /* Components do*/
        else                                           /* not match, so*/
        {                                              /*check if seg  */
                                                       /*              */
            found = FALSE;                             /*type. If so we*/
            if( odt_type( odt_current ) >= OC_SEG )    /*increment the */
            {                                          /*level counter */
                                                       /*to track segs */
                level++;                               /*before the one*/
                                                       /*we want.      */
            }                                          /****************/
            if( (odt_counter >= odt_next_offset( odt_current ))
             || (odt_counter >  odt_off( odt ))
             || (odt_counter == 0)
              )
            {
                                         /******************************/
               found = FALSE;            /* Why this big condition?    */
               break;                    /* In order to determine if   */
                                         /* current level search has   */
                                         /* been exausted and we have  */
                                         /* not found the component the*/
                                         /* following must be true:    */
                                         /*-We have searched past the  */
                                         /*  bounds of the ODT;        */
                                         /*-if the current offset is   */
                                         /*  not zero indicating we are*/
                                         /*  not at the first entry at */
                                         /*  the current level then we */
                                         /*  determine if we are at the*/
                                         /*  last entry at the current */
                                         /*  level by the next offset  */
                                         /*  being less than or equal  */
                                         /*  to the previous at this   */
                                         /*  level.                    */
                                         /*-for the special case when  */
                                         /*  there are only 2 adjacent */
                                         /*  components at the current */
                                         /*  level the previous of the */
                                         /*  last one points to the    */
                                         /*  current.                  */
                                         /*That logic handles every    */
                                         /*possible case if the ODT is */
                                         /*coded properly.  If it is   */
                                         /*not infinite looping is like*/
                                         /*probable and that is why a  */
                                         /*watchdog timer is set.      */
                                         /*********End of wind**********/

            }
            odt_counter = odt_nxto( odt_current );
            odt_current = odt + odt_counter;
         }
     }
     while( ! found ) ;
   }
   while( found && ! last ) ;

}  /* End of search for components to parse and compute.  Now finish
      the data base path and clean up                                  */


   ++path_length;
   s_put( (*db_path), END_OF_DBPATH, path_length   );

                                              /************************/
                                              /* We are done. Tweek   */
s_smp_ul(*db_path ) = path_length - 1;        /* the DMS Path string  */
s_drop( &period   );                          /* length to the amount */
s_drop( &work_str );                          /* we really stuffed in */
   return(0);
}                                             /************************/
