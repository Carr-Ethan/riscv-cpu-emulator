#include <iostream>
#include <bitset>
#include <cstdint>
#include <fstream>
#include "../include/disasm.h"
#include "../include/components.h"
#include "../include/cpu.h"

std::string instructionFile1 = "../instructionFiles/sample_part1.txt";
std::string instructionFile2 = "../instructionFiles/sample_part2.txt";

int main(){
    std::fstream file;

    file.open(instructionFile1);
    std::vector<std::string> iMem1;

    if(file.is_open()){
        std::string str;
        while(getline(file, str)){
            iMem1.push_back(str);
        }
        file.close();
    }

    std::cout << "\nTEST 1: " << std::endl;

    CPU cpuA;
    cpuA.iMem = iMem1;
    
    cpuA.initTest1();
    while(true){
        cpuA.tick();
        if(cpuA.pc >= cpuA.iMem.size()*4) break;
    }

    file.open(instructionFile2);
    std::vector<std::string> iMem2;

    if(file.is_open()){
        std::string str;
        while(getline(file, str)){
            iMem2.push_back(str);
        }
        file.close();
    }

    std::cout << "\nTEST 2: " << std::endl;

    CPU cpuB;
    cpuB.iMem = iMem2;
    
    cpuA.initTest2();
    while(true){
        cpuB.tick();
        if(cpuB.pc >= cpuB.iMem.size()*4) break;
    }
    return 0;
}