#include "linked_list.h"
#include "process.h"
#include <stdlib.h>

linked_list_ptr create_list() 
{
    linked_list_ptr list = (linked_list_ptr) malloc(sizeof(linked_list)); //allocate memory for linked list
    list->head = NULL; //set head to NULL (empty list)
    list->size = 0;
    return list;
}

/* 
    Example:
    assume state of linked list before enqueue_front:
    ll -> [1] -> [2] -> [3] -> NULL

    enqueue_front(ll, [4]) //insert [4] into begining of ll

    state of linked list after enqueue_front:
    ll -> [4] -> [1] -> [2] -> [3] -> NULL
*/
node_ptr enqueue_front(linked_list_ptr ll, struct process* p)
{
    node_ptr new_node = (node_ptr) malloc(sizeof(node)); //allocate memory for node
    new_node->data = p; //set data to process p
    new_node->next = ll->head; //to insert head, set next to current head
    ll->head = new_node; //and set head to new node
    ll->size++; //increment size
    return new_node;
}

/* 
    Example:
    assume state of linked list before enqueue_back:
    ll -> [1] -> [2] -> [3] -> NULL

    enqueue_back(ll, [4]) //insert [4] into end of ll

    state of linked list after enqueue_back:
    ll -> [1] -> [2] -> [3] -> [4] -> NULL
*/
node_ptr enqueue_back(linked_list_ptr ll, struct process* p)
{
    node_ptr new_node = (node_ptr) malloc(sizeof(node)); //allocate memory for node
    new_node->data = p; //set data to process p
    new_node->next = NULL; //to insert tail, set next to NULL
    if (ll->head == NULL) //if list is empty, set head to new node
    {
        ll->head = new_node;
    } 
    else //otherwise, move to tail and set next to new node
    {
        node_ptr current = ll->head; //start at head
        while (current->next != NULL) //while next is not NULL (not tail)
            current = current->next; //move to next node

        current->next = new_node; //current is now tail, set next to new node, so new node is becomes tail
    }
    ll->size++; //increment size
    return new_node;
}

/*
    Example:
    assume state of linked list before enqueue_after:
    ll -> [1] -> [2] -> [3] -> NULL

    enqueue_after([2], [4]) //insert [4] after [2]

    state of linked list after enqueue_after:
    ll -> [1] -> [2] -> [4] -> [3] -> NULL
*/
node_ptr enqueue_after(linked_list_ptr ll, node_ptr n, struct process* p)
{
    node_ptr new_node = (node_ptr) malloc(sizeof(node)); //allocate memory for node
    new_node->data = p; //set data to process p
    new_node->next = n->next; //set next to next of node n (insert after n)
    n->next = new_node; //update next of node n to new node (so node n is now behind new node)
    ll->size++; //increment size
    return new_node;
}

node_ptr find(linked_list_ptr ll, uint32_t pid)
{
    node_ptr current = ll->head; //start at head
    while (current != NULL && current->data->pid != pid) //while pid is not found and we are not at tail
        current = current->next; //move to next node

    return current; //return node with pid or NULL if not found (since tail->next is NULL)
}

/*
    Example:
    assume state of linked list before dequeue:
    ll -> [1] -> [2] -> [3] -> NULL

    dequeue_node(ll, [2]) //delete [2] from ll

    state of linked list after dequeue:
    ll -> [1] -> [3] -> NULL
*/
void dequeue_node(linked_list_ptr ll, node_ptr n)
{
    if (ll->head == n) //if node to be deleted is head
    {
        ll->head = n->next; //set head to next node
    } 
    else 
    {
        node_ptr current = ll->head; //start at head
        while (current->next != n) //while next is not node to be deleted
            current = current->next; //move to next node

        current->next = n->next; //current is now node before n, set next to next of n (so n is now skipped)
    }
    free(n); //free memory of node n
    ll->size--; //decrement size
}