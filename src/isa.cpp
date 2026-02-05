#include "isa.h"

#include <cstdint>
#include <string>
#include <unordered_map>

////////////////////////////////////////////////////////////////

constexpr uint8_t BIT_WIDTH = 8;
constexpr uint32_t BIT_LIMIT = (1 << BIT_WIDTH) - 1;
constexpr uint8_t REG_AMOUNT = 8;
constexpr uint8_t IO_AMOUNT = 8;
constexpr uint8_t FLAG_AMOUNT = 8;

//constexpr uint8_t ROM_ADDRESS_SIZE = 14;
constexpr uint8_t OPCODE_BIT_SIZE = 5;
constexpr uint8_t OPCODE_SHIFT = 3;

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

void reset_cpu(){
    pc = 0;
    std::fill(std::begin(reg), std::end(reg), 0);
    std::fill(std::begin(flag), std::end(flag), 0);
    std::fill(std::begin(out), std::end(out), 0);
}

void update_flags(uint32_t aluOut){
    flag[0] = true;
    flag[1] = (aluOut & BIT_LIMIT) != 0; //NEQ, NZERO
    flag[2] = (aluOut & BIT_LIMIT) == 0; //EQ, ZERO
    flag[3] = aluOut < (BIT_LIMIT + 1); //LT, NCARRY
    flag[4] = (aluOut < (BIT_LIMIT + 1)) || (aluOut & BIT_LIMIT); //LEQ
    flag[5] = (aluOut >= (BIT_LIMIT + 1)) && ((aluOut & BIT_LIMIT) != 0); //GT
    flag[6] = (aluOut >= (BIT_LIMIT + 1)); //GEQ
    flag[7] = (aluOut & 1) == 1; //MSB/LSB    curent setting = LSB
}

void exec_instr(uint32_t instruction){
    static uint8_t immediates = 0;
    static uint32_t opcode = 0;
    static uint32_t x = 0;
    static uint32_t tmp = 0;

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
            case 6:
                
                break;
            case 7:
                
                break;
            case 8:
                
                break;
            case 9:
                
                break;
            case 10:
                
                break;
            case 11:
                
                break;
            case 12:
                
                break;
            case 13:
                
                break;
            case 14: //IMM
                immediates = 1;
                break;
            case 15:
                
                break;
            case 16:
                
                break;
            case 17:
                
                break;
            case 18:
                
                break;
            case 19:
                
                break;
            case 20: //OUT
                out[x] = acc;
                break;
            case 21: //BRC
                immediates = 1;
                break;
            case 22:
                
                break;
            case 23:
                
                break;
            case 24:
                
                break;
            case 25:
                
                break;
            case 26:
                
                break;
            case 27:
                
                break;
            case 28:
                
                break;
            case 29:
                
                break;
            case 30:
                
                break;
            case 31:
                
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
        case 21: //BRC
            if (flag[x] == true) pc = instruction - 1;
        default:
            break;
        }
        immediates -= 1;
    }
    pc = (pc + 1) & ((1 << ROM_ADDRESS_SIZE) - 1);
}