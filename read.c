#include <stdio.h>
#include <string.h> 

int main () {
    FILE * file = fopen("examples/add_2_numbers/add_2_numbers.mi", "rb");
    struct read
    {
        /* data */
        int a;
    };

    struct read my_read;
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    for (int i = 0; i < 1; i++) {
        fread(&my_read, sizeof(struct read), 1, file);
        printf("%d\n", my_read.a);
    }
    int opcode = my_read.a & 0b1111111;
    int func3 = (my_read.a >> 12) & 0b111;
    int rs1 = (my_read.a >> 15) & 0b11111;
    int rd = (my_read.a >> 7) & 0b11111;
    int rs2 = (my_read.a >> 20) & 0b11111;
    int func7 = (my_read.a >> 25) & 0b1111111;
    printf("Opcode: %d\nfunc3: %d\nrs1: %u\nrd: %u\nrs2: %u\nfunc7: %u\n", opcode, func3, rs1, rd, rs2, func7);
    fread(&my_read, sizeof(struct read), 1, file);
    opcode = my_read.a & 0b1111111;
    rd = (my_read.a >> 7) & 0b11111;
    int imm = (my_read.a >> 12) & 0b11111111111111111111;
    int proper = ((imm >> 9) & 0b1111111111) | ((imm & 0b11111111) << 11 ) | ((imm & 0b100000000) << 2);
    printf("Opcode: %u\nrd: %u\nimm: %u\nProper imm: %u\n", opcode, rd, imm, proper);
    switch(opcode) {
        case 0b0110011:
            printf("R\n");
            break;
        case 0b0010011:
            printf("I\n");
            break;
        case 0b0100011: 
            printf("S\n");
            break;
        case 0b1100011:
            printf("SB\n");
            break;
        case 0b0110111:
            printf("U\n");
            break;
        case 0b1101111:
            printf("UJ\n");
            break;    
        default:
            printf("Is none\n");
            break;    
    }
    
    fclose(file);
    return 0;
}
/**
 *    instruction->opcode = bin_instr & 0b1111111;
   switch(instruction->opcode) {
    case 0b0110011: // Type R
        instruction->rd = (bin_instr >> 7) & 0b11111;
        instruction->func3 = (bin_instr >> 12) & 0b111;
        instruction->rs1 = (bin_instr >> 15) & 0b11111;
        instruction->rs2 = (bin_instr >> 20) & 0b11111;
        instruction->func7 (bin_instr >> 25) & 0b1111111;
        break;
    case 0b0010011: // Type I
        instruction->rd = (bin_instr >> 7) & 0b11111;
        instruction->func3 = (bin_instr >> 12) & 0b111;
        instruction->rs1 = (bin_instr >> 15) & 0b11111;
        break;
    case 0b0100011: // Type S
        instruction->func3 = (bin_instr >> 12) & 0b111;
        instruction->rs1 = (bin_instr >> 15) & 0b11111;
        instruction->rs2 = (bin_instr >> 20) & 0b11111;
        break;
    case 0b1100011: // Type SB
        instruction->func3 = (bin_instr >> 12) & 0b111;
        instruction->rs1 = (bin_instr >> 15) & 0b11111;
        instruction->rs2 = (bin_instr >> 20) & 0b11111;
        break;
    case 0b0110111: // Type U
        instruction->rd = (bin_instr >> 7) & 0b11111;
        break;
    case 0b1101111: // Type UJ
        instruction->rd = (bin_instr >> 7) & 0b11111;
        break;    
    default:
        printf("Is none\n");
        break;
   }
*/