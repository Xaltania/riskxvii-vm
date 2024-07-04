// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#ifndef CPU_H
#define CPU_H

#define WORD_SIZE 32
#define REGISTER_COUNT 32
#define u_int unsigned int

#define R_OPCODE 0b0110011
#define I_OPCODE 0b0010011
#define I_OPCODE_2 0b0000011
#define I_OPCODE_3 0b1100111
#define S_OPCODE 0b0100011
#define SB_OPCODE 0b1100011
#define U_OPCODE 0b0110111
#define UJ_OPCODE 0b1101111

#define RD_OFFSET 7
#define I_IMM_OFFSET 20
#define S_IMM_1_OFFSET 7
#define S_IMM_1_SIZE 5
#define S_IMM_2_OFFSET 25
#define SB_IMM_1_OFFSET 7
#define SB_IMM_2_OFFSET 25
#define SB_IMM_1_SIZE 5
#define U_IMM_OFFSET 12
#define UJ_IMM_OFFSET 12
#define FUNC3_OFFSET 12
#define RS1_OFFSET 15
#define RS2_OFFSET 20
#define FUNC7_OFFSET 25

#define I_IMM_LEN 12
#define S_IMM_LEN 12
#define SB_IMM_LEN 12
#define U_IMM_LEN 20
#define UJ_IMM_LEN 20

#define OPCODE_FIELD 0b1111111
#define RD_FIELD 0b11111
#define I_IMM_FIELD 0b111111111111
#define S_IMM_1_FIELD 0b11111
#define S_IMM_2_FIELD 0b1111111
#define SB_IMM_1_FIELD 0b11111
#define SB_IMM_2_FIELD 0b111111100000
#define SB_12TH_BIT 0b100000000000
#define FUNC3_FIELD 0b111
#define RS1_FIELD 0b11111
#define RS2_FIELD 0b11111
#define FUNC7_FIELD 0b1111111

#define UJ_10BIT_FIELD 0b1111111111
#define UJ_8BIT_FIELD 0b11111111
#define UJ_10BIT_SHIFT 9
#define UJ_8BIT_SHIFT 11
#define UJ_11TH_FIELD 0b100000000
#define UJ_11TH_SHIFT 2

struct instruction {
    u_int rd; // 5 bits
    u_int func3; // 3 bits
    u_int opcode; // 7 bits
    u_int rs1; // 5 bits
    u_int rs2; // 5 bits
    u_int func7; // 7 bits
    u_int imm;
};

struct cpu {
    /**
     * CPU consisting of 32 registers
     * Program counter is an instruction counter
     * Points to entire instructions (4 bytes)
    */
    int reg[REGISTER_COUNT];
    u_int pc;
    struct instruction current_instr;

};

struct cpu cpu_init();
int fetch(struct cpu * cpu, struct memory * mem);
void cpu_display(struct cpu * cpu);
void decode(int bin_instr, struct instruction * instruction);
void execute(struct cpu * cpu, struct memory * mem);

#endif