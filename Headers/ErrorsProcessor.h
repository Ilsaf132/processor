#ifndef ERRORS_
#define ERRORS_

const int LABELS_SIZE_MAX = 10;
const int REG_LEN = 10;
const int NUMBERS_LEN = 1000;
const int RAM_MEMORY = 100;
const int MAX_LEN_NAME = 6;
const int MAX_LEN_CODE_START = 3;
const int MAX_LEN_X_FORMAT = 100;
const int REG_SIZE = 5;
const int SIZE_ASM = 5;

enum Error_proc {
    LABEL_EMPTY = -1,
    ERROR_OK = 0,
    ERROR_MASSIVE = 1,    
    ERROR_HLT = 2,
    ERROR_ARRAY_CODE = 3,
    ERROR_STRUCT = 4,
    ERROR_LABELS = 5,
    ERROR_WRONG_COMMAND = 6,
    ERROR_ADDRESS_LABELS = 7,
    ERROR_ADDRESS_TEXT = 8,
    ERROR_ADDRESS_TEXT_LEN = 9,
    ERROR_ADDRESS_PROCESSOR = 10,
    ERROR_ADDRESS_CODE = 11,
    ERROR_ADDRESS_FILE = 12,
    ERROR_GETCODE = 13,
    ERROR_ADDRESS_PROC_CODE = 14,
};
enum Code_const {
    PUSH_CODE = 1,
    ADD_CODE  = 2,
    SUB_CODE  = 3,
    DIV_CODE  = 4,
    MUL_CODE  = 5,
    OUT_CODE  = 6,
    IN_CODE   = 7,
    DUMP_CODE = 8,
    SQRT_CODE = 9,
    SIN_CODE  = 10,
    COS_CODE  = 11,
    POP_CODE = 13,
    JA_CODE = 14,
    JAE_CODE = 15,
    JB_CODE = 16,
    JBE_CODE = 17,
    JE_CODE = 18,
    JNE_CODE = 19,
    JMP_CODE = 20,
    HLT_CODE  = 43962,
};

enum Push_const {
    PUSH_NUMB_CODE = 1,
    PUSH_REG_CODE = 2,
    PUSH_PLUS_CODE = 3,
    PUSH_BRACK_NUMB_CODE = 5,
    PUSH_BRACK_REG_CODE = 6,
    PUSH_BRACK_PLUS_CODE = 7,
};

enum Registers_const {
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
};

#endif