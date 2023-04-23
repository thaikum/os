#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <unistd.h>
#include <sys/wait.h>

int main(){
    q_item *head = NULL;
    q_item *tail = NULL;

    PCB *pcb = malloc(1 * sizeof(q_item));
    pcb->pid = 1;

    enqueue(pcb, &head, &tail);
    printf("queue status: %d\n", !!head);
    dequeue(&head);
//    remove_by_pid(1, &head, &tail);
    printf("queue status: %d\n", !!head);
}