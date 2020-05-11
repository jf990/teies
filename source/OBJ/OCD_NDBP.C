/********************************************************************

    :NAME.               OCD_DBP.C
    :LEVEL.              OBJ
    :AUTHOR.             John L. Foster
    :DATE.               September 01, 1987
    :PARAM.
            *ocb         pointer to the Object's Control Block

            *ocd         pointer to the object component descriptor
                         of the object field we need to compute a DMS
                         segment path to.

            *db_path     A TEIES string where we will place the resulting
                         data base path. The USER LENGTH of this string
                         is the length of the DMS path.

    :INPUT_GLOBALS.      * NONE *
    :OUTPUT_GLOBALS.     * NONE *

    :PURPOSE.
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
    :CALLS.
             odt_find(), s_init(), s_comp(), s_move(), s_first(),
             odt_lvl(), odt_off(), s_drop(), os_mema(), os_memd(),
             os_memc().
    :ERRORS. 20023 : OCD not found in ODT specified.
             20024 :
             20025 : Root component not found in the ODT
    :LIMITS.
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
    :MODS.
     Date        Author                 Description
 ********************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj_i.h"
#include "debug.h"


extern odtrecord *odt_find();




sint   ocd_ndbp ( ocb, ocd, db_path )

struct OCB *ocb    ;            /* ptr, to the obj control block       */
ocdrecord  *ocd    ;            /* ptr, to the ocd string and indx key */
string     *db_path;            /* TEIES str where we put the DB Path  */

{

sint       level        =  1;   /* Track the field level we are at     */
byte       rs_level     =  0;   /* Track the number of root seg OCD's  */
odtrecord *odt          =  NIL; /* Pointer to the Object Def Table     */
odtrecord *odt_current  =  NIL; /* Pointer to where we are in the table*/
string     work_str     =S_NUL; /* Work string for intermediate fields */
string     period       =S_NUL; /* Defines the OCD field separator     */
sint       odt_counter  =  1;   /* Current OCD record number we are at */
sint       path_length  =  0;   /* Total bytes written into Path so far*/
sint       last_dot     =  0;   /* Position of last . in OCD string    */
sint       prev_dot     =  0;   /* Position of last . in previous str  */
sint       rc           =  0,   /* Error detection and return code     */
           i            =  0;   /* Intermediate counter & place holder */
boolean    found        =FALSE; /* Flag to detect component in ODT     */
boolean    last         =FALSE; /* Flag to detect last ODT in search   */
boolean    phantom      =FALSE; /* True if there is a phantom segment  */


   #D_start (ocd_ndbp,OBJ,'ocb_ptr=0x%x; ocd_ptr=0x%x; dta_ptr=0x%x.',\
                         'ocb,ocd,db_path') ;
   #D_str (ocd->name,  'Input OCDSTRING'  ) ;
   #D_str (*db_path,   'Data base path before computation'   ) ;


   odt = ocb->odt;

   odt_current = odt + 1;
   s_init( db_path, "..............................................");

   s_init(  &period, "." );
   s_copy(  &work_str, ocd->name, 0,
            (last_dot = s_first( period, ocd->name, 0, 0) - 1 ) );
   #D_str('work_str',' root component ');
   while( !found )
   {

      if( odt_lvl( odt+odt_counter ) == 0 )
      {

          ++rs_level;
          if(s_comp(work_str,odt_name(odt+odt_counter),0,0,'M',&rc)==0)
          {

             found = TRUE;

          }

      }
      if( !found && ((odt_counter = odt_nxto(odt+odt_counter)) == 1
        || odt_counter > odt_off(odt) ))
      {

          #D_leave( 20025, 'Root Component not found in ODT' );

      }                                 /******************************/
   }                                    /*                            */
#D_show('Root component=%d','rs_level');/* We found the Root Segment. */
                                        /* Now go through each OCD,   */
                                        /* find it in the ODT, and    */
                                        /* fill in the DBP accordingly*/
                                        /* Remember, once we find a   */
                                        /* non-segment data type we   */
                                        /* are done.                  */
                                        /*                            */
                                        /******************************/

   s_put( *db_path, rs_level, ++path_length );
   found = FALSE;
   do
   {

      prev_dot = last_dot + 2;
      last_dot = s_first(period, ocd->name, prev_dot,s_len(ocd->name))-1;
      #D_show('Found dot at pos %d.','last_dot+1');
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
      #D_str( 'work_str', 'Next component to find' );
      odt_counter++;

      do
      {

        odt_current = odt+odt_counter;
        #D_str('odt_name(odt_current)','...trying to match...');
        if(s_comp(work_str,odt_name(odt_current),0,0,'M',&rc)==0)
        {

             found = TRUE;
             #D_show('Matched at lvl=%d; type=%d.',\
                     'odt_current->level,odt_current->type');
             if(  odt_current->type >= OC_SEG )
             {
                i = odt_prvo( odt_current );
                while( odt_current != odt+i )        /******************/
                {                                    /*               */
                                                     /*               */
                   #D_say(' ...checking for non-segments...');
                   if( odt_type( odt+i ) < OC_SEG )  /*Now test to see*/
                   {                                 /*if there is a  */
                                                     /*phantom segment*/
                      s_put(*db_path, (byte)++level, /*at this level. */
                             ++path_length         );
                      break;                         /*If so we must  */
                                                     /*account for it */
                   }                                 /*as segment 01  */
                   i = odt_prvo( odt+i );            /*at this level  */
#D_show('i=%d; type=%d; prv=%d.','i,odt_type(odt+i),odt_prvo(odt+i)');
                                                     /*and increment  */
                }                                    /*our level count*/

                   switch(  odt_current->type )
                   {

                      case OC_SEG :   /* ==> Repeating Segment <== */
                      {

                         s_put( *db_path, (byte)0x80, ++path_length );
                         os_memc( (hword)ocd->bias,
                                  s_buffer(*db_path)+(++path_length),
                                  2  );
                         ++path_length;
                         break;

                      }
                      case OC_NDX :   /* ==> Indexed Segments  <== */
                      {

                         ++path_length;
                         s_put(*db_path,(byte)(128+s_len(ocd->key)),
                               path_length                    );
                         os_memc( &(ocd->bias),
                               s_buffer(*db_path)+path_length,  4 );
                         path_length += 4;
                         os_memc(s_buffer(ocd->key),
                               s_buffer(*db_path)+path_length,
                               s_len(ocd->key));
                         path_length += s_len( ocd->key );
                         break;

                      }
                      case OC_ARY :   /* ==> Array Segment     <== */
                      {

                         break;

                      }
                      case OC_HSEG :  /* ==> Hierarchal Segment <== */
                      {

                         if( level > 127 )
                         {

                           #D_show('Warning: HSeg=%d; setting to 127',\
                                   'level' ):
                           level = 127;

                         }
                         s_put( *db_path,(byte)level,++path_length);
                         break;

                      }
                      case OC_HHSG  : /* ==> High Hierarchal Seg <== */
                      {

                         break;

                      }
                      default :
                      {

      #D_show('Not a known segment type: %d','( odt_current->type)-256');
                         s_put( *db_path,(byte)level,++path_length);
                         break;

                      }
                   }
                   odt_current = odt + ( ++odt_counter );
                                                       /****************/
                }                                      /*              */
                else                                   /*We found a non*/
                {                                      /*segment type, */
                   #D_say('  ...Last entry! ... ');    /*so indicate we*/
                   last = TRUE;                        /*are done, then*/
                   phantom = FALSE;                    /*check to see  */
                   i = odt_prvo( odt_current );        /*if we add the */
                   while( odt_current != (odt + i ) )
                   {

#D_show('i=%d; type=%d; prv=%d.','i,odt_type(odt+i),odt_prvo(odt+i)');
                      if( odt_type( odt+i ) >= OC_SEG )
                      {

                         phantom = TRUE;

                      }
                      i = odt_prvo( odt+i );
                   }
                   if( phantom == TRUE )               /*extra segment */
                   {                                   /****************/

                      s_put( *db_path, '\01', ++path_length );

                   }
                                                       /*              */
                }                                      /*--------------*/
         }                                             /* Components do*/
         else                                          /* not match, so*/
         {                                             /*check if seg  */
                                                       /*              */
            #D_say('Components do not match.');
            found = FALSE;                             /*type. If so we*/
            if( odt_type( odt_current ) >= OC_SEG )    /*increment the */
            {                                          /*level counter */
                                                       /*to track segs */
                level++;                               /*before the one*/
                                                       /*we want.      */
            }                                          /****************/
            odt_counter      = odt_nxto( odt_current );
            if( odt_counter == 0
             || odt_counter > odt_off( odt )
             ||(odt_nxto( odt_current ) <= odt_prvo( odt_current )
                                        && odt_off(  odt_current) != 0 )
             || odt_counter == odt_prvo( odt+odt_counter-1 )
              )
            {

               found = FALSE;
               break;

            odt_current = odt + (odt_counter = odt_nxto(odt_current));
            }
         }

  #D_show('ODT Counter is at %d;, addr=0x%x.','odt_counter,odt_current');
     }
     while( !found );
   }
   while( found && !last );

   s_put( *db_path, '\0', ++path_length   );

                                              /************************/
                                              /* We are done. Tweek   */
s_smp_ul(*db_path ) = path_length - 1;        /* the DMS Path string  */
s_drop( &period   );                          /* length to the amount */
s_drop( &work_str );                          /* we really stuffed in */
#D_mdump( '*db_path', 'path_length-1' );      /* it and deallocate all*/
#D_str('*db_path', 'Resulting data base Pth');/* memory we used for   */
#D_return (0) ;                               /* temp work space.     */
}                                             /************************/
