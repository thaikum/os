#include "computer.h"
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

cpu_reg *reg;
int PT;
int TQ;

void clean_registers(){
    reg->IR0 = 6;
    reg->AC = 0;
    reg->IR1 = 0;
    reg->PC = 0;
    reg->PID = 1;
    reg->exec_status = COMPLETED;
}
/**
 * process_set_registers - sets registers form the PCB
 */
void process_init_registers(void)
{
	reg = malloc(sizeof(PCB));
    clean_registers();
}

int main(int argc, char *argv[])
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
    fscanf(fp, "%d", &TQ);
    fscanf(fp, "%d", &PT);
	fclose(fp);

	print_init();
	process_init_registers();
	mem_init(size);

    init_idle_process();
    if (argc == 2){
        fp = fopen(argv[1], "r");
        char file[30];
        char *line = malloc(35);
        size_t len = 7;
        int base;
        int words;

        while(getline(&line, &len, fp) != EOF){
            sscanf(line, "%s%d", file, &base);
            words = load_prog(file, base);
            if (words > 0){
                process_submit(file, base, words);
            }
            bzero(file, 30);
            bzero(line, 35);
        }
        free(line);
        fclose(fp);
    }

    pthread_create(&shell, NULL, (void *(*)(void *)) &shell_init, NULL);
	process_execute();

	pthread_join(shell, NULL);

	wait(NULL); //waits for the printer
}
