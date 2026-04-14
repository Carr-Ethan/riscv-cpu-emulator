#include <iostream>
#include "../include/cpu.h"
#include "../include/disasm.h"
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

CPU::CPU(){
    pc = 0;
}

int32_t CPU::fetch(std::vector<std::string> iMem, int32_t pc){
    return std::stol(iMem[pc/4], nullptr, 2);
}

void CPU::tick(){
    int32_t machineCode = fetch(iMem, pc);
    std::cout << machineCode << std::endl;
    int32_t next_pc = pc + 4;

    std::unique_ptr<instruction> insn = disassemble(machineCode);
    control.setSignals(*insn); //needs to be implemented

    

}