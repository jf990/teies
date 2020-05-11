/********************************************************************

    :NAME.               ocd_scn.C
    :LEVEL.              OBJ
    :AUTHOR.             Steve Kuo
    :DATA.               June 22, 1987
    :INPUT.
           *ocd                 pointer to ocdstr and index key
           *odt                 pointer to the odt table
           *odt_entry           pointer to the target ocd in odt
           *scn_ptr             pointer to the ocd to be scanned
           **data               pointer to the pointer of the dbp
           *len                 pointer to the length of dbp
            tag_lvl             level of the target ocd
            tag_num             record # of the target ocd in odt
    :OUTPUT.
           *DATA   LEN
            ->     ->    return 0 always

    :INPUT_GLOBALS.
     boolean     seg_typ(:MAX:)      is there segment type ocd within the
                                       level
     boolean     non_seg(:MAX:)      is there non segment type ocd within
                                       the level
     boolean     tag_seg(:MAX:)      is this the target segment we want
     string      path   (:MAX:)      teies strings represent dbp of the level
     sint        seg_cnt(:MAX:)      segment type ocd count within the level


    :OUTPUT_GLOBALS.
    :PURPOSE.
            Generate data base path (dbp) for the given odtentry in the
            given odt table. The dbp will be pointed to by pointer data,
            and len pointer points to the length of it.

    :CALLS. ocd_scn(), s_init(), s_put()
    :ERRORS.
    :LIMITS.
            ocd level MAX 20.
            Same level non segment type ocd must show before segment
            type ocd in the odt.
            Input odt_entry can not be segment type ocd.
            No multi index segment available currently.

    :MODS.
     Date        Author                 Description
    09-01-87     JF             Altered recursive call to ocd_scn to
                                increment OCD pointer in the call.
 ********************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj_i.h"
#include "debug.h"

#define  MAX      20                /* MAX may not bigger than 254 */

/*  global variables  */
/*------------------------------*/
extern boolean    seg_typ(:MAX:);
extern boolean    non_seg(:MAX:);
extern boolean    tag_seg(:MAX:);
extern string     path   (:MAX:);
extern sint       seg_cnt(:MAX:);




/*--------------------------------------------------------------------*/
sint   ocd_scn (ocd, odt, odt_entry, scn_ptr, data, len, tag_lvl, tag_num)

ocdrecord  *ocd    ;        /* INPUT, pointer to ocdstr and indx key   */
odtrecord  *odt,            /* INPUT,pointer to the odt table          */
           *odt_entry,      /* INPUT,pointer to the target ocd in odt  */
           *scn_ptr;        /* IN/OUT,pointer to the ocd to be scanned */
string     *data   ;        /* IN/OUT,pointer to the Tstr ptr to DBP   */
tint       *len    ;        /* IN/OUT,pointer to the length of dbp     */
byte        tag_lvl;        /* INPUT,level of the target ocd           */
sint        tag_num;        /* INPUT,record # of the target ocd in odt */

{
   sint      cur_num ;          /* record # of the scanning ocd        */
   byte      cur_lvl ;          /* level of the scanning ocd           */
   hword     cur_typ ;          /* type of the scanning ocd            */
   byte      prv_lvl ;          /* previous level of scanning level    */
   string    cur_path=S_NULL;   /* current path of that level          */
   string    offset  =S_NULL;   /* a string contains bias              */
   string    index   =S_NULL;   /* a string contains index             */
   byte     *temp;              /* use to put bias into string offset  */
   boolean   compute = FALSE;   /* determine to compute dbp or not     */
   boolean   end_lvl = FALSE;   /* if reach the level-end              */
   sint      i       = 0,       /* temporary variable                  */
             rc      = 0;

#D_start(ocd_scn,OBJ,\
'*ODT=x%x; *TODT=x%x; *OCD=x%x; *LEN=x%x(%d); TLVL=%d; TNUM=%d',\
'odt,odt_entry,scn_ptr,len,*len,tag_lvl,tag_num'                );
#D_str( '*data','Data base path string pointer:'                );


   cur_lvl = odt_lvl( scn_ptr );
   cur_typ = odt_type( scn_ptr );
   cur_num = ocd_num( odt, scn_ptr );
   s_init( &cur_path, "                                   " );
   if ( odt_nxto( scn_ptr ) < cur_num )
      end_lvl = TRUE;
   else
      end_lvl = FALSE;

   /*-------------------------------------------------
     skip root segments that do not contain the target ocd
     We only need to know how many roots have passed, which
     is monitored in seg_cnt(0). We are at a root when cur_lvl == 0.
     -------------------------------------------------*/
   if ( cur_lvl == 0 && odt_nxto( scn_ptr ) <= tag_num && !end_lvl )
   {

      #D_say('We think we are at a Level 0 OCD' );
      scn_ptr = odt + odt_nxto( scn_ptr );
      seg_cnt(:0:) = seg_cnt(:0:) + 1;
      #D_show('Segment counter = %d','seg_cnt(:0:)');

   }
   else
   {
      /*--------------------------------------------------
        skip ocd's whose level lower than the target ocd
        because it doesn't affect the dbp computing
        --------------------------------------------------*/
      if ( cur_lvl <= tag_lvl )
      {
         /*--------------
           begin scanning
           --------------*/
         #D_show ('scan ocd # %d ', 'cur_num');

         if ( cur_lvl != 0 )
            prv_lvl = cur_lvl - 1;
         else
         {
            prv_lvl = 0;
            path(:prv_lvl:) = S_NULL;
         }

         if (cur_typ==OC_SEG  || cur_typ==OC_NDX || cur_typ==OC_ARY ||
             cur_typ==OC_HSEG || cur_typ==OC_HHSG                    )
         {
            seg_typ(:cur_lvl:) = TRUE;
            seg_cnt(:cur_lvl:) = seg_cnt(:cur_lvl:) + 1;

            /*------------------------------------------------------
              find the target segment which covers target ocd inside
              ------------------------------------------------------*/
            if ( odt_nxto( scn_ptr ) > tag_num )
            {
               tag_seg(:cur_lvl:) = TRUE;
               compute = TRUE;
            }
            else
            {
               if ( end_lvl )
               {
                                           /* this bring you some time */
                                           /* to figure out            */
                  if ( tag_seg(:prv_lvl:) || cur_lvl == 0 )
                  {
                    tag_seg(:cur_lvl:) = TRUE;
                    compute = TRUE;
                  }
                  else
                  {
                    tag_seg(:cur_lvl:) = FALSE;
                    compute = FALSE;
                  }
               }
               else
               {
                 tag_seg(:cur_lvl:) = FALSE;
                 compute = FALSE;
               }
            } /* end find target segment */

            /*----------------------
              compute data base path
              ----------------------*/
            if ( compute && cur_num < tag_num )
                                           /* should in case the seg   */
                                           /* is below the target ocd  */
            {
               #D_say( 'compute data base path' );
               if (non_seg(:cur_lvl:))
                                           /* this is the limitation   */
                                           /* that non_seg should      */
                                           /* ahead of seg type ocd    */
                                           /* within same level        */
                  s_put( cur_path, seg_cnt(:cur_lvl:) + 1, 1);
               else
                  s_put( cur_path, seg_cnt(:cur_lvl:), 1);
               #D_show( 'current seg count = 0x%X','*s_buffer( cur_path )');

               /*------------------------------------------
                 put key & offset into dbp if index segment
                 ------------------------------------------*/
               if (cur_typ == OC_NDX)
               {
                  #D_say( 'putting key & offset into dbp' );
                  s_init( &index,"1" );
                  i = s_len( ocd->key ) + 128;
                  s_put( index,  i,         1 );
                  #D_show( 'index = 0x%x','i' );
                  s_cat( &cur_path, cur_path, index, 1, 1 );
                  s_init( &offset,"1234" );
                  temp = &ocd->bias;            /* to take out one byte hex */
                  s_put( offset, *temp,     1 );
                  s_put( offset, *(temp+1), 2 );
                  s_put( offset, *(temp+2), 3 );
                  s_put( offset, *(temp+3), 4 );
                  #D_show( 'offset = 0x %X %X %X %X','temp(:0:),temp(:1:), \
                            temp(:2:),temp(:3:)' );
                  s_cat( &cur_path, cur_path, offset, 1, 4 );
                  s_cat( &cur_path, cur_path, ocd->key, 0, 0 );
                  #D_say( 'key =' );
                  #D_mdump( 's_buffer(ocd->key)','s_len(ocd->key)');
                  #D_show( 'index= 0x%X  offset= 0x%X','index, offset');
                  s_drop( &index );
                  s_drop( &offset );
               } /* end index segment */

               /*------------------------------------------
                 generate data base path upto current level
                 ------------------------------------------*/
               s_cat( &(path(:cur_lvl:)), path(:prv_lvl:), cur_path, 0, 0 );
               #D_show( 'current path for cur_lvl  %d :','cur_lvl');
               #D_mdump('s_buffer(path(:cur_lvl:))','s_len(path(:cur_lvl:))');

            } /* end compute dbp */
            else
               #D_say( 'not target segment' );

         } /* end segment type ocd */
         else
         {
            non_seg(:cur_lvl:) = TRUE;
            #D_say( 'non segment type ocd' );
         }

         /*---------------------------
           if end of the current level
           we need to do the reset
           ---------------------------*/
         if ( end_lvl )
         {

            if ( cur_lvl == tag_lvl && cur_num >= tag_num )
            {
                                                 /*----------------------
                                                 if reach the level-end
                                                 of target ocd
                                                 then recursive ends
                                                 now comes what we want,
                                                 we found the OCD and
                                                 logged all levels to it.
                                                 You won't know until
                                                 reach target level end
                                                 that's why it ends here
                                                 ----------------------*/
               #D_say('Current Level, Tag Level and Counters all match');
               if ( seg_typ(:cur_lvl:) )
               {

                  #D_say('Updating path strings:');
                  s_put( cur_path, '\001', 1 );
                  #D_say('Now set the string vectors in the globalarea');
                  s_cat( &(path(:prv_lvl:)),path(:prv_lvl:),cur_path,1,1);
               }
               #D_str(      '*data',        ' path string ' );
               #D_str(  'path(:prv_lvl:)' , ' path string ' );
               *len = s_len( *data );
               s_copy( path(:prv_lvl:), *data, 0, s_len(*data) );

               #D_say(   'dbp result from ocd_scn:'                );
               #D_mdump( '*data','*len'                            );
               #D_str(   '*data','Data base path'                  );
               #D_show ( 'data_ptr= %X, len_ptr= %X.n', '*data,len');
               #D_show ( 'length  = %d','*len'                     );

               /*----------------------
                 start recursive return
                 ----------------------*/
               #D_show( 'drop ocd # %d cur_path','cur_num');
               s_drop( &cur_path );
               s_drop( &index    );
               s_drop( &offset   );
               #D_return(0);

            } /* end target level-end */

            /*------------------------------------------------------
              reset level counters and flags for next same level ocd
              ------------------------------------------------------*/
            non_seg(:cur_lvl:) = FALSE;
            seg_typ(:cur_lvl:) = FALSE;
            seg_cnt(:cur_lvl:) = 0;
         } /* end reset */
      } /* end ocd which level higher than target ocd */
   } /* end root segment that contains the target ocd */

   /*---------------
     recursive start
     Go to the next OCD in the ODT and try again.
     ---------------*/

   ocd_scn (ocd, odt, odt_entry, ++scn_ptr, data, len, tag_lvl, tag_num);

   /*----------------
     recursive return
     ----------------*/
   #D_show( 'drop ocd # %d cur_path','cur_num');
   s_drop( &cur_path );

   #D_return(0);
}

