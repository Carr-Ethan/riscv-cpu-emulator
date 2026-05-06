#pragma once
#include "components.h"
#include "disasm.h"
#include <vector>
#include <string>
#include <cstdint>

#ifndef CPU_H
#define CPU_H



template <typename T>
class StageRegister {
    private:
        T state;
        T next_state;
        bool valid;
        bool next_valid;

    public:
        StageRegister() : valid(false), next_valid(false), state{}, next_state{} {}

        const T& read() const { return state; }
        bool isValid() const { return valid; }

        void write(const T& newData) {
            next_state = newData;
            next_valid = true;
        }

        void flush() {
            next_valid = false;
            // Optionally zero out next_state here if desired for debugging
            next_state = T{}; 
        }

        void nop(){
            flush();
        }


        void clock_edge() {
            state = next_state;
            valid = next_valid;
            //Default to stall for safety
            next_valid = false; 
        }
};

struct IF_ID_Data {
    uint32_t pc = 0;
    int32_t machineCode = 0;
};

struct ID_EX_Data {
    uint32_t pc = 0;
    int32_t readData1 = 0;
    int32_t readData2 = 0;
    int32_t immi = 0;
    int8_t rs1 = 0, rs2 = 0, rd = 0;
    ControlSignals ctrlSignals = {0};
    aluCtrlOp aluCtrl = aluCtrlOp::ADD;
};

struct EX_MEM_Data {
    uint32_t pc = 0;
    int32_t aluVal = 0;
    int32_t readData2 = 0;
    int8_t rd = 0;
    ControlSignals ctrlSignals = {0};
};

struct MEM_WB_Data {
    uint32_t pc = 0;
    int32_t readDataDmem = 0;
    int32_t aluVal = 0;
    int8_t rd = 0;
    ControlSignals ctrlSignals = {0};
};

class CPU {
    public:
        uint32_t pc;
        uint32_t global_ticks;

        std::vector<std::string> iMem;
        ALU alu;
        RegisterFile rf;
        ControlUnit control;
        Memory memory;
        Decoder decoder;


        StageRegister<IF_ID_Data> IF_ID;
        StageRegister<ID_EX_Data> ID_EX;
        StageRegister<EX_MEM_Data> EX_MEM;
        StageRegister<MEM_WB_Data> MEM_WB;

        CPU();
        void tick();
        bool isDone();


        void initTest1();
        void initTest2();

    private:
        void fetch();
        void decode(bool &stall);
        void execute(bool &flush, uint32_t &branch_target);
        void mem_stage();
        void writeback();
};

#endif