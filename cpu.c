// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#include <stdio.h>

#include "memory.h"
#include "vm.h"
#include "cpu.h"
#include "instructions.h"

struct cpu cpu_init() {
    // enum registers { // Register enumeration
    //     R0, R1, R2, R3, 
    //     R4, R5, R6, R7, 
    //     R8, R9, R10, R11, 
    //     R12, R13, R14, R15, 
    //     R16, R17, R18, R19, 
    //     R20, R21, R22, R23, 
    //     R24, R25, R26, R27, 
    //     R28, R29, R30, R31, 
    //     REGISTER_MAX
    // };
    struct cpu cpu = {
        {0}, 0, {0}
    }; 
    // cpu.pc = 0;
    // cpu.reg = {0};
    // cpu.reg[0] = 0;

    return cpu;
}

void cpu_display(struct cpu * cpu) {
    printf("PC: %x\n", cpu->pc);
    for (int i = 0; i < REGISTER_COUNT; i++) {
        printf("R%d: %u     |   ", i, cpu->reg[i]);
        if (i % 8 == 0) {
            printf("\n");
        }
    }
    printf("rd: %d\n", cpu->current_instr.rd);
    printf("func3: %d\n", cpu->current_instr.func3);
    printf("opcode: %d\n", cpu->current_instr.opcode);
    printf("rs1: %d\n", cpu->current_instr.rs1);
    printf("rs2: %d\n", cpu->current_instr.rs2);
    printf("func7: %d\n", cpu->current_instr.func7);
    printf("imm: %d\n____________\n", cpu->current_instr.imm);
}

int fetch(struct cpu * cpu, struct memory * mem) {
    /**
     * Gets instruction at program counter
     * Reads the instruction binary from the memory
     * Returns the instruction binary as an integer
    */
    int * mem_ptr = (int*) &mem->instr_mem[cpu->pc];
    int instruction_binary = * mem_ptr;
    return instruction_binary;
}

void decode(int bin_instr, struct instruction * instruction) {
    /**
     * Decodes the instruction via bitwise operations and shifts.
    */
   instruction->opcode = bin_instr & 0b1111111;
   int shorter_imm = 0;
   switch(instruction->opcode) {
    case R_OPCODE: // Type R
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        instruction->rs2 = (bin_instr >> RS2_OFFSET) & RS2_FIELD;
        instruction->func7 = (bin_instr >> FUNC7_OFFSET) & FUNC7_FIELD;
        break;
    case I_OPCODE: // Type I
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        shorter_imm = (bin_instr >> I_IMM_OFFSET) & I_IMM_FIELD;
        instruction->imm = sext(shorter_imm, I_IMM_LEN, WORD_SIZE);
        break;
    case I_OPCODE_2: // Type I
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        shorter_imm = (bin_instr >> I_IMM_OFFSET) & I_IMM_FIELD;
        instruction->imm = sext(shorter_imm, I_IMM_LEN, WORD_SIZE);
        break;
    case I_OPCODE_3: // Type I
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        shorter_imm = (bin_instr >> I_IMM_OFFSET) & I_IMM_FIELD;
        instruction->imm = sext(shorter_imm, I_IMM_LEN, WORD_SIZE);
        break;
    case S_OPCODE: // Type S
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        instruction->rs2 = (bin_instr >> RS2_OFFSET) & RS2_FIELD;
        shorter_imm = ((bin_instr >> S_IMM_1_OFFSET) & S_IMM_1_FIELD) |
                        ((bin_instr >> S_IMM_2_OFFSET) << S_IMM_1_SIZE);
        instruction->imm = sext(shorter_imm, S_IMM_LEN, WORD_SIZE);
        break;
    case SB_OPCODE: // Type SB
        instruction->func3 = (bin_instr >> FUNC3_OFFSET) & FUNC3_FIELD;
        instruction->rs1 = (bin_instr >> RS1_OFFSET) & RS1_FIELD;
        instruction->rs2 = (bin_instr >> RS2_OFFSET) & RS2_FIELD;
        int left_f = (bin_instr >> SB_IMM_2_OFFSET) << SB_IMM_1_SIZE;
        int left_bit = left_f & SB_12TH_BIT;
        int right_f = (bin_instr >> SB_IMM_1_OFFSET) & SB_IMM_1_FIELD;
        int right_bit = right_f & 0b1;
        shorter_imm = left_bit | (left_f >> 1) | right_bit << 11 | right_f >> 1;
        instruction->imm = sext(shorter_imm, SB_IMM_LEN, WORD_SIZE);
        

        break;
    case U_OPCODE: // Type U
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        shorter_imm = (bin_instr >> U_IMM_OFFSET);
        instruction->imm = sext(shorter_imm, U_IMM_LEN, WORD_SIZE);
        break;
    case UJ_OPCODE: // Type UJ
        instruction->rd = (bin_instr >> RD_OFFSET) & RD_FIELD;
        int scrambled = bin_instr >> UJ_IMM_OFFSET;
        shorter_imm = ((scrambled >> UJ_10BIT_SHIFT) & UJ_10BIT_FIELD) |
                        ((scrambled & UJ_8BIT_FIELD) << UJ_8BIT_SHIFT) |
                        ((scrambled & UJ_11TH_FIELD) << UJ_11TH_SHIFT) | 
                        ((scrambled >> (UJ_IMM_LEN - 1)) << (UJ_IMM_LEN - 1));
        instruction->imm = sext(shorter_imm, UJ_IMM_LEN, WORD_SIZE);
        break;    
    default:
        perror("Error! Invalid opcode.\n");
        break;
   }
}
void execute(struct cpu * cpu, struct memory * mem) {
    int opcode = cpu->current_instr.opcode;
    int func3 = cpu->current_instr.func3;
    int func7 = cpu->current_instr.func7;
    cpu->reg[0] = 0;

    if (opcode == R_OPCODE) {
        if (func3 == 0b000 && func7 == 0b0000000) {
            add(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b000 && func7 == 0b0100000) {
            sub(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b100 && func7 == 0b0000000) {
            xor(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b110 && func7 == 0b0000000) {
            or(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b111 && func7 == 0b0000000) {
            and(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b001 && func7 == 0b0000000) {
            sll(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b101 && func7 == 0b0000000) {
            srl(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b101 && func7 == 0b0100000) {
            sra(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b010 && func7 == 0b0000000) {
            slt(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b011 && func7 == 0b0000000) {
            sltu(cpu, &cpu->current_instr);
        }
    }
    else if (opcode == I_OPCODE) {
        if (func3 == 0b000) {
            addi(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b100) {
            xori(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b110) {
            ori(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b111) {
            andi(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b010) {
            slti(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b011) {
            sltiu(cpu, &cpu->current_instr);
        }
    }
    else if (opcode == I_OPCODE_2) {
        if (func3 == 0b000) {
            lb(cpu, &cpu->current_instr, mem);
        }
        else if (func3 == 0b001) {
            lh(cpu, &cpu->current_instr, mem);
        }
        else if (func3 == 0b010) {
            lw(cpu, &cpu->current_instr, mem);
        }
        else if (func3 == 0b100) {
            lbu(cpu, &cpu->current_instr, mem);
        }
        else if (func3 == 0b101) {
            lhu(cpu, &cpu->current_instr, mem);
        }
    }
    else if (opcode == I_OPCODE_3 && func3 == 0b000) {
        jalr(cpu, &cpu->current_instr);
        cpu->pc = cpu->pc - 4;
    }
    else if (opcode == S_OPCODE) {
        if (func3 == 0b000) {
            sb(cpu, &cpu->current_instr, mem);
        }
        if (func3 == 0b001) {
            sh(cpu, &cpu->current_instr, mem);
        }
        if (func3 == 0b010) {
            sw(cpu, &cpu->current_instr, mem);
        }
    }
    else if (opcode == SB_OPCODE) {
        // Sign extend SHIFTED imm values after adjusting for PC
        if (func3 == 0b000) {
            beq(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b001) {
            bne(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b100) {
            blt(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b110) {
            bltu(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b101) {
            bge(cpu, &cpu->current_instr);
        }
        else if (func3 == 0b111) {
            bgeu(cpu, &cpu->current_instr);
        }
    }
    else if (opcode == U_OPCODE) {
        lui(cpu, &cpu->current_instr);
    }
    else if (opcode == UJ_OPCODE) {
        // Sign extend SHIFTED imm values after adjusting for PC
        jal(cpu, &cpu->current_instr);
        cpu->pc = cpu->pc - 4;
    }
    cpu->reg[0] = 0;
    cpu->pc = cpu->pc + 4;

}