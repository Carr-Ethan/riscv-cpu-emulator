#pragma once
#include "components.h"
#include "disasm.h"
#include <vector>
#include <string>
#include <cstdint>

#ifndef CPU_H
#define CPU_H

class CPU {
    public:
        uint32_t pc;
        uint32_t next_pc;
        uint32_t global_ticks;

        std::vector<std::string> iMem;
        ALU alu;
        RegisterFile rf;
        ControlUnit control;
        Memory memory;
        Decoder decoder;


        
        CPU();
        int32_t fetch(std::vector<std::string> iMem, int32_t pc);
        std::unique_ptr<instruction> decode(int32_t machineCode);
        ALU::result execute(instruction insn, int32_t readData1, int32_t readData2);
        int32_t mem(ALU::result aluRes, int32_t readData2);
        void writeback(int32_t rd, int32_t val);
        void tick();


        //TEST FUNCTIONS
        void initTest1();
};

#endif