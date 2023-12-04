#include "process.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

scheduling_algorithm SCHEDULING_ALGORITHM = SCHEDULING_ALGORITHM_FCFS;

uint32_t latest_pid = 0;
uint32_t cpu_counter = 0;
queue_ptr job_queue = NULL;
queue_ptr ready_queue = NULL;
queue_ptr waiting_queue = NULL;
queue_ptr terminated_queue = NULL;

struct process* spawn(uint32_t arrival_time, uint32_t priority, uint32_t total_burst_time, uint32_t io_burst, uint32_t burst_time_before_io)
{
    struct process* p = (struct process*) malloc(sizeof(struct process)); //allocate memory for process
    p->pid = ++latest_pid; //increment latest_pid and assign to pid
    p->arrival_time = arrival_time;
    p->flag = PROCESS_FLAG_NEW; //set flag to new
    p->program_counter = 0;
    p->priority = priority;
    p->burst_time = total_burst_time; //this is total burst time to complete
    p->burst_time_before_io = burst_time_before_io; //this is required burst time before io, after that we need to wait for io (put it into waiting queue)
    p->io_burst = io_burst;
    p->io_burst_counter = 0;
    p->io_burst_total = 0;

    enqueue_orderby_arrival(job_queue, p); //enqueue into job queue, ascending by arrival time
    return p;
}

void exit_process(struct process* p)
{
    p->flag = PROCESS_FLAG_TERMINATED; //set flag to terminated
    p->cpu_time = cpu_counter;
    enqueue_orderby_pid(terminated_queue, p); //enqueue into terminated queue
    printf("Process pid %d terminated (T=%d)\n", p->pid, p->cpu_time);
}
