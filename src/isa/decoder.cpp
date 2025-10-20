/**
 * @file decoder.cpp
 * @brief Implements the RISC-V instruction decode logic.
 *
 * The decode() function extracts bitfields from a 32-bit instruction
 * and identifies its opcode type (R/I/S/B/U/J). This is the foundation
 * for the simulator's fetch/decode/execute pipeline.
 *
 * Created: 10/20/2025
 * Last Updated: 10/20/2025
 */

#include "decoder.hpp"
#include <bitset>
#include <iostream>

Instruction decode(uint32_t fetchInstr) {
    Instruction instr{};
    instr.fetchInstr = fetchInstr;

    // Extract base fields 
    uint8_t opcode_field = fetchInstr & 0x7F;   // bits [6:0]
    instr.rd     = (fetchInstr >> 7)  & 0x1F;   // bits [11:7]
    instr.funct3 = (fetchInstr >> 12) & 0x07;   // bits [14:12]
    instr.rs1    = (fetchInstr >> 15) & 0x1F;   // bits [19:15]
    instr.rs2    = (fetchInstr >> 20) & 0x1F;   // bits [24:20]
    instr.funct7 = (fetchInstr >> 25) & 0x7F;   // bits [31:25]

    // Classify instruction type by opcode
    switch (opcode_field) {
        case 0x37: instr.opcode = Opcode::LUI;    instr.debugMnemonic = "LUI";    break;
        case 0x17: instr.opcode = Opcode::AUIPC;  instr.debugMnemonic = "AUIPC";  break;
        case 0x6F: instr.opcode = Opcode::JAL;    instr.debugMnemonic = "JAL";    break;
        case 0x67: instr.opcode = Opcode::JALR;   instr.debugMnemonic = "JALR";   break;
        case 0x63: instr.opcode = Opcode::BRANCH; instr.debugMnemonic = "BRANCH"; break;
        case 0x03: instr.opcode = Opcode::LOAD;   instr.debugMnemonic = "LOAD";   break;
        case 0x23: instr.opcode = Opcode::STORE;  instr.debugMnemonic = "STORE";  break;
        case 0x13: instr.opcode = Opcode::OP_IMM; instr.debugMnemonic = "OP_IMM"; break;
        case 0x33: instr.opcode = Opcode::OP;     instr.debugMnemonic = "OP";     break;
        case 0x73: instr.opcode = Opcode::SYSTEM; instr.debugMnemonic = "SYSTEM"; break;
        default:   instr.opcode = Opcode::INVALID; instr.debugMnemonic = "INVALID"; break;
    }

    // Decode immediates 
    switch (instr.opcode) {
        case Opcode::LUI:
        case Opcode::AUIPC:
            // U-type immediate: bits [31:12]
            instr.imm = fetchInstr & 0xFFFFF000;
            break;

        case Opcode::JAL: {
            // J-type immediate: [20|10:1|11|19:12]
            uint32_t imm20     = (fetchInstr >> 31) & 0x1;
            uint32_t imm10_1   = (fetchInstr >> 21) & 0x3FF;
            uint32_t imm11     = (fetchInstr >> 20) & 0x1;
            uint32_t imm19_12  = (fetchInstr >> 12) & 0xFF;
            instr.imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);

            // Sign-extend immediate
            if (imm20) instr.imm |= 0xFFF00000;
            break;
        }

        case Opcode::BRANCH: {
            // B-type immediate: [12|10:5|4:1|11]
            uint32_t imm12    = (fetchInstr >> 31) & 0x1;
            uint32_t imm10_5  = (fetchInstr >> 25) & 0x3F;
            uint32_t imm4_1   = (fetchInstr >> 8)  & 0xF;
            uint32_t imm11    = (fetchInstr >> 7)  & 0x1;
            instr.imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);

            if (imm12) instr.imm |= 0xFFFFE000;  // sign-extend 13-bit immediate
            break;
        }

        case Opcode::STORE: {
            // S-type immediate: [11:5|4:0]
            uint32_t imm11_5 = (fetchInstr >> 25) & 0x7F;
            uint32_t imm4_0  = (fetchInstr >> 7)  & 0x1F;
            instr.imm = (imm11_5 << 5) | imm4_0;

            if (imm11_5 & 0x40) instr.imm |= 0xFFFFF000; // sign-extend 12-bit immediate
            break;
        }

        case Opcode::LOAD:
        case Opcode::JALR:
        case Opcode::OP_IMM:
        case Opcode::SYSTEM:
            // I-type immediate: bits [31:20]
            instr.imm = (fetchInstr >> 20) & 0xFFF;
            if (instr.imm & 0x800) instr.imm |= 0xFFFFF000; // sign-extend 12-bit
            break;

        case Opcode::OP:
        case Opcode::INVALID:
        default:
            // R-type or invalid — no immediate field
            instr.imm = 0;
            break;
    }

    return instr;
}
