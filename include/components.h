#pragma once
#include "constant.h"
#include <cstdint>
#include <array>
#include <memory>
#include "disasm.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

class ALU {
    public:
        struct result {
            int32_t val;
            bool aluZero;
        };

        result execute(int32_t opA, int32_t opB, aluCtrlOp aluCtrl);
};

class RegisterFile {
    public: 
        std::array<int32_t, 32> registerFile{}; //initialized to zero
        
        int32_t read(int8_t reg);
        void write(int8_t reg, int32_t val);
};

class Memory {
    public:
        std::array<int32_t, 32> dMem{};

        int32_t load(int8_t address);
        void store (int8_t address, int32_t value);
};

struct ControlSignals {
    int8_t regWrite;
    int8_t WBSel;
    int8_t memWrite;
    int8_t branch;
    int8_t memRead;
    int8_t aluASrc;
    int8_t aluBSrc;
    int8_t ALUOP;
};

class ControlUnit {
    public:
        ControlSignals ctrlSignals;

        aluCtrlOp aluCtrl(int8_t ALUOp, instruction insn);
};

#endif