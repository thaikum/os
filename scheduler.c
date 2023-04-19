#include <stdarg.h>
#include <stdlib.h>
#include "main.h"

extern int global_pid; //keeps track of the latest pid
PCB *current_pcb;

PCB **PCBs;
int pcb_index = 0;

/**
 * process_init_PCBs - initializes several PCBs
 * @param number_of_processes - number of processes to initialize
 */
void process_init_PCBs(int number_of_processes)
{
    PCBs = calloc(number_of_processes, sizeof(PCB *));
}

/**
 * process_init_PCB - initializes a PCB for a new process
 * @param file_name: process file name
 */
//void process_init_PCB(char *file_name)
//{
//	current_pcb = malloc(sizeof(PCB));
//
//	current_pcb->file_name = file_name;
//	current_pcb->pid = global_pid;
//	global_pid++;
//    PCBs[pcb_index] = current_pcb;
//}

/**
 * process_dispose - disposes a PCB entry for an existing process
 */
//void process_dispose_PCB()
//{
//	free(current_pcb);
//}

/**
 * process_init_readyQ - initializes a readyQ
 */
void process_init_readyQ()
{
	init();
}

/**
 * process_insert_readyQ - inserts to the readyQ
 */
void process_insert_readyQ()
{
	enqueue(*current_pcb);
}

/**
 * process_fetch_readyQ - fetches a process to execute from the ready queue
 */
PCB *process_fetch_readyQ()
{
	PCB *pcb;

	// todo figure out how to implement the round robin
	pcb = get_process_by_id(0);

	return (pcb);
}

/**
 * process_dump_readyQ - prints all the processes in the readyQ
 */
void process_dump_readyQ()
{
	print_all_pids();
}

/**
 * process_context_switch - context switch two processes
 */
void process_context_switch()
{
	//todo implement body
}

/**
 * process_init - initializes scheduling
 */
void process_init()
{
	//todo figure out how to init PCBs
	process_init_readyQ();
}

/**
 * process_submit - submits a process
 * @param filename: the file name of the process being submited
 */
void process_submit(char *file_name, int base)
{
//	process_init_PCB(file_name);
	process_insert_readyQ();
	//todo print_init_spool
}

/**
* process_execute
*/
void process_execute()
{
	PCB *pcb;
	process_context_switch();
	pcb = process_fetch_readyQ();

	if (!pcb)
	{
		//todo execute idle process
	}
	else
	{
		//todo cpu_operation
	}
}

/**
* process_exit - exits a process
*/
void process_exit()
{
//	process_dispose_PCB();
	//todo print_end_spool
}
