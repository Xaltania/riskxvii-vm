// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "cpu.h"
#include "instructions.h"
#include "vm.h"

/**
 * Virtual machine
 * Takes in program and loads it into memory
 * Utilises the 'CPU' to
 * fetch - done by reading a file
 * decode - parsing, identifying the operations, registers etc
 * execute - comparing opcodes, finding the operation
 *      and performing said operations
*/
void vm_init() {

}

int vm_load(char program_file[], struct memory * memory) {
    /**
     * Loads the program into the virtual machine's memory
     * Returns 0 on successful read
     * Returns 1 on file not found
    */

    FILE * program = fopen(program_file, "rb");
    if (program == NULL) {
        perror("Error! File not found.");
        return 1;
    }
    fread(memory->instr_mem, INSTR_MEM_SIZE, 1, program);
    fread(memory->data_mem, DATA_MEM_SIZE, 1, program);
    fclose(program);

    return 0;
}

void vm_run(char program[]) {
    struct cpu cpu_struct = cpu_init();
    struct cpu * cpu = &cpu_struct;
    struct memory memory_struct = init_memory();
    struct memory * vm_memory = &memory_struct;
    vm_load(program, vm_memory);
    int i = 0;
    while (1) {
        int bin_instr = fetch(cpu, vm_memory);
        decode(bin_instr, &cpu->current_instr);
        // cpu_display(cpu);
        execute(cpu, vm_memory);
        i++;
    }
}
