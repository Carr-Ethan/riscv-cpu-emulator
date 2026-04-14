#include "../include/components.h"
#include <cstdint>

ALU::result  ALU::execute(int32_t opA, int32_t opB, int8_t aluCtrl){
    ALU::result res;
    return res;
}

int32_t RegisterFile::read(int8_t reg){
    return 0;
}
void RegisterFile::write(int8_t reg){}

int32_t Memory::load(int8_t address){
    return 0;
}
void Memory::store(int8_t address){}

