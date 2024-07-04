// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#include <stdio.h>

#include "memory.h"
#include "cpu.h"
#include "instructions.h"
#include "vm.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        perror("Error! No program supplied.");
        return 1;
    }
    // printf("Initialising %s\n", argv[1]);
    char * program = argv[1];
    vm_run(program);

    return 0;
}