// name: Terry Yu
// unikey: chyu9221
// SID: 510540014

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define LUI_SHIFT 12
#define BYTE 0b11111111
#define HALF_WORD 0b1111111111111111
#define WORD_DIV_4 30

// typedef enum instructions {
//     add, addi, sub,
//     lui,
//     xor, xori, or, ori,
//     and, andi,
//     sll, srl, sra,
//     lb, lh, lw, lbu, lhu,
//     sb, sh, sw, 
//     slt, slti, sltu, sltiu, 
//     beq, bne, blt, bltu, 
//     bge, bgeu, 
//     jal, jalr
// } instruction;

int get_filled_bits(int exponent);
int sext(int num, int original_len,int target_len);
void add(struct cpu * cpu, struct instruction * current_instr);
void addi(struct cpu * cpu, struct instruction * current_instr);
void sub(struct cpu * cpu, struct instruction * current_instr);
void lui(struct cpu * cpu, struct instruction * current_instr);
void xor(struct cpu * cpu, struct instruction * current_instr);
void xori(struct cpu * cpu, struct instruction * current_instr);
void or(struct cpu * cpu, struct instruction * current_instr);
void ori(struct cpu * cpu, struct instruction * current_instr);
void and(struct cpu * cpu, struct instruction * current_instr);
void andi(struct cpu * cpu, struct instruction * current_instr);
void sll(struct cpu * cpu, struct instruction * current_instr);
void srl(struct cpu * cpu, struct instruction * current_instr);
void sra(struct cpu * cpu, struct instruction * current_instr);
void lb(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void lh(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void lw(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void lbu(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void lhu(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void sb(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void sh(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void sw(struct cpu * cpu, struct instruction * current_instr, struct memory * memory);
void slt(struct cpu * cpu, struct instruction * current_instr);
void slti(struct cpu * cpu, struct instruction * current_instr);
void sltu(struct cpu * cpu, struct instruction * current_instr);
void sltiu(struct cpu * cpu, struct instruction * current_instr);
void beq(struct cpu * cpu, struct instruction * current_instr);
void bne(struct cpu * cpu, struct instruction * current_instr);
void blt(struct cpu * cpu, struct instruction * current_instr);
void bltu(struct cpu * cpu, struct instruction * current_instr);
void bge(struct cpu * cpu, struct instruction * current_instr);
void bgeu(struct cpu * cpu, struct instruction * current_instr);
void jal(struct cpu * cpu, struct instruction * current_instr);
void jalr(struct cpu * cpu, struct instruction * current_instr);
#endif