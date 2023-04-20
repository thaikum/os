#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printer.h"
#include "spool_ds.h"

FILE *printer_output;

/**
 * printer_init_spool - opens a spool file with the pid.spool
 * @param pid - file to open.
 * @return
 */

FILE *printer_init_spool(int pid)
{
	char str[13];
	int x = pid, y = 0;
	sprintf(str, "%d.spool", pid);

	FILE *fp = fopen(str, "a+");
	return (fp);
}

/**
 * printer_init - initializes the printer module by opening the print.out file
 */
void printer_init(void)
{
	int done = 1;
	int pid;
	FILE *fp;
	close(printer_ack_pipe[READ]);
	printer_output = fopen("printer.out", "w");
	write(printer_ack_pipe[WRITE], &done, sizeof(int));
	close(printer_ack_pipe[WRITE]);
}

void printer_end_spool(int pid)
{
	FILE *fp;
	char *buff = malloc(10 * sizeof(char));
	size_t n = 10;
	fp = find_spool_by_pid(pid);

	if (fp)
	{
		printf("file was found");
		while (getline(&buff, &n, fp) != EOF);
		fprintf(fp, "\n ------- process %d -------- \n %s", pid, buff);
	}

	fclose(fp);
	remove_spool_by_pid(pid);
}

void printer_dump_spool()
{
	print_all_spools();
}

void printer_print(int pid)
{
	char *buff;
	size_t size;
	FILE *fp;

	close(data_spool_pipe[WRITE]);
	read(data_spool_pipe[READ], &size, sizeof(size_t));
	buff = malloc(size);
	read(data_spool_pipe[READ], &buff, size);
	close(data_spool_pipe[READ]);

	fp = find_spool_by_pid(pid);

	if (fp)
	{
		fprintf(fp, "%s ", buff);
	}
	free(buff);
};

void printer_terminate()
{
	char *buff = malloc(10 * sizeof(char));
	size_t x = 10;

	while (spool_head)
	{
		while (getline(&buff, &x, spool_head->fp) != EOF);
		fprintf(printer_output,
			"\n------------ %d ------------- \n %s \n -------"
			"--------- porcess did not complet -------------\n",
			spool_head->pid, buff);
		spool_dequeue();
	}
	fclose(printer_output);
}

void printer_main()
{
	int pid;
	int sig[2];
	FILE *fp;

	printer_init();

	close(new_spool_pipe[WRITE]);

	while (read(new_spool_pipe[READ], &sig, sizeof(sig)))
	{
		switch (sig[0])
		{
		case INIT_SPOOL:
			fp = printer_init_spool(sig[1]);
			spool_enqueue(fp, sig[1]);
			break;
		case END_SPOOL:
			printf("I was called");
			printer_end_spool(sig[1]);
			break;
		case DUMP_SPOOL:
			printer_dump_spool();
			break;
		case TERMINATE:
			printer_terminate();
			break;
		case PRINT:
			printer_print(sig[1]);
			break;
		default:
			fprintf(stderr, "Invalid printer command");
		}
	};

	close(new_spool_pipe[READ]);
}