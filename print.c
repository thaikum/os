#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "printer.h"

int printer_ack_pipe[2];
int new_spool_pipe[2];
int data_spool_pipe[2];

void send_data(int action, int pid)
{
        int sig[] = {action, pid};

        close(new_spool_pipe[READ]);

        write(new_spool_pipe[WRITE], sig, sizeof sig);

}

/**
 * print_init - initializes the printer
 */
void print_init()
{
	int id, ack;

	if (pipe(printer_ack_pipe) == -1 || pipe(new_spool_pipe) == -1 || pipe(data_spool_pipe) == -1)
	{
		fprintf(stderr, "Error in opening pipes");
		exit(70);
	}


	id = fork();

	if (id == -1)
	{
		printf("failed forking");
		exit(71);
	}
	else if (id == 0)
	{
		printer_main();
	}
	else
	{
		close(printer_ack_pipe[WRITE]);
		if (read(printer_ack_pipe[READ], &ack, sizeof(int)) == -1)
		{
			fprintf(stderr, "couldn't read");
			exit(72);
		}
		close(printer_ack_pipe[READ]);
	}
}

void print_init_spool(int pid)
{
	send_data(INIT_SPOOL, pid);
}

void print_end_spool(int pid)
{
	send_data(END_SPOOL, pid);
}

void print_terminate()
{
	send_data(TERMINATE, 0);
    close(new_spool_pipe[WRITE]);
    close(data_spool_pipe[WRITE]);
}

void print_print(int buf, int pid)
{
    char str[9] ;
    sprintf(str, "%d", buf);
	size_t buf_len = strlen(str);
    send_data(PRINT, pid);

	close(data_spool_pipe[READ]);
	write(data_spool_pipe[WRITE], &buf_len, sizeof(size_t));
	write(data_spool_pipe[WRITE], str, buf_len + 1);
//	close(data_spool_pipe[WRITE]);
}

