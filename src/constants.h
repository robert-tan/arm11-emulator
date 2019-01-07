#ifndef CONSTANTS_H
#define CONSTANTS_H

//BOOLEANS
#define TRUE              1
#define FALSE             0

//ARM MACHINE MEMORY CONSTANTS
#define REG_BYTES         4
#define REG_NUM           17
#define MEM_SIZE          65536
#define INSTR_BITS        32
#define BYTE_BITS         8

//GLOBAL 32-BIT UNSIGNED INTEGER MASKS
#define MSB               31
#define MAX_INT           0xffffffff
#define BYTE_MASK         0x000000ff

//DATA TRANSFER OPERATION CODES
#define OP_AND            0x0
#define OP_EOR            0x1
#define OP_SUB            0x2
#define OP_RSB            0x3
#define OP_ADD            0x4
#define OP_TST            0x8
#define OP_TEQ            0x9
#define OP_CMP            0xa
#define OP_ORR            0xc
#define OP_MOV            0xd

//REGISTERS
#define REG_PC            15
#define REG_CPSR          16

//CONDITION CODES
#define EQ                0x0
#define NE                0x1
#define GE                0xa
#define LT                0xb
#define GT                0xc
#define LE                0xd
#define AL                0xe

//INSTRUCTION SIZES
#define BIT_SIZE          1
#define REG_PTR_SIZE      4
#define SMALL_OFFSET_SIZE 12
#define OPCODE_SIZE       4
#define COND_SIZE         4

//INSTRUCTION TYPE CODES
#define HALT              0
#define TYPE_DATAP        1
#define TYPE_MULT         2
#define TYPE_SDT          3
#define TYPE_BRNCH        4

//INSTRUCTION TYPE DETERMINING BITS
#define DET_DATAP         0x02000000
#define DET_MULT          0x00000090
#define DET_SDT           0x04000000
#define DET_BRNCH         0x0a000000

//CPSR FLAGS
#define CPSR_N            0x80000000
#define CPSR_Z            0x40000000
#define CPSR_C            0x20000000
#define CPSR_V            0x10000000

//GLOBAL INSTRUCTION LOCATIONS
#define ALL_COND_LSB      28
#define ALL_I_LSB         25
#define ALL_SET_COND_LSB  20

//DATA PROCESSING LOCATIONS
#define DTP_OPCODE_LSB    21
#define DTP_RN_LSB        16
#define DTP_RD_LSB        12
#define DTP_OPERAND2_LSB  0

//MULTIPLY LOCATIONS
#define MUL_A_LSB         21
#define MUL_RD_LSB        16
#define MUL_RN_LSB        12
#define MUL_RS_LSB        8
#define MUL_RM_LSB        0

//SINGLE DATA TRANSFER LOCATIONS
#define SDT_P_LSB         24
#define SDT_U_LSB         23
#define SDT_L_LSB         20
#define SDT_OFFSET_LSB    0
#define SDT_RN_LSB        16
#define SDT_RD_LSB        12

//BRANCH MASKS
#define BRC_OFFSET_M      0x00ffffff
#define BRC_SIGN_M        0x02000000
#define BRC_FLIP_SIGN_M   0xfc000000
#define BRC_SHIFT_M       0x2

//SHIFTING MASKS
#define OFFSET_IMM_M      0x000000ff
#define OFFSET_ROT_M      0x00000f00
#define OFFSET_CONST_M    0x00000f80
#define OFFSET_TYPE_M     0x00000060
#define OFFSET_RM_M       0x0000000f
#define OFFSET_RS_M       0x00000f00
#define OFFSET_ROT_L      8
#define OFFSET_CONST_L    7
#define OFFSET_TYPE_L     5
#define OFFSET_SHIFT_LSB  4
#define OFFSET_RS_L       8
#define SHIFT_PER_TIME    2

//SHIFT TYPES
#define SHIFT_LSL         0x0
#define SHIFT_LSR         0x1
#define SHIFT_ASR         0x2
#define SHIFT_ROR         0x3

//GPIO
#define GPIO_MEM_1        0x20200000
#define GPIO_MEM_2        0x20200004
#define GPIO_MEM_3        0x20200008
#define GPIO_CLEAR        0x20200028
#define GPIO_SET          0x2020001C


// INSTRUCTION
#define MOV_DEF           0xe0a00000
#define ADD_DEF           0xe0800000
#define SUB_DEF           0xe0400000
#define RSB_DEF           0xe0600000
#define AND_DEF           0xe0000000
#define EOR_DEF           0xe0200000
#define ORR_DEF           0xe0800000
#define TST_DEF           0xe0100000
#define TEQ_DEF           0xe0300000
#define CMP_DEF           0xe0500000
#define MUL_DEF           0xe0000090
#define MLA_DEF           0xe0200090
#define LDR_DEF           0xe4900000
#define STR_DEF           0xe4800000
#define BEQ_DEF           0x0a000000
#define BNE_DEF           0x1a000000
#define BGE_DEF           0xaa000000
#define BLT_DEF           0xba000000
#define BGT_DEF           0xca000000
#define BLE_DEF           0xda000000
#define B_DEF             0xea000000
#define LSL_DEF           0xe1a00000



// EACH POSITION OF ARGUMENTS
#define RD                0x00001000
#define RN                0x00010000
#define TID_ARG           0x00000100
#define SHI_P             0x00000010
#define IM_P              0x01000000
#define TYPE              0x00100000
#endif
