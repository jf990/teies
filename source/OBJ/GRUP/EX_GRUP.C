/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ex_grup()                                              |
  |  <AUTHOR>  Rana Ajaz-Ur-Rehman,  Tanmay S. Kumar                  |
  |  <DATE>    September 6, 1988     August 16, 1989                  |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   list_ocb    OCB **    INPUT   Pointer to list object   |
  |  <PARAM>   what_to_get string    INPUT   string to check only one |
  |                                          id at a time.            |
  |  <PARAM>   search_type hword     INPUT   defined in obj_group.h   |
  |                                          and could be one of the  |
  |                                          following                |
  |                 (default)                CHECK_GRUP_ID            |
  |                                          CHECK_GRUP_LABEL         |
  |                 (default)                CHECK_GRUP_ALL           |
  |                                              AND                  |
  |                 (default)                G_NEED_ONE_OID           |
  |                                          G_NEED_MANY_OID          |
  |                                              AND                  |
  |                 (default)                STRING_TO_OID            |
  |                                             (last one is not )    |
  |                                             (implemented yet )    |
  |  <PARAM>   params      void*     INPUT   used to return the oid if|
  |                                          only one is asked for.   |
  |                                          or bad_string for listobj|
  |  <PURPOSE> Check for existance of  Group/(s)                      |
  |            Depending upon the above mentioned "search_type"       |
  |            this routine checks for the required component         |
  |            (i.e ID, LABEL, OID) passed to it on the master server.|
  |            if the request is made on the local server. If the     |
  |            request is made on the master server then the user must|
  |            set the flag for G_NO_NEED_TO_XMIT in the search type. |
  |  <OUTPUT>  sint                                                   |
  |  <ERRORS>  7680 : GROUP_ID_ALREADY_IN_SGI                         |
  |            7681 : GROUP_ID_NOT_IN_SGI                             |
  |            7682 : GROUP_LABEL_ALREADY_IN_SGI                      |
  |            7683 : GROUP_LABEL_NOT_IN_SGI                          |
  |            7684 : GROUP_ALREADY_EXISTS                            |
  |            7685 : GROUP_DOES_NOT_EXIST                            |
  |            7686 : GROUP_ID_NOT_PROVIDED                           |
  |            7687 : GROUP_LABEL_NOT_PROVIDED                        |
  |            7688 : GROUP_ID_AND_LABEL_ALREADY_IN_CRI               |
  |  <CALLS>   t_sysndx(), except(), ocd_new(), obj_get(),            |
  |            s_copy();                                              |
  |            oid_cmp()  which is a macro                            |
  |  <ERRORS>  20995 : CRI_MISSING_OR_INVALID                         |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_group.h"



sint     ex_grup( list_ocb, what_to_get, search_type, params )

OCB      **list_ocb;
string     what_to_get;
hword     search_type;
void     **params;

{

   OCB      *sgi_ocb        =     NIL,
           *glist_ocb       =     NIL;


   OCD      *sgi_ocd        =     NIL,
            *grup_ocd       =     NIL;

   string   grup_id         =     S_NULL,
            result_str      =     S_NULL,
            return_string   =     S_NULL,
            parser          =     S_NULL,
            extra           =     S_NULL,
            g_prefix        =     S_NULL,
            prefix          =     S_NULL,
            item            =     S_NULL,
            semi_colon      =     S_NULL,
            *strvec1        =        NIL,
            *strvec2        =        NIL,
            grup_label      =     S_NULL;

   sint     list_size       =          0,
            i               =          0,
            xc              =          0,
            rc              =          0,
            rrc             =          0;

   hword    on_ms_already   =          0,
            conv_type       =          0,
            what_is_needed  =          0;

   OID      *grp_oid         =        NIL,
            sgi_oid;


on_ms_already = search_type & 0x0f00;           /* if calling from MS */
what_is_needed = search_type & 0xf000;             /* get top nibble */

if ( TM_master->usrtype == MASTER_SERVER )
{
   sgi_ocb = t_sysndx( SGI_OBJ );
   if ( ! OCB_POINTER_OK( sgi_ocb ) )
   {
      except( CRI_MISSING_OR_INVALID, CONTROL_ERROR, S_NULL,
                              S_NULL, S_NULL, S_NULL, S_NULL );
      rrc = CRI_MISSING_OR_INVALID ;
   }
   conv_type = search_type & 0x000f;             /* get lower byte */


/* if what_is_needed is not G_NEED_ONE_OID we assume it to be many oid */

   if ( ( (what_is_needed == G_NEED_ONE_OID) && (params == NIL) &&
                                     (on_ms_already == G_ON_MS_ALREADY) )
      || ( (list_ocb == NIL) && (on_ms_already == G_ON_MS_ALREADY) ) )
   {
      rrc = CANNOT_STORE_RESULT_OID;
   }
   else
   {
      if (what_is_needed == G_NEED_ONE_OID)
      {
         if ( (params != NIL) && (*params != NIL) )
         {
            grp_oid = (OID *)*params;
         }
         else
         {
            grp_oid = (OID *) os_mema(sizeof(OID) );
            os_memn(grp_oid, sizeof (OID) );
            if ( (params != NIL) && (*params == NIL) )
            {
               *params = grp_oid;
            }
         }
         s_valloc( & strvec1, 4                  );
         s_valloc( & strvec2, 3                  );
      }
      else
      {
         what_is_needed = G_NEED_MANY_OID;
         if ( (list_ocb != NIL) && (*list_ocb != NIL) )
         {
            glist_ocb = *list_ocb;
         }
         else
         {
            glist_ocb = obj_all(LIST_OBJ, LOCAL_OBJECT, NIL);
            if ( (list_ocb != NIL) && (*list_ocb == NIL) )
            {
               *list_ocb = glist_ocb;
            }
         }
         s_valloc( & strvec1, 40                 );
         s_valloc( & strvec2, 3                  );

      }
      if (rrc == 0)
      {
         s_init(&semi_colon, ";" );
        /* parse out string based on ',' supposedly blanks are removed */
         s_init(   & parser, "@~.. @n ,."       );
         s_parse(  & what_to_get, parser, S_NULL, strvec1, &extra   );

         if  ( s_len ( (string)strvec1 ) > 0 )
         {
            isv_get( &g_prefix,   ISV_GROUP, 1 );
            s_init(   & parser, " @$n @n "       );
            ocd_new(&grup_ocd, "LIST.LIST.OID");
            ocd_new( &sgi_ocd, "SGI.ID.OID"             );
            os_memn(&sgi_oid, sizeof (OID) );

            for ( i=1; i<= s_len( (string)strvec1 ); i++ )
            {
               s_copy(&item, *(strvec1 + i), 0, 0);
               switch( conv_type )
               {
                   case CHECK_GROUP_ID :
                   case CHECK_GROUP_ALL :
                        s_parse(&item, parser, S_NULL, strvec2, &extra);
                        if (s_len( (string) strvec2) >= 2)
                        {
                           s_copy(&prefix, *(strvec2 + 1), 0, 0);
                           if ( (s_len(prefix) == 0)
                            || (s_comp(prefix,g_prefix,0,0,'F',&xc)== 0))
                           {
                              s_copy(&(sgi_ocd->key),*(strvec2 + 2),0,0);
                              s_init(&(sgi_ocd->name), "SGI.ID.OID" );
                              rc = obj_get( sgi_ocb, sgi_ocd, &sgi_oid);
                              if ( rc == 0 )
                              {
                                 rrc = GROUP_ID_ALREADY_IN_SGI;
                                 break;
                              }
                              else
                              {
                                 rrc = GROUP_ID_NOT_IN_SGI;
                                 /* check for label starting with G */
                              }
                              if (conv_type == CHECK_GROUP_ID)
                              {
                                 break;
                              }
                           }
                           else
                           {
                              /* assume that a label was passed */
                           }
                        }

                   case CHECK_GROUP_LABEL :

                         s_copy(&(sgi_ocd->key),*(strvec1 + i), 0, 0);
                         s_init(&(sgi_ocd->name), "SGI.LABEL.OID" );
                         rc = obj_get( sgi_ocb, sgi_ocd, &sgi_oid      );
                         if ( rc == 0 )
                         {
                            rrc = GROUP_LABEL_ALREADY_IN_SGI;
                         }
                         else
                         {
                            if (conv_type == CHECK_GROUP_ALL )
                            {
                               rrc = GROUP_LABEL_OR_ID_NOT_IN_SGI;
                            }
                            else
                            {
                               rrc = GROUP_LABEL_NOT_IN_SGI;
                            }
                         }
                         break;

                   default :
                         rc = NO_DATA_FOUND;
                         rrc = GROUP_ID_NOT_PROVIDED;
                         break;

               }  /* closing switch statement  */
               if  (what_is_needed == G_NEED_ONE_OID)
               {
                  os_memc(&sgi_oid, grp_oid, sizeof(OID));
                  break;
               }
               if  (what_is_needed == G_NEED_MANY_OID)
               {
                  s_init(&(grup_ocd->name), "LIST.LIST.OID");
                  s_alloc(&(grup_ocd->key), 4);
                  s_smp_ul(grup_ocd->key) = 4;
                  os_memc( &i, s_buffer(grup_ocd->key ), 4 );
                  obj_rep(glist_ocb, grup_ocd, &sgi_oid, SEG_UPDATE);
                  s_init(&(grup_ocd->name), "LIST.LIST.ENTRY");
                  obj_rep(glist_ocb, grup_ocd,*(strvec1 + i),SEG_UPDATE);
                  obj_find(glist_ocb, "LIST.SIZE", &list_size);
                  list_size++;            /* ...increment size of list */
                  obj_upd(glist_ocb, "LIST.SIZE", &list_size);
                  if (rc != 0)
                  {
                     if (params != NIL)
                     {
                       s_cat((string *)params, (string)*params,
                                                   *(strvec1 + i), 0, 0);
                       s_cat((string *)params,(string)*params,semi_colon,
                                                                   0, 0);
                     }
                  }
               }
            }
            ocd_drp(&grup_ocd);
            ocd_drp(& sgi_ocd);
            if ( (what_is_needed == G_NEED_MANY_OID) && (params != NIL)
                                       && (s_len((string)*params) > 0))
            {
               s_smp_ul((string)*params) = s_len((string)*params) - 1;
            }
         }
      }
      if ( on_ms_already != G_ON_MS_ALREADY)
      {
         if (what_is_needed == G_NEED_ONE_OID)
         {
            s_alloc(&return_string, sizeof(OID) );
            s_smp_ul(return_string) = sizeof(OID);
            os_memc(grp_oid, s_buffer(return_string), sizeof(OID) );
            t_csend( t_invoker(), rrc, return_string );
         }
         else if (what_is_needed == G_NEED_MANY_OID)
         {
            rc = t_m_obj(t_invoker(), glist_ocb,
                      (void **)(&result_str), FALSE, (sint)search_type);
            t_csend( t_invoker(), rrc, (string)*params );

         }
      }
      s_vdrop(&strvec1);
      s_vdrop(&strvec2);
   }
   if (OCB_POINTER_OK(sgi_ocb) )
   {
      obj_cls(sgi_ocb);
   }
}
else
{
   if (s_len(what_to_get) >= 0)
   {

      if (what_is_needed == G_NEED_ONE_OID)
      {
         if (params != NIL)
         {
            if (*params == NIL)
            {
               *params = (OID *) os_mema (sizeof (OID) );
            }
            os_memn(*params, sizeof (OID) );
            rrc = t_remote(TM_master->MS,GROUP_OBJ,EXIST_ACT,search_type,
                                   NIL, what_to_get , (void **)(params));
         }
         else
         {
            rrc = CANNOT_STORE_RESULT_OID;
         }
      }

      if ( (what_is_needed != G_NEED_ONE_OID) && (list_ocb != NIL) &&
                                                     (*list_ocb != NIL) )
      {
         rrc = t_spawn(*list_ocb,what_to_get,search_type,
                                                  (void**)(&result_str));
         rc = t_waitc( TM_master->MS, result_str );
      }
      else if (list_ocb != NIL)
      {
         rrc = t_remote(TM_master->MS, GROUP_OBJ, EXIST_ACT, search_type,
                            NIL, what_to_get , (void **)(&result_str) );
         rc = t_waitc( TM_master->MS, result_str );

      }
   }
   else
   {
      rrc =  GROUP_ID_OR_LABEL_NOT_PROVIDED;
   }
   s_drop(&semi_colon);
   s_drop(&extra);
   s_drop(&prefix);
   s_drop(&parser);
   s_drop(&item);
}
   return(rrc);

}
