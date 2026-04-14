#include <iostream>
#include <cassert>
#include <memory>
#include <bitset>
#include <iomanip>
#include "../include/constant.h"
#include "../include/disasm.h"

// Helper to run a test and check results with verbose logging
void test_instruction(
    const std::string& name,
    int32_t code,
    type expectedType,
    mnemonic expectedOp,
    int rd, int rs1, int rs2, int32_t imm, int f3, int f7
) {
    
    // Identifier for easier debugging
    std::cout << "[RUNNING] " << name << std::endl;

    auto insn = disassemble(code); 
    
    assert(insn->insnType == expectedType);
    assert(insn->operation == expectedOp);
    assert(insn->rd == rd);
    assert(insn->rs1 == rs1);
    assert(insn->rs2 == rs2);
    assert(insn->immi == imm);
    assert(insn->funct3 == f3);
    assert(insn->funct7 == f7);

    std::cout << "  --> [PASSED]" << std::endl;
}

int main() {
    std::cout << "Starting RISC-V Disassembler Tests...\n" << std::endl;

    // Format: Name, Code, Type, Mnemonic, rd, rs1, rs2, imm, f3, f7

    // --- R-Type Instructions ---
    // add   x5, x4, x3
    test_instruction("ADD",  0x003202b3, type::R, mnemonic::ADD,  5,  4, 3, 0, 0x0, 0x00); // 00000000001100100000001010110011
    // sub   x6, x4, x3
    test_instruction("SUB",  0x40320333, type::R, mnemonic::SUB,  6,  4, 3, 0, 0x0, 0x20); // 01000000001100100000001100110011
    // sll   x7, x4, x3
    test_instruction("SLL",  0x003213b3, type::R, mnemonic::SLL,  7,  4, 3, 0, 0x1, 0x00); // 00000000001100100001001110110011
    // slt   x8, x4, x3
    test_instruction("SLT",  0x00322433, type::R, mnemonic::SLT,  8,  4, 3, 0, 0x2, 0x00); // 00000000001100100010010000110011
    // sltu  x9, x4, x3
    test_instruction("SLTU", 0x003234b3, type::R, mnemonic::SLTU, 9,  4, 3, 0, 0x3, 0x00); // 00000000001100100011010010110011
    // xor   x10, x4, x3
    test_instruction("XOR",  0x00324533, type::R, mnemonic::XOR,  10, 4, 3, 0, 0x4, 0x00); // 00000000001100100100010100110011
    // srl   x11, x4, x3
    test_instruction("SRL",  0x003255b3, type::R, mnemonic::SRL,  11, 4, 3, 0, 0x5, 0x00); // 00000000001100100101010110110011
    // sra   x12, x4, x3
    test_instruction("SRA",  0x40325633, type::R, mnemonic::SRA,  12, 4, 3, 0, 0x5, 0x20); // 01000000001100100101011000110011
    // or    x13, x4, x3
    test_instruction("OR",   0x003266b3, type::R, mnemonic::OR,   13, 4, 3, 0, 0x6, 0x00); // 00000000001100100110011010110011
    // and   x14, x4, x3
    test_instruction("AND",  0x00327733, type::R, mnemonic::AND,  14, 4, 3, 0, 0x7, 0x00); // 00000000001100100111011100110011

    // --- I-Type Instructions (Arithmetic/Logic) ---
    // addi  x15, x4, 10
    test_instruction("ADDI",  0x00a20793, type::I, mnemonic::ADDI,  15, 4, 0, 10,  0x0, 0); // 00000000101000100000011110010011
    // slti  x16, x4, -16
    test_instruction("SLTI",  0xff022813, type::I, mnemonic::SLTI,  16, 4, 0, -16, 0x2, 0); // 11111111000000100010100000010011
    // sltiu x17, x4, 10
    test_instruction("SLTIU", 0x00a23893, type::I, mnemonic::SLTIU, 17, 4, 0, 10,  0x3, 0); // 00000000101000100011100010010011
    // xori  x18, x4, 15
    test_instruction("XORI",  0x00f24913, type::I, mnemonic::XORI,  18, 4, 0, 15,  0x4, 0); // 00000000111100100100100100010011
    // ori   x19, x4, 15
    test_instruction("ORI",   0x00f26993, type::I, mnemonic::ORI,   19, 4, 0, 15,  0x6, 0); // 00000000111100100110100110010011
    // andi  x20, x4, 15
    test_instruction("ANDI",  0x00f27a13, type::I, mnemonic::ANDI,  20, 4, 0, 15,  0x7, 0); // 00000000111100100111101000010011
    // slli  x21, x4, 5
    test_instruction("SLLI",  0x00521a93, type::I, mnemonic::SLLI,  21, 4, 0, 5,   0x1, 0x00); // 00000000010100100001101010010011
    // srli  x22, x4, 5
    test_instruction("SRLI",  0x00525b13, type::I, mnemonic::SRLI,  22, 4, 0, 5,   0x5, 0x00); // 00000000010100100101101100010011
    // srai  x23, x4, 5
    test_instruction("SRAI",  0x40525b93, type::I, mnemonic::SRAI,  23, 4, 0, 5,   0x5, 0x20); // 01000000010100100101101110010011

    // --- I-Type Instructions (Loads) ---
    // lb    x24, 0(x2)
    test_instruction("LB", 0x00010c03, type::I, mnemonic::LB, 24, 2, 0, 0, 0x0, 0); // 00000000000000010000110000000011
    // lh    x25, 2(x2)
    test_instruction("LH", 0x00211c83, type::I, mnemonic::LH, 25, 2, 0, 2, 0x1, 0); // 00000000001000010001110010000011
    // lw    x26, 4(x2)
    test_instruction("LW", 0x00412d03, type::I, mnemonic::LW, 26, 2, 0, 4, 0x2, 0); // 00000000010000010010110100000011

    // --- S-Type Instructions (Stores) ---
    // sb    x27, 0(x2)
    test_instruction("SB", 0x01b10023, type::S, mnemonic::SB, 0, 2, 27, 0, 0x0, 0); // 00000001101100010000000000100011
    // sh    x28, 2(x2)
    test_instruction("SH", 0x01c11123, type::S, mnemonic::SH, 0, 2, 28, 2, 0x1, 0); // 00000001110000010001000100100011
    // sw    x29, 4(x2)
    test_instruction("SW", 0x01d12223, type::S, mnemonic::SW, 0, 2, 29, 4, 0x2, 0); // 00000001110100010010001000100011

    // --- SB-Type Instructions (Branches) ---
    // beq   x4, x3, 0x00000010
    test_instruction("BEQ", 0x00320863, type::SB, mnemonic::BEQ, 0, 4, 3, 16, 0x0, 0); // 00000000001100100000100001100011
    // bne   x4, x3, 0x0000000c
    test_instruction("BNE", 0x00321663, type::SB, mnemonic::BNE, 0, 4, 3, 12, 0x1, 0); // 00000000001100100001011001100011
    // blt   x4, x3, 0x00000008
    test_instruction("BLT", 0x00324463, type::SB, mnemonic::BLT, 0, 4, 3, 8, 0x4, 0); // 00000000001100100100010001100011
    // bge   x4, x3, 0x00000004
    test_instruction("BGE", 0x00325263, type::SB, mnemonic::BGE, 0, 4, 3, 4, 0x5, 0); // 00000000001100100101001001100011

    // --- UJ-Type and I-Type (Jumps) ---
    // jal   x1, 0x00000004
    test_instruction("JAL",  0x004000ef, type::UJ, mnemonic::JAL,  1, 0, 0, 4, 0x0, 0); // 00000000010000000000000011101111
    // jalr  x0, x1, 0
    test_instruction("JALR", 0x00008067, type::I,  mnemonic::JALR, 0, 1, 0, 0,  0x0, 0); // 00000000000000001000000001100111

    std::cout << "\nAll test cases passed successfully!" << std::endl;
    return 0;
}