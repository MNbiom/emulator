#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

void exec_instr(uint32_t instruction);
void reset_cpu();

constexpr uint8_t BIT_WIDTH = 8;
constexpr uint32_t BIT_LIMIT = (1 << BIT_WIDTH) - 1;

constexpr uint8_t REG_AMOUNT = 8;
constexpr uint8_t IO_AMOUNT = 8;
constexpr uint8_t FLAG_AMOUNT = 8;
constexpr uint8_t SP_ADDRESS_SIZE = 3;
constexpr uint8_t CSP_ADDRESS_SIZE = 3;
constexpr uint8_t RAM_ADDRESS_SIZE = 8;

constexpr uint8_t ROM_ADDRESS_SIZE = 14;
constexpr uint8_t OPCODE_BIT_SIZE = 5;
constexpr uint8_t OPCODE_SHIFT = 3;

constexpr double CLOCK_SPEED = 8;

extern const std::unordered_map<std::string, uint32_t> codes;

extern uint16_t pc;
extern uint32_t reg[];
extern uint32_t acc;
extern bool flag[];
extern uint32_t out[];
extern uint32_t stack[];
extern uint32_t cStack[];
extern uint32_t ram[];
extern uint32_t poi;