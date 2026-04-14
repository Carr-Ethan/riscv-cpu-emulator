#include <iostream>
#include "../include/cpu.h"
#include <vector>
#include <string>
#include <cstdint>

CPU::CPU(){
    pc = 0;
}

int32_t CPU::fetch(std::vector<std::string> iMem, int32_t pc){
    return std::stol(iMem[pc/4], nullptr, 2);
}

void CPU::tick(){
    int32_t machineCode = fetch(iMem, pc);
    std::cout << machineCode << std::endl;
    pc += 4;
}