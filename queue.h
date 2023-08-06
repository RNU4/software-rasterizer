#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include "init.h"
#define queue_type shape_triangle_t

typedef struct queue_node{
    queue_type* value;
    struct queue_node *next;
    
}queue_node_t;

typedef struct queue
{
    /* data */
    queue_node_t *head;
    queue_node_t *tail;
    int size;
}queue_t;


void init_queue(queue_t *q){
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

bool enqueue(queue_t *q, queue_type value){
    queue_node_t* newnode = (queue_node_t*)malloc(sizeof(queue_node_t));
    if (newnode==NULL) return false;
    newnode->value = malloc(sizeof(shape_triangle_t));
    *newnode->value = value;
    newnode->next = NULL;
    // if tail connect to new tail
    if (q->tail!= NULL){
        q->tail->next = newnode;
    }
    
    q->tail = newnode;
    //head maeks sense
    if (q->head == NULL){
        q->head = newnode;
    }
    q->size++;
    return true;
}

queue_node_t* dequeue(queue_t *q){
    if (q->head == NULL) return NULL;
    queue_node_t *tmp = q->head;
    queue_node_t* result = tmp;
    q->head = q->head->next;
    if (q->head == NULL){
        q->tail=NULL;
    }
    q->size--;
    free(tmp);
    return result;
}


#endif /* QUEUE_H */