/******************************************************************

 NAME.    prv_isv.c

 LEVEL.   prv

 AUTHOR.  Pamela Cham, Samir Chopra

 DATE.    January 4,1989

 MODS.    May 3, 1989 : Samir Chopra

          Setting of bits in PRIVS structure is now carried out
          by calling prv_map().

          May 31st, 1989 : Samir Chopra

          Added COORDINATOR ROLE

 PARAM.   PHRASE     string    INPUT

          PRV_1      pointer   INPUT


 PURPOSE.  Match an input string to an entry in the ISV table and return
           the ISV # (key) corresponding to that role.  The bit setting
           for that role is carried out by calling PRV_MAP().

 OUTPUT.

 CALLS.    isv_match ()
           prv_set()

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


sint  prv_isv( phrase, prv_1)

   string    phrase;
   PRIVS     **prv_1;

{

   PRIVS     *priv_ptr = NIL;
   sint      isv_key = 0;
   boolean   allocated = FALSE;
   sint      rc        = 0;


   if (prv_1 == NIL)

   {


   }

   else if (*prv_1 == NIL)

   {

      priv_ptr = ( PRIVS *) os_mema (sizeof(PRIVS));
      *prv_1 = priv_ptr;
      allocated = TRUE;

   }

   else

   {

      priv_ptr = *prv_1;

   }

   priv_ptr->one = 0;
   priv_ptr->two = 0;
   priv_ptr->three = 0;
   priv_ptr->four = 0;
   priv_ptr->five = 0;
   priv_ptr->six = 0;

   isv_key =isv_match (phrase, ISV_MEMBER,ISV_COORDINA);

   rc = prv_map (isv_key, priv_ptr);


         if (allocated)

         {

            os_memd(priv_ptr);

         }




   return(isv_key);

}
