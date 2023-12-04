#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stddef.h>

typedef struct node_struct{
    struct process* data;
    struct node_struct* next;
} node, *node_ptr;

typedef struct {
    node_ptr head;
    uint32_t size;
} linked_list, *linked_list_ptr;

//allocates and initializes a new linked list
linked_list_ptr create_list();

//enqueues process p into head (first element) of linked list ll
node_ptr enqueue_front(linked_list_ptr ll, struct process* p);

//enqueues process p into tail (last element) of linked list ll
node_ptr enqueue_back(linked_list_ptr ll, struct process* p);

//enqueues process p into linked list q after node n
node_ptr enqueue_after(linked_list_ptr ll, node_ptr n, struct process* p);

//finds process by pid
node_ptr find(linked_list_ptr ll, uint32_t pid);

//dequeues given process by node n from linked list ll
void dequeue_node(linked_list_ptr ll, node_ptr n);

#endif
