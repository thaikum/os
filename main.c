#include <stdio.h>
#include "main.h"
#include <unistd.h>
#include <sys/wait.h>

int main(){
    print_init();

    char tt[] = "hello";
    print_init_spool(30);
    print_print(tt, 30);
//    print_end_spool(30);
    print_terminate();
    wait(NULL);
    return (0);
}