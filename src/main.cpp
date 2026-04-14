#include <iostream>
#include <bitset>
#include <cstdint>
#include "../include/disasm.h"

int main(){
    while(true){
        std::string machine_code; 
        std::cout << "Enter an Instruction: ";
        std::cin >> machine_code;
        if(machine_code == "-1"){
            break;
        }
        const int32_t insn = std::stol(machine_code, nullptr, 2);
        disassemble(insn);
    }
    return 0;
}