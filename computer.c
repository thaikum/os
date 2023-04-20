#include "main.h"
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>

cpu_reg *reg;
PCB *pcb;

/**
 * process_set_registers - sets registers form the PCB
 */
void process_init_registers(void)
{
	reg = malloc(sizeof(PCB));
}

int main()
{
	FILE *fp = fopen("config.sys", "r");
	int size;

	if (!fp)
	{
		fprintf(stderr, "Unable to locate config.sys\n");
		exit(99);
	}

	fscanf(fp, "%d", &size);
	fclose(fp);

	print_init();
	process_init_registers();
	mem_init(size);
	init_idle_process();
	shell_init();

    print_terminate();
//    wait(NULL);
}
