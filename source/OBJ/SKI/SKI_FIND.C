/******************************************************************
   <DOC>
   <NAME>      ski_find.c
   <LEVEL>     SKI
   <AUTHOR>    Pamela Cham
   <DATE>      June 14, 1989
   <PARAMS>    primary_key    string   INPUT
               secondary_key  string   INPUT
               primary_key and secondary_key: are concatenated
               to obtain the search key which is of the form
               word.word or word (in the case where secondary_key
               is null).

               **oid          OID      OUTPUT
               oid: holds the pointer to the pointer to the oid
               of the SKI entry which corresponds to the search key.

               *status        sint     OUTPUT
               status: holds a pointer to the status value for the
               corresponding SKI entry found.

   <PURPOSE>  This routine finds the oid and status of an entry
              in the System Keyword Index (SKI) based on the
              given primary and secondary keys.  If both the
              primary key and secondary keys are non-null strings,
              they are concatenated to form the search key.  On
              the other hand, if the secondary key is null, then
              the oid indexed by the single key is searched for.
   <OUTPUT>   rc             sint
   <ERRS>     SKI_NO_ENTRY_FOUND
   </DOC>
******************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"

sint ski_find (primary_key, secondary_key, oid, status)
string primary_key, secondary_key;
OID **oid;
sint *status;

{

   OCB *ski_ocb=NIL;
   OCD *ski_ocd=NIL;
   sint rc=0;
   string keyval=S_NULL;


                                       /* determine search key         */
   ocd_new (&ski_ocd,"SKI.INDEX.OID");
   if (primary_key == S_NULL && secondary_key == S_NULL)
   {
   }
   else
   {
      s_pad (&primary_key, primary_key, 16, '\0', 'R');
      if (s_len(secondary_key)< 1) /* secondary key is nil       */
      {
         s_copy ( &(ski_ocd->key), primary_key, 0, 0);
      }
      else                             /* concat primary & secondary   */
                                       /*   keys to get the search key */
      {
         s_pad (&secondary_key, secondary_key, 16, '\0', 'R');
         s_cat (&keyval, primary_key, secondary_key, 0, 0);
         s_copy ( &(ski_ocd->key), keyval, 0, 0);
      }
                                       /* check if pointers are valid  */
      ski_ocb = t_sysndx(SKI_OBJ);
      if (OCB_POINTER_OK(ski_ocb))
      {
         if (oid != NULL)
         {
            if (!OID_POINTER_OK(*oid))
            {
               *oid = (OID *) os_mema (sizeof (OID));
            }
            if (status == NULL)
               status = (sint *) os_mema (sizeof (sint));


                                       /* get oid and status indexed   */
                                       /*   by key in ski_ocd          */
            rc = obj_get (ski_ocb, ski_ocd, *oid);
            if (rc != 0)
            {
               rc = SKI_NO_ENTRY_FOUND;
            }
            else
            {
               s_init ( &(ski_ocd->name), "SKI.INDEX.STATUS");
               obj_get (ski_ocb, ski_ocd, status);
            }
         }
         else
         {
            rc = INVALID_OID_POINTER;
         }
         obj_cls (ski_ocb);
      }
      else
      {
         except (INVALID_OCB, OBJ_LEVEL_ERROR, S_NULL, S_NULL, S_NULL,
                 S_NULL, S_NULL);
      }
   }
   ocd_drp (&ski_ocd);
   s_drop (&keyval);
   return(rc);
}
