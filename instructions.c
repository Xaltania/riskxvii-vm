// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#include <stdio.h>
#include <math.h>

#include "memory.h"
#include "cpu.h"
#include "instructions.h"

int get_filled_bits(int exponent) {
    /**
     * Returns binary filled with ones
     * get_filled_bits(5) returns 0b11111 (five 1s binary)
    */
   int value = 2; // Base 2
   if (exponent == 0 || exponent == 1) {
    return exponent;
   }
   for (int i = 0; i < (exponent - 1); i++) {
    value = value * 2;
   }
   return (value - 1);
}

void add(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] + \
    cpu->reg[current_instr->rs2];
}

void addi(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] + \
    current_instr->imm;
}

void sub(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] - \
    cpu->reg[current_instr->rs2];
}

void lui(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = current_instr->imm << LUI_SHIFT; //
}

void xor(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] ^ \
    cpu->reg[current_instr->rs2];
}

void xori(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] ^ \
    current_instr->imm;
}

void or(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] | \
    cpu->reg[current_instr->rs2];
}

void ori(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] | \
    current_instr->imm;
}

void and(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] & \
    cpu->reg[current_instr->rs2];
}

void andi(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->reg[current_instr->rs1] & \
    current_instr->imm;
}

void sll(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = (unsigned int) cpu->reg[current_instr->rs1] << \
    cpu->reg[current_instr->rs2];
}

void srl(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = (unsigned int) cpu->reg[current_instr->rs1] >> \
    cpu->reg[current_instr->rs2];
    // printf("original %x\nnew %x\n", cpu->reg[current_instr->rs1], cpu->reg[current_instr->rd]);
}

void sra(struct cpu * cpu, struct instruction * current_instr) {
    /**
     * Formula for bit rotate:
     * Value AND (as many 1s as shifts) << (32 - shifts)
     * (R[rs1] & 2^(R[rs2] - 1)) << (word size - R[rs2]) | R[rs1] >> R[rs2]
    */
    int r_shift = cpu->reg[current_instr->rs2];
    int preserved_bits = cpu->reg[current_instr->rs1] & \
    (get_filled_bits(r_shift));
    int l_append_bits = preserved_bits << \
    (WORD_SIZE - r_shift);
    int r_shifted = cpu->reg[current_instr->rs1] >> r_shift;
    cpu->reg[current_instr->rd] = r_shifted | l_append_bits;
}

int sext(int num, int original_len,int target_len) {
    /**
     * Sign extends a value to the target length
     * 4 bit 6 = 0b0110
     * sext(6, 4, 16) = 0b0000000000000110
     * 5 bit -2 = 0b11110
     * sext(-2, 5, 16) = 0b1111111111111110
    */
    int msb = num >> (original_len - 1);
    if (msb == 0) {
        // Positive numbers - padded with zeroes by default in integers
        return num;
    }
    else if (msb == 1) {
        // Negative numbers
        int pad_bits = get_filled_bits(target_len - original_len) << original_len;
        int new_num = pad_bits | num;
        return new_num; 
    }
    return num;
}

void lb(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int read_val = mem_read(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm);
    int ls_byte =  read_val & BYTE; // least significant byte of imm
    cpu->reg[current_instr->rd] = sext(ls_byte, 8, WORD_SIZE);
}
void lh(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int read_val = mem_read(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm);
    int ls_half =  read_val & HALF_WORD; // least significant byte of imm
    cpu->reg[current_instr->rd] = sext(ls_half, 16, WORD_SIZE);
}
void lw(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int read_val = mem_read(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm);
    cpu->reg[current_instr->rd] =  read_val;
}
void lbu(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int read_val = mem_read(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm);
    int ls_byte =  read_val & BYTE; // least significant byte of imm
    cpu->reg[current_instr->rd] = ls_byte;
}
void lhu(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int read_val = mem_read(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm);
    int ls_half =  read_val & HALF_WORD; // least significant byte of imm
    cpu->reg[current_instr->rd] = ls_half;
}
void sb(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int write = cpu->reg[current_instr->rs2] & BYTE;
    mem_write(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm, write, BYTE_SIZE);
}
void sh(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int write = cpu->reg[current_instr->rs2] & HALF_WORD;
    mem_write(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm, write, HALF_WORD_SIZE);
}
void sw(struct cpu * cpu, struct instruction * current_instr, \
struct memory * memory) {
    int write = cpu->reg[current_instr->rs2];
    mem_write(memory, cpu->reg[current_instr->rs1] + \
    current_instr->imm, write, WORD_SIZE);
}
void slt(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = \
    (cpu->reg[current_instr->rs1] < cpu->reg[current_instr->rs2]) ? 1 : 0;
}
void slti(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = \
    (cpu->reg[current_instr->rs1] < current_instr->imm) ? 1 : 0;
}
void sltu(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = \
    ((unsigned int) cpu->reg[current_instr->rs1] < \
    (unsigned int) cpu->reg[current_instr->rs2]) ? 1 : 0;
}
void sltiu(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = \
    ((unsigned int) cpu->reg[current_instr->rs1] < \
    (unsigned int) current_instr->imm) ? 1 : 0;
    }
void beq(struct cpu * cpu, struct instruction * current_instr){
    if (cpu->reg[current_instr->rs1] == cpu->reg[current_instr->rs2]) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void bne(struct cpu * cpu, struct instruction * current_instr) {
    if (cpu->reg[current_instr->rs1] != cpu->reg[current_instr->rs2]) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void blt(struct cpu * cpu, struct instruction * current_instr) {
    if ((cpu->reg[current_instr->rs1] < cpu->reg[current_instr->rs2])) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void bltu(struct cpu * cpu, struct instruction * current_instr) {
    if ((unsigned int) (cpu->reg[current_instr->rs1] < \
    (unsigned int) cpu->reg[current_instr->rs2])) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void bge(struct cpu * cpu, struct instruction * current_instr) {
    if ((cpu->reg[current_instr->rs1] >= cpu->reg[current_instr->rs2])) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void bgeu(struct cpu * cpu, struct instruction * current_instr) {
    if ((unsigned int) (cpu->reg[current_instr->rs1] >= \
    (unsigned int) cpu->reg[current_instr->rs2])) {
        cpu->pc = cpu->pc + (current_instr->imm << 1);
        cpu->pc = cpu->pc - 4;
    }
}
void jal(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->pc + 4;
        cpu->pc = cpu->pc + (current_instr->imm << 1);
}
void jalr(struct cpu * cpu, struct instruction * current_instr) {
    cpu->reg[current_instr->rd] = cpu->pc + 4;
    // Should expect previously factored PC
    cpu->pc = cpu->reg[current_instr->rs1] + current_instr->imm; 
}