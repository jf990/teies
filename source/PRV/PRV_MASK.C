/*******************************************************************

 NAME.    PRV_MASK.C

 LEVEL.   PRIV

 AUTHOR.  PAMELA CHAM,SAMIR CHOPRA

 DATE.    DECEMBER 2, 1988

 PARAM.   PRV_RESULT  pointer   INPUT

          PRV_1       pointer   INPUT

          PRV_2       pointer   INPUT

          OPCODE      sint      INPUT

OUTPUT.   PRV_RESULT  pointer

PURPOSE.  Perform a bitwise operation on two privilege structures and
          place the result in a third privilege structure.

**********************************************************************/

#include "defs.h"
#include "osk.h"
#include "osk_i.h"
#include "str.h"
#include "obj.h"
#include "debug.h"
#include "priv.h"

prv_mask (prv_result, prv_1, prv_2, opcode)

   PRIVS **prv_result;
   PRIVS *prv_1, *prv_2;
   sint opcode;

{
   PRIVS *result_ptr;
   byte  *byte_ptr1,*byte_ptr2,*byte_ptr3, *temp_ptr;
   sint i=0;
   sint rcode=0;


   byte_ptr1=NIL;        /* Initialization of pointers follows...*/
   byte_ptr2=NIL;
   byte_ptr3=NIL;
   temp_ptr =NIL;
   result_ptr=NIL;


   if (prv_result == NIL)

   {


   }

   else  if (*prv_result ==NIL)

   {

     result_ptr = ( PRIVS * ) os_mema(sizeof(PRIVS));
     *prv_result = result_ptr;

   }

   else

   {

      result_ptr = * prv_result;

   }

   byte_ptr1 = (byte*)prv_1;
   byte_ptr2 = (byte*)prv_2;
   byte_ptr3 = (byte*)result_ptr;


   switch (opcode)

   {

      case PRV_OR:
         for (i= 1; i<= sizeof(PRIVS); i++)

         {

            *byte_ptr3 = *byte_ptr1 | *byte_ptr2;

            byte_ptr3++;
            byte_ptr1++;
            byte_ptr2++;

         }

         break;

      case PRV_AND:

         for (i=1; i<= sizeof(PRIVS); i++)

         {

            *byte_ptr3 = *byte_ptr1 & *byte_ptr2;

            byte_ptr3++;
            byte_ptr1++;
            byte_ptr2++;

         }

         break;

      case PRV_XOR:

         for (i=1; i<= sizeof(PRIVS); i++)

         {

            *byte_ptr3 = *byte_ptr1 ^ *byte_ptr2;
            byte_ptr3++;
            byte_ptr1++;
            byte_ptr2++;

         }

         break;

      case PRV_MAX:

         temp_ptr = (byte*)prv_1;
         for (i=1; i<= sizeof(PRIVS); i++)

         {

            if (*byte_ptr1 < *byte_ptr2)

            {

               temp_ptr = (byte*)prv_2;
               break;                  /* Get out of for loop          */

            }

            else if (*byte_ptr2 < *byte_ptr1)

            {

               temp_ptr = (byte*)prv_1;
               break;                  /* Get out of for loop          */

            }

            else

            {

               byte_ptr2++;
               byte_ptr1++;

            }

         }

         for (i=1;i<=sizeof(PRIVS);i++)

         {

            *byte_ptr3 = *temp_ptr;
            byte_ptr3++;
            temp_ptr++;

         }

         break;

      case PRV_MIN:

         byte_ptr1 = (byte*)(prv_1 + sizeof(PRIVS)-1);
         byte_ptr2 = (byte*)(prv_2 + sizeof(PRIVS)-1);
         temp_ptr = (byte*)prv_1;

         for (i=1;i<= sizeof(PRIVS); i++)

         {

            if (*byte_ptr1 > *byte_ptr2)

            {

              temp_ptr = (byte*)prv_2;      /* Results found...      */
               break;                  /* Get out of for loop          */

            }

            else if (*byte_ptr2 > *byte_ptr1)

            {

               temp_ptr = (byte*)prv_1; /* Results found...            */
                break;                 /* Get out of for loop          */

            }

            else

            {

               byte_ptr2--;
               byte_ptr1--;

            }

         }

         for (i=1;i<=sizeof(PRIVS);i++)

         {

            *byte_ptr3 = *temp_ptr;
            byte_ptr3++;
            temp_ptr++;

         }

         break;

      default:

         rcode = PRV_BAD_OPCODE;
         break;

   }

   return(rcode);

}
