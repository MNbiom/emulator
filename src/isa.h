#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

void exec_instr(uint32_t instruction);
void reset_cpu();

constexpr uint8_t ROM_ADDRESS_SIZE = 14;

extern const std::unordered_map<std::string, uint32_t> codes;

extern uint16_t pc;
extern uint32_t reg[];
extern uint32_t acc;
extern bool flag[];
extern uint32_t out[];