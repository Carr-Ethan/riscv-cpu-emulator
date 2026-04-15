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
    int32_t machineCode = std::stol(iMem[pc/4], nullptr, 2);
    std::cout << machineCode << std::endl;
    int32_t next_pc = pc + 4;

    return machineCode;
}

std::unique_ptr<instruction> CPU::decode(int32_t machineCode) {
    std::unique_ptr<instruction> insn = disassemble(machineCode);
    control.setSignals(*insn); //needs to be implemented
    return insn;
}

ALU::result CPU::execute(instruction insn, int32_t readData1, int32_t readData2) {
    int32_t aluOpB = control.condSignals.aluSrc ? insn.immi : readData2;

    aluCtrlOp aluCtrl = control.aluCtrl(control.condSignals.ALUOP, insn);
    ALU::result aluRes = alu.execute(readData1, aluOpB, aluCtrl);

    bool branch = control.condSignals.branch && aluRes.aluZero;
    int32_t branchTarget = pc + insn.immi;

    if (branch) next_pc = branchTarget;
    pc = next_pc;

    return aluRes;
}

int32_t CPU::mem(ALU::result aluRes, int32_t readData2) {
    bool memRead = control.condSignals.memRead;
    bool memWrite = control.condSignals.memWrite;

    int32_t readDataDmem = 0;

    if (memWrite) {
        memory.store((int8_t)aluRes.val, readData2);
        ++global_ticks;
        return 0;
    }

    if (memRead) {
        readDataDmem = memory.load((int8_t)aluRes.val);
    }

    return control.condSignals.memToReg ? readDataDmem : aluRes.val;
}

void CPU::writeback(int32_t rd, int32_t val) {
    rf.write(rd, val);
    ++global_ticks;
}

void CPU::tick(){
    int32_t machineCode = CPU::fetch(iMem, pc);

    std::unique_ptr<instruction> insn = CPU::decode(machineCode);
    int32_t readData1 = rf.read(insn->rs1);
    int32_t readData2 = rf.read(insn->rs2);

    ALU::result aluRes = execute(*insn, readData1, readData2);

    int32_t resultData = mem(aluRes, readData2);

    writeback(insn->rd, resultData);
}