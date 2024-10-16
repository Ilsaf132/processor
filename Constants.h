#ifndef CONSTANTS_
#define CONSTANTS_

#include <stdio.h>
#include "Check.h"
#define printf_int(x,y) printf("element %d: %d\n", x, y);

typedef int stackElem_t;
typedef long long int canary_type;
const size_t start_t = 10;
const size_t huge_const_t = 100000000;
const int StackNull_t = -666;
const canary_type left_check_t = 0xDEDEDED;
const canary_type right_check_t = 0xDEDEDED;
const int canary_data_t = 10; // what type?
const size_t first_bite_stk = sizeof(canary_type) ON_DEBUG(+ sizeof(const char*)*2 + sizeof(int));
const size_t stk_bites = sizeof(stackElem_t*) + sizeof(size_t)*2;

#endif