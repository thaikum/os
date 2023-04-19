#ifndef MAIN_H
#define MAIN_H

typedef struct cpu_reg{
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

typedef struct pcb{
    int pid;
    int ac;
    int pc;
    int IR0;
    int IR1;
    int MAR;
    int MDR;
    int Base;
    int num_of_instructions;
    int number_of_static_data;
    int number_of_data;
    char *file_name;
    int p_status;
} PCB;

typedef struct item{
    PCB pcb;
    struct item *next;
} q_item;

/* readyQ */
q_item *enqueue(PCB pcb);
void dequeue();
void print_all_pids();
void remove_by_pid(int);
PCB *get_process_by_id(int pid);
void init();

/* scheduler.c */
//void process_init_PCBs(int number_of_processes);
//void process_init_PCB(char *file_name);
//void process_dispose_PCB();
//void process_init_readyQ();
//void process_insert_readyQ();
//PCB *process_fetch_readyQ();
//void process_dump_readyQ();
//void process_context_switch();
//void process_init();
//void process_submit(char *file_name, int base);
//void process_execute();
//void process_exit();

/* memory functions */
void mem_init(int size);
int readMemory();
void writeMemory();
void clean();

/* load functions */
void load_prog(char *fname, int base);

/* cpu */
void cpu_operation();
#endif