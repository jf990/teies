/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_rep                                                |
  |  <AUTHOR>  Nestor Voronka, John L. Foster                         |
  |  <DATE>    January 21, 1988 (original July 2, 1987 )              |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb     struct OCB    INPUT  Pointer to the Object's  |
  |                                          Control Block.           |
  |                                                                   |
  |  <PARAM>   *ocd     struct OCD    INPUT  Pointer to the Component |
  |                                          Descriptor to replace.   |
  |                                                                   |
  |  <PARAM>   *buffer  void          INPUT  Pointer to section of    |
  |                                          memory containing data   |
  |                                          to place into new segmnt |
  |                                                                   |
  |  <PARAM>    flag    byte          INPUT  Flag stating when to do  |
  |                                          the DMS update           |
  |                                          Hold    -- SEG_HOLD      |
  |                                          Update  -- SEG_UPDATE    |
  |                                                                   |
  |  <PURPOSE> The purpose of this routine is to replace the indica-  |
  |            ted component of an object.  If the component does not |
  |            exist yet, it is created.                              |
  |                                                                   |
  |            If the pointer to the data to replace is NIL, we will  |
  |            NIL out the respective data field in the segment.      |
  |                                                                   |
  |            For replacement to operating system files, the bias    |
  |            field of the OCD has the record # to replace.  If 0    |
  |            then we just append to the end of the file.  The used  |
  |            length of the TEIES string data buffer is the number of|
  |            bytes in the record.  At this time, you can only write |
  |            one full record of the file at a time.                 |
  |                                                                   |
  |            Some notes on the logic:  There are 2 important var-   |
  |            iables: seg_allocated and segptr.  If seg_allocated is |
  |            true then the segment pointed to by segptr was alloca- |
  |            ted here and should eventually be released.  If the    |
  |            segment was not allocated here then the DMS has the    |
  |            memory and we need not worry about it.  If the flag    |
  |            argument is set to segment hold we only cache the seg- |
  |            ment if we allocated it, otherwise the DMS is not told |
  |            to write it until the flag is segment update.          |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   odt_ptr(), ocd_dbp(), ocd_find(), ocd_sgln(), os_memc()|
  |            f_write()                                              |
  |  <ERRORS>  20020 : OCD not valid for this object type.            |
  |            20023 : OCB not valid at all.                          |
  |            20051 : OCB is not in update mode.                     |
  |  <LIMITS>  If flag is not SEG_HOLD then SEG_UPDATE is assumed.    |
  |  <MODS>                                                           |
  |            11-02-88  JF    Corrected problem when a string is     |
  |                            longer than its allocated space in a   |
  |                            segment by forcing ocb->buf_len to the |
  |                            smaller of odt_len(component) and the  |
  |                            string buffer.                         |
  |            12-13-88  JF    Added logic to return if there is no   |
  |                            segment to update.                     |
  |            12-13-88  JF    Check if the replacement is a STR and  |
  |                            is the last field in the segment,      |
  |                            ( ocd_isvar() ); if so, then correct   |
  |                            the segment length to just the size of |
  |                            everything up to this string and the   |
  |                            s_len() of the string, NOT the reported|
  |                            segment length ocd_sgln() returns.     |
  |            04-07-89  JF    Added mode check for OBJ_UPDATE mode.  |
  |            04-19-89  JF    Corrected variable segment logic.      |
  |            01-09-90  JF    Added option to write out cache if ocd |
  |                            and data pointer are both NIL and the  |
  |                            flag is SEG_UPDATE.                    |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "stdio.h"
#include "file.h"
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "dms.h"
#include "obj.h"





sint obj_rep ( ocb, ocd, buffer, flag )

OCB        *ocb    ;              /* Pointer to the OBJ control block  */
OCD        *ocd    ;              /* The Obj Component Descriptor      */
void       *buffer ;              /* String containing data to replace */
byte        flag   ;              /* Indicates whether to 'H'old or to */
                                  /* 'U'pdate immediately              */


{



   string     dbp    = S_NULL ;   /* String containing DB path         */
   ODT       *ocdptr =   NIL  ;   /* Pointer to OCD's record in ODT    */
   byte      *segptr =   NIL  ,   /* Pointer to segment in memory      */
             *newseg =   NIL  ;   /* Pointer to possible new segment   */
   sint       nwsgln =   0    ,   /* Bytes needed for new segment      */
              count  =   0    ,   /* Counter for loop                  */
              rc     =   0    ,   /* Return code from functions        */
              seglen =   0    ,   /* Length  of segment in memory      */
              ocd_seg_len = 0 ;   /* Length of segment according to ODT*/
   boolean  seg_allocated = FALSE,/* True if obj_rep allocated the seg */
            nil_data      = FALSE,/* True if pointer to data is NIL    */
            got_from_dms  = FALSE,/* True if dms_find is called.       */
            key_replaced  = FALSE;/* True if OCD is a key component    */








if( ! OCB_POINTER_OK( ocb ) )
{

   except( INVALID_OCB, CONTROL_ERROR,
                        S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
   return(INVALID_OCB);

}



if( ! (ocb->mode & OBJ_UPDATE) )
{

   return(OBJECT_IS_READ_ONLY);

}


if( buffer == (byte *) -1 )
{

   except( INVALID_DATA_POINTER, CONTROL_ERROR,
                                 S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
   return(INVALID_DATA_POINTER);

}

if( ! (ocb->mode & LOCAL_OS_FILE) )
{
                                  /* * * * * * * * * * * * * * * * * * */
                                  /* Check if an update of the Cache is*/
                                  /* requested, and if so do it and get*/
                                  /* out.                              */
                                  /* * * * * * * * * * * * * * * * * * */
   if( (ocd == NIL)  &&  (buffer == NIL)  &&  (flag == SEG_UPDATE)
    && (s_len( ocb->lastdbp ) > 0)
    && (ocb->last_seg_len     > 0)      )
   {

      rc = ocb_segflush( ocb, OCB_CACHE_FLUSH );
   return(rc);

   }                                  /* * * * * * * * * * * * * * * * */
                                      /*                               */
                                      /* Get ODTrecord of the component*/
                                      /* so we know the defined segment*/
   ocdptr = odt_find( ocb, ocd );     /* attributes.                   */
   if ( ODT_POINTER_OK( ocdptr ) )    /*                               */
   {                                  /* * * * * * * * * * * * * * * * */

      ocd_seg_len = odt_sgln( ocdptr, ocb->type );
      if( ocd_seg_len < 1 )
      {

         rc = NO_SEGMENT_TO_REPLACE_DATA;

      }
   }
   else
   {

      rc = OCD_NOT_IN_ODT;

   }



if( rc == 0 )
{

   if( ocd_key( ocdptr ) )      /* * * * * * * * * * * * * * * * * * * */
   {                            /* Direct manipuation of the key of an */
                                /* index segment requires special      */
      key_replaced = TRUE;      /* handling as the key is moved to the */
                                /* OCD and is not in the segment.      */
   }                            /* * * * * * * * * * * * * * * * * * * */

   ocd_dbp ( ocb, ocd, & dbp ) ;

   if ( s_comp(  ocb->lastdbp, dbp, 0, 0, 'M', & rc ) != 0 )
   {
                                  /* * * * * * * * * * * * * * * * * * */
                                  /* If DB path is not same as last    */
                                  /* Set DB path and find the data from*/
                                  /* the DMS.  If we do have matching  */
                                  /* paths then the data is either in  */
                                  /* the OBJ Cache or can be found with*/
                                  /* dms_qd() without a call to dms_fnd*/
                                  /* * * * * * * * * * * * * * * * * * */
      if( obj_test_mode( ocb, OBJ_CACHE_PENDING ) )
      {

         rc = ocb_segflush( ocb, OCB_CACHE_CLEAR );

      }

      dms_sdp(       ocb->dmscb, s_buffer( dbp ), s_len( dbp ) ) ;
      rc = dms_find( ocb->dmscb );
      if( rc == DMS_FIND_DATA_FOUND )
      {

         rc           = 0;
         got_from_dms = TRUE;
         segptr       = dms_qd  ( ocb->dmscb   );
         seglen       = dms_qdl ( ocb->dmscb   );

      }
                                  /* - - - - - - - - - - - - - - - - - */
                                  /* Drop old DBP and set new last one */
                                  /* - - - - - - - - - - - - - - - - - */
      s_copy( & ( ocb->lastdbp ), dbp, 0, 0 );
                                  /* - - - - - - - - - - - - - - - - - */
                                  /* We will not yet put the segment in*/
                                  /* the OBJ Cache so continue to use  */
                                  /* the DMS memory to manipulate the  */
                                  /* segment.  Just remember to place  */
                                  /* it in the Cache on exit if SEGHOLD*/
                                  /* is set.                           */
                                  /* - - - - - - - - - - - - - - - - - */
      seg_allocated = FALSE;

   }
   else
   {

      seglen = ocb->last_seg_len;
      segptr = ocb->last_seg    ;

   }

                                    /* * * * * * * * * * * * * * * * * */
   if ( segptr==NIL || seglen==0 )  /*  DMS_find turned up nothing and */
   {                                /*  the OBJ Cache was empty, then  */
                                    /*  check to see if we have to     */
                                    /*  allocate a new segment. Other- */
                                    /*  wise use the Cache as the seg- */
                                    /*  ment data.                     */
                                    /* * * * * * * * * * * * * * * * * */
      seglen = ocd_seg_len;
      segptr = os_mema (seglen) ;
      os_memn( segptr, seglen );
      seg_allocated = TRUE;

   }


   if( ( (odt_offset( ocdptr ) + odt_len( ocdptr, ocb->type )) > seglen )
      && (ocd_seg_len > seglen) )
   {

      byte * new_segment = NIL;

      new_segment = os_mema( ocd_seg_len );
      os_memc( segptr, new_segment, seglen );
      if( seg_allocated )
      {

         os_memd( segptr );

      }                                                /****************/
      segptr        = new_segment;                     /*  Only Nullout*/
      seg_allocated = TRUE;                            /*  the extended*/
      os_memn( segptr+seglen+1, ocd_seg_len-seglen-1 );/*  portion of  */
      seglen        = ocd_seg_len;                     /*  the segment */
                                                       /****************/

   }
   else
   {

      ocd_seg_len = seglen;

   }
                                  /*************************************/
                                  /*Move data into the segment depends */
                                  /*on real data or NIL data pointer.  */
                                  /*If the OCD is a string then only   */
                                  /*move the USED bytes, NOT all the   */
                                  /*bytes reported in odt_len().       */
                                  /*************************************/

   if( odt_type( ocdptr ) != OC_STR )
   {

      ocb->buf_len = odt_len( ocdptr, ocb->type );

   }
   else
   {

      sint length_of_component = 0,
           length_of_string    = 0;

      length_of_component = odt_len( ocdptr, ocb->type );
      length_of_string    = s_len( (string) buffer );
      if( length_of_component < length_of_string )
      {

         length_of_string = length_of_component;

      }

      if( ocd_is_variable_length( ocdptr, ocb->type ) )
      {

         length_of_string = s_len( (string) buffer );
         seglen = length_of_string + odt_offset( ocdptr ) + S_OVERHEAD;

      }

      ocb->buf_len = length_of_string + S_OVERHEAD;

   }
   if( buffer != NIL )
   {

      os_memc( buffer, segptr + odt_off(ocdptr), ocb->buf_len ) ;

   }
   else
   {

      os_memn( segptr + odt_off( ocdptr ), ocb->buf_len );

   }

                                  /*************************************/
                                  /* If string data, correct used and  */
                                  /* allocated len inside the segment  */
                                  /*************************************/

   if (odt_type(ocdptr) == OC_STR)
   {

      string seg_offset    = S_NULL;
      hword  component_len = 0;


      seg_offset = (string) (segptr + odt_off( ocdptr ));
      component_len = odt_len( ocdptr, ocb->type ) ;
      s_smp_al (    seg_offset    ) = component_len;
      if ( s_len( (string) buffer ) > component_len)
      {

          s_smp_ul( seg_offset ) = (hword) component_len;

      }

   }
   obj_set_mode( ocb, OBJ_SYNC_PENDING );
   obj_set_mode( ocb, OBJ_CACHE_UPDATE );
                                             /* - - - - - - - - - - - */
                                             /* Set the Data so we can*/
                                             /* find the data for the */
                                             /* next replace. Actually*/
                                             /* replace the segment if*/
                                             /* requested to do so. We*/
                                             /* can release the segmen*/
                                             /* since SD actually move*/
                                             /* the data to a new spot*/
                                             /* = = = = = = = = = = = */
   if (  flag != SEG_HOLD  )
   {

      if( obj_test_mode( ocb, OBJ_CACHE_PENDING ) )
      {

         rc = ocb_segflush( ocb, OCB_CACHE_CLEAR );

      }
      else
      {

         rc = dms_sdp( ocb->dmscb, s_buffer( dbp ), s_len( dbp ) );
         if( rc == DMS_SDP_OK )
         {

            rc = dms_sd(  ocb->dmscb, segptr, seglen );
            if( rc == DMS_SD_OK )
            {

               rc = dms_rpl( ocb->dmscb );
               if( rc == DMS_REPLACE_DATA_STORED )
               {

                  rc = 0;
                  obj_set_mode(   ocb, OBJ_SYNC_PENDING );
                  obj_reset_mode( ocb, OBJ_CACHE_UPDATE );
                  s_drop( & ( ocb->lastdbp ) );

               }
               else
               {

                  rc = DATA_NOT_REPLACED;

               }
            }
         }
      }
      if( seg_allocated )
      {

         os_memd( segptr );
         segptr = NIL;
         seglen =   0;

      }
   }
   else
   {
                                  /*************************************/
                                  /* SEG_HOLD was requested so we must */
                                  /* put things away properly.  If the */
                                  /* segment was already in the Cache, */
                                  /* leave it alone.  If dms_find found*/
                                  /* nothing and we allocated a new seg*/
                                  /* then just place that in the Cache.*/
                                  /* If dms_find gave us a segment then*/
                                  /* copy that one into the Cache (as  */
                                  /* we directly manipulated it through*/
                                  /* out this routine).                */
                                  /*************************************/
      if( got_from_dms )
      {

         rc = ocb_setcache( ocb, segptr, seglen   );

      }
      else
      {

         if( seg_allocated )
         {

            rc = ocb_setcache( ocb, segptr, seglen );
            os_memd( segptr );

         }
      }
      rc = 0;

   }
}
s_drop( & dbp );

}
else
{

   rc = fwrite( s_buffer( (string)buffer ), s_len( (string)buffer ), 1,\
                (FILE *)ocb->dmscb                     );
   if( rc != 1 )
   {

      rc = DATA_NOT_REPLACED;
      except( rc, DATA_ERROR, ocb->gfd, S_NUL, S_NUL, S_NUL, S_NUL );

   }
   else if( ! obj_test_mode( ocb, OBJ_BINARY_MODE ) )
   {

      byte * new_line = "\n" ;


      fwrite( new_line, 1, 1, ( FILE * ) ocb->dmscb  );
      rc = 0;

   }
}


   return(rc);
}
