#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

cpu_reg *reg;
PCB *pcb;

int global_pid = 0;

/**
 * process_init_PCB - initializes an empty PCB but with a unique id;
 */
void process_init_PCB()
{
	pcb = malloc(sizeof(PCB));

	pcb->file_name = NULL;
	pcb->MDR = 0;
	pcb->IR0 = 0;
	pcb->pid = global_pid;
	pcb->ac = 0;
	pcb->pc = 0;
	pcb->IR1 = 0;
	pcb->MAR = 0;

	global_pid++;
}

/**
 * process_set_registers - sets registers form the PCB
 */
void process_set_registers(void)
{
	reg = malloc(sizeof(PCB *));

	reg->IR1 = pcb->IR1;
	reg->MDR = pcb->MDR;
	reg->IR0 = pcb->IR0;
	reg->PID = pcb->pid;
	reg->AC = pcb->ac;
	reg->PC = pcb->pc;
	reg->Base = pcb->Base;
	reg->exec_status = 1;
}

int main()
{
	FILE *fp = fopen("config.sys", "r");
	int size, base;
	char input[50], *file_name;
	char *pt;

	if (!fp)
	{
		fprintf(stderr, "Unable to locate config.sys\n");
		exit(99);
	}

	fscanf(fp, "%d", &size);
	fclose(fp);

	mem_init(size);

	printf("Enter the file to read followed by base: ");
	scanf("%s %d", file_name, &base);

	load_prog(file_name, base);
	process_init_PCB();
	pcb->Base = base;
	process_set_registers();

	cpu_operation();
	clean();
}
