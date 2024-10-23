#ifndef ERROR_
#define ERROR_

enum Error_t{
    FOUND_OK = 0,
    ERROR_ADDRESS = 1,
    ERROR_DATA = 2,
    ERROR_OVERFLOW = 3,
    ERROR_SIZE = 4,
    ERROR_CANARY_STACK = 5,
    ERROR_CANARY_DATA = 6,
    ERROR_HASH_STACK = 7,
    ERROR_HASH_DATA = 8,
    ERROR_ELEM = 9
};

#endif