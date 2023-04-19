#include <stdio.h>
#include <stdlib.h>
#include "main.h"

extern int *Mem;
extern int mem_size;


/**
 * load_finish - cleans up and close the program file
 * @param f: the file to close
*/
void load_finish(FILE *f)
{
	fclose(f);
}

/**
 * load_prog - loads a program into the memory
 * @param fname: the program file
 * @param base: the memory offset
*/
void load_prog(char *fname, int base)
{
	FILE *fp;
	int i_len, d_len, len, x;

	Mem = calloc(50, sizeof(int));

	if (base > mem_size)
	{
		fprintf(stderr, "invalid base\n");
		clean();
		exit(98);
	}

	fp = fopen(fname, "r");
	if (!fp)
	{
		fprintf(stderr, "Cannot read file %s\n", fname);
		clean();
		exit(98);
	}

	fscanf(fp, "%d", &i_len);
	fscanf(fp, "%d", &d_len);
	len = (i_len * 2) + d_len + base;

	for (x = base; x < len; x++)
	{
		fscanf(fp, "%d", &Mem[x]);
	}

	load_finish(fp);
}
