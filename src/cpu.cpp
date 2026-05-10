#include <iostream>
#include "../include/cpu.h"
#include "../include/disasm.h"
#include "../include/components.h"
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

CPU::CPU(){
    pc = 0;
    global_ticks = 0;
}

bool CPU::isDone() {
    return (pc / 4 >= iMem.size()) && !IF_ID.isValid() && !ID_EX.isValid() && !EX_MEM.isValid() && !MEM_WB.isValid();
}

void CPU::fetch(){
    if (pc / 4 < iMem.size()) {
        IF_ID_Data nextData;
        nextData.pc = pc;
        nextData.machineCode = std::stol(iMem[pc / 4], nullptr, 2);
        
        IF_ID.write(nextData); 
        pc += 4;
    }
}

void CPU::decode(bool &stall) {
    auto if_id = IF_ID.read();
    std::unique_ptr<instruction> insn = decoder.disassemble(if_id.machineCode, control);

    bool usesRs1 = false;
    bool usesRs2 = false;
    if (insn->insnType == type::R || insn->insnType == type::S || insn->insnType == type::SB) {
        usesRs1 = true;
        usesRs2 = true;
    } else if (insn->insnType == type::I) {
        usesRs1 = true;
        usesRs2 = false;
    }

    bool hazard = false;
    auto id_ex = ID_EX.read();
    auto ex_mem = EX_MEM.read();
    auto mem_wb = MEM_WB.read();

    if (usesRs1 && insn->rs1 != 0) {
        if (ID_EX.isValid() && id_ex.ctrlSignals.regWrite && id_ex.rd == insn->rs1) hazard = true;
        if (EX_MEM.isValid() && ex_mem.ctrlSignals.regWrite && ex_mem.rd == insn->rs1) hazard = true;
    }
    if (usesRs2 && insn->rs2 != 0) {
        if (ID_EX.isValid() && id_ex.ctrlSignals.regWrite && id_ex.rd == insn->rs2) hazard = true;
        if (EX_MEM.isValid() && ex_mem.ctrlSignals.regWrite && ex_mem.rd == insn->rs2) hazard = true;
    }

    if (hazard) {
        stall = true;
        return;
    }

    ID_EX_Data nextData;
    nextData.pc = if_id.pc;
    nextData.readData1 = rf.read(insn->rs1);
    nextData.readData2 = rf.read(insn->rs2);
    nextData.immi = insn->immi;
    nextData.rs1 = insn->rs1;
    nextData.rs2 = insn->rs2;
    nextData.rd = insn->rd;
    nextData.ctrlSignals = control.ctrlSignals;
    nextData.aluCtrl = control.aluCtrl(control.ctrlSignals.ALUOP, *insn);

    ID_EX.write(nextData);
}

void CPU::execute(bool &flush, uint32_t &branch_target) {
    auto id_ex = ID_EX.read();

    int32_t aluOpB = id_ex.ctrlSignals.aluBSrc ? id_ex.immi : id_ex.readData2;
    int32_t aluOpA = id_ex.ctrlSignals.aluASrc ? id_ex.pc : id_ex.readData1;

    ALU::result aluRes = alu.execute(aluOpA, aluOpB, id_ex.aluCtrl);
    bool branch = static_cast<bool>(id_ex.ctrlSignals.branch == 1) && aluRes.aluZero;

    if (branch) {
        branch_target = id_ex.pc + id_ex.immi;
        flush = true;
    } else if (id_ex.ctrlSignals.branch == 2) {
        branch_target = aluRes.val;
        flush = true;
    }

    EX_MEM_Data nextData;
    nextData.pc = id_ex.pc;
    nextData.aluVal = aluRes.val;
    nextData.readData2 = id_ex.readData2;
    nextData.rd = id_ex.rd;
    nextData.ctrlSignals = id_ex.ctrlSignals;

    EX_MEM.write(nextData);
}

void CPU::mem_stage() {
    auto ex_mem = EX_MEM.read();
    MEM_WB_Data nextData;

    nextData.pc = ex_mem.pc;
    nextData.aluVal = ex_mem.aluVal;
    nextData.rd = ex_mem.rd;
    nextData.ctrlSignals = ex_mem.ctrlSignals;

    if (ex_mem.ctrlSignals.memWrite == 1) {
        memory.store(static_cast<int8_t>(ex_mem.aluVal), ex_mem.readData2);
        std::cout << "total_clock_cycles " << global_ticks << ":\n";
        std::cout << "memory 0x" << std::hex << ex_mem.aluVal << " is modified to 0x" << ex_mem.readData2 << std::dec << "\n";
        std::cout << "pc is modified to 0x" << std::hex << nextData.pc+4 << std::dec << "\n\n";
    }

    if (ex_mem.ctrlSignals.memRead == 1) {
        nextData.readDataDmem = memory.load(static_cast<int8_t>(ex_mem.aluVal));
    } else {
        nextData.readDataDmem = 0;
    }

    MEM_WB.write(nextData);
}

void CPU::writeback() {
    auto mem_wb = MEM_WB.read();
    std::cout << "total_clock_cycles " << global_ticks << ":\n";

    if (mem_wb.ctrlSignals.regWrite == 1 && mem_wb.rd != 0) {
        int32_t resultData = 0;
        if (mem_wb.ctrlSignals.WBSel == 0) {
            resultData = mem_wb.readDataDmem;
        } else if (mem_wb.ctrlSignals.WBSel == 1) {
            resultData = mem_wb.aluVal;
        } else {
            resultData = mem_wb.pc + 4;
        }

        rf.write(mem_wb.rd, resultData);
        std::cout << "x" << (int)mem_wb.rd << " is modified to 0x" << std::hex << resultData << std::dec << std::endl;
    }
    std::cout << "pc is modified to 0x" << std::hex << mem_wb.pc+4 << std::dec << "\n\n";
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

    bool flush_control = false;
    uint32_t branch_target = 0;
    bool stall = false;


    if (MEM_WB.isValid()) writeback();
    if (EX_MEM.isValid()) mem_stage();
    if (ID_EX.isValid()) execute(flush_control, branch_target);
    if (IF_ID.isValid() && !flush_control) decode(stall);
    if (!stall && !flush_control) fetch();

    if (flush_control) {
        pc = branch_target;
        IF_ID.flush();
        ID_EX.flush();
    } else if (stall) {
        IF_ID.write(IF_ID.read()); 
        ID_EX.nop();
    }

    IF_ID.clock_edge();
    ID_EX.clock_edge();
    EX_MEM.clock_edge();
    MEM_WB.clock_edge();

}