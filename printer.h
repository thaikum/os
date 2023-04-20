#ifndef PRINTER_H
#define PRINTER_H

#define WRITE 1
#define READ 0
#define INIT_SPOOL 1
#define END_SPOOL 2
#define DUMP_SPOOL 3
#define TERMINATE 4
#define PRINT 5

extern int printer_ack_pipe[2];
extern int new_spool_pipe[2];
extern int data_spool_pipe[2];

void printer_init();
void printer_main();
#endif