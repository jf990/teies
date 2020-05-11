#include "defs.h"
#include "hash.h"

hword D_set(variable,value)
byte *variable;
byte *value;

{
   struct H_entry *h_ptr;
   hword rc;

   h_ptr = (struct H_entry *) malloc(sizeof(struct H_entry));
   if (h_ptr == NIL) return(1);

   h_ptr->name(:0:) = ' ';
   ccopy(h_ptr->name,((byte *) (h_ptr->name)) + 1, 15);
   ccopy(variable,h_ptr->name,strlen(variable));
   printf("D_set: variable '%s' value '%c hash %d'\n"\
   ,variable,*value,D_hashit(h_ptr->name));
   h_ptr->next = NIL;

   h_ptr->value = 0;
   if (*value == '1'){
      h_ptr->value = 1;
      }

   rc = D_setit(h_ptr);
return (rc);


}
