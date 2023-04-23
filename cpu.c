#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define LOAD_C 1
#define LOAD 2
#define ADD 3
#define MUL 4
#define STORE 5
#define  IFGO 6
#define PRINT 7
#define SLEEP 8
#define SHELL 9
#define EXIT 0

extern cpu_reg *reg;
extern int *Mem;
extern pthread_mutex_t sh;

/**
 * cpu_mem_address - calculates the physical memory address
 * @param absolute_addr - absolute address
 * @return - physical address
 */
int cpu_mem_address(int absolute_addr) {
    reg->MAR = (reg->Base + absolute_addr);
    return (reg->MAR);
}

/**
 * cpu_fetch_instruction - fetches the instruction and its its value
 */
void cpu_fetch_instruction(void) {
    reg->MAR = reg->PC + reg->Base;
    reg->IR0 = readMemory();

    reg->MAR++;

    reg->IR1 = readMemory();
    cpu_mem_address(reg->IR1);
}

/**
* cpu_execute_instruction - executes instructions 1 by 1
*/
void cpu_execute_instruction(void) {
    switch (reg->IR0) {
        case LOAD_C:
            break;
        case LOAD:
            reg->AC = readMemory();
            break;
        case ADD:
            reg->AC += readMemory();
            break;
        case MUL:
            reg->AC *= readMemory();
            break;
        case STORE:
            reg->MAR = cpu_mem_address(reg->IR1);
            writeMemory();
            break;
        case IFGO:
            reg->PC = reg->IR1 - 2;
            break;
        case PRINT:
            printf("AC: %d\n", reg->AC);
            print_print(reg->AC, reg->PID);
            break;
        case SLEEP:
            sleep(readMemory());
            break;
        case SHELL:
            shell_command(reg->IR1);
            break;
        case EXIT:
            print_end_spool(reg->PID);
            process_exit(reg->PID);
            break;
        default:
            reg->exec_status = 0;
            fprintf(stderr, "Invalid command encountered %d\n", reg->IR0);
            break;
    }
    reg->PC += 2;
}

/**
* cpu_operation - executes commands in a loop
*/
void cpu_operation(void) {
        cpu_fetch_instruction();
        cpu_execute_instruction();
}