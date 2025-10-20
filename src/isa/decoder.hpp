/**
 * @file decoder.hpp
 * @brief Defines the RISC-V instruction format and decode interface.
 *
 * This header provides the Instruction struct, Opcode enumeration, 
 * and a decode function prototype used to extract fields from a 
 * 32-bit RISC-V instruction word. It serves as the foundation for 
 * the instruction decoding stage in the simulator pipeline.
 *
 * Created: 10/20/2025
 * Last Updated: 10/20/2025
 */

#pragma once
#include <string>
#include <cstdint>

enum class Opcode {
    LUI, AUIPC, JAL, JALR,
    BRANCH, LOAD, STORE,
    OP_IMM, OP, SYSTEM,
    INVALID
};

struct Instruction {
    uint32_t fetchInstr;
    Opcode opcode;
    uint8_t rs1, rs2, rd;
    uint32_t imm;
    uint8_t funct3, funct7;
    std::string debugMnemonic;
};

Instruction decode(uint32_t fetchInstr);