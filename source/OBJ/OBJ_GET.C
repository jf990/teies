/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_get                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 16, 1988 (phased out 07/02/87 version)         |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>                                                         |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block            |
  |            *ocd     struct OCD  INPUT    Pointer to the Object    |
  |                                          Component Descriptor to  |
  |                                          get from the DBASE       |
  |            *buffer  byte       OUTPUT    A pointer to a buffer    |
  |                                          that will contain the    |
  |                                          data which you requested.|
  |                                          If it is an OS FILE obj  |
  |                                          then this is a pointer to|
  |                                          a pointer to a teies str.|
  |                                                                   |
  |  <PURPOSE> This routine will return the object that you asked for |
  |            in 'ocd' and returns it in 'buffer'.                   |
  |                                                                   |
  |            If the OCB has the segment requested already active,   |
  |            then DO NOT call DMS_find() to get the data, use the   |
  |            current copy we have in the OCB.                       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocb_dbp (), odt_find (), dms_sdp (), dms_find (),      |
  |            dms_qd (), dms_qdl (), except ()                       |
  |  <ERRORS>  20023 : data requested was not found.                  |
  |            20024 : Forward search for key yielded no more keys.   |
  |            20020 : if OCD not found in ODT                        |
  |            20023 : if DMS returns null pointer or length          |
  |            20024 : Forward search for key yielded no more keys.   |
  |            20018 : if component is of invalid type                |
  |  <LIMITS>                                                         |
  |            NOTE: You better have COPIED the data base path that   |
  |            is assigned in the OCB or else this routine will DROP  |
  |            it if a new data base path is created!!                |
  |                                                                   |
  |            With the Local OS file handling, ocd->bias indicates   |
  |            what record to get.  The code should be updated to     |
  |            handle the case for record 0 and -1 (last record of the|
  |            file).                                                 |
  |                                                                   |
  |            With local OS file handling, ocb->buf_len has the file |
  |            record length.  If the buffer passed is not large      |
  |            enough or the record length is smaller than the real   |
  |            file record length then possible memory over run will  |
  |            occur.                                                 |
  |                                                                   |
  |            NEED TO REMOVE MULTIPLE returns AS IT LEAVES ALLOCATED |
  |            MEMORY DANGLING!!                                      |
  |                                                                   |
  |  <MODS>                                                           |
  |            06_Dec_88    JF         Changed the String component   |
  |                                    extraction logic to use temp   |
  |                                    variables with proper casting. |
  |                                    Also improved efficiency by    |
  |                                    adding pre-computation of      |
  |                                    pointers and amounts to copy.  |
  |                                                                   |
  |            19_Apr_89    JF         Changed direct request for KEYs|
  |                                    logic to not request the re-   |
  |                                    lated data segment.  Also, the |
  |                                    cache and last DBP are not up- |
  |                                    dated to avoid confusion with  |
  |                                    a component request with the   |
  |                                    same DBP.                      |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "stdio.h"
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "exc.h"
#include "obj.h"
#include "isv.h"
#include "file.h"



extern sint s_corrupt() ;
extern byte *fgets   () ;
extern sint strlen   () ;








sint   obj_get ( ocb, ocd, buffer )

OCB            * ocb        ;
OCD            * ocd        ;
byte           * buffer     ;



{


   string     dbp          = S_NULL ;
   byte      *dms_segment  =   NULL ,
             *seg_holder   =   NULL ,
             *ret_code     =   NULL ;
   ODT       *odtptr       =   NULL ;
   sint       rc           =   0    ,
              seglen       =   0    ;
   boolean    key          =   FALSE,
              got_from_DMS =    TRUE;






   if( ! OCB_POINTER_OK(ocb) )
   {

       except( INVALID_OCB, INFO, S_NULL,S_NULL,S_NULL,S_NULL,S_NULL );
   return(INVALID_OCB);

   }

if( (ocb->mode & LOCAL_OS_FILE) == FALSE )
{
                                  /* Get ODTrecord of the OCD          */
   odtptr = odt_find (ocb, ocd) ;
                                  /* Check to see if OCD found in ODT  */
   if ( ! ODT_POINTER_OK( odtptr ) )
   {

   return(OCD_NOT_IN_ODT);

   }


                                  /* Convert OCD to a DB path          */
   ocd_dbp (ocb, ocd, &dbp );


                                  /*************************************/
                                  /* Check to see if the new DBP is the*/
                                  /* same as the current DBP.  If so,  */
                                  /* then the segment in question is   */
                                  /* (or should be) already in the OCB.*/
                                  /*************************************/


   if( ocd_key( odtptr ) )
   {

      key         = TRUE;

   }
   else
   {

      key         = FALSE;

   }

   if ( 0 == s_comp (  ocb->lastdbp, dbp, 0, 0, 'M', NIL  ) )
   {

      if( key )
      {

         dms_segment = s_buffer( ocb->last_key );
         seglen      = s_len(    ocb->last_key );

      }
      else
      {

         dms_segment = ocb->last_seg;
         seglen      = ocb->last_seg_len;

      }
      got_from_DMS = FALSE;

   }
   else
   {


      dms_sdp(  ocb->dmscb, s_buffer ( dbp ), s_len ( dbp )  ) ;
      rc = dms_find (  ocb->dmscb  )  ;
      s_copy( & (ocb->lastdbp),  dbp,       0, 0 );
      s_copy( & (ocb->last_ocd), ocd->name, 0, 0 );
      got_from_DMS = TRUE;
                                  /*************************************/
                                  /* Check to see if key requested.    */
                                  /* This is a bad Tweek since it does */
                                  /* not handle multi valued keys or   */
                                  /* structured keys.  Only works if   */
                                  /* the Key component follows the NDX */
                                  /* segment component in the ODT.     */
                                  /*************************************/


      if(  key  )
      {

                                  /* Get number of keys and location   */
         if( rc != DMS_FIND_NO_DATA_FOUND )
         {

            string    dms_key = S_NULL;

                                                  /*********************/
                                                  /*We got the key, now*/
                                                  /*update the OCD with*/
                                                  /*the key and get the*/
                                                  /*associated data seg*/
                                                  /*Make sure to update*/
                                                  /*current DBP and seg*/
                                                  /*cache.             */
                                                  /*********************/
            seglen = dms_qkn (ocb->dmscb) ;

            dms_key = *(string *) dms_qk( ocb->dmscb ) ;
            dms_segment  = (byte *) dms_key;
            seglen  = s_len( (string) dms_segment );


            s_copy( &(ocd->key), (string) dms_segment, 0, 0 );
            s_drop( &(ocb->lastdbp) );
            rc = 0;

         }
         else
         {

            rc = NO_DATA_FOUND;

         }
      }
      else                        /* Else data requested               */
      {                           /* Get location and length of seg    */

         if( rc != DMS_FIND_NO_DATA_FOUND )
         {

            rc = 0;
            dms_segment = dms_qd  (ocb->dmscb) ;
            seglen      = dms_qdl (ocb->dmscb) ;

         }
         else
         {

            rc = NO_DATA_FOUND;

         }
      }
   }
                                  /*-----------------------------------*/
                                  /* Check to see if something found.  */
                                  /* If not, return the err message but*/
                                  /* first make sure the DBP is cleared*/
                                  /* so we do not confuse this failure */
                                  /* on the next obj operation.        */
                                  /*-----------------------------------*/

   if (dms_segment == NULL || seglen == 0)
   {

      if( rc == 0 ) {    rc = NO_DATA_FOUND;  }
      s_drop( &(ocb->lastdbp) );
      s_drop( & dbp );
   return(rc);

   }


                                  /*-----------------------------------*/
                                  /* Now make sure of the following:   */
                                  /*  - the segment length matches our */
                                  /*    buffer length.                 */
                                  /*                                   */
                                  /*  - the segment is the proper size.*/
                                  /*-----------------------------------*/


   ocb->buf_len = odt_len( odtptr, ocb->type );

   if( seglen < ocb->buf_len )
   {

      ocb->buf_len = seglen;

   }

   if( odtptr->type == OC_STR )
   {

      string   target        = S_NULL; /* Synonym for Buffer as a Str  */
      string   from_string   = S_NULL;
      sint     amount_to_get =      0;


      target = * (string *) buffer;
      if( key )
      {


         from_string = ocd->key;

      }
      else
      {

         from_string = (string) (dms_segment + odt_off( odtptr ));

      }
      if( (amount_to_get = s_len( from_string )) > 0 )
      {

         if ( s_len( target ) <= 0 )
         {

             s_alloc ( (string *)buffer, amount_to_get           );

         }
         else if ( ocb->buf_len > s_smp_al( target ) )
         {

            s_drop(  (string *)buffer                );
            s_alloc( (string *)buffer, amount_to_get );

         }

         target = * (string *) buffer;


         os_memc(   s_buffer( from_string ), s_buffer( target ),
                    amount_to_get                                );

         s_smp_ul( target ) = amount_to_get ;


      }
      else
      {

         rc = NO_DATA_FOUND;

      }
   }
   else if( (odtptr->type == OC_ISV) && ((ocb->mode & OBJ_RAW) == 0) )
   {
                            /* - - - - - - - - - - - - - - - - - - - - *\
                             |                                         |
                             |  If the component is of type ISV then   |
                             |  first we have to get the ISV key out   |
                             |  of the object.  It is stored as a 1/2  |
                             |  word.  Then call ISV_GET to get a      |
                             |  string and return that.  If the RAW    |
                             |  flag is set then we do not do this and |
                             |  just return the isvkey 1/2 word.       |
                             |                                         |
                            \* - - - - - - - - - - - - - - - - - - - - */
      hword   isvkey    =      0;
      string  isvholder = S_NULL;


      os_memc( dms_segment + odt_off( odtptr ), & isvkey, 2 );
      if( isvkey >= ISV_UNKNOWN && isvkey <= ISV_ISV_MAX )
      {

         rc = isv_get( & isvholder, isvkey,    0 );
         if( rc != 0 )
         {

            rc = NO_DATA_FOUND;

         }
         else
         {

            s_copy(   (string *) buffer,  isvholder, 0, 0       );

         }
      }
      else
      {

         s_nulout( (string *) buffer );

      }
   }
   else
   {

      if( key )
      {

         seg_holder = s_buffer( ocd->key );

      }
      else
      {

         seg_holder = dms_segment;

      }

      os_memc(seg_holder + odt_off(odtptr), buffer, ocb->buf_len) ;


   }
   if( got_from_DMS   &&   ! key )
   {

       rc = ocb_setcache( ocb, dms_segment, seglen );
       s_copy( & (ocb->lastdbp),  dbp,        0, 0 );

   }
   if( ocd->bias != 0 )
   {

       s_copy( &(ocd->key), *(string *)dms_qk(ocb->dmscb), 0, 0 );

   }
   if( s_len( ocd->key ) > 0 )
   {

      s_copy( & (ocb->last_key), ocd->key, 0, 0 );

   }
   s_drop ( & dbp );

                                  /* This is not a TEIES object so just*/
}                                 /* read the file record indicated by */
else                              /* the ocd bias.                     */
{

   string     str_buf= S_NULL ;   /* String containg line from file    */
   byte      *line   =   NIL  ;   /* String containg C Str from file   */


   str_buf = *((string *)buffer);
   if( ocb->buf_len < 1 )
   {

      ocb->buf_len = 132;

   }
   s_alloc( (string *)buffer, ocb->buf_len+1 );
   str_buf = *((string *)buffer);
   line = s_buffer(  str_buf );
   if( ocb->status > ocd->bias )
   {

      ocb->status = 0;
      while( ocb->status < ocd->bias - 1 )
      {

         ret_code = fgets( line, ocb->buf_len, ocb->dmscb );
         if( ret_code == NULL )
         {

            break;

         }
         ocb->status++;

      }
   }
   while( ocb->status < ocd->bias )
   {

      ret_code = fgets( line, ocb->buf_len, ocb->dmscb );
      if( ret_code == NULL )
      {

         break;

      }
      ocb->status++;

   }
   ret_code = fgets( line, ocb->buf_len, (FILE *)ocb->dmscb );
   if( ret_code == NULL )
   {

       rc = NO_DATA_FOUND;

   }
   else
   {
      string   new_line_char = S_NULL;
      sint     pos           =      0;

      ocb->status++;
      s_init( &(new_line_char), "\n" );
      s_smp_ul( str_buf ) = strlen(line)-1;
      if( (pos = s_last( str_buf, new_line_char, 0, 0 )) )
      {

         s_put( str_buf, ' ', pos );

      }
      s_trim( &str_buf, 'T' );
      s_drop( &(new_line_char) );

   }
}

   return(rc);
}
