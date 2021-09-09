#include <stdlib.h>
#include <stdio.h>
#include "array.h"

int main(){
    Array a=array_new(4);
    array_insertBack(&a, 6);
    array_insertBack(&a, 6);
    array_insertBack(&a, 6);
    array_insertBack(&a, 6);
    array_insertBack(&a, 6);
    array_insertBack(&a, 6);


    array_print(a);
    return 0;
}