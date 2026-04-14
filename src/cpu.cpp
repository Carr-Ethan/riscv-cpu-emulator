#include <iostream>
#include "../include/cpu.h"

CPU::CPU(){
    pc = 0;
}

void CPU::tick(){
    std::cout << "Hello" << std::endl;
}