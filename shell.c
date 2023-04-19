#include "main.h"
#include <stdio.h>
#include <stdlib.h>

extern cpu_reg *reg;
extern int mem_size;
extern int *Mem;

/**
 * shell_print_registers - prints register values
 */
void shell_print_registers(void)
{
	printf("AC: %d\n", reg->AC);
	printf("PC: %d\n", reg->PC);
	printf("MAR: %d\n", reg->MAR);
	printf("IR0: %d\n", reg->IR0);
	printf("IR1: %d\n", reg->IR1);
}

/**
 * shell_print_memory - prints all the content in the memory
 */
void shell_print_memory(void)
{
	int x = 0;

	for (; x < mem_size; x++){
		printf("%d ", Mem[x]);
	}
}

/**
 * shell_command - handles shell commands
 */
void shell_command(int cmd)
{
	char *file_name = NULL;
	int base;

	switch (cmd)
	{
	case 0:
		exit(0);
	case 1:
//		printf("Enter the file name followed by the base: \n");
//		scanf("%s\n%d", file_name, &base);
//		process_submit(file_name, base);
		break;
	case 2:
		shell_print_registers();
		break;
	case 3:
		break;
	case 4:
//		process_dump_readyQ();
		break;
	case 5:
		break;
	case 6:
		break;
	case 9:
		shell_print_registers();
		break;
	default:
		break;
	}
}

/**
* shell_init - initializes the shell
*/
void shell_init(void)
{
	int cmd;

	printf("Input a a command  1 - 9: ");
	scanf("%d", &cmd);
	shell_command(cmd);
}