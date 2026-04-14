.text
.globl main
main:
    # R-Type Instructions
    add   x5, x4, x3
    sub   x6, x4, x3
    sll   x7, x4, x3
    slt   x8, x4, x3
    sltu  x9, x4, x3
    xor   x10, x4, x3
    srl   x11, x4, x3
    sra   x12, x4, x3
    or    x13, x4, x3
    and   x14, x4, x3

    # I-Type Instructions (Arithmetic/Logic)
    addi  x15, x4, 10
    slti  x16, x4, -16
    sltiu x17, x4, 10
    xori  x18, x4, 15
    ori   x19, x4, 15
    andi  x20, x4, 15
    slli  x21, x4, 5
    srli  x22, x4, 5
    srai  x23, x4, 5

    # I-Type Instructions (Loads)
    lb    x24, 0(x2)
    lh    x25, 2(x2)
    lw    x26, 4(x2)

    # S-Type Instructions (Stores)
    sb    x27, 0(x2)
    sh    x28, 2(x2)
    sw    x29, 4(x2)

    # SB-Type Instructions (Branches)
    beq   x4, x3, target_beq
    bne   x4, x3, target_bne
    blt   x4, x3, target_blt
    bge   x4, x3, target_bge

target_beq:
target_bne:
target_blt:
target_bge:

    # UJ-Type and I-Type (Jumps)
    jal   x1, target_jal
target_jal:
    jalr  x0, x1, 0


    # you can ignore these
    li a7, 10
    ecall