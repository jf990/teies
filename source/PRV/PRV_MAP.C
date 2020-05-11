/******************************************************************

  NAME.    prv_map.c

  LEVEL.   prv

  AUTHOR.  Samir Chopra

  DATE.    May 3,1989

  PARAM.   isv_key      sint     INPUT

           priv_ptr    pointer  INPUT


  PURPOSE.  Takes as input a sint, ISV_KEY that corresponds to the entry
            for a role in the ISV table. The bit setting
            for that role is carried out by calling PRV_SET().

            Checks if pointer to PRIVS is bad.
            Checks if isv_key is out of range.

  MODS.     May 31st, 1989  : Samir Chopra

            Added COORDINATOR role

  OUTPUT.

  CALLS.    prv_set()


*******************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "priv.h"
#include "isv.h"
#include "obj_prim.h"


sint  prv_map( isv_key, priv_ptr)

   sint      isv_key;
   PRIVS     *priv_ptr;

{
   sint rc  = 0;





   if (priv_ptr == NIL)

   {

       rc = BAD_POINTER_TO_PRIVS;

   }

   switch (isv_key)

   {

      case ISV_MEMBER :
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_AUTHOR);
         break;

      case ISV_MONITOR :
         prv_set(priv_ptr, PRV_INDEXER);
         prv_set(priv_ptr, PRV_DELETE);
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_AUTHOR);
         prv_set(priv_ptr, PRV_CREATE);
         prv_set(priv_ptr, PRV_REPLY);
         prv_set(priv_ptr, PRV_APPROVE);
         break;

      case ISV_EDITOR :
         prv_set(priv_ptr, PRV_MODIFY);
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_AUTHOR);
         prv_set(priv_ptr, PRV_CREATE);
         break;

      case ISV_INDEXER :
         prv_set(priv_ptr, PRV_INDEXER);
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_AUTHOR);
         break;

      case ISV_OBSERVER :
         prv_set(priv_ptr, PRV_READ);
         break;

      case ISV_LURKER :
         prv_set(priv_ptr, PRV_READ);
         break;

      case ISV_CONTRIBU :
         prv_set(priv_ptr, PRV_AUTHOR);
         prv_set(priv_ptr, PRV_CREATE);
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_MODIFY);
         break;

      case ISV_RESPONDE :
         prv_set(priv_ptr, PRV_REPLY);
         prv_set(priv_ptr, PRV_READ);
         break;

      case ISV_OWNER :
         prv_set(priv_ptr, PRV_OWNER);
         break;

      case ISV_MASQUERA :
         prv_set(priv_ptr, PRV_JOIN);
         break;

      case ISV_COORDINA :
         prv_set(priv_ptr, PRV_INDEXER);
         prv_set(priv_ptr, PRV_DELETE);
         prv_set(priv_ptr, PRV_READ);
         prv_set(priv_ptr, PRV_AUTHOR);
         prv_set(priv_ptr, PRV_CREATE);
         prv_set(priv_ptr, PRV_REPLY);
         prv_set(priv_ptr, PRV_APPROVE);


      default:
         rc = ISV_KEY_OUT_OF_RANGE;

         break;

   }

   return(rc);

}
