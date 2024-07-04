#ifndef VM_H
#define VM_H

#include "memory.h"


void vm_init();
void vm_run(char program[]);
int vm_load(char program_file[], struct memory * memory);

#endif