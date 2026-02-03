#pragma once

#include <cstdint>
#include <unordered_map>

////////////////////////////////////////////////////////////////

constexpr uint8_t ROM_ADDRESS_SIZE = 14;
constexpr uint8_t OPCODE_SHIFT = 3;

const std::unordered_map<std::string, uint32_t> codes = {
    //opcode
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