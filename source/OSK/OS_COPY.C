#include "defs.h"
/*
.page
.cen general purpose copy routine
hopefully this will replaced with an assembly program
on those machines that support block move instructions */
hword ccopy(a,b,len)
byte *a, *b;
hword len;
{    hword i, found;
    found = 0;
    i = 0;
    if( len > 0){
        for (i=0;i<len;i++){
            *b = *a;
            b++;
            a++;}}
    else {    len = -len;
        for(i=0;i<len;i++){
            *b = *a;
            b--;
            a--;}}}


