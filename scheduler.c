#include <stdlib.h>
#include <stdio.h>
#include "main.h"

extern const int TQ;
int terminate_flag = 0;

extern cpu_reg *reg;
extern int *Mem;

int global_pid = 1; //keeps track of the latest pid

q_item *PCBs_head = NULL;
q_item *PCBs_tail = NULL;

q_item *readyQ_head = NULL;
q_item *readyQ_tail = NULL;

q_item *temp_item;

/**
 * process_init_PCBs - initializes several PCBs
 * @param number_of_processes - number of processes to initialize
 */
void process_init_PCBs() {
    PCBs_head = NULL;
    PCBs_tail = NULL;
}

/**
 * process_init_PCB - initializes a PCB for a new process
 * @param file_name: process file name
 */
/**
 * process_init_PCB - initializes an empty PCB but with a unique id;
 */
PCB *process_init_PCB(char *file_name, int base, int prog_len) {
    PCB *pcb;
    pcb = malloc(sizeof(PCB));

    pcb->file_name = file_name;
    pcb->Base = base;
    pcb->pid = global_pid;
    pcb->MDR = 0;
    pcb->IR0 = 0;
    pcb->ac = 0;
    pcb->pc = 0;
    pcb->IR1 = 0;
    pcb->MAR = 0;
    pcb->no_of_words = prog_len;
    pcb->exec_status = READY;

    global_pid++;

    enqueue(pcb, &PCBs_head, &PCBs_tail);

    return (pcb);
}

/**
 * process_dispose - disposes a PCB entry for an existing process
 */
void process_dispose_PCB(int pid) {
    remove_by_pid(pid, &PCBs_head, &PCBs_tail);
}

/**
 * process_dump_PCBs - prints the pids of all the PCBs in the PCB list
 */
void process_dump_PCBs() {
    print_all_pids(PCBs_head);
}

/**
 * process_init_readyQ - initializes a readyQ
 */
void process_init_readyQ() {
    readyQ_head = NULL;
    readyQ_tail = NULL;
}

/**
 * process_insert_readyQ - inserts to the readyQ
 */
void process_insert_readyQ(PCB *pcb) {
    enqueue(pcb, &readyQ_head, &readyQ_tail);
}

/**
 * process_context_switch - context switch two processes
 * @direction: i if in and o if out
 */
void process_context_switch(PCB *n_pcb) {
    PCB *old_pcb = get_process_by_id(reg->PID, PCBs_head);

    if (old_pcb && old_pcb->pid != 1) {
        printf("Process %d out with a PC of: %d\n", old_pcb->pid, reg->PC);
        old_pcb->IR1 = reg->IR1;
        old_pcb->MDR = reg->MDR;
        old_pcb->IR0 = reg->IR0;
        old_pcb->ac = reg->AC;
        old_pcb->pc = reg->PC;
        old_pcb->MAR = reg->MAR;
        old_pcb->exec_status = WAITING;
        process_insert_readyQ(old_pcb);
    }

    if (n_pcb) {
        printf("Process %d in with a PC of: %d\n", old_pcb->pid, reg->PC);
        reg->IR1 = n_pcb->IR1;
        reg->MAR = n_pcb->MAR;
        reg->MDR = n_pcb->MDR;
        reg->IR0 = n_pcb->IR0;
        reg->PID = n_pcb->pid;
        reg->AC = n_pcb->ac;
        reg->PC = n_pcb->pc;
        reg->Base = n_pcb->Base;
        reg->exec_status = EXECUTING;
    }
}

/**
 * process_fetch_readyQ - fetches a process to execute from the ready queue
 * This function uses fetches in the FIFO approach.
 * This covers the round robin apart from the time quantum that is implemented in teh process_execute method
 */
PCB *process_fetch_readyQ() {
    return (dequeue(&readyQ_head, &readyQ_tail)->pcb);
}

/**
 * process_dump_readyQ - prints all the processes in the readyQ
 */
void process_dump_readyQ() {
    if (readyQ_head)
        print_all_pids(readyQ_head);
    else
        printf("The ready queue is empty\n");
}


/**
 * process_init - initializes scheduling
 */
void process_init() {
    process_init_PCBs();
    process_init_readyQ();
}

/**
 * process_submit - submits a process
 * @param filename: the file name of the process being submited
 */
void process_submit(char *file_name, int base, int prog_len) {
    PCB *pcb = process_init_PCB(file_name, base, prog_len);

    process_insert_readyQ(pcb);
    print_init_spool(pcb->pid);
}

/**
* process_execute - executes a process
*/
void *process_execute() {
    int no_of_execution = 0; //for implementing the round-robin
    PCB *pcb;

    init_idle_process();

    while (terminate_flag != 1) {

        if (readyQ_head && (no_of_execution >= TQ || reg->exec_status == COMPLETED)) {
            pcb = process_fetch_readyQ();

            if (pcb != NULL)
                process_context_switch(pcb);

            no_of_execution = 0;
        } else if (!readyQ_head && reg->exec_status == COMPLETED) {
            pcb = get_process_by_id(1, PCBs_head);
            process_context_switch(pcb);
            printf("CPU is currently idle\n");
        }

        cpu_operation();
        no_of_execution = no_of_execution > TQ ? 0 : no_of_execution + 1;
    }

    print_terminate();

    return NULL;
}

/**
* process_exit - exits a process
*/
void process_exit(int pid) {
    PCB *p = get_process_by_id(pid, PCBs_head);
    if (p) {
        printf("Process %d completed execution\n", reg->PID);
        clean_registers();
        clear_memory(p->Base, p->no_of_words);

        remove_by_pid(pid, &PCBs_head, &PCBs_tail);
        free(p);
    }
    //todo remember to print spool
}

/**
 * init_idle_process - initializes idle process into the system
 */
void init_idle_process() {
    int idle_base = 0;
    int instructions[] = {6, 0};
    Mem[0] = instructions[0];
    Mem[1] = instructions[1];
    process_init_PCB("idle process", idle_base, 1);
}