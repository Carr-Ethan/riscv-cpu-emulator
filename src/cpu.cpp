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
    // ===== IF ===== //

    int32_t machineCode = fetch(iMem, pc);
    std::cout << machineCode << std::endl;
    int32_t next_pc = pc + 4;

    std::unique_ptr<instruction> insn = disassemble(machineCode);
    control.setSignals(*insn); //needs to be implemented


    // ===== ID ===== //

    int32_t readData1 = rf.read(insn->rs1);
    int32_t readData2 = rf.read(insn->rs2);

    // ===== EXE ===== //

    int32_t aluOpB = control.condSignals.aluSrc ? insn->immi : readData2;

    aluCtrlOp aluCtrl = control.aluCtrl(control.condSignals.ALUOP, *insn);
    ALU::result aluRes = alu.execute(readData1, aluOpB, aluCtrl);

    bool branch = control.condSignals.branch && aluRes.aluZero;
    int32_t branchTarget = pc + insn->immi;

    if (branch) next_pc = branchTarget;

    // ===== MEM ===== //

    bool memRead = control.condSignals.memRead;
    bool memWrite = control.condSignals.memWrite;

    int32_t readDataDmem = 0;

    if (memWrite) {
        memory.store((int8_t)aluRes.val, readData2);
        pc = next_pc;
        return;
    }

    if (memRead) {
        readDataDmem = memory.load((int8_t)aluRes.val);
    }

    int32_t resultData = control.condSignals.memToReg ? readDataDmem : aluRes.val;

    // ===== WB ===== //
    
    rf.write(insn->rd, resultData);

    pc = next_pc;
}