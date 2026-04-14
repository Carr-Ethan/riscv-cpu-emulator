#pragma once
#include "components.h"
#ifndef CPU_H
#define CPU_H

class CPU {
    public:
        uint32_t pc;
        uint32_t global_ticks;

        ALU alu;
        RegisterFile rf;
        ControlUnit control;
        Memory memory;
        
        CPU();
        void tick();
};

#endif