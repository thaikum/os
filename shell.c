#include "computer.h"
#include <stdio.h>
#include <stdlib.h>

extern cpu_reg *reg;
extern int mem_size;
extern int *Mem;
extern q_item *readyQ_head;
extern q_item *PCBs_head;
extern int terminate_flag;

/**
 * load_finish - cleans up and close the program file
 * @param f: the file to close
*/
void load_finish(FILE *f) {
    fclose(f);
}

/**
 * load_prog - loads a program into the memory
 * @param fname: the program file
 * @param base: the memory offset
*/
int load_prog(char *fname, int base) {
    FILE *fp;
    int i_len, d_len, len, x;

    if (base > mem_size) {
        fprintf(stderr, "invalid base\n");
        clean();
        exit(98);
    }

    fp = fopen(fname, "r");
    if (!fp) {
        fprintf(stderr, "Cannot read file %s\n", fname);
        return -1;
    }

    fscanf(fp, "%d", &i_len);
    fscanf(fp, "%d", &d_len);
    len = (i_len * 2) + d_len + base;

    for (x = base; x < len; x++) {
        fscanf(fp, "%d", &Mem[x]);
    }

    load_finish(fp);
    return (len);
}

/**
 * shell_print_registers - prints register values
 */
void shell_print_registers(void) {
    printf("AC: %d\n", reg->AC);
    printf("PC: %d\n", reg->PC);
    printf("MAR: %d\n", reg->MAR);
    printf("IR0: %d\n", reg->IR0);
    printf("IR1: %d\n", reg->IR1);
}

/**
 * shell_print_memory - prints all the content in the memory
 */
void shell_print_memory(void) {
    int x = 0;

    for (; x < mem_size; x++) {
        printf("%d ", Mem[x]);
    }
    printf("\n");
}

void submit() {
    char file_name[50];
    int base, no_of_words;
    pthread_t t;

    printf("Enter the file name followed by the base: ");
    scanf("%s %d", file_name, &base);

    no_of_words = load_prog(file_name, base);

    if (no_of_words != -1)
        process_submit(file_name, base, no_of_words);
}

/**
 * shell_command - handles shell commands
 */
void shell_command(int cmd) {
    switch (cmd) {
        case 0:
            terminate_flag = 1;
        case 1:
            submit();
            break;
        case 2:
            shell_print_registers();
            break;
        case 3:
            shell_print_memory();
            break;
        case 4:
            process_dump_PCBs();
            break;
        case 5:
            process_dump_readyQ();
            break;
        case 6:
            printer_dump_spool();
            break;
        default:
            fprintf(stderr, "Invalid command\n");
            break;
    }
}
/**
* shell_init - initializes the shell
*/
void *shell_init(void) {
    int cmd;

    while (1) {
        scanf("%d", &cmd);
        shell_command(cmd);
    }
}