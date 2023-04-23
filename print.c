#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "printer.h"

int printer_ack_pipe[2];
int new_spool_pipe[2];
int data_spool_pipe[2];

void send_data(int action, int pid) {
    int sig[] = {action, pid};

    close(new_spool_pipe[READ]);

    write(new_spool_pipe[WRITE], sig, sizeof sig);

}

void listen_for_acknowledgement(){
    int ack;

    close(printer_ack_pipe[WRITE]);

    if (read(printer_ack_pipe[READ], &ack, sizeof(int)) == -1) {
        fprintf(stderr, "couldn't read");
        exit(72);
    } else {
        printf("printer started successfully\n");
    }
    close(printer_ack_pipe[READ]);
    return;
}


/**
 * print_init - initializes the printer
 */
void print_init() {
    int id, ack;

    if (pipe(printer_ack_pipe) == -1 || pipe(new_spool_pipe) == -1 || pipe(data_spool_pipe) == -1) {
        fprintf(stderr, "Error in opening pipes");
        exit(70);
    }


    id = fork();

    if (id == -1) {
        printf("failed forking");
        exit(71);
    } else if (id == 0) {
        printer_main();
    } else {
       pthread_t t;
        pthread_create(&t, NULL, (void *(*)(void *)) &listen_for_acknowledgement, NULL);
    }
}

void print_init_spool(int pid) {
    send_data(INIT_SPOOL, pid);
}

void print_end_spool(int pid) {
    send_data(END_SPOOL, pid);
}

void print_terminate() {
    send_data(TERMINATE, 0);
    close(new_spool_pipe[WRITE]);
    close(data_spool_pipe[WRITE]);
}

void print_print(int buf, int pid) {
    send_data(PRINT, pid);
    close(data_spool_pipe[READ]);
    write(data_spool_pipe[WRITE], &buf, sizeof(int));
}

