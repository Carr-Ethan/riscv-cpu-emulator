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

    next_pc = pc + 4;
    return machineCode;
}

std::unique_ptr<instruction> CPU::decode(int32_t machineCode) {
    std::unique_ptr<instruction> insn = decoder.disassemble(machineCode, control);
    return insn;
}

ALU::result CPU::execute(instruction insn, int32_t readData1, int32_t readData2) {
    int32_t aluOpB = control.ctrlSignals.aluSrc ? insn.immi : readData2;

    aluCtrlOp aluCtrl = control.aluCtrl(control.ctrlSignals.ALUOP, insn);
    ALU::result aluRes = alu.execute(readData1, aluOpB, aluCtrl);

    bool branch = control.ctrlSignals.branch && aluRes.aluZero;
    int32_t branchTarget = pc + insn.immi;

    if (branch) next_pc = branchTarget;
    pc = next_pc;

    return aluRes;
}

int32_t CPU::mem(ALU::result aluRes, int32_t readData2) {
    bool memRead = control.ctrlSignals.memRead;
    bool memWrite = control.ctrlSignals.memWrite;

    int32_t readDataDmem = 0;

    if (memWrite) {
        memory.store(static_cast<int8_t>(aluRes.val), readData2);
        return 0;
    }

    if (memRead) {
        readDataDmem = memory.load(static_cast<int8_t>(aluRes.val));
    }


    return control.ctrlSignals.memToReg ? readDataDmem : aluRes.val;
}

void CPU::writeback(int32_t rd, int32_t val) {
    if(control.ctrlSignals.memToReg == true){
        std::cout << "x" << rd << " is modified to 0x" << std::hex << val << std::dec << std::endl;
        rf.write(rd, val);
    }
}

void CPU::initTest1(){
    rf.write(1, 0x20);
    rf.write(2, 0x5);
    rf.write(10, 0x70);
    rf.write(11, 0x4);

    memory.store(0x70, 0x5);
    memory.store(0x74, 0x10);
}

void CPU::tick(){
    ++global_ticks;
    std::cout << "\ntotal_clock_cycles " << global_ticks << ":\n";
    int32_t machineCode = CPU::fetch(iMem, pc);

    std::unique_ptr<instruction> insn = CPU::decode(machineCode);
    int32_t readData1 = rf.read(insn->rs1);
    int32_t readData2 = rf.read(insn->rs2);

    ALU::result aluRes = execute(*insn, readData1, readData2);

    int32_t resultData = mem(aluRes, insn->rd);
    std::cout << control.ctrlSignals.memToReg << "\n";
    writeback(insn->rd, resultData);

    std::cout << "pc is modified to " << std::hex << pc << std::dec << "\n";
}