#include <gtest/gtest.h>
#include "isa/decoder.hpp"

// R-type ADD instruction (add x10, x11, x12)
TEST(Decoder, DecodesADD) {
    uint32_t raw = 0x00C58533; 
    Instruction instr = decode(raw);

    EXPECT_EQ(instr.opcode, Opcode::OP);
    EXPECT_EQ(instr.rd, 10);
    EXPECT_EQ(instr.rs1, 11);
    EXPECT_EQ(instr.rs2, 12);
    EXPECT_EQ(instr.funct3, 0);
    EXPECT_EQ(instr.funct7, 0);
    EXPECT_STREQ(instr.debugMnemonic.c_str(), "OP");
}

// U-type LUI instruction (lui x5, 0x12345)
TEST(Decoder, DecodesLUI) {
    uint32_t raw = (0x12345 << 12) | (5 << 7) | 0x37;
    Instruction instr = decode(raw);

    EXPECT_EQ(instr.opcode, Opcode::LUI);
    EXPECT_EQ(instr.rd, 5);
    EXPECT_EQ(instr.imm, 0x12345000);
    EXPECT_STREQ(instr.debugMnemonic.c_str(), "LUI");
}