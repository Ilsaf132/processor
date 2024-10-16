#ifndef PUSH_AND_POP_
#define PUSH_AND_POP_

#include "Constants.h"
#include "Check.h"

struct Stack_t {
    canary_type left_canary = left_check_t;
    ON_DEBUG(const char* name;)
    ON_DEBUG(const char* file;)
    ON_DEBUG(int line;)
    stackElem_t* data;
    size_t size; 
    size_t capacity; 
    HashStruct hash_s; 
    canary_type right_canary = right_check_t;
};

Error_t StackCtor(struct Stack_t* stk ON_DEBUG(, const char* name, const char* file, int line));
Error_t StackPush(struct Stack_t* stk, stackElem_t elem);
Error_t StackPop(struct Stack_t* stk, stackElem_t* elem);
Error_t StackDtor(struct Stack_t* stk);

#endif