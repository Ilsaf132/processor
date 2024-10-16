#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "Check.h"
#include "Constants.h"
#include "Error.h"
#include "FuncStack.h"


void StackAssertFunc(struct Stack_t* stk) {
    if(StackError(stk)) {
        StackDump(stk);
        assert(0);
    }
}

Error_t StackError(struct Stack_t* stk) {
    if(stk == NULL) { 
        return ERROR_ADDRESS;
    }
    if (stk -> size > stk -> capacity) {
        return ERROR_OVERFLOW;
    }
    if (stk -> size > huge_const_t) {
        return ERROR_SIZE;
    }
    if (stk -> left_canary != left_check_t || stk -> right_canary != right_check_t) {
        return ERROR_CANARY_STACK;
    }
    if (stk -> data[0] != canary_data_t || stk -> data[stk -> capacity + 1] != canary_data_t) {
        return ERROR_CANARY_DATA;
    }
    if (Hash((char*) stk + first_bite_stk, stk_bites) != stk -> hash_s.hash_stack) {
        return ERROR_HASH_STACK;
    }
    return FOUND_OK;
}

void StackDump(struct Stack_t* stk) {
    printf("\n");

    if (stk != NULL) {
        printf("address: %p\n", stk);

        printf("size: %d\n", stk -> size);
        printf("capacity: %d\n", stk -> capacity);

        if (stk -> data != NULL) {
            printf("data address: %p\n", stk -> data);

            if (stk -> size < huge_const_t && stk -> size <= stk -> capacity) {
                printf("elements of stack:\n");
                for(size_t i = 0; i < stk -> size; ++i) {
                    printf_int(i + 1, stk -> data[i]);
                }
            }
            printf("canary left: %llx\n", stk -> left_canary);
            printf("canary right: %llx\n", stk -> right_canary);
            printf("hash of stack: %lu\n", stk -> hash_s.hash_stack);
            printf("hash of data: %lu\n", stk -> hash_s.hash_data);

        } else {
            printf("error in data address!\n");
        }

    } else {
        printf("error in address of stack!\n");
    }

    #ifdef DEBUG
        printf("Error in stack %s in file %s at line %d\n", stk -> name, stk -> file, stk -> line);
    #endif

    printf("\n");
}

Error_t StackCapacityRealloc(struct Stack_t* stk, size_t new_capacity) {
    if (stk == NULL) {
        printf("ERROR IN ADDRESS OF STACK!\n");
        return ERROR_ADDRESS;
    }
    stk -> capacity = new_capacity;
    stk->data = (stackElem_t*) realloc(stk -> data, (stk -> capacity + 2) * sizeof(stackElem_t));

    return FOUND_OK;
}

unsigned long Hash(char* data, size_t size)
{
    if (data == NULL) {
        assert(0);
    }
    unsigned long hash = 5381;
    int c = 0;
    char* str = data;
    size_t i = 0;

    while (i++ < size) {
        c = *str++;
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 + c */
    }

    return hash;
}