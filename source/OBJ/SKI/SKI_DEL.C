/******************************************************************
   <DOC>
   <NAME>      ski_del.c
   <LEVEL>     SKI
   <AUTHOR>    Pamela Cham
   <DATE>      September 15, 1989
   <PARAMS>    primary_key    string   INPUT
               secondary_key  string   INPUT
               primary_key and secondary_key: are concatenated to
               obtain a search key of the form word.word or word (in
               the case where the secondary key is null).

   <PURPOSE>   This routine deletes the entry corresponding to the
               given key string from the System Keyword Index (SKI).
               A return code of 0 implies successful deletion.

   <OUTPUT>    rc             sint
   <ERRS>      SKI_NO_ENTRY_FOUND
   </DOC>
******************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"

sint ski_del (primary_key, secondary_key)
string primary_key, secondary_key;
{

   sint rc=0;
   OID *oid=NIL;
   OCB *ski_ocb=NIL;
   OCD *ski_ocd=NIL;
   string keyval=S_NULL;
   sint status=0;


                                       /* check for null key values    */
   if (primary_key == S_NULL && secondary_key == S_NULL)
   {
   }
   else
   {
      ocd_new (&ski_ocd, "SKI.INDEX.OID");   /* form entry's key value */
      s_pad (&primary_key, primary_key, 16, '\0', 'R');
      if (s_len(secondary_key) < 1)
      {
         s_copy (&(ski_ocd->key), primary_key, 0, 0);
      }
      else
      {
         s_pad (&secondary_key, secondary_key, 16, '\0', 'R');
         s_cat (&keyval, primary_key, secondary_key, 0, 0);
         s_copy (&(ski_ocd->key), keyval, 0, 0);
      }

      ski_ocb = t_sysndx(SKI_OBJ);
      if (OCB_POINTER_OK(ski_ocb))
      {
         oid = (OID *) os_mema (sizeof (OID));
         rc = obj_get (ski_ocb, ski_ocd, oid);
         if (rc != 0)
         {
            rc = SKI_NO_ENTRY_FOUND;
         }
         else
         {
                                       /* check corresponding status   */
            s_init (&(ski_ocd->name), "SKI.INDEX.STATUS");
            rc = obj_get (ski_ocb, ski_ocd, &status);

            s_init (&(ski_ocd->name), "SKI.INDEX");
            rc = obj_rmv (ski_ocb,ski_ocd);
            if (rc == 0)
            {
            }
            else
            {
            }

         }
         obj_cls (ski_ocb);
      }
      else
      {
         except (INVALID_OCB, OBJ_LEVEL_ERROR, S_NULL, S_NULL, S_NULL,
                 S_NULL, S_NULL);
      }
      ocd_drp (&ski_ocd);
      s_drop (&keyval);
   return(rc);
   }
}
