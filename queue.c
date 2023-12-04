#include "queue.h"
#include "process.h"

queue_ptr create_queue()
{
    return create_list(); //since queue is just a linked list with some extra functions on top of it we can just use linked list functions
}

node_ptr enqueue_orderby_pid(queue_ptr q, struct process* p)
{
    node_ptr current = q->head; //start from head
    node_ptr previous = NULL; //keep track of previous node
    while (current != NULL && current->data->pid < p->pid) //find the right place to insert the new node
    {
        previous = current; //keep track of previous node
        current = current->next; //move to next node
    }
    if (previous == NULL)  //if previous is NULL, we are at the head (or list is empty)
    {
        return enqueue_front(q, p); //so insert at head
    } 
    else //otherwise
    {
        return enqueue_after(q, previous, p); //insert after previous node
    }
}

node_ptr enqueue_orderby_arrival(queue_ptr q, struct process* p)
{
    node_ptr current = q->head; //start from head
    node_ptr previous = NULL; //keep track of previous node
    while (current != NULL && current->data->arrival_time < p->arrival_time)  //find the right place to insert the new node
    {
        previous = current; //keep track of previous node
        current = current->next; //move to next node
    }
    if (previous == NULL) //if previous is NULL, we are at the head (or list is empty)
    {
        return enqueue_front(q, p); //so insert at head
    } 
    else //otherwise
    {
        return enqueue_after(q, previous, p); //insert after previous node
    }
}

node_ptr enqueue_orderby_priority(queue_ptr q, struct process* p)
{
    node_ptr current = q->head; //start from head
    node_ptr previous = NULL; //keep track of previous node
    while (current != NULL && current->data->priority < p->priority)  //find the right place to insert the new node
    {
        previous = current; //keep track of previous node
        current = current->next; //move to next node
    }
    if (previous == NULL) //if previous is NULL, we are at the head (or list is empty)
    {
        return enqueue_front(q, p); //so insert at head
    } 
    else //otherwise
    {
        return enqueue_after(q, previous, p); //insert after previous node
    }
}

node_ptr enqueue_orderby_burst_time(queue_ptr q, struct process* p)
{
    node_ptr current = q->head; //start from head
    node_ptr previous = NULL; //keep track of previous node
    while (current != NULL && current->data->burst_time < p->burst_time)  //find the right place to insert the new node
    {
        previous = current; //keep track of previous node
        current = current->next; //move to next node
    }
    if (previous == NULL) //if previous is NULL, we are at the head (or list is empty)
    {
        return enqueue_front(q, p); //so insert at head
    } 
    else //otherwise
    {
        return enqueue_after(q, previous, p); //insert after previous node
    }
}

struct process* find_process_by_pid(queue_ptr q, uint32_t pid)
{
    node_ptr current = q->head; //start from head
    while (current != NULL && current->data->pid != pid) //find the right node
        current = current->next;

    if (current == NULL) //if current is NULL, we didn't find the node
        return NULL; //so return NULL
    else //otherwise
        return current->data; //return the data (process)
}

void dequeue(queue_ptr q, uint32_t pid)
{
    node_ptr process_node = find(q, pid); //find the node
    if (process_node != NULL) //if the node exists
        dequeue_node(q, process_node); //dequeue it
}

struct process* peek(queue_ptr q)
{
    if (q->head == NULL)
    {
        return NULL;
    }
    return q->head->data; //return the data (process) of the head node
}