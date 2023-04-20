#ifndef SPOOL_H
#define SPOOL_H
#include <stdio.h>

typedef struct sp{
	FILE *fp;
	int pid;
	struct sp *next;
} spool_item;

extern spool_item *spool_head;

void spool_enqueue(FILE *fp, int pid);
void spool_dequeue();
FILE *find_spool_by_pid(int pid);
void remove_spool_by_pid(int pid);
void print_all_spools();
#endif