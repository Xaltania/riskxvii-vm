#ifndef MEMORY_H
#define MEMORY_H

#define BYTE_SIZE 8
#define HALF_WORD_SIZE 16
#define INSTR_MEM_SIZE 1024
#define DATA_MEM_SIZE 1024
#define V_ROUTINE_SIZE 256
#define INSTR_MEM_INT 1024 / 4
#define DATA_MEM_INT 1024 / 4
#define V_ROUTINE_MEM_INT 256 / 4
#define BANK_SIZE_INT 64 / 4
#define BANK_COUNT 128

#define W_BYTE 0b11111111
#define W_HALF_WORD 0b1111111111111111
#define WORD 0b11111111111111111111111111111111
#define FREE_BYTE 0b11111111111111111111111100000000

#define INSTR_MEM_START 0x0000
#define INSTR_MEM_END 0x3ff
#define DATA_MEM_START 0x0400
#define DATA_MEM_END 0x7ff
#define VROUTINE_START 0x0800
#define VROUTINE_END 0x8ff
#define HEAP_BANK_START 0xb700
#define HEAP_BANK_SIZE 128 * 64
#define PROGRAM_SIZE 2048

#define CWC_ADDR 0x0800
#define CWSI_ADDR 0x0804
#define CWUI_ADDR 0x0808
#define HALT_ADDR 0x080C
#define CRC_ADDR 0x0812
#define CRSI_ADDR 0x0816
#define DUMP_PC_ADDR 0x0820
#define DUMP_REGS_ADDR 0x0824
#define MEM_DUMP_ADDR 0x0828

struct memory {
    char instr_mem[INSTR_MEM_SIZE];
    char data_mem[DATA_MEM_SIZE];
    char v_routine_mem[V_ROUTINE_SIZE];
    char heap_bank[BANK_COUNT][BANK_SIZE_INT];
};

void write(char * mem_val, int write, int write_size);
int mem_read(struct memory *mem, int address);
void mem_write(struct memory * mem, int address, int to_write, int data_size);
struct memory init_memory();

#endif