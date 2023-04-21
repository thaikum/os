#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printer.h"
#include "spool_ds.h"
#define PRINTER_OUTPUT "printer.out"


/**
 * printer_init_spool - opens a spool file with the pid.spool
 * @param pid - file to open.
 * @return
 */

void *printer_init_spool(int pid)
{
	char str[13];

	FILE *fp = find_spool_by_pid(pid);

	if (!fp)
	{
		sprintf(str, "%d.spool", pid);
		fp = fopen(str, "a+");
		spool_enqueue(fp, pid);
	}
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
	fp = fopen(PRINTER_OUTPUT, "w");
	write(printer_ack_pipe[WRITE], &done, sizeof(int));
	close(printer_ack_pipe[WRITE]);
	fclose(fp);
}

void printer_end_spool(int pid)
{
	FILE *fp = find_spool_by_pid(pid);
	char file_name[13];
	char ch;
	char *buff = malloc(10);
	size_t t = 10;
	FILE *printer_output;

	printer_output = fopen(PRINTER_OUTPUT, "a");

	sprintf(file_name, "%d.spool", pid);
	fclose(fp);
	fp = fopen(file_name, "r");


	if (fp)
	{
		fprintf(printer_output, "\n--------------------- Process: %d -----------------------\n", pid);
		while (getline(&buff, &t, fp) != EOF)
		{
			fprintf(printer_output, "%s", buff);
		}
	}

	fclose(fp);
	fclose(printer_output);
	remove(file_name);
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
	if (read(data_spool_pipe[READ], &size, sizeof(size_t)))
	{
		buff = malloc(size);

        printf("Size is: %lu", size);
		if (read(data_spool_pipe[READ], buff, size))
		{
            printf("string is: %s\n", buff);


			fp = find_spool_by_pid(pid);

			if (fp)
			{
				fprintf(fp, "%s ", buff);
			}
		}
		free(buff);
	}
};

void printer_terminate()
{
	char *buff = malloc(10 * sizeof(char));
	FILE *fp;
	char filename[13];
	size_t x = 10;
	FILE *printer_output;

	printer_output = fopen(PRINTER_OUTPUT, "a");

	while (spool_head)
	{
		sprintf(filename, "%d.spool", spool_head->pid);
		printf("filename: %s", filename);
		fclose(spool_head->fp);

		fp = fopen(filename, "r");


		if (fp)
		{
			fprintf(printer_output, "\n--------------------- Process: %d -----------------------\n",
				spool_head->pid);
			while (getline(&buff, &x, fp) != EOF)
			{
				fprintf(printer_output, "%s", buff);
			}
			fprintf(printer_output, "\n-------------- Process: %d did not complete -------------\n",
				spool_head->pid);
			fclose(fp);
			remove(filename);
		}

		spool_dequeue();
	}
	fclose(printer_output);
}

void printer_main()
{
	int sig[2];
	FILE *fp;

	printer_init();

	close(new_spool_pipe[WRITE]);

	while (read(new_spool_pipe[READ], sig, sizeof(sig)))
	{
		switch (sig[0])
		{
		case INIT_SPOOL:
			printer_init_spool(sig[1]);
			break;
		case END_SPOOL:
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
			fprintf(stderr, "Invalid printer command\n");
		}
	}
	close(new_spool_pipe[READ]);
    close(data_spool_pipe[READ]);
}