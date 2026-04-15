#include "../include/constant.h"
#include "../include/components.h"
#include "../include/disasm.h"
#include "../include/cpu.h"
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <iostream>

aluCtrlOp ControlUnit::aluCtrl(int8_t ALUOp, instruction insn) {
    switch (ALUOp) {
        case 0b00: { // load/store
            return aluCtrlOp::ADD;
        }
        case 0b01: { // beq
            return aluCtrlOp::SUB;
        }
        case 0b10: { // R-type
            if (insn.funct3 == 0b000) {
                if (insn.insnType == type::R && (insn.funct7 & 0x20)){
                    return aluCtrlOp::SUB;
                }
                else return aluCtrlOp::ADD;
            } else {
                if (insn.funct3 == 0b111) return aluCtrlOp::AND;
                if (insn.funct3 == 0b110) return aluCtrlOp::OR;
                throw std::invalid_argument("Invalid or unsupported funct3 for R-type ALU operation.");
            }
        }
        default: {
            throw std::invalid_argument("Invalid ALUOp: " + std::to_string(ALUOp));
        }
    }

}

ALU::result  ALU::execute(int32_t opA, int32_t opB, aluCtrlOp aluCtrl){
    ALU::result res;

    switch (aluCtrl) {
        case aluCtrlOp::AND: {
            res.val = opA & opB;
            break;
        }
        case aluCtrlOp::OR: {
            res.val = opA | opB;
            break;
        }
        case aluCtrlOp::ADD: {
            res.val = opA + opB;
            break;
        }
        case aluCtrlOp::SUB: { 
            std::cout << opA << " " << opB << std::endl;
            res.val = opA - opB;
            break;
        }
    }

    res.aluZero = res.val == 0;
    return res;
}

int32_t RegisterFile::read(int8_t reg){
    return registerFile.at(reg);
}
void RegisterFile::write(int8_t reg, int32_t val){
    registerFile.at(reg) = val;
}

int32_t Memory::load(int8_t address){
    return dMem.at(address >> 2);
}
void Memory::store(int8_t address, int32_t value){
    dMem.at(address >> 2) = value;
}
