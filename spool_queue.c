#include "spool_ds.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

spool_item *spool_head = NULL;
spool_item *spool_tail = NULL;

void spool_enqueue(FILE *fp, int pid)
{
	spool_item *new_item = malloc(sizeof(spool_item));

	new_item->fp = fp;
	new_item->pid = pid;

	if (spool_head == NULL)
	{
		spool_head = new_item;
		spool_tail = new_item;
	}
	else
	{
		spool_tail->next = new_item;
		spool_tail = new_item;
	}
}

spool_item *front(){
	return spool_head;
}
void spool_dequeue()
{
	spool_item *o_head = spool_head;
	if (spool_head)
	{
		spool_head = spool_head->next;
		fclose(spool_head->fp);
		free(spool_head);
	}
}


FILE *find_spool_by_pid(int pid)
{
	spool_item *n_head = spool_head;

	for (; n_head; n_head = n_head->next)
	{
		if (n_head->pid == pid)
		{
			return (n_head->fp);
		}
	}
	return (NULL);
};

void remove_spool_by_pid(int pid)
{
	spool_item *n_head = spool_head;
	spool_item *prev;

	for (; n_head; prev = n_head, n_head = n_head->next)
	{
		if (n_head->pid == pid)
		{
			if (n_head == spool_head)
			{
				spool_head = spool_head->next;
				free(n_head);
				return;
			}
			else
			{
				if (n_head == spool_tail)
				{
					free(spool_tail);
					spool_tail = prev;
					prev->next = NULL;
				}
				else
				{
					prev->next = n_head->next;
					free(n_head);
					return;
				}
			}
		}
	}
}

void print_all_spools()
{
	spool_item *n_head = spool_head;
	for (; n_head; n_head = n_head->next){
		printf("PID: %d\n", n_head->pid);
	}
}
