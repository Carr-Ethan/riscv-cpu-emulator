#pragma once
#include "components.h"
#include <vector>
#include <string>
#include <cstdint>

#ifndef CPU_H
#define CPU_H

class CPU {
    public:
        uint32_t pc;
        uint32_t global_ticks;

        std::vector<std::string> iMem;
        ALU alu;
        RegisterFile rf;
        ControlUnit control;
        Memory memory;
        
        CPU();
        int32_t fetch(std::vector<std::string> iMem, int32_t pc);
        void writeback(int32_t rd, int32_t val);
        void tick();
};

#endif