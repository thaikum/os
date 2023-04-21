#include "main.h"
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

cpu_reg *reg;
PCB *pcb;
int TQ = 5;

void clean_registers(){
    reg->IR0 = 0;
    reg->AC = 0;
    reg->IR1 = 0;
    reg->PC = 0;
    reg->PID = 1;
    reg->PID = COMPLETED;
}
/**
 * process_set_registers - sets registers form the PCB
 */
void process_init_registers(void)
{
	reg = malloc(sizeof(PCB));
    clean_registers();
}

int main()
{
	FILE *fp = fopen("config.sys", "r");
	int size;
	pthread_t shell, scheduler;

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


    pthread_create(&shell, NULL, (void *(*)(void *)) &shell_init, NULL);
	process_execute();
	pthread_join(shell, NULL);

	wait(NULL);
}
