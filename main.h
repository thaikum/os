#ifndef MAIN_H
#define MAIN_H

#define EXECUTING 1;
#define READY 2;
#define WAITING 3;

typedef struct cpu_reg
{
	int PID;
	int PC;
	int IR0;
	int IR1;
	int AC;
	int MAR;
	int MDR;
	int Base;
	int exec_status;
} cpu_reg;

typedef struct pcb
{
	int pid;
	int ac;
	int pc;
	int IR0;
	int IR1;
	int MAR;
	int MDR;
	int Base;
	int no_of_words;
	char *file_name;
	int exec_status;
} PCB;

typedef struct item
{
	PCB *pcb;
	struct item *next;
} q_item;

/* readyQ */
q_item *enqueue(PCB *pcb, q_item **, q_item **);

q_item *dequeue(q_item **, q_item **);

void print_all_pids(q_item *);

void remove_by_pid(int, q_item **, q_item **);

PCB *get_process_by_id(int pid, q_item *);

void init();

/* scheduler.c */
//void process_init_PCBs(int number_of_processes);
//void process_init_PCB(char *file_name);
//void process_dispose_PCB();
void process_dump_PCBs();

void init_idle_process();

//void process_init_readyQ();
//void process_insert_readyQ();
//PCB *process_fetch_readyQ();
void process_dump_readyQ();

//void process_context_switch();
//void process_init();
void process_submit(char *file_name, int base, int prog_len);

void process_execute();

void process_exit(int pid);


/* memory functions */
void mem_init(int size);

void clear_memory(int start, int size);

int readMemory();

void writeMemory();

void clean();


/* load functions */
int load_prog(char *fname, int base);


/* cpu */
void cpu_operation();


/* shell */
void shell_command(int cmd);

void shell_init(void);

/* computer */
void process_set_registers(void);

/* print */
void print_init();
void send_data(int action, int pid);
void print_init();
void print_init_spool(int pid);
void print_end_spool(int pid);
void print_terminate();
void print_print(char *buf, int pid);

#endif