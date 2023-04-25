#include "computer.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * dispose_list_item - frees a list item
 */
void dispose_list_item(q_item *qItem) {
    free(qItem->pcb);
    free(qItem);
}

/**
 * pcb_enqueue - adds a process in the ready queue
 * @pcb: a pcb for the new item
 */
q_item *enqueue(PCB *pcb, q_item **q_head, q_item **q_tail) {
    q_item *new_item = malloc(sizeof(q_item));

    if (!new_item) {
        printf("Failed to insert process in the queue");
        exit(20);
    }

    new_item->pcb = pcb;
    new_item->next = NULL;

    if (!!*q_head) {
        (*q_tail)->next = new_item;
        *q_tail = new_item;
    } else {
        *q_head = new_item;
        *q_tail = new_item;
    }

    return (new_item);
}

/**
 * pcb_dequeue - pops the first element from the queue
 */
q_item *dequeue(q_item **q_head, q_item **temp_item) {
    if (*q_head) {
        *temp_item = *q_head;
        *q_head = (*q_head)->next;
    }
    return (*temp_item);
}

/**
 * remove_by_pid - removes a process from the readyQ by its pid
 * @param pid: id of the process to remove
 */
void remove_by_pid(int pid, q_item **q_head, q_item **q_tail) {
    q_item *cur_item = *q_head, *prev_item = NULL;

    for (; cur_item; prev_item = cur_item, cur_item = cur_item->next) {
        if (cur_item->pcb->pid == pid) {
            if (cur_item == *q_head) {
                *q_head = cur_item->next;
            } else {
                prev_item->next = cur_item->next;

                if (cur_item == *q_tail) {
                    *q_tail = prev_item;
                }
            }
            free(cur_item);
            return;

        }
    }
}

/**
 * get_process_by_id -  gets a process based on its pid
 * @param pid: the id of the process to get
 */
PCB *get_process_by_id(int pid, q_item *q_head) {
    q_item *cur_item = q_head;

    for (; cur_item; cur_item = cur_item->next) {
        if ((cur_item->pcb)->pid == pid) {
            return (cur_item->pcb);
        }
    }

    return (NULL);
}

/**
 * pcb_dump: prints all information in the pcb
 */
void print_all_pids(q_item *q_head) {
    q_item *temp_item = q_head;

    if (temp_item) {
        while (temp_item) {
            printf("Process ID: %d\n", (temp_item->pcb)->pid);
            temp_item = temp_item->next;
        }
    }
}
