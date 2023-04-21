#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

extern cpu_reg *reg;
extern int *Mem;

int global_pid = 1; //keeps track of the latest pid

q_item *PCBs_head;
q_item *PCBs_tail;

q_item *readyQ_head;
q_item *readyQ_tail;

q_item *temp_item;

/**
 * process_init_PCBs - initializes several PCBs
 * @param number_of_processes - number of processes to initialize
 */
void process_init_PCBs()
{
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
PCB *process_init_PCB(char *file_name, int base, int prog_len)
{
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
void process_dispose_PCB(int pid)
{
	remove_by_pid(pid, &PCBs_head, &PCBs_tail);
}

/**
 * process_dump_PCBs - prints the pids of all the PCBs in the PCB list
 */
void process_dump_PCBs()
{
	print_all_pids(PCBs_head);
}

/**
 * process_init_readyQ - initializes a readyQ
 */
void process_init_readyQ()
{
	readyQ_head = NULL;
	readyQ_tail = NULL;
}

/**
 * process_insert_readyQ - inserts to the readyQ
 */
void process_insert_readyQ(PCB *pcb)
{
	enqueue(pcb, &readyQ_head, &readyQ_tail);
}

/**
 * process_context_switch - context switch two processes
 * @direction: i if in and o if out
 */
void process_context_switch(PCB *n_pcb)
{
	PCB *old_pcb = get_process_by_id(reg->PID, PCBs_head);

	if (old_pcb)
	{
		old_pcb->IR1 = reg->IR1;
		old_pcb->MDR = reg->MDR;
		old_pcb->IR0 = reg->IR0;
		old_pcb->ac = reg->AC;
		old_pcb->pc = reg->PC;
		old_pcb->MAR = reg->MAR;
		old_pcb->exec_status = WAITING;
	}

	if (n_pcb)
	{
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
 */
PCB *process_fetch_readyQ()
{
	// todo figure out how to implement the round robin
	if (temp_item)
	{
		free(temp_item);
		temp_item = NULL;
	}

	if (readyQ_head)
	{
		dequeue(&readyQ_head, &temp_item);

		process_context_switch(temp_item->pcb);
		return (temp_item ? temp_item->pcb : NULL);
	}
	return (NULL);
}

/**
 * process_dump_readyQ - prints all the processes in the readyQ
 */
void process_dump_readyQ()
{
	print_all_pids(readyQ_head);
}


/**
 * process_init - initializes scheduling
 */
void process_init()
{
	process_init_PCBs();
	process_init_readyQ();
}

/**
 * process_submit - submits a process
 * @param filename: the file name of the process being submited
 */
void process_submit(char *file_name, int base, int prog_len)
{
	PCB *pcb = process_init_PCB(file_name, base, prog_len);
	process_insert_readyQ(pcb);
	process_execute();
	//todo print_init_spool
}

/**
* process_execute
*/
void process_execute()
{
	PCB *pcb;

	while (readyQ_head)
	{
		pcb = process_fetch_readyQ();

		if (!pcb)
		{
			goto idle;
		}
		else
		{
			cpu_operation();
		}
	}

    idle:
	pcb = get_process_by_id(1, PCBs_head); //get the idle process
	process_context_switch(pcb);
	cpu_operation();
}

/**
* process_exit - exits a process
*/
void process_exit(int pid)
{
	PCB *p = get_process_by_id(pid, readyQ_head);
	if (p)
	{
		clear_memory(p->Base, p->no_of_words);
		free(p);
		remove_by_pid(pid, &PCBs_head, &PCBs_tail);
		remove_by_pid(pid, &readyQ_head, &readyQ_tail);
	}
	//todo remember to print spool
}

/**
 * init_idle_process - initializes idle process into the system
 */
void init_idle_process()
{
	int idle_base = 0;
	int instructions[] = { 6, 0 };
	Mem[0] = instructions[0];
	Mem[1] = instructions[1];
	process_init_PCB("idle process", idle_base, 2);
}