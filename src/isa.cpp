#include "isa.h"

#include <cstdint>
#include <string>
#include <unordered_map>

#include "editor.h"

////////////////////////////////////////////////////////////////

const std::unordered_map<std::string, uint32_t> codes = {
    //opcode
    {"NOOP", 0 << OPCODE_SHIFT},
    {"RST", 0 << OPCODE_SHIFT},
    {"AST", 1 << OPCODE_SHIFT},
    {"ADD", 2 << OPCODE_SHIFT},
    {"BSUB", 3 << OPCODE_SHIFT},
    {"SUB", 4 << OPCODE_SHIFT},
    {"CMP", 5 << OPCODE_SHIFT},
    {"INC", 6 << OPCODE_SHIFT},
    {"DEC", 7 << OPCODE_SHIFT},
    {"RSH", 8 << OPCODE_SHIFT},
    {"LSH", 9 << OPCODE_SHIFT},
    {"XOR", 10 << OPCODE_SHIFT},
    {"OR", 11 << OPCODE_SHIFT},
    {"AND", 12 << OPCODE_SHIFT},
    {"NOT", 13 << OPCODE_SHIFT},
    {"IMM", 14 << OPCODE_SHIFT},
    {"IMA", 15 << OPCODE_SHIFT},
    {"SWP", 16 << OPCODE_SHIFT},
    {"CALL", 17 << OPCODE_SHIFT},
    {"RET", 18 << OPCODE_SHIFT},
    {"IN", 19 << OPCODE_SHIFT},
    {"OUT", 20 << OPCODE_SHIFT},
    {"BRC", 21 << OPCODE_SHIFT},
    {"PUSH", 22 << OPCODE_SHIFT},
    {"POP", 23 << OPCODE_SHIFT},
    {"POI", 24 << OPCODE_SHIFT},
    {"MST", 25 << OPCODE_SHIFT},
    {"MLD", 26 << OPCODE_SHIFT},
    {"ADDC", 27 << OPCODE_SHIFT},
    {"SUBC", 28 << OPCODE_SHIFT},
    {"NEG", 29 << OPCODE_SHIFT},
    {"NEGA", 30 << OPCODE_SHIFT},
    {"HALT", 31 << OPCODE_SHIFT},
    
    //regs
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    
    //flags
    {"TRUE", 0},
    {"NEQ", 1},
    {"NZERO", 1},
    {"EQ", 2},
    {"ZERO", 2}
};

uint16_t pc = 0;

uint32_t reg[REG_AMOUNT] = {0};
uint32_t acc = 0;
bool flag[FLAG_AMOUNT] = {0};
uint32_t out[IO_AMOUNT] = {0};
uint32_t stack[1<<SP_ADDRESS_SIZE] = {0};
uint32_t sp = 0;
uint32_t cStack[1<<SP_ADDRESS_SIZE] = {0};
uint32_t cSp = 0;
uint32_t ram[1<<RAM_ADDRESS_SIZE] = {0};
uint32_t poi = 0;

void reset_cpu(){
    pc = 0;
    acc = 0;
    std::fill(std::begin(reg), std::end(reg), 0);
    std::fill(std::begin(flag), std::end(flag), 0);
    std::fill(std::begin(out), std::end(out), 0);
    std::fill(std::begin(stack), std::end(stack), 0);
    sp = 0;
    std::fill(std::begin(cStack), std::end(cStack), 0);
    cSp = 0;
    std::fill(std::begin(ram), std::end(ram), 0);
    poi = 0;
}

void update_flags(uint32_t aluOut){
    flag[0] = true;
    flag[1] = (aluOut & BIT_LIMIT) != 0; //NEQ, NZERO
    flag[2] = (aluOut & BIT_LIMIT) == 0; //EQ, ZERO
    flag[3] = aluOut < (BIT_LIMIT + 1); //LT, NCARRY
    flag[4] = (aluOut < (BIT_LIMIT + 1)) || (aluOut & BIT_LIMIT); //LEQ
    flag[5] = (aluOut >= (BIT_LIMIT + 1)) && ((aluOut & BIT_LIMIT) != 0); //GT
    flag[6] = (aluOut >= (BIT_LIMIT + 1)); //GEQ, CARRY
    flag[7] = (aluOut & 1) == 1; //MSB/LSB    curent setting = LSB
}

void exec_instr(uint32_t instruction){
    static uint8_t immediates = 0;
    static uint32_t opcode = 0;
    static uint32_t x = 0;
    static uint32_t tmp = 0;
    static uint32_t SWPtmp = 0;
    if (immediates == 0){
        x = instruction & 7;
        opcode = (instruction >> OPCODE_SHIFT) & ((1 << OPCODE_BIT_SIZE)-1);
        switch (opcode){
            case 0: //RST
                reg[x] = acc;
                break;
            case 1: //AST
                acc = reg[x];
                break;
            case 2: //ADD
                tmp = acc + reg[x];
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 3: //BSUB
                tmp = reg[x] + (~acc&BIT_LIMIT) + 1;
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 4: //SUB
                tmp = acc + (~reg[x]&BIT_LIMIT) + 1;
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 5: //CMP
                update_flags(acc + (~reg[x]&BIT_LIMIT) + 1);
                break;
            case 6: //INC
                tmp = reg[x] + 1;
                update_flags(tmp);
                reg[x] = tmp & BIT_LIMIT;
                break;
            case 7: //DEC
                tmp = reg[x] + BIT_LIMIT;
                update_flags(tmp);
                reg[x] = tmp & BIT_LIMIT;
                break;
            case 8: //RSH
                tmp = reg[x] >> 1;
                update_flags(tmp);
                reg[x] = tmp & BIT_LIMIT;
                break;
            case 9: // LSH
                tmp = reg[x] << 1;
                update_flags(tmp);
                reg[x] = tmp & BIT_LIMIT;
                break;
            case 10: //XOR
                tmp = acc ^ reg[x];
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 11: //OR
                tmp = acc | reg[x];
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 12: //AND
                tmp = acc & reg[x];
                update_flags(tmp);
                acc = tmp & BIT_LIMIT;
                break;
            case 13: //NOT
                tmp = ~reg[x]&BIT_LIMIT;
                update_flags(tmp);
                reg[x] = tmp & BIT_LIMIT;
                break;
            case 14: //IMM
                immediates = 1;
                break;
            case 15: //IMA
                immediates = 1;
                break;
            case 16: // SWP
                immediates = 2;
                break;
            case 17: //CALL
                immediates = 2;
                cStack[cSp] = pc + 3;
                cSp++;
                break;
            case 18: //RET
                cSp--;
                pc = cStack[cSp] - 1;
                break;
            case 19: //IN
                //request_input();
                break;
            case 20: //OUT
                out[x] = acc;
                break;
            case 21: //BRC
                immediates = 1;
                break;
            case 22: //PUSH
                stack[sp] = acc;
                sp++;
                break;
            case 23: //POP
                sp--;
                tmp = stack[sp];
                update_flags(tmp);
                acc = tmp;
                break;
            case 24: //POI
                poi = reg[x];
                break;
            case 25: //MST
                ram[poi] = acc;
                break;
            case 26: //MLD
                tmp = ram[poi];
                update_flags(tmp);
                acc = tmp;
                break;
            case 27: //ADDC
                tmp = acc + reg[x] + flag[6];
                update_flags(tmp);
                acc = tmp;
                break;
            case 28: //SUBC
                tmp = acc + (~reg[x]&BIT_LIMIT) + flag[6];
                update_flags(tmp);
                acc = tmp;
                break;
            case 29: //NEG
                tmp = (~reg[x]&BIT_LIMIT) + 1;
                update_flags(tmp);
                reg[x] = tmp;
                break;
            case 30: //NEGA
                tmp = (~acc&BIT_LIMIT) + 1;
                update_flags(tmp);
                acc = tmp;
                break;
            case 31: //HALT
                isPaused = true;
                break;
            default:
                break;
        }
    }
    else {
        switch (opcode){
        case 14: //IMM
            reg[x] = instruction & BIT_LIMIT;
            break;
        case 15: //IMA
            acc = instruction & BIT_LIMIT;
            break;
        case 16: //SWP
            if (immediates == 2) SWPtmp = instruction & 0b111111;
            else pc = (SWPtmp | (instruction << 6)) - 1;
            break;
        case 17: //CALL
            if (immediates == 2) SWPtmp = instruction & 0b111111;
            else pc = (SWPtmp | (instruction & 0b11111111000000)) - 1; //like this, bc i want it to be universal, and paging like MNPU2 has is unusual i think
            break;
        case 21: //BRC
            if (flag[x] == true) pc = instruction - 1;
        default:
            break;
        }
        immediates -= 1;
    }
    pc = (pc + 1) & ((1 << ROM_ADDRESS_SIZE) - 1);
}