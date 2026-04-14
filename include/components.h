#pragma once
#include <cstdint>
#include <array>

#ifndef COMPONENTS_H
#define COMPONENTS_H

class ALU {
    public:
        struct result {
            int32_t val;
            bool aluZero;
        };

        result execute(int32_t opA, int32_t opB, int8_t aluCtrl);
};

class RegisterFile {
    public: 
        std::array<int8_t, 32> registerFile{}; //initialized to zero
        
        int32_t read(int8_t reg);
        void write(int8_t reg);
};

class Memory {
    public:
        std::array<int32_t, 32> dMem{};

        int32_t load(int8_t address);
        void store (int8_t address);
};

struct ControlSignals {
    bool regWrite;
    bool memToReg;
    bool memWrite;
    bool branch;
    bool memRead;
    int8_t aluSrc;
    int8_t ALUOP;
};

class ControlUnit {
    public:
        ControlSignals condSignals;
};

#endif