/*
    Q0: take a time to inspect code and its comment, try to understand what every line does.
    Q1: complete TODOs
    Q2: notice that all scheduling methods are preemptive. explain how preemtion is achived in the code.
    Q3: test all schedulers by changing SCHEDULING_ALGORITHM variable in process.c, for each scheduler build & run (gcc *.c && ./a.out).
        The program will output detailed information. use this information to draw gantt charts (both for cpu and io).
*/

//IMPORTANT: please do not add or remove any printf. Leave the output as is. Do not modify functions marked with "!this function does not requires modifications!"
#include "process.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void scheduler();
void fcfs_scheduler();
void sjf_scheduler();
void priority_scheduler();
void enter_io();
void finish_io();
void io_completion();
void print_statistics();

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        SCHEDULING_ALGORITHM = (scheduling_algorithm)atoi(argv[1]);
    }
    //create process queues
    job_queue = create_queue();
    ready_queue = create_queue();
    waiting_queue = create_queue();
    terminated_queue = create_queue();

    //spawn processes (populates job queue)

    /*
        arrival time = 0
        priority = 5
        total burst time = 3
        io burst = 2

        single burst before io = 2
    */
    spawn(0, 5, 3, 2, 2);

    /*
        arrival time = 0
        priority = 0
        total burst time = 2
        io burst = 4
        single burst before io = 1
    */
    spawn(0, 0, 2, 4, 1);

    /*
        arrival time = 2
        priority = 1
        total burst time = 1
        io burst = 3
        single burst before io = 2
    */
    spawn(2, 1, 1, 3, 2);

    /*
        arrival time = 5
        priority = 1
        total burst time = 2
        io burst = 2
        single burst before io = 1
    */
    spawn(5, 1, 2, 2, 1);
    //start scheduler
    scheduler();

    return 0;
}

void scheduler()
{
    struct process *p;
    while(job_queue->size > 0 || ready_queue->size > 0 || waiting_queue->size > 0) //while there is any process to schedule
    {
        //each cpu cycle, execute according to scheduler rules

        io_completion(); //check if any process has finished io
        cpu_counter++; //increment cpu counter (i.e. cpu is executed one cycle)
        switch(SCHEDULING_ALGORITHM) //run scheduler algorithm
        {
            case SCHEDULING_ALGORITHM_FCFS: fcfs_scheduler();
            break;
            case SCHEDULING_ALGORITHM_SJF: sjf_scheduler();
            break;
            case SCHEDULING_ALGORITHM_PRIORITY: priority_scheduler();
            break;
        }
    }
    print_statistics();
}


void fcfs_scheduler()
{
    struct process* p;
    node_ptr current;
    //check if any process has arrived
    while(job_queue->size > 0)
    {
        p = peek(job_queue);
        if (p == NULL) //check if there is any process in job queue
        {
            break;
        }

        if (p->arrival_time < cpu_counter) //check if process has arrived
        {
            p->flag = PROCESS_FLAG_READY; //set flag to ready
            dequeue(job_queue, p->pid); //remove from job queue
            enqueue_orderby_arrival(ready_queue, p); //move process from job queue to ready queue
        }
        else
        {
            break; //no more process has arrived yet
        }
    }

    //check if any process is exited io
    current = waiting_queue->head;
    while (current != NULL) //loop waiting queue to find READY flag processes
    {
        p = current->data;
        current = current->next;
        if (p->flag == PROCESS_FLAG_READY)
        {
            dequeue(waiting_queue, p->pid); //remove from waiting queue
            if (p->program_counter >= p->burst_time) //cpu execution of process has already finished but waited io to complete before exiting (i.e. write to file)
            {
                exit_process(p); //exit process
            }
            else
            {
                enqueue_orderby_arrival(ready_queue, p); //move process from waiting queue to ready queue
            }
        }
    }

    p = peek(ready_queue);
    if (p != NULL) //check if there is any process in ready queue
    {
        printf("P%d executing (T=%d)\n", p->pid, cpu_counter - 1);
        p->flag = PROCESS_FLAG_RUNNING; //set flag to running
        dequeue(ready_queue, p->pid); //remove from ready queue
        
        //simulate process execution
        p->cpu_time = cpu_counter; //set cpu time
        p->program_counter++; //execute process by 1 step

        if (p->program_counter >= p->burst_time_before_io) //process is about to enter IO
        {
            enter_io(p);
        }
        
        if (p->flag != PROCESS_FLAG_WAITING)
        {
            if (p->program_counter >= p->burst_time) //check if process has finished
            {
                exit_process(p); //exit process
            }
            else
            {
                p->flag = PROCESS_FLAG_READY;
                enqueue_front(ready_queue, p); //move back to head of ready queue (not completed yet)
            }
        }
    }
    else
    {
        printf("idle (T=%d)\n", cpu_counter - 1);
    }
}

void sjf_scheduler()
{
    //TODO: implement Shortest Job First Scheduler
}

void priority_scheduler()
{
    //TODO: implememnt Priority Scheduler
}


//starts io operation for process p and moves it to waiting queue
//!this function does not requires modifications!
void enter_io(struct process* p)
{
    printf("Process pid %d is entering io (T=%d)\n", p->pid, cpu_counter);
    if (p->io_burst <= 0) //immediately finish io if io burst is 0 (i.e. data is already in cpu cache)
    {
        finish_io(p);
        return;
    }
    p->flag = PROCESS_FLAG_WAITING; //set flag to waiting
    p->io_burst_counter = 0; //reset io burst counter
    p->burst_time_before_io += p->program_counter; //schedule next io time
    enqueue_back(waiting_queue, p); //enqueue into waiting queue
}

//finishes io operation for process p and moves it to ready queue again
//!this function does not requires modifications!
void finish_io(struct process* p)
{
    printf("Process pid %d is exiting io (T=%d)\n", p->pid, cpu_counter);
    p->io_burst_counter = 0; //reset io burst counter
    p->flag = PROCESS_FLAG_READY; //set flag to ready
}

//checks if any process has finished io operation (i.e. simulates interrupt or event)
//!this function does not requires modifications!
void io_completion()
{
    struct process* p;
    node_ptr current = waiting_queue->head;
    while (current != NULL)
    {
        p = current->data;
        if (p->flag == PROCESS_FLAG_WAITING && p->cpu_time < cpu_counter)
        {
            p->io_burst_counter++; //increment io burst counter
            p->io_burst_total++; //increment total io time
            if (p->io_burst_counter >= p->io_burst) //check if io is completed
            {
                finish_io(p); //finish io
            }
        }
        current = current->next;
    }
}

//calculates and prints average turnaround & waiting time.
//!this function does not requires modifications!
void print_statistics()
{
    node_ptr current;
    uint32_t total_tat, total_wt;
    current = terminated_queue->head;
    while(current != NULL)
    {
        total_tat += current->data->cpu_time - current->data->arrival_time; //turnaround time = exit time - arrived time
        total_tat -= current->data->io_burst_total; //exclude time spent in waiting queue
        total_wt += current->data->cpu_time - current->data->arrival_time - current->data->burst_time; //waiting time = turn around time - burst time
        total_wt -= current->data->io_burst_total; //exclude time spent in waiting queue
        current = current->next;
    }
    printf("Average Turnaround Time=%.2f\n", total_tat / (float)terminated_queue->size);
    printf("Average Waiting Time=%.2f\n", total_wt / (float)terminated_queue->size);
}