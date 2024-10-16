#include "FuncStack.h"
#include "Check.h"
#include "Constants.h"
#include <string.h>
#include <stdlib.h>

Error_t StackPush(struct Stack_t* stk, stackElem_t elem) {
    StackAssert(stk);

    if (stk -> size == stk -> capacity) {
        StackCapacityRealloc(stk, stk -> capacity * 2);
        memset(stk -> data + sizeof(stackElem_t)*((stk -> capacity)/2 + 1), StackNull_t, sizeof(stackElem_t)*(stk -> capacity)/2);
        stk -> data[stk -> capacity + 1] = canary_data_t;

        printf("New capacity: %d\n", stk -> capacity);
    }

    stk -> data[stk -> size + 1] = elem;
    stk -> size++;
    stk -> hash_s.hash_data = Hash((char*) stk -> data, sizeof(stackElem_t)*(stk -> capacity + 2));
    stk -> hash_s.hash_stack = Hash((char*) stk + first_bite_stk, stk_bites);

    printf("New ");
    printf_int(stk -> size, elem);

    StackAssert(stk);
    return FOUND_OK;
}

Error_t StackPop(struct Stack_t* stk, stackElem_t* elem) {
    if (elem == NULL) {
        printf("ERROR IN ADDRESS OF ELEM!\n");
        return ERROR_ELEM;
    }
    StackAssert(stk);

    if (stk -> size == 0) {
        printf("Trying to pop an element without capacity!\n");
        StackDump(stk);
        return ERROR_SIZE;
    } else if ((stk -> size == stk -> capacity / 4) && (stk -> capacity >= start_t*4)) {
        StackCapacityRealloc(stk, stk -> capacity / 4);
        stk -> data[stk -> capacity + 1] = canary_data_t;

        printf("New capacity: %d\n", stk -> capacity);
    }

    --stk -> size;
    printf("size: %d\n", stk -> size);

    *elem = stk -> data[stk -> size + 1];
    stk -> data[stk -> size + 1] = StackNull_t;
    printf("Deleted ");
    printf_int(stk -> size + 1, *elem);

    stk -> hash_s.hash_data = Hash((char*) stk -> data, sizeof(stackElem_t)*(stk -> capacity + 2));
    stk -> hash_s.hash_stack = Hash((char*) stk + first_bite_stk, stk_bites);

    StackAssert(stk);
    return FOUND_OK;
}

Error_t StackCtor(struct Stack_t* stk ON_DEBUG(, const char* name, const char* file, int line)) {
    if (stk == NULL) {
        printf("ERROR IN ADDRESS OF STACK!\n");
        return ERROR_ADDRESS;
    }

    stk -> data = (stackElem_t*)calloc(start_t + 2, sizeof(stackElem_t));
    if (stk -> data == NULL) {
        printf("ERROR IN ADDRESS OF DATA!\n");
        return ERROR_DATA;
    }
    stk -> data[0] = canary_data_t;
    stk -> data[start_t + 1] = canary_data_t;
    stk -> size = 0;
    stk -> capacity = start_t;

    #ifdef DEBUG
        stk -> name = name;
        stk -> file = file;
        stk -> line = line;
    #endif
    stk -> hash_s.hash_stack = Hash((char *) stk + first_bite_stk, stk_bites);
    stk -> hash_s.hash_data = Hash((char *) stk -> data, sizeof(stackElem_t)*(start_t+2));


    StackAssert(stk);
    return FOUND_OK;
}

Error_t StackDtor(struct Stack_t* stk) {
    if (stk == NULL) {
        printf("ERROR IN ADDRESS OF STACK!\n");
        return ERROR_ADDRESS;
    }
    StackAssert(stk);

    free(stk -> data);

    return FOUND_OK;
}
