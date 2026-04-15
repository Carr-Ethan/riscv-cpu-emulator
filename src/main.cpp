#include <iostream>
#include <bitset>
#include <cstdint>
#include <fstream>
#include "../include/disasm.h"
#include "../include/components.h"
#include "../include/cpu.h"

std::string instructionFile = "../instructionFiles/sample_part1.txt";

int main(){
    std::fstream file;

    file.open(instructionFile);
    std::vector<std::string> iMem;

    if(file.is_open()){
        std::string str;
        while(getline(file, str)){
            iMem.push_back(str);
        }
        file.close();
    }

    CPU cpu;
    cpu.iMem = iMem;
    
    cpu.initTest1();
    while(true){
        cpu.tick();
        if(cpu.pc >= cpu.iMem.size()*4) break;
    }
    return 0;
}