#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

//we are using linked list data structure to implement queue
typedef linked_list_ptr queue_ptr;

//allocates and initializes new queue
queue_ptr create_queue();

//enqueues process p into queue q, asceding by pid
node_ptr enqueue_orderby_pid(queue_ptr q, struct process* p);

//enqueues process p into queue q, asceding by arrival time
node_ptr enqueue_orderby_arrival(queue_ptr q, struct process* p);

//enqueues process p into queue q, asceding by priority
node_ptr enqueue_orderby_priority(queue_ptr q, struct process* p);

//enqueues process p into queue q, asceding by burst time
node_ptr enqueue_orderby_burst_time(queue_ptr q, struct process* p);

//finds process by pid
struct process* find_process_by_pid(queue_ptr q, uint32_t pid);

//dequeues process by given pid from queue q
void dequeue(queue_ptr q, uint32_t pid);

//peeks the first process in queue q (i.e. returns head without removing)
struct process* peek();

#endif
