#pragma once
#include <memory>
#include "constant.h"
#ifndef DISASM_H
#define DISASM_H

#define BIT_MASK_1 0x1
#define BIT_MASK_3 0x7
#define BIT_MASK_4 0XF
#define BIT_MASK_5 0x1F
#define BIT_MASK_6 0x3F
#define BIT_MASK_7 0x7F
#define BIT_MASK_8 0xFF
#define BIT_MASK_10 0x3FF
#define BIT_MASK_11 0x7FF
#define BIT_MASK_22 0x3FFFFF


struct instruction{
    int32_t machineCode;
    type insnType;
    mnemonic operation;
    int32_t rs1, rs2, rd;
    int32_t immi;
    int32_t funct3, funct7;
};

//Use unique_ptr for unit tests
std::unique_ptr<instruction> disassemble(int32_t machineCode);
// void disassemble(int32_t machineCode);

#endif