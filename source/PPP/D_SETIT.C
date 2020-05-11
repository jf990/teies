/*+--------------------------------------------------------------------+
  |                                                                    |
  |      :NAME.         D_set.c                                        |
  |                                                                    |
  |      :AUTHOR.       Anoop K. Srivastava.                           |
  |                                                                    |
  |      :DATE.         June 3, 1988.                                  |
  |                                                                    |
  |      :LEVEL.        DEBUG.                                         |
  |                                                                    |
  |      :GLOBAL.       .....                                          |
  |                                                                    |
  |      :PARAM.        h_ptr   :  INPUT  : pointer to the new entry   |
  |                                         in the Hash table.         |
  |      :PURPOSE.      This routine adds a new entry to the hash table|
  |                                                                    |
  |                                                                    |
  |                                                                    |
  |                                                                    |
  |                                                                    |
  |                                                                    |
  |      :OUTPUT.       Always returns 0.                              |
  |                                                                    |
  |      :CALLS.        none.                                          |
  |                                                                    |
  |      :ERRORS.       none.                                          |
  |                                                                    |
  |      :LIMITS.       none known.                                    |
  |                                                                    |
  |      :END.                                                         |
  |                                                                    |
  +--------------------------------------------------------------------*/

#include "defs.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>


D_setit ( h_ptr)
    struct H_entry    *h_ptr;
{

    int       value = 0;
    int       count = 1;
    int       rc;
    int       flag;
    struct H_entry      *temp_ptr;
    struct H_entry      *old_ptr;

    /* Calculate the value of hash function */
#if 1
   value = D_hashit(h_ptr->name);
#else
    while( (h_ptr)->name(:count:) != BLANK && count <  VARLEN) {
         value = value * 2 + trans(: (h_ptr)->name(:count:) :);
         if (value > 256 )
              value = (value % 256) + value/256;
         count++;
    }
#endif
   if (H_debug == TRUE){
      printf("D_setit: hash %d value %d\n",value,h_ptr->value);
      mdump(h_ptr->name,16);
      }

    if (H_array(:value:).first == NULL)     /* If there is no element */
         H_array(:value:).first = h_ptr;    /*the table at this lcation*/
                                            /*attach this pointer */
    else {
         temp_ptr = H_array(:value:).first;      /* OTHERWISE */
         flag = FALSE;
         while ( (temp_ptr->next) != NULL  && !flag) {
#if 0
              count = 1;/* traverse */
              rc = 0;                                     /*the chain */
              while ( h_ptr->name(:count:) != BLANK &&    /*and compar*/
                        count <  VARLEN ) {               /*the newitem*/
                   if (trans(:h_ptr->name(:count:):) !=   /*with all  */
                        trans(:temp_ptr->name(:count:) :) )/*existing */
                             rc = -1;                      /* ones. if*/
                    count++;
              }                                            /*matches  */
#else
              rc = 0;
              if(ccomp(h_ptr->name,temp_ptr->name,VARLEN)){
                 rc = -1;
                 }
#endif
              if (rc == 0)                       /*change the value   */
                   flag = TRUE;                  /*else attach the new*/
              else {                             /*item at the end of */
#if 1 /* added 14 June 1988 */
                   old_ptr = temp_ptr;
#endif
                   temp_ptr = temp_ptr->next ;   /*the chain.         */
                 }
         }
         if ( flag ) {
            if (H_debug == TRUE){
               printf("D_setit: rep value with %d\n",h_ptr->value);
               }

            temp_ptr->value = h_ptr->value;
            free(h_ptr);
         }

         else {
                 if (H_debug == TRUE){
                    printf("D_setit: insert new element\n");
                    }
#if 0 /* old is first, replace 14 June 1988 */
              temp_ptr->next = h_ptr;
#else
              old_ptr->value = h_ptr->value;
#endif

              }
    }
    return(flag);
}
