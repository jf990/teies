/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     MD_LIST C                                              |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <PARAM>    list_ocb   OCB*       IN   Indicates LIST to add new   |*/
/*|                                          OID's to.                |*/
/*| <PARAM>    modifer    string     IN   Sort data component for add |*/
/*|                                          operation; else ignored. |*/
/*| <PARAM>    level      hword      IN   Indicates operation as      |*/
/*|                                          elaborated in PURPOSE.   |*/
/*| <PARAM>    Parms      *          I/O  Pointer to item data (OID   |*/
/*|                                          or ENTRY string), to add |*/
/*|                                          or removed, ignored for  |*/
/*|                                          modify operation.        |*/
/*| <PURPOSE>  Adds elemets to list when                              |*/
/*|            level = OBJ_LIST_ADD, deletes elements from list if    |*/
/*|            level = OBJ_LIST_DEL, else loops through list when     |*/
/*|            level = MODIFY_ACT.                                    |*/
/*|                                                                   |*/
/*|            As the nature of the modifier data string does not     |*/
/*|            matter, any string data can be used for the sort key.  |*/
/*|                                                                   |*/
/*|            When level is OBJ_LIST_ADD_ENTRY, then an ENTRY field  |*/
/*|            is filled in for the the item if it exists or one      |*/
/*|            without an OID is created.  If no key (modifer) is     |*/
/*|            provided, the default is used.                         |*/
/*|                                                                   |*/
/*|            When level is OBJ_LIST_DEL_ENTRY, then the ENTRY is    |*/
/*|            deleted for the item indicated by the key.             |*/
/*|                                                                   |*/
/*| <OUTPUT>   sint                0 for success or error RC.         |*/
/*| <CALLS>    ocd_new(), obj_rep(), obj_all(), obj_get()             |*/
/*| <MODS>     30_Nov_88   K.J.Walsh   Implemented.                   |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "obj_prim.h"

sint    md_list( list_ocb, modifier, level, params )

OCB             *list_ocb;             /* Target list (if defined)     */
string           modifier;             /* Key data (for ADD or DEL)    */
hword            level;                /* Action parmeter              */
void            *params;               /* ADD/DEL OID                  */
{

   sint      rc       = 0    ;         /* Presumed RC                  */
   sint      get_rc   = 0    ;         /* RC from get_obj()            */
   sint      list_size= 0    ;         /* Number of items in index     */

   OCD       *list_ocd= NIL  ;         /* OCD of list object (for add) */
   OID       *add_oid = NIL  ;         /* OID to be added              */
   OID      **store_oid=NIL  ;         /* OID to be added              */

   OID        moda_oid;                /* An object to modify          */
   OCB       *moda_ocb;                /* An object to modify          */
   hword      moda_type;               /* Type of object for t_jmpst   */
   boolean    more_in_list = TRUE;     /* More oids in list being mod? */

   OCD       *del_ocd = NIL   ;        /* OCD to find by key for DEL   */

   OID        oid_in,                  /* params input for OID compare */
              dummy_oid;               /* Used to see if data already  */
                                       /* exists                       */



   /*+----------------------------------------------------------------+*/
   /*| Process only if we have a good list...                         |*/
   /*+----------------------------------------------------------------+*/
   if ( OCB_POINTER_OK(list_ocb) )
   {
      switch(level)
      {

         case OBJ_LIST_ADD:
            /*+-------------------------------------------------------+*/
            /*| Adds an OID to the existing LIST with key "modifier"  |*/
            /*+-------------------------------------------------------+*/
            ocd_new(&list_ocd,"LIST.LIST.OID");
            if ( s_len(modifier) == 0 )
            {
               /*+----------------------------------------------------+*/
               /*| No object component named--use the current date &  |*/
               /*| time as the list sort key.                         |*/
               /*+----------------------------------------------------+*/
               s_alloc( &( list_ocd->key ),  sizeof( datetime ));
               s_smp_ul(   list_ocd->key ) = sizeof( datetime  );
               os_dttm((datetime*)s_buffer(list_ocd->key));
            }
            else
            {

               s_copy(&(list_ocd->key),modifier,0,0);

            }
                                  /* Get is made before a replace so if*/
                                  /* data already exists, we do not    */
                                  /* have to write it or increment the */
                                  /* size of the list. This should be  */
                                  /* be taken out once obj_rep returns */
                                  /* an indication that it over-wrote  */
                                  /* already existing data so we do not*/
                                  /* increment the list size./Sal011589*/

            rc = obj_get( list_ocb, list_ocd, &dummy_oid );

            if( rc == NO_DATA_FOUND )
            {


               rc = obj_rep(list_ocb, list_ocd, params, SEG_UPDATE);

               if ( rc == 0 )             /* If successfully replaced..*/
               {

                  obj_find(list_ocb,"LIST.SIZE",&list_size);
                  list_size++;            /* ...increment size of list */
                  obj_upd(list_ocb,"LIST.SIZE",&list_size);

               }

            }  /* if no data was found */

         break;

         case MODIFY_ACT:
            /*+-------------------------------------------------------+*/
            /*| Loop through list and do modify action for each       |*/
            /*| element in list.                                      |*/
            /*+-------------------------------------------------------+*/
            ocd_new(&list_ocd,"LIST.LIST.OID");
            list_ocd->bias = 1;

            do /* While there is anything left in list */
            {

               rc = obj_get(list_ocb,list_ocd,&moda_oid);

               if ( rc == 0 )          /* Apparently valid obj found   */
               {

                  moda_ocb = obj_opn(&moda_oid,OBJ_READ);
                  moda_type = obj_type( &moda_oid );
                  rc = t_jmpst(moda_type,MODIFY_ACT,OBJ_LEVEL_ZERO,
                     S_NUL,moda_ocb,NIL);
                  if ( rc != 0 )
                  {
                  }
               }
               else if ( rc == NO_DATA_FOUND )
               {
                  more_in_list = FALSE;
                  rc = 0;              /* Not an error for caller      */
               }
               else
               {
                  more_in_list = FALSE;
               }
            }
            while( more_in_list );
            break;

         case OBJ_LIST_DEL:
            /*+-------------------------------------------------------+*/
            /*| Delete the item indicated by either Key or OID.       |*/
            /*+-------------------------------------------------------+*/
            ocd_new(&list_ocd,"LIST.LIST.SORT_KEY");
            if ( s_len(modifier) == 0 )
            {
               list_ocd->bias = 1;

               do /* While there is anything left in list */
               {

                  rc = obj_get(list_ocb,list_ocd,&moda_oid);

                  if ( rc == 0 )       /* Apparently valid obj found   */
                  {
                     /*+----------------------------------------------+*/
                     /*| If this is OID we want, delete and fake end  |*/
                     /*| of list, decrement list size.                |*/
                     /*+----------------------------------------------+*/
                     oid_in = *((OID*)params);
                     if ( oid_cmp(oid_in,moda_oid) == TRUE )
                     {
                        rc = obj_rmv(list_ocb,list_ocd);
                        more_in_list = FALSE;
                        if ( rc == 0 )
                        {
                           obj_find(list_ocb,"LIST.SIZE",&list_size);
                           list_size--;
                           obj_upd(list_ocb,"LIST.SIZE",&list_size);
                        }
                      }
                  }
                  else if ( rc == NO_DATA_FOUND )
                  {
                     more_in_list = FALSE;
                  }
                  else
                  {
                     more_in_list = FALSE;
                  }
               }
               while( more_in_list );
            }
            else
            {
               list_ocd->key = modifier;
               rc = obj_rmv(list_ocb,list_ocd);
               if ( rc == 0 )
               {
                  obj_find(list_ocb,"LIST.SIZE",&list_size);
                  list_size--;
                  obj_upd(list_ocb,"LIST.SIZE",&list_size);
               }
            }
            break;


         case OBJ_LIST_ADD_ENTRY:
            /*+-------------------------------------------------------+*/
            /*| Adds an ENTRY field to item indicated by key          |*/
            /*| (modifier).                                           |*/
            /*+-------------------------------------------------------+*/
            ocd_new(&list_ocd,"LIST.LIST.ENTRY");
            if ( s_len(modifier) == 0 )
            {
               /*+----------------------------------------------------+*/
               /*| No object component named--use the current date &  |*/
               /*| time as the list sort key.                         |*/
               /*+----------------------------------------------------+*/
               s_alloc(&(list_ocd->key),sizeof(datetime));
               s_smp_ul(list_ocd->key) = sizeof(datetime);
               os_dttm((datetime*)s_buffer(list_ocd->key));
            }
            else
            {

               s_copy(&(list_ocd->key),modifier,0,0);

            }
                                  /* Get is made before a replace so if*/
                                  /* data already exists, we do not    */
                                  /* have to write it or increment the */
                                  /* size of the list. This should be  */
                                  /* be taken out once obj_rep returns */
                                  /* an indication that it over-wrote  */
                                  /* already existing data so we do not*/
                                  /* increment the list size./Sal011589*/

            get_rc = obj_get( list_ocb, list_ocd, &dummy_oid );

            rc = obj_rep(list_ocb, list_ocd, (byte *)params, SEG_UPDATE);

            /*+-------------------------------------------------------+*/
            /*| If we created an new list item increment the list size|*/
            /*+-------------------------------------------------------+*/
            if( get_rc == NO_DATA_FOUND && rc == 0 )
            {

               obj_find(list_ocb,"LIST.SIZE",&list_size);
               list_size++;            /* ...increment size of list */
               obj_upd(list_ocb,"LIST.SIZE",&list_size);

            }

         break;

         case OBJ_LIST_DEL_ENTRY:
            /*+-------------------------------------------------------+*/
            /*| Delete the ENTRY from the item indicated by the key.  |*/
            /*+-------------------------------------------------------+*/
            if ( s_len(modifier) == 0 )
            {
               rc = OBJ_LIST_KEY_REQUIRED;
            }
            else
            {
               ocd_new(&list_ocd,"LIST.LIST.ENTRY");
               list_ocd->key = modifier;

               get_rc = obj_get( list_ocb, list_ocd, &dummy_oid );

               if ( get_rc != 0 )
               {
                  rc = get_rc;
               }
               else
               {
                  rc = obj_rep(list_ocb, list_ocd, S_NULL, SEG_UPDATE);
               }
             }

            break;


         case OBJ_LIST_ADD_NUMBER:
            /*+-------------------------------------------------------+*/
            /*| Adds a NUMBER field to item indicated by key          |*/
            /*| (modifier).                                           |*/
            /*+-------------------------------------------------------+*/
            ocd_new(&list_ocd,"LIST.LIST.NUMBER");
            if ( s_len(modifier) == 0 )
            {
               /*+----------------------------------------------------+*/
               /*| No object component named--use the current date &  |*/
               /*| time as the list sort key.                         |*/
               /*+----------------------------------------------------+*/
               s_alloc(&(list_ocd->key),sizeof(datetime));
               s_smp_ul(list_ocd->key) = sizeof(datetime);
               os_dttm((datetime*)s_buffer(list_ocd->key));
            }
            else
            {

               s_copy(&(list_ocd->key),modifier,0,0);

            }
                                  /* Get is made before a replace so if*/
                                  /* data already exists, we do not    */
                                  /* have to write it or increment the */
                                  /* size of the list. This should be  */
                                  /* be taken out once obj_rep returns */
                                  /* an indication that it over-wrote  */
                                  /* already existing data so we do not*/
                                  /* increment the list size./Sal011589*/

            get_rc = obj_get( list_ocb, list_ocd, &dummy_oid );

            rc = obj_rep(list_ocb, list_ocd, (byte*) params, SEG_UPDATE);

            /*+-------------------------------------------------------+*/
            /*| If we created an new list item increment the list size|*/
            /*+-------------------------------------------------------+*/
            if( get_rc == NO_DATA_FOUND && rc == 0 )
            {

               obj_find(list_ocb,"LIST.SIZE",&list_size);
               list_size++;            /* ...increment size of list */
               obj_upd(list_ocb,"LIST.SIZE",&list_size);

            }

         break;

         case OBJ_LIST_DEL_NUMBER:
            /*+-------------------------------------------------------+*/
            /*| Delete the NUMBER from the item indicated by the key. |*/
            /*+-------------------------------------------------------+*/
            if ( s_len(modifier) == 0 )
            {
               rc = OBJ_LIST_KEY_REQUIRED;
            }
            else
            {
               ocd_new(&list_ocd,"LIST.LIST.NUMBER");
               list_ocd->key = modifier;

               get_rc = obj_get( list_ocb, list_ocd, &dummy_oid );

               if ( get_rc != 0 )
               {
                  rc = get_rc;
               }
               else
               {
/* ??? */         rc = obj_rep(list_ocb, list_ocd, NULL, SEG_UPDATE);
               }
             }

            break;


         default:
            /*+----------------------------------------------------------+*/
            /*| Level given is not known.                                |*/
            /*+----------------------------------------------------------+*/
            rc = OBJ_LEVEL_UNSUPPORTED;
         break;

      } /* switch end */

      ocd_drp( & del_ocd  );
      ocd_drp( & list_ocd );

   }

   else
   {
      rc = INVALID_OCB;
   }

   return(rc);
}
