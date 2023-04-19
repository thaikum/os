#include <stdlib.h>
#include "main.h"
#include <stdio.h>

int *Mem;
int mem_size;
extern cpu_reg *reg;

/**
 * mem_init - initializes a memory and allocates zeros in all the addresses
 * @param size - size of the memory to initialize
 */
void mem_init(int size)
{
	mem_size = size;
	Mem = calloc(size, sizeof(int));

	if (!Mem)
		exit(100);
}

/**
 * readMemory - reads memory content at a given address
 * @param address - memory address to read from
 * @return - content at the specified address
 */
int readMemory()
{
	return Mem[reg->MAR];
}

/**
 * writeMemory - writes to a given address in the memory
 * @param address - memory location to write
 * @param value - value to write
 */
void writeMemory()
{
	Mem[reg->MAR] = reg->AC;
}

/**
 * clean - frees the memory
 */
void clean()
{
	free(Mem);
}