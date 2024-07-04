// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#include <stdio.h>
#include <stdlib.h>

// #include "instructions.h"
#include "memory.h"
#include "cpu.h"

struct memory init_memory() {
    struct memory memory = {
        {0},
        {0}, 
        {0},
        {{0}, {0}}
    };
    return memory;
}

void write(char * mem_val, int write, int write_size) {
    for (int byte = 0; byte < write_size; byte++) {
        * (mem_val + byte) = (*(mem_val + byte) & 0) | (char) (write & W_BYTE);
        write = write >> BYTE_SIZE;
    }
}

int mem_read(struct memory * mem, int address) {
    int struct_offset = 0;
    char * mem_val = 0;
    int usr_in = 0;
    int ret_val = 0;
    // char writeables[4] = {0};
    if (address >= INSTR_MEM_START && address <= INSTR_MEM_END) {
        struct_offset = (address - INSTR_MEM_START);
        mem_val = &mem->instr_mem[struct_offset];
    }
    else if (address >= DATA_MEM_START&& address <= DATA_MEM_END) {
        struct_offset = (address - DATA_MEM_START);
        mem_val = &mem->data_mem[struct_offset];
    }
    else if (address >= VROUTINE_START && address <= VROUTINE_END) {
        struct_offset = (address - VROUTINE_START);
        mem_val = &mem->v_routine_mem[struct_offset];
        if (address == CRC_ADDR) {
            scanf("%c", &mem->v_routine_mem[struct_offset]);
        }
        else if (address == CRSI_ADDR) {            
            scanf("%d", &usr_in);
            write(mem_val, usr_in, sizeof(int));
        }
    }
    else if (address >= HEAP_BANK_START) {

    }
    for (int byte = sizeof(int) - 1; byte >= 0 ; byte--) {
        ret_val = ret_val << BYTE_SIZE;
        ret_val = (ret_val | (* (mem_val + byte) & W_BYTE));
    }
    return ret_val;
}

void mem_write(struct memory * mem, int address, int to_write, int data_size) {
    int struct_offset = 0;
    int bytes_to_write = (int) (data_size / 8);
    char * mem_val = 0;
    if (data_size == BYTE_SIZE) {
        to_write = (char) to_write;
    }
    else if (data_size == HALF_WORD_SIZE) {
        to_write = (short) to_write;
    }

    if (address >= INSTR_MEM_START && address <= INSTR_MEM_END) {
        struct_offset = (address - INSTR_MEM_START);
        mem_val = &mem->instr_mem[struct_offset];
        write(mem_val, to_write, bytes_to_write);
    }
    else if (address >= DATA_MEM_START&& address <=DATA_MEM_END) {
        struct_offset = (address - DATA_MEM_START);
        mem_val = &mem->data_mem[struct_offset];
        write(mem_val, to_write, bytes_to_write);
    }
    else if (address >= VROUTINE_START && address <= VROUTINE_END) {
        struct_offset = (address - VROUTINE_START);
        mem_val = &mem->v_routine_mem[struct_offset];
        write(mem_val, to_write, bytes_to_write);
        int int_print = 0;
        for (int byte = bytes_to_write - 1; byte >= 0 ; byte--) {
            int_print = int_print << BYTE_SIZE;
            int_print = (int_print | \
            (* (mem_val + byte) & W_BYTE));
        }
        // Virtual routine special returns
        if (address == HALT_ADDR) {
            printf("CPU Halt Requested\n");
            exit(0);
        }
        else if (address == CWSI_ADDR) {
            printf("%d", int_print);
        }
        else if (address == CWUI_ADDR) {
            printf("%x", int_print);
        }
        else if (address == CWC_ADDR) {
            printf("%c", mem->v_routine_mem[struct_offset]);
        }
        else if (address == MEM_DUMP_ADDR) {
            printf("0x%x", address);
        }
    }
}
