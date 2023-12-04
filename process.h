#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "queue.h"

//flags for process
typedef enum
{
    PROCESS_FLAG_NEW = 0, //process is just created
    PROCESS_FLAG_READY = 1, //process is ready to run
    PROCESS_FLAG_RUNNING = 2, //process is running
    PROCESS_FLAG_WAITING = 3, //process is waiting for something (io, etc.)
    PROCESS_FLAG_TERMINATED = 4 //process is terminated
} process_flag;

//scheduling algorithms
typedef enum
{
    SCHEDULING_ALGORITHM_FCFS = 0, //first come first serve
    SCHEDULING_ALGORITHM_SJF = 1, //shortest job first
    SCHEDULING_ALGORITHM_PRIORITY = 2, //priority
} scheduling_algorithm;

struct process
{
    uint32_t pid;
    uint32_t arrival_time; //in cpu cycles
    process_flag flag;
    uint32_t priority;
    uint32_t burst_time; //in cpu cycles
    uint32_t burst_time_before_io; //in cpu cycles
    uint32_t io_burst; //in cpu cycles
    uint32_t io_burst_counter; //in cpu cycles (keep track of how much io burst completed)
    uint32_t io_burst_total; //time spent in io
    uint32_t program_counter; //in cpu cycles (local time for process, i.e. how much burst time completed)
    uint32_t cpu_time; //in cpu cycles (global time for process, i.e. last cpu cycle when process was running, like last time seen in cpu)
    //we can also include other information like program memory, stack pointer, registers, etc.
    //but we don't need them for this project
};


//global variables
extern scheduling_algorithm SCHEDULING_ALGORITHM; //scheduling algorithm to use
extern uint32_t latest_pid; //latest created process pid
extern uint32_t cpu_counter; //cpu cycle counter

//global process queues
extern queue_ptr job_queue; //job queue, contains all processes not yet ready to run
extern queue_ptr ready_queue; //ready queue, contains all processes ready to run but not running
extern queue_ptr waiting_queue; //waiting queue (io queue), contains all processes waiting for something (disk write, memory read, other io devices, etc.)
extern queue_ptr terminated_queue; //terminated queue (exit queue), contains all processes that have been terminated

//allocates and initializes a new process with given priority
struct process* spawn(uint32_t arrival_time, uint32_t priority, uint32_t total_burst_time, uint32_t io_burst, uint32_t burst_time_before_io);

//exists process
void exit_process(struct process* p);

#endif