/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_focd                                               |
  |  <AUTHOR>  Tanmay S Kumar                                         |
  |  <DATE>    June 6, 1988
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *cocb     struct OCB   INPUT  Pointer to the OCB that  |
  |                                          contains the pointer to  |
  |                                          the ODT                  |
  |            *ocd      struct OCD   INPUT  Descriptor of the Object |
  |                                          Component Descriptor     |
  |            *dataptr  byte         OUT    Pointer to data          |
  |            offset    sint         INPUT  offset of the OCD to get |
  |                                                                   |
  |  <PURPOSE> To return a pointer to the data for the required offset|
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_first (), s_comp (), s_drop (), s_init (), s_copy () |
  |            s_len(), s_nulout(), s_cat(),   obj_get().             |
  |                                                                   |
  |            marco - ocb_odt(), odt_nxto(), odt_prvo(), odt_name()  |
  |                    odt_lvl().                                     |
  |                                                                   |
  |  <LIMITS>  NOTE: when calling this function need to allocate      |
  |             memory for the data and then pass the pointer to it,  |
  |             also it is advisable to use cnull() to null out the   |
  |             allocated memory for the data.                        |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

sint obj_focd(cocb, ocd, ptr)

OCB           *cocb;
OCD           *ocd;
byte          *ptr;

{
   odtrecord *odtptr  = NIL    ;  /* Pointer to the ODT                */
   string     period  = S_NULL ;  /* String containing a period '.'    */
   string     searstr = S_NULL ;  /* String to search for              */
   sint       prevdot =  0     ;  /* Position of the previous '.'      */
   sint       currdot =  0     ;  /* Position of the current '.'       */
   boolean    found   = TRUE   ;  /* Indicates if match has been made  */
   boolean    last    = FALSE  ;  /* Indicates if last searstr parsed  */
   sint       currec  =  0     ;  /* Current record number             */
   sint       nxtrec  =  0     ;  /* Next record number                */
   sint       fstrec  =  0     ;  /* Number of first record in loop    */
   sint       curlvl  = -1     ;  /* Current level for error checking  */
   sint       i       =  0     ;  /* Return code from s_comp           */
   sint       j       =  0     ;  /* Watchdog counter                  */
   string     ocdstr  = S_NULL ;  /* Contains the name of the ocd      */
   sint       rc      = 0      ;

   #D_start (obj_focd, OBJ,'OCB_ptr=0x%X OCD_ptr=0x%X','cocb,ocd');
   #D_tshow('cocb = %x','cocb');
   #D_tshow('ocd = %x','ocd');
   #D_tstr(ocd->name,'ocd->name');

   /* check the length of name string passed */

   if( s_len( ocd->name ) < 1 )
     {
      printf("No OCD to search for\n");
      #D_leave( 0, 'No OCD to search for.' );
     }

   /* copy the name string into ocdstr */

   rc = s_copy (&ocdstr, ocd->name, 0, 0) ;
   #D_tstr (ocdstr,'name to search for') ;

   /* macro which returns the pointer to the Object Definition Table ODT
      given the pointer to the Object Control Block OCB.  */

   odtptr = ocb_odt (cocb) ;
   #D_tshow ('odt_ptr=0x%X odt_type={%d}','odtptr,odtptr->type') ;

   /* check the value of odtptr */

   if( odtptr == NIL || odtptr == -1 || odtptr->type > 100 ||
                                        odtptr->type <   0    )
     {
       s_drop(&ocdstr);
       printf("The ODT has been determined to be invalid\n");
       #D_leave(-1, 'The ODT has been determined to be invalid.' );
     }

   s_init (&period, ".") ;

   do
   {
      prevdot = currdot ;         /* Find period                       */
      currdot = s_first (period, ocdstr, prevdot+1, 0) ;

      if (currdot == 0)           /* If the last period has been found */
        {
         last = TRUE ;            /* Set last=TRUE and get search str  */
         s_copy (&searstr, ocdstr, prevdot+1, 0) ;
         #D_tstr(searstr,'searstr');
        }
      else                        /* Else just get search string       */
         s_copy (&searstr, ocdstr, prevdot+1,currdot-prevdot-1) ;

      found = FALSE ;             /* Searstr hasn't been found yet     */
      curlvl++ ;                  /* Increment current level           */
      currec++ ;                  /* Increment current record #        */
      fstrec = currec ;           /* Save the first record # searched  */
      #D_tshow ('fnd={%d} lvl=%3d fstrec=%3d','found,curlvl,fstrec') ;


      do                          /* Do comparison of names            */
      {
         #D_tshow ('cur = %2d nxt=%2d prv=%2d', \
            'currec,odt_nxto(odtptr+currec),odt_prvo(odtptr+currec)') ;
         #D_tstr  (odtptr+currec,'curname') ;

         if (s_comp (searstr,odt_name(odtptr+currec),0,0,'M',&rc) == 0)

         {   /* If names match then Check to see that the levels match */
            if (odt_lvl((odtptr+currec)) == curlvl)
               found = TRUE ;
            else
              {
               s_drop (&ocdstr ) ;
               s_drop (&period ) ;
               s_drop (&searstr) ;
               printf("Levels do not match\n");
               #D_leave(20040, 'Levels don''t match') ;
              }
         }
         else
         {   /* If names do not match then increment the record # and */
             /* check to see if we are going back to the first record  */
             /* if so then the name we are searching for at that level */
             /* is invalid.                                            */
            if ((currec = odt_nxto (odtptr+currec)) <= fstrec)
              {
               #D_tshow( 'Currec = 0X%x fstrec = 0X%x','currec,fstrec' );
               s_drop (&ocdstr ) ;
               s_drop (&period ) ;
               s_drop (&searstr) ;
               printf("Invalid name string in OCD\n");
               #D_leave(-1, 'Invalid name string in OCD');
              }
         }
      }
      while (!found && (fstrec != currec)) ;
   }
   while (found && !last);
   if (found)
      ptr = odtptr + currec;
   else
      ptr = NIL;
  #D_return(0);
}
