#include <iostream>
#include "../include/cpu.h"
#include "../include/disasm.h"
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

CPU::CPU(){
    pc = 0;
    global_ticks = 0;
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
    int32_t aluOpB = control.ctrlSignals.aluBSrc ? insn.immi : readData2;
    int32_t aluOpA = control.ctrlSignals.aluASrc ? pc : readData1;

    aluCtrlOp aluCtrl = control.aluCtrl(control.ctrlSignals.ALUOP, insn);
    ALU::result aluRes = alu.execute(aluOpA, aluOpB, aluCtrl);
    bool branch = static_cast<bool>(control.ctrlSignals.branch) && aluRes.aluZero;


    if(branch){
        next_pc = pc + insn.immi;
    }
    if(control.ctrlSignals.branch == 2){
        next_pc = aluRes.val;
    }

    std::cout << "next_pc " << next_pc << std::endl;
    return aluRes;
}

int32_t CPU::mem(ALU::result aluRes, int32_t readData2) {
    int8_t memRead = control.ctrlSignals.memRead;
    int8_t memWrite = control.ctrlSignals.memWrite;

    int32_t readDataDmem = 0;

    if (memWrite == 1) {
        memory.store(static_cast<int8_t>(aluRes.val), readData2);
        std::cout << "memory 0x" << std::hex << aluRes.val << " is modified to 0x" << readData2 << "\n";
        return 0;
    }


    if (memRead == 1) {
        readDataDmem = memory.load(static_cast<int8_t>(aluRes.val));
    }
    std::cout << "aluRes: " << aluRes.val << std::endl;


    if(control.ctrlSignals.WBSel == 0){
        return readDataDmem;
    }
    else if (control.ctrlSignals.WBSel == 1){
        return aluRes.val;
    }
    else {
        return pc + 4;
    }
}

void CPU::writeback(int32_t rd, int32_t val) {
    if(control.ctrlSignals.regWrite == true){
        std::cout << "x" << rd << " is modified to 0x" << std::hex << val << std::dec << std::endl;
        rf.write(rd, val);
    }
    pc = next_pc;
}

void CPU::initTest1(){
    rf.write(1, 0x20);
    rf.write(2, 0x5);
    rf.write(10, 0x70);
    rf.write(11, 0x4);

    memory.store(0x70, 0x5);
    memory.store(0x74, 0x10);
}

void CPU::initTest2(){
    rf.write(8, 0x20);
    rf.write(10, 0x5);
    rf.write(11, 0x2);
    rf.write(12, 0xa);
    rf.write(13, 0xf);
}

void CPU::tick(){
    ++global_ticks;
    std::cout << "total_clock_cycles " << global_ticks << ":\n";
    int32_t machineCode = CPU::fetch(iMem, pc);

    std::unique_ptr<instruction> insn = CPU::decode(machineCode);
    int32_t readData1 = rf.read(insn->rs1);
    int32_t readData2 = rf.read(insn->rs2);

    ALU::result aluRes = execute(*insn, readData1, readData2);

    int32_t resultData = mem(aluRes, readData2);
    writeback(insn->rd, resultData);

    std::cout << "pc is modified to 0x" << std::hex << pc << std::dec << "\n\n";
}