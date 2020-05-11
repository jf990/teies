/************************************************************************
    <DOC>
    <NAME>    obj_next(OCB *cocb, OCD *ocd, byte **dataptr, sint offset,
                                          hword what_to_get, sint length)
    <AUTHOR>  Tanmay S Kumar
    <DATE>    May 16, 1988
    <LEVEL>   OBJ
    <PARAM>   *cocb     struct OCB   INPUT  Pointer to the OCB that
                                            contains the pointer to
                                            the ODT
              *ocd      struct OCD   INPUT  Descriptor of the Object
                                            Component Descriptor
              **dataptr byte         OUT    Pointer to data pointer
              offset    sint         INPUT  offset of the OCD to get
              what_to_get hword      INPUT  Type of data to get
                                            defined in obj_comm.h
              length    sint         IN/OUT length of data buffer
    <PURPOSE> To get the data for the given OCB, OCD, offset and type
              or to just get the OCD for the given offset.
              If space for the data is not allocated then the routine
              does the allocation depending on the type of data and the
              size of the data. The size is returned in the length
              parameter if the final ocd name can be computed.
              The different type of data to get are as follows:
              OCD_ONLY : This will not get any data it will only compute
              the final OCD name for the given offset if it exists only
              at the same level.
              If an initial ocd name is not provided then the first name
              in the odt table is assumed.
              STAY_IN_LEVEL: This is similar to OCD_ONLY except the it
              returns data for the final ocd name computed.
              If an initial ocd name is not provided then the routine
              returns a value of 0. i.e it does nothing.
              SEGMENT_ONLY: This flag will get data only for components
              which are of OC_SEG type for the given offset if it exists.
              If an initial ocd name is not provided then the first name
              in the odt table is assumed. The final ocd name computed
              is returned.
              COMPONENT_ONLY: This flag will get data for all components
              which are not of OC_SEG type for the given offset if it
              exists. If an initial ocd name is not provided then the
              first name in the odt table is assumed. The final ocd name
              computed is returned.
              ANY_COMPONENT: This flag will get data for any type of
              component for the given offset if it exists. The final ocd
              name computed is returned. If an initial ocd name is not
              provided then the first name in the odt table for the
              passed OCB is asumed.
    <OUTPUT>  Sint.
              Returns 0 if sucessful or if there is no OCD to search
              for.
              error code 20040 if levels do not match
              error code 20041 if offset is in the wrong direction
              error code 20042 if there is only one record at that
              level.
              returns -1 if invalid odt or invalid name string in OCD
              Any other error code returned depends on the lower
              level routines.
    <CALLS>   s_first (), s_comp (), s_drop (), s_init (), s_copy ()
              s_len(), s_nulout(), s_cat(),   obj_get().
              macro - ocb_odt(), odt_nxto(), odt_prvo(), odt_name()
                      odt_lvl().
    <MODS>     June 20, 1989     JF     Added OC_STR null record
                                          skip logic.
    </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "mem.h"
#include "obj.h"
#include "debug.h"

sint obj_next (cocb, ocd, dataptr, offset, what_to_get,length)

OCB            *cocb;
OCD             *ocd;
byte       **dataptr;
sint          offset;
hword    what_to_get;
sint         *length;

{
   odtrecord *odtptr  = NIL    ;  /* Pointer to the ODT                */
   odtrecord *nxtrec  = NIL    ;  /* Pointer to an OCD in the ODT      */
   odtrecord *finrec  = NIL    ;  /* Pointer to an OCD in the ODT      */
   string     period  = S_NULL ;  /* String containing a period '.'    */
   string     searstr = S_NULL ;  /* String to search for              */
   sint       prevdot =  0     ;  /* Position of the previous '.'      */
   sint       currdot =  0     ;  /* Position of the current '.'       */
   boolean    found   = TRUE   ;  /* Indicates if match has been made  */
   boolean    last    = FALSE  ;  /* Indicates if last searstr parsed  */
   boolean    flag    = FALSE  ;  /* Indicates if type of data is STR  */
   sint       currec  =  0     ;  /* Current record number             */
   sint       nextrec =  0     ;  /* Next record number                */
   sint       fstrec  =  0     ;  /* Number of first record in loop    */
   sint       curlvl  = -1     ;  /* Current level for error checking  */
   sint       lvl     =  0     ;  /* level of a record in the ODT      */
   sint       lvl1    =  0     ;  /* level of a record in the ODT      */
   sint       rc      =  0     ;  /* Return code                       */
   sint       size    =  0     ;  /* size of data to be returned       */
   string     ocdstr  = S_NULL ;  /* Contains the name of the ocd      */
   string     tempstr = S_NULL ;  /* Contains the name of the new ocd  */
   string     tempstr1= S_NULL ;  /* Contains the name of the new ocd  */
   boolean    local_ocd=FALSE  ;  /* if OCD is locally allocated       */



   /* check the length of name string passed */

   if (s_len( ocd->name ) < 1 )
   {
      if (what_to_get == STAY_IN_LEVEL)
      {
   return(0);
      }
      else
      {  /* start at the first odt in the ODT table */
         ocd_new( & ocd, (byte *) odt_name( cocb->odt + 1 ) );
         local_ocd = TRUE;
      }
   }
   /* copy the name string into ocdstr */

   s_copy (&ocdstr, ocd->name, 0, 0) ;

   /* macro which returns the pointer to the Object Definition     */
   /* Table ODT given the pointer to the Object Control Block OCB. */

   odtptr = ocb_odt (cocb) ;

   /* check the value of odtptr */

   if (odtptr == NIL || odtptr == (ODT *) 0xFFFFFFFF ||
       odtptr->type > 100 || odtptr->type < 0 )
   {
      s_drop(&ocdstr);
      rc  = ODT_IS_INVALID;
   }
   else
   {

      s_init (&period, ".") ;

      do
      {
         prevdot = currdot ;         /* Find period                */
         currdot = s_first (period, ocdstr, prevdot+1, 0) ;

         if (currdot == 0)        /* If the last period has been found */
         {
            last = TRUE ;         /* Set last=TRUE and get search str  */
            s_copy (&searstr, ocdstr, prevdot+1, 0) ;
         }
         else                    /* Else just get search string       */
         {
            s_copy (&searstr, ocdstr, prevdot+1,currdot-prevdot-1) ;
         }

         found = FALSE ;          /* Searstr hasn't been found yet     */
         curlvl++ ;               /* Increment current level           */
         currec++ ;               /* Increment current record #        */
         fstrec = currec ;        /* Save the first record # searched  */

         do                       /* Do comparison of names            */
         {

            if (
                s_comp (searstr,odt_name(odtptr+currec),0,
                        s_len( odt_name( odtptr+currec) ), 'M', & rc )
                == 0 )

            {
             /* If names match then Check to see that the levels match */
               if (odt_lvl((odtptr+currec)) == curlvl)
                  found = TRUE ;
               else
               {
                  s_drop (&ocdstr ) ;
                  s_drop (&period ) ;
                  s_drop (&searstr) ;
                  rc = OCD_LEVELS_DO_NOT_MATCH;
                  break;
               }
            }
            else
            {
             /* If names do not match then increment the record # and */
             /* check to see if we are going back to the first record */
             /* if so then the name we are searching for at that level*/
             /* is invalid.                                           */

               if ((currec = odt_nxto (odtptr+currec)) <= fstrec)
               {
                  s_drop (&ocdstr ) ;
                  s_drop (&period ) ;
                  s_drop (&searstr) ;
                  rc = INVALID_OCD_STRING;
                  break;
               }
            }
         }
         while (!found && (fstrec != currec)) ;
         if (rc != 0)
         {
            break;
         }
      }
      while (found && !last) ;    /* Finally found it                  */

      /* check to see what kind of data we are getting */

      if (rc == 0)
      {
         if ((what_to_get > STAY_IN_LEVEL) &&
                                      (what_to_get <= ANY_COMPONENT))
         {
            /* set nxtrec to where we are in the table */
            nxtrec = odtptr + currec;

            if (offset > 0)
            {
               ++nxtrec;
               switch (what_to_get)
               {
                  case SEGMENT_ONLY:
                        while ((offset >0) && (nxtrec <=
                                                 odtptr + odt_off(odtptr)))
                          {  /* if type is of SEGMENT type then */
                             /* only decrement the offset       */

                             if (odt_type(nxtrec) >= OC_SEG)
                             {
                                offset -= 1;
                             }
                             ++nxtrec;
                          }
                          --nxtrec;

                          if (offset > 0)
                          {
                             rc = IMPROPER_DIRECTION_IN_ODT;
                          }
                          break;

                  case COMPONENT_ONLY:
                        while ((offset >0) && (nxtrec <=
                                                 odtptr + odt_off(odtptr)))
                        {  /* only if type is not a SEGMENT type*/
                           /* decrement the offset              */

                           if (odt_type(nxtrec) < OC_SEG)
                           {
                              offset -= 1;
                           }
                           ++nxtrec;
                        }
                        --nxtrec;

                        if (offset > 0)
                        {
                           rc = IMPROPER_DIRECTION_IN_ODT;
                        }
                        break;

                  case ANY_COMPONENT:
                        while ((offset >0) && (nxtrec <=
                                                 odtptr + odt_off(odtptr)))
                        {
                           offset -= 1;
                           ++nxtrec;
                        }
                        --nxtrec;

                        if (offset > 0)
                        {
                           rc = IMPROPER_DIRECTION_IN_ODT;
                        }
                        break;

                  default:
                        break;
               }

               if( ( odt_type(            nxtrec ) == OC_STR )  &&
                   ( odt_next_offset(     nxtrec ) ==      1 )  &&
                   ( odt_previous_offset( nxtrec ) ==      1 )    )
               {

                  nxtrec ++ ;

               }
            }
            if (offset < 0)
            {
               --nxtrec;
               switch (what_to_get)
               {
                 case SEGMENT_ONLY:
                       while ((offset <0) && (nxtrec >= odtptr + 1))
                       {  /* decrement offset only if type is */
                          /* of segment type                  */

                          if (odt_type(nxtrec) >= OC_SEG)
                          {
                             offset += 1;
                          }
                          --nxtrec;
                       }
                       ++nxtrec;

                       if (offset < 0)
                       {
                          rc = IMPROPER_DIRECTION_IN_ODT;
                       }
                       break;

                 case COMPONENT_ONLY:
                       while ((offset <0) && (nxtrec >= odtptr + 1))
                       {  /* decrement offset only if type is */
                          /* not a segment type               */

                          if (odt_type(nxtrec) < OC_SEG)
                          {
                             offset += 1;
                          }
                          --nxtrec;
                       }
                       ++nxtrec;

                       if (offset < 0)
                       {
                          rc = IMPROPER_DIRECTION_IN_ODT;
                       }
                       break;

                 case ANY_COMPONENT:
                       while ((offset <0) && (nxtrec >= odtptr + 1))
                       {
                          offset += 1;
                          --nxtrec;
                       }
                       ++nxtrec;

                       if (offset < 0)
                       {
                          rc = IMPROPER_DIRECTION_IN_ODT;
                       }
                       break;

                 default:
                       break;
               }

               if( ( odt_type(            nxtrec ) == OC_STR )  &&
                   ( odt_next_offset(     nxtrec ) ==      1 )  &&
                   ( odt_previous_offset( nxtrec ) ==      1 )    )
               {

                  nxtrec -- ;

               }
            }

            if (rc == 0)
            {  /* If type is not string type set the flag */

               if (odt_type(nxtrec) != OC_STR)
               {
                  flag = TRUE;
               }
               /* following code is for computing the final ocd->name string */
               finrec = nxtrec;
               s_copy(&tempstr, odt_name(nxtrec), 0, 0);
               lvl = odt_lvl(nxtrec);

               while (lvl > 0)
               {
                  --nxtrec;
                  lvl1 = odt_lvl(nxtrec);

                  if (lvl1 < lvl)
                  {
                     s_copy(&tempstr1, odt_name(nxtrec),0,0);
                     s_cat(&tempstr1, tempstr1, period, 0,0);
                     s_cat(&tempstr, tempstr1, tempstr, 0,0);
                     lvl -= 1;
                  }
               }
            }
         }
         if ((what_to_get == STAY_IN_LEVEL) || (what_to_get == OCD_ONLY))
         {
            while (1)
             {
               if (offset > 0)
               {
                  if ((nextrec = odt_nxto(odtptr + currec)) > currec)
                  {
                     offset -= 1;
                     currec = nextrec;
                  }
                  else if (nextrec < currec)
                  {
                     rc = IMPROPER_DIRECTION_IN_ODT;
                     break;
                  }
                  else
                  {
                     rc = OCD_NOT_IN_ODT;
                     break;
                  }
               }
               else if (offset < 0)
               {
                  if ((nextrec = odt_prvo(odtptr + currec)) < currec)
                  {
                     offset += 1;
                     currec = nextrec;
                  }
                  else if (nextrec > currec)
                  {
                     rc = IMPROPER_DIRECTION_IN_ODT;
                     break;
                  }
                  else
                  {
                     rc = OCD_NOT_IN_ODT;
                     break;
                  }
               }
               else
               {  /* computing the final ocd->name string */

                  if (prevdot != 0)
                  {
                      s_copy(&tempstr1, ocdstr, 1, prevdot);
                  }
                  s_cat(&tempstr, tempstr1, odt_name(odtptr + currec), 0,0);
                  break;
               }
            }
         }
      }
     /* replace the ocd name by the one whose data we are going to get */

      if (rc == 0)
      {
     /* replace the ocd name by the one whose data we are going to get */
         s_nulout(&(ocd->name));
         s_copy(&(ocd->name), tempstr, 0, s_len(tempstr));

         if (what_to_get == OCD_ONLY)
         {
            s_drop (&tempstr) ;
            s_drop (&tempstr1) ;
            s_drop (&ocdstr ) ;
            s_drop (&period ) ;
            s_drop (&searstr) ;
   return(rc);
         }

         if (flag)
         {
           /* if flag is set then the data we are going to get is not */
           /* string type data                                        */

            size = odt_len(finrec, cocb->type);

            if (*dataptr == NIL)
            {
               *dataptr = (byte *)os_mema(size);
            }
            else if (size > *length)
            {  /* if size of data we are getting is greater than the */
               /* buffer length then dealocate the buffer and        */
               /* reallocate for the required size */
               os_memd(*dataptr);
               *dataptr = (byte *)os_mema(size);
            }
            *length = size;
            rc = obj_get(cocb, ocd, *dataptr);
         }

         else  /* type of data we are getting is string type */
         {
            rc = obj_get(cocb, ocd, dataptr);
            *length = s_len((string) (*dataptr));
         }
         s_drop (&tempstr) ;
         s_drop (&tempstr1) ;
      }
   }
   s_drop (&ocdstr ) ;
   s_drop (&period ) ;
   s_drop (&searstr) ;

   if( local_ocd )   /* if ocd was allocated in this routine then deall*/
   {
      ocd_drp ( & ocd );
   }

   return(rc);
}
