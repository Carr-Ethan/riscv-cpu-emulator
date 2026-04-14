#include <iostream>
#include <bitset>
#include <cstdint>
#include <memory>
#include <cassert>
#include "../include/disasm.h"
#include "../include/constant.h"

static int32_t getOpcode(const int32_t raw){
    return (raw & BIT_MASK_7);
}

static int32_t getRs1(const int32_t raw){
    return ((raw >> 15) & BIT_MASK_5);
}

static int32_t getRs2(const int32_t raw){
    return ((raw >> 20) & BIT_MASK_5);
}

static int32_t getRd(const int32_t raw){
    return ((raw >> 7) & BIT_MASK_5);
}

static int32_t getFunct3(const int32_t raw){
    return ((raw >> 12) & BIT_MASK_3);
}

static int32_t getFunct7(const int32_t raw){
    return ((raw >> 25) & BIT_MASK_7);
}
static int32_t getFunct7_I(const int32_t raw){
    return ((raw >> 25) & BIT_MASK_7);
} 

static int32_t getImmi_I(const int32_t raw){
    return ((static_cast<int32_t>(raw) >> 20));
}

static int32_t getImmi_S(const int32_t raw){
    return ((static_cast<int32_t>(((raw >> 7) & BIT_MASK_5) | 
        (((raw >> 25) & BIT_MASK_7) << 5)) << 20) >> 20);
}

static int32_t getImmi_SB(const int32_t raw){
    return ((static_cast<int32_t>((((raw >> 7) & BIT_MASK_1) << 11) | 
        (((raw >> 8) & BIT_MASK_4) << 1) | 
        (((raw >> 25) & BIT_MASK_6) << 5) | 
        (((raw >> 31) & BIT_MASK_1) << 12)) << 19) >> 19);
}

static int32_t getImmi_U(const int32_t raw){
    return(static_cast<int32_t>(raw) >> 12);
}


// Bit 31    -> bit 20
// Bit 30-21 -> bit 10-1
// Bit 20    -> bit 11
// Bit 19-12 -> bit 19-12

static int32_t getImmi_UJ(const int32_t raw) {
    return (static_cast<int32_t>(
        (((raw >> 31) & BIT_MASK_1) << 20) |
        (((raw >> 21) & BIT_MASK_10) << 1) | 
        (((raw >> 20) & BIT_MASK_1) << 11) |
        (((raw >> 12) & BIT_MASK_8) << 12)
    ) << 11) >> 11;
}

static int32_t getShamt(const int32_t raw){
    return ((raw >> 20) & BIT_MASK_5);
}

static void printInsn(const std::unique_ptr<instruction>& insn){
    std::cout << "Instruction Type: " << typeToString(insn->insnType) <<
        "\nOperation: " << mnemonicToString(insn->operation) << std::endl;
    if(insn->insnType == type::R){
        std::cout << "Rs1: x" << static_cast<int>(insn->rs1) <<
            "\nRs2: x" << insn->rs2 <<
            "\nRd: x" << insn->rd <<
            "\nFunct3: " << insn->funct3 <<
            "\nFunct7: " << insn->funct7 << std::endl;
    }
    else if(insn->insnType == type::I){
        std::cout << "Rs1: x" << insn->rs1 <<
            "\nRd: x" << insn->rd <<
            "\nImmediate: " << static_cast<int>(insn->immi) << 
            " (or 0x" << std::hex << static_cast<int>(insn->immi) << ")" << 
            std::dec << std::endl;
    }
    else if(insn->insnType == type::S || insn->insnType == type::SB){
        std::cout << "Rs1: x" << insn->rs1 <<
            "\nRs2: x" << insn->rs2 <<
            "\nImmediate: " << static_cast<int>(insn->immi) << 
            " (or 0x" << std::hex << static_cast<int>(insn->immi) << ")" << 
            std::dec << std::endl;
    }
    else if(insn->insnType == type::U || insn->insnType == type::UJ){
        std::cout << "Rd: x" << insn->rd <<
            "\nImmediate: " << static_cast<int>(insn->immi)  <<
            " (or 0x" << std::hex << static_cast<int>(insn->immi) << ")" <<
            std::dec << std::endl;
    }
}

static void disasmR(const std::unique_ptr<instruction>& insn){
    insn->funct3 = getFunct3(insn->machineCode);
    insn->funct7 = getFunct7(insn->machineCode);

    switch (insn->funct3)
    {
    case 0x0:
        if(insn->funct7 == 0x0){
            insn->operation = mnemonic::ADD;
        }
        else{
            insn->operation = mnemonic::SUB;
        }
        break;
    case 0x1:
        insn->operation = mnemonic::SLL;
        break;
    case 0x2:
        insn->operation = mnemonic::SLT;
        break;
    case 0x3:
        insn->operation = mnemonic::SLTU;
        break;
    case 0x4:
        insn->operation = mnemonic::XOR;
        break;
    case 0x5:
        if(insn->funct7 == 0x0){
            insn->operation = mnemonic::SRL;
        }
        else{
            insn->operation = mnemonic::SRA;
        }
        break;
    case 0x6:
        insn->operation = mnemonic::OR;
        break;
    case 0x7:
        insn->operation = mnemonic::AND;
        break;
    default:
        insn->operation = mnemonic::UNKNOWN;
        break;
    }

    insn->rs1 = getRs1(insn->machineCode);
    insn->rs2 = getRs2(insn->machineCode);
    insn->rd = getRd(insn->machineCode);
    printInsn(insn);
}

static void disasmI(const std::unique_ptr<instruction>& insn, const int32_t opcode){
    insn->funct3 = getFunct3(insn->machineCode);
    insn->immi = getImmi_I(insn->machineCode);
    if(opcode == 0x3){
        switch(insn->funct3){
            case 0x0:
                insn->operation = mnemonic::LB;
                break;
            case 0x1:
                insn->operation = mnemonic::LH;
                break;
            case 0x2:
                insn->operation = mnemonic::LW;
                break;
            default:
                insn->operation = mnemonic::UNKNOWN;
                break;
        }
    }
    else if (opcode == 0x13){
        switch (insn->funct3)
        {
        case 0x0:
            insn->operation = mnemonic::ADDI;
            break;
        case 0x1:
            insn->operation = mnemonic::SLLI;
            break;
        case 0x2:
            insn->operation = mnemonic::SLTI;
            break;
        case 0x3:
            insn->operation = mnemonic::SLTIU;
            break;
        case 0x4:
            insn->operation = mnemonic::XORI;
            break;
        case 0x5: {
            int32_t f7 = getFunct7_I(insn->machineCode);
            insn->immi = getShamt(insn->machineCode);
            if(f7 == 0x0){
                insn->operation = mnemonic::SRLI;
                insn->funct7 = f7;
            }
            else if(f7 == 0x20){
                insn->operation = mnemonic::SRAI;
                insn->funct7 = f7;
            }
            else{
                insn->operation = mnemonic::UNKNOWN;
            }
            break;
        }
        case 0x6:
            insn->operation = mnemonic::ORI;
            break;
        case 0x7:
            insn->operation = mnemonic::ANDI;
            break;
        default:
            insn->operation = mnemonic::UNKNOWN;
            break;
        }
    }
    else if (opcode == 0x67){
        insn->operation = mnemonic::JALR;
    }
    insn->rs1 = getRs1(insn->machineCode);
    insn->rd = getRd(insn->machineCode);
    printInsn(insn);
}

static void disasmS(const std::unique_ptr<instruction>& insn){
    insn->funct3 = getFunct3(insn->machineCode);
    switch (insn->funct3)
    {
    case 0x0:
        insn->operation = mnemonic::SB;
        break;
    case 0x1:
        insn->operation = mnemonic::SH;
        break;
    case 0x2:
        insn->operation = mnemonic::SW;
        break;
    default:
        break;
    }
    insn->rs1 = getRs1(insn->machineCode);
    insn->rs2 = getRs2(insn->machineCode);
    insn->immi = getImmi_S(insn->machineCode);
    printInsn(insn);
}

static void disasmSB(const std::unique_ptr<instruction>& insn){
    insn->funct3 = getFunct3(insn->machineCode);
    switch (insn->funct3)
    {
    case 0x0:
        insn->operation = mnemonic::BEQ;
        break;
    case 0x1:
        insn->operation = mnemonic::BNE;
        break;
    case 0x4:
        insn->operation = mnemonic::BLT;
        break;
    case 0x5:
        insn->operation = mnemonic::BGE;
        break;
    default:
        break;
    }
    insn->rs1 = getRs1(insn->machineCode);
    insn->rs2 = getRs2(insn->machineCode);
    insn->immi = getImmi_SB(insn->machineCode);
    printInsn(insn);
}

static void disasmUJ(const std::unique_ptr<instruction>& insn){
    insn->operation = mnemonic::JAL;
    insn->rd = getRd(insn->machineCode);
    insn->immi = getImmi_UJ(insn->machineCode);
    printInsn(insn);
}

//return unique_ptr to use unit tests
// std::unique_ptr<instruction> disassemble(int32_t machineCode){
void disassemble(int32_t machineCode){
    const int32_t opcode = getOpcode(machineCode);

    std::unique_ptr<instruction> insn = std::make_unique<instruction>();
    insn->machineCode = machineCode;

    if(opcode == 0x33){
        insn->insnType = type::R;
        disasmR(insn);
    }
    else if(opcode == 0x3 || opcode == 0x13 || opcode == 0x67){
        insn->insnType = type::I;
        disasmI(insn, opcode);
    }
    else if(opcode == 0x23){
        insn->insnType = type::S;
        disasmS(insn);
    }
    else if(opcode == 0x63){
        insn->insnType = type::SB;
        disasmSB(insn);
    }
    else if(opcode == 0x6F){
        insn->insnType = type::UJ;
        disasmUJ(insn);
    }
    // return insn;
}