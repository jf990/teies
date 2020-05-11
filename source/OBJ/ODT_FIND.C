/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_find                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    June  9, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   *ocbptr   struct OCB   INPUT  Pointer to the OCB that  |
  |                                          contains the pointer to  |
  |                                          the ODT                  |
  |            ocd       struct OCD   INPUT  Descriptor of the Object |
  |                                          Component Descriptor     |
  |                                                                   |
  |  :PURPOSE. To return a pointer to the OCD descripted by ocdstr    |
  |                                                                   |
  |  :OUTPUT.  returns a pointer to the OCD and if not found a null   |
  |                                                                   |
  |  :CALLS.   s_first (), s_comp (), s_drop (), s_init (), s_copy () |
  |                                                                   |
  |  :ERRORS.  0 if OCD not in ODT                                    |
  |           -1 if ODT invalid                                       |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"




odtrecord *odt_find (object, ocd)

OCB           *object ;
OCD           *ocd    ;



{
   odtrecord *odtptr     = NIL ,  /* Pointer to the ODT                */
             *return_odt = NIL ;
   string     period  = S_NULL ,  /* String containing a period '.'    */
              searstr = S_NULL ,  /* String to search for              */
              component  = S_NULL ;  /* Contains the name of the ocd      */
   sint       prevdot =  0     ,  /* Position of the previous '.'      */
              currdot =  0     ,  /* Position of the current '.'       */
              currec  =  0     ,  /* Current record number             */
              fstrec  =  0     ,  /* Number of first record in loop    */
              curlvl  = -1     ,  /* Current level for error checking  */
              rc      =  0     ,  /* Return code from s_comp           */
              len_to_compare = 0, /*                                   */
              j       =  0     ;  /* Watchdog counter                  */
   boolean    found   =  TRUE  ,  /* Indicates if match has been made  */
              last    = FALSE  ;  /* Indicates if last searstr parsed  */


   #D_start (odt_find, OBJ,'OCB_ptr=0x%X OCD_ptr=0x%X','object,ocd   ');
   #D_str( 'ocd->name', 'OCD to find.' );


if( OCD_POINTER_OK( ocd ) )
{

   if( s_len( ocd->name ) < 1 )
   {

      return_odt = NIL;
      #D_say( 'No OCD to search for.' );

   }
   else
   {

      rc = s_copy ( & component, ocd->name, 0, 0 ) ;
      odtptr = ocb_odt( object ) ;

      #D_str ( 'component', 'Name to search for') ;
      #D_show ('odt_ptr=0x%X odt_type={%d}','odtptr,odtptr->type') ;

      if( ! ODT_POINTER_OK( odtptr ) || odtptr->type > 100 ||
                                        odtptr->type <   0    )
      {

          return_odt = (ODT *) 0xFFFFFFFF;
          #D_say(  'The ODT has been determined to be invalid.' );

      }
      else
      {

         s_init ( & period, "." ) ;

         do
         {

            prevdot = currdot ;
            currdot = s_first (  period, component, prevdot + 1, 0  ) ;

            if (  currdot == 0  )
            {

               last = TRUE ;
               s_copy ( & searstr, component, prevdot + 1, 0 ) ;

            }
            else
            {

               s_copy ( & searstr, component, prevdot+1, currdot-prevdot-1 );

            }
            #D_str( 'searstr', 'searched component name' );
            found = FALSE ;
            curlvl++ ;
            currec++ ;
            fstrec = currec ;

            #D_show ('fnd={%d} lvl=%3d fstrec=%3d','found,curlvl,fstrec') ;


            do                       /* Do comparison of names         */
            {

               #D_show ('cur = %2d nxt=%2d prv=%2d', 'currec,\
          odt_next_offset(odtptr+currec), odt_previous_offset(odtptr+currec)');
               #D_str  ('odt_name(odtptr+currec)','curname') ;

               if ( odt_comp( odtptr+currec, searstr ) )
               {

                  if ( odt_level( (odtptr+currec) ) == curlvl )
                  {

                     found = TRUE ;

                  }
                  else
                  {

                     found = FALSE;
                     #D_say('Levels do not match') ;
                     break;

                  }
               }
               else
               {

                  if ( (currec = odt_next_offset( odtptr+currec )) <= fstrec )
                  {

                     #D_show( 'Currec = 0X%x fstrec = 0X%x','currec,fstrec' );
                     found = FALSE;
                     #D_say('Invalid name string in OCD');
                     break;

                  }
               }
            }
            while (  ! found && (fstrec != currec) ) ;

            if ( found  &&  (odt_type( odtptr + currec ) == OC_INHR) )
            {
                 odtptr = odt_inhr(odtptr + currec);
                 currec = 0;
                 fstrec = 0;   /* if the current obj comp type is   */
                 curlvl = -1;  /* of inheritance type then reset all*/
                               /* counters and start from begining  */
                               /* the new table......               */
            }
         }
         while (   found   &&   !  last   ) ;

      }
      s_drop ( & component ) ;
      s_drop ( & period    ) ;
      s_drop ( & searstr   ) ;

      if ( found )
      {

         return_odt = odtptr + currec ;
         #D_say( 'The OCD has been found in the ODT.');

      }
   }
}

#D_return( 'return_odt' );
}
