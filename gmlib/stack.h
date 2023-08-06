#ifndef STACK_H_   /* Include guard */
#define STACK_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../init.h"
#include "../structs.h"


stack_t* stack_new(int size){
    stack_t* s = (stack_t*)calloc(sizeof(stack_t),1);
    s->top = 0;
    s->size = size;
    s->data = (shape_triangle_t *)calloc(sizeof(shape_triangle_t),size);
    return s;
}

int grow_stack(stack_t* s,size_t size){
    s->size+=size;
    s->data = (shape_triangle_t*)realloc(s->data,s->size*sizeof(shape_triangle_t));
    if (s->data == NULL){
        printf("ERROR REALLOC FAILED\n");
        return 0;
    }
    return 1;
}

int64_t stack_push(stack_t *s, shape_triangle_t o){
    if (s->top+2 >= s->size){
        grow_stack(s,4);
    }
    s->data[s->top++]=o;
    return s->top;
}

shape_triangle_t stack_pop(stack_t *s){
    return s->data[--(s->top)];
}

shape_triangle_t stack_peak(stack_t *s){
    return s->data[s->top - 1];
}
#endif