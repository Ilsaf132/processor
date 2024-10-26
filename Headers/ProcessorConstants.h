#ifndef ERRORS_
#define ERRORS_
#include <math.h>

const int LABELS_MAX_SIZE    = 10;
const int REG_LEN            = 6;
const int REG_SIZE           = 5;
const int NUMBERS_LEN        = 10;
const int RAM_MEMORY         = 121;
const int TABLE_RAM_LEN      = 11;
const int MAX_LEN_CODE_START = 3;
const int MAX_LEN_X_FORMAT   = 10;
const int HEADER_SIGN_LEN    = 5;
const int ACCURACY           = 2;
const int ACCURACY_TEN       = pow(10, ACCURACY);

typedef int elem_t;

enum Error_proc {
    LABEL_EMPTY             = -1,
    ERROR_OK                = 0,
    ERROR_MASSIVE           = 1,    
    ERROR_HLT               = 2,
    ERROR_ARRAY_CODE        = 3,
    ERROR_STRUCT            = 4,
    ERROR_LABELS            = 5,
    ERROR_WRONG_COMMAND     = 6,
    ERROR_ADDRESS_LABELS    = 7,
    ERROR_ADDRESS_TEXT      = 8,
    ERROR_ADDRESS_TEXT_LEN  = 9,
    ERROR_ADDRESS_PROCESSOR = 10,
    ERROR_ADDRESS_CODE      = 11,
    ERROR_ADDRESS_FILE      = 12,
    ERROR_GETCODE           = 13,
    ERROR_ADDRESS_PROC_CODE = 14,
    ERROR_TWICE_LABEL       = 15,
    ERROR_LABEL_NAME        = 16,
    ERROR_ARG_NAME          = 17,
    ERROR_BRACKETS          = 18,
};

#define DEF_CMD(name, num, ...) name##_CODE = num,
enum Code_const {
    #include "Commands.h"
};
#undef DEF_CMD

enum Push_const {
    PUSH_NUMB_CODE       = 1,
    PUSH_REG_CODE        = 2,
    PUSH_PLUS_CODE       = 3,
    PUSH_BRACK_NUMB_CODE = 5,
    PUSH_BRACK_REG_CODE  = 6,
    PUSH_BRACK_PLUS_CODE = 7,
};

enum Registers_const {
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
};

#endif