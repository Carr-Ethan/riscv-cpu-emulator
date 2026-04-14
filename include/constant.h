#pragma once
#include <string>
#include "../include/constant.h"
#include <unordered_map>

#ifndef CONSTANT_H
#define CONSTANT_H

enum class type : uint8_t{
    R,
    I,
    S,
    SB,
    U, 
    UJ,
    UNKNOWN
};

enum class mnemonic : uint8_t{
    ADD,
    AND,
    BEQ,
    BLT,
    JAL,   
    LB,    
    LW,    
    OR,
    SB,    
    SLL,   
    SLT,   
    SLTIU, 
    SRA,   
    SRL,   
    SUB,   
    XOR,
    ADDI,  
    ANDI,  
    BGE,  
    BNE,   
    JALR,  
    LH,    
    ORI,   
    SH,
    SLLI,  
    SLTI,  
    SLTU,  
    SRAI,  
    SRLI,  
    SW,    
    XORI,  
    UNKNOWN
};

inline std::string typeToString(type t){
    switch (t) {
        case type::R:
            return "R";
        case type::I:
            return "I";
        case type::S:
            return "S";
        case type::SB:
            return "SB";
        case type::U:
            return "U";
        case type::UJ:
            return "UJ";
        default:
            return "unknown";
    }
}

inline std::string mnemonicToString(mnemonic m){
    switch (m) {
        case mnemonic::ADD:   
            return "add";
        case mnemonic::AND:   
            return "and";
        case mnemonic::BEQ:   
            return "beq";
        case mnemonic::BLT:   
            return "blt";
        case mnemonic::JAL:   
            return "jal";
        case mnemonic::LB:    
            return "lb";
        case mnemonic::LW:    
            return "lw";
        case mnemonic::OR:    
            return "or";
        case mnemonic::SB:    
            return "sb";
        case mnemonic::SLL:   
            return "sll";
        case mnemonic::SLT:   
            return "slt";
        case mnemonic::SLTIU: 
            return "sltiu";
        case mnemonic::SRA:   
            return "sra";
        case mnemonic::SRL:   
            return "srl";
        case mnemonic::SUB:   
            return "sub";
        case mnemonic::XOR:   
            return "xor";
        case mnemonic::ADDI:  
            return "addi";
        case mnemonic::ANDI:  
            return "andi";
        case mnemonic::BGE:   
            return "bge";
        case mnemonic::BNE:   
            return "bne";
        case mnemonic::JALR:  
            return "jalr";
        case mnemonic::LH:    
            return "lh";
        case mnemonic::ORI:   
            return "ori";
        case mnemonic::SH:    
            return "sh";
        case mnemonic::SLLI:  
            return "slli";
        case mnemonic::SLTI:  
            return "slti";
        case mnemonic::SLTU:  
            return "sltu";
        case mnemonic::SRAI:  
            return "srai";
        case mnemonic::SRLI:  
            return "srli";
        case mnemonic::SW:    
            return "sw";
        case mnemonic::XORI:  
            return "xori";
        default:              
            return "unknown";
    }
}

#endif