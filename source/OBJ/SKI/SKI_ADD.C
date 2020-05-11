/******************************************************************
   <DOC>

   <NAME>      ski_add.c

   <LEVEL>     SKI

   <AUTHOR>    Pamela Cham

   <DATE>      June 9, 1989

   <PARAMS>    primary_key       string      INPUT
               secondary_key     string      INPUT
               primary_key and secondary_key: are concatenated
               to form the key string for the entry to be added.
               The resulting key string is of the form word.word or
               word (if secondary_key is null).

               *oid              OID         INPUT
               oid: points to the oid of the new SKI entry.

               status            sint        INPUT
               status: gives the value for the status field of
               the new SKI entry.

   <PURPOSE>   This routine adds an entry in the system keyword
               index, given the primary key, secondary key, oid,
               and status.

   <OUTPUT>    rc                sint

   <ERRS>      INVALID_OID_POINTER

   </DOC>

******************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"

sint ski_add (primary_key, secondary_key, oid, status)
string primary_key, secondary_key;
OID *oid;
sint status;
{
   OCB *ski_ocb=NIL;
   OCD *ski_ocd=NIL;
   byte *buffer=NIL;
   sint rc=0;
   string keyval=S_NULL;


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

                                       /* check ocb & oid pointers     */
      ski_ocb = t_sysndx(SKI_OBJ);
      if (OCB_POINTER_OK(ski_ocb))
      {                                /* add the new SKI key entry    */
         s_init (&(ski_ocd->name),"SKI.INDEX.OID");
         if (!OID_POINTER_OK(oid))
         {
            rc = INVALID_OID_POINTER;
         }
         else
         {                             /* add the new oid & status     */
            buffer = (byte *) oid;
            rc = obj_rep (ski_ocb, ski_ocd, buffer, SEG_UPDATE);
            if (rc == 0)
            {
               s_init (&(ski_ocd->name),"SKI.INDEX.STATUS");
               buffer = (byte *) &status;
               rc = obj_rep (ski_ocb, ski_ocd, buffer, SEG_UPDATE);
            }
            else
            {
            }
         }
         obj_cls(ski_ocb);
      }
      else
      {
         except(INVALID_OCB, OBJ_LEVEL_ERROR, S_NULL, S_NULL, S_NULL,
                S_NULL, S_NULL);
      }
      ocd_drp (&ski_ocd);
   }
   s_drop (&keyval);
   return(rc);
}
