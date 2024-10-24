#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <TXLib.h>
#include <math.h>
#include "ErrorsProcessor.h"
#include "Stack.h"

#define DUMP_PRINTF(size, len, arr)     for(int i = 0; i < size; ++i) {\
                                            printf(" %0*X", len, arr);\
                                        }\
                                        printf("\n");

Error_proc GetCode(struct SPU* proc_t, const char* file_read);
Error_proc RunCode(struct SPU* proc_t);
Error_proc InitProcessor(struct SPU* proc_t, char** code_txt, FILE* fp, const char* file_read);
Error_proc DumpProcessor(struct SPU* proc_t);
Error_proc DtorProcessor(struct SPU* proc_t);
void VideoCard(struct SPU* proc_t);
int CharToInt(char* word);
int GetArgPush(struct SPU* proc_t);
int* GetArgPop(struct SPU* proc_t);

typedef int elem_t;

struct Header {
    const char* signature;
    const char* version;
    char* size;
    uint32_t reserved;
};

struct SPU {
    elem_t* code;
    elem_t* registers;
    struct Stack_t stk; 
    struct Stack_t stk_adr;
    elem_t* RAM;
    int ip;
    int size;
    size_t max_len_code;
};

int main() {
    struct SPU proc_t = {}; //инициализация
    const char* file_read = "./Text_files/program_code.txt";
    if (GetCode(&proc_t, file_read)) {
        printf("ERROR IN GETCODE FUNCTION!\n");
        return -1;
    }
    for(int i = 0; i < proc_t.size; ++i) {
        printf("%d ", proc_t.code[i]);
    }
    printf("\n");
    if(RunCode(&proc_t)) {
        return -1;
    }
    DtorProcessor(&proc_t);

}

Error_proc RunCode(struct SPU* proc_t) {
    if(proc_t == NULL) {
        return ERROR_STRUCT;
    }

    int work_programm = 1;
    while(work_programm) {
        if (proc_t -> size < proc_t -> ip) {
            printf("Code doesn`t have hlt!\n");
            return ERROR_HLT;
        }
        elem_t a = 0;
        elem_t b = 0;
        elem_t elem = 0;
        int elem_pop = 0;
        double elem_double = 0;
        int* addr = 0;
        switch(proc_t -> code[proc_t -> ip]) {
            case PUSH_CODE: StackPush(&proc_t -> stk, GetArgPush(proc_t));
                            break;

            case ADD_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            StackPush(&proc_t -> stk, a + b); 
                            proc_t -> ip += 1;
                            break;

            case SUB_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            StackPush(&proc_t -> stk, b - a);
                            proc_t -> ip += 1;
                            break;
            
            case DIV_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            StackPush(&proc_t -> stk, (b*accuracy_ten)/a);
                            proc_t -> ip += 1;
                            break;

            case MUL_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            StackPush(&proc_t -> stk, (a*b)/accuracy_ten);
                            proc_t -> ip += 1;
                            break;
            
            case OUT_CODE:  StackPop(&proc_t -> stk, &a);
                            printf("%.*lf\n", accuracy, double(a)/accuracy_ten);
                            proc_t -> ip += 1;
                            break;
            
            case IN_CODE:   scanf("%.*lf", accuracy, &elem_double);
                            StackPush(&proc_t -> stk, (int)(elem_double*accuracy_ten));
                            proc_t -> ip += 1;
                            break;

            case DUMP_CODE: DumpProcessor(proc_t);
                            proc_t -> ip += 1;
                            break;
            
            case SQRT_CODE: StackPop(&proc_t -> stk, &elem);
                            elem_double = sqrt(double(elem) / accuracy_ten);
                            StackPush(&proc_t -> stk, (int)(elem_double*accuracy_ten));
                            proc_t -> ip += 1;
                            break; 
            
            case SIN_CODE:  StackPop(&proc_t -> stk, &elem);
                            elem_double = sin(double(elem) / accuracy_ten);
                            StackPush(&proc_t -> stk, (int)(elem_double*accuracy_ten));
                            proc_t -> ip += 1;
                            break; 

            case COS_CODE:  StackPop(&proc_t -> stk, &elem);
                            elem_double = cos(double(elem) / accuracy_ten);
                            StackPush(&proc_t -> stk, (int)(elem_double*accuracy_ten));
                            proc_t -> ip += 1;
                            break; 
            
            case POP_CODE:  StackPop(&proc_t -> stk, &elem_pop);
                            addr = GetArgPop(proc_t);
                            *addr = elem_pop;
                            break;

            case JA_CODE:   StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            if (b > a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            break;
            
            case JAE_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            if (b >= a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            break;
            
            case JB_CODE:   StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            printf("a: %d,  b: %d\n", a,b);
                            if (b < a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            printf("ip: %d\n", proc_t -> ip);
                            printf("jb to: %d\n",  proc_t -> code[proc_t -> ip + 1]);
                            break;
            
            case JBE_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            if (b <= a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            break;
            
            case JE_CODE:   StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            if (b == a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            break;

            case JNE_CODE:  StackPop(&proc_t -> stk, &a);
                            StackPop(&proc_t -> stk, &b);
                            if (b != a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            else proc_t -> ip += 2;
                            break;
                        
            case JMP_CODE:  proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            printf("jmp to: %d\n",  proc_t -> code[proc_t -> ip + 1]);
                            break;

            case CALL_CODE: StackPush(&proc_t -> stk_adr, proc_t -> ip + 2);
                            proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
                            break;

            case RET_CODE:  StackPop(&proc_t -> stk_adr, &proc_t -> ip);
                            break;
            
            case HLT_CODE:  printf("Program`s end\n");
                            work_programm = 0;
                            break;

            default:        printf("PIZDEC V KODE! NAHUI NAPISAL?\n");
                            return ERROR_ARRAY_CODE;
        }
    }
    return ERROR_OK;
}

void VideoCard(struct SPU* proc_t) {
    for(int i = 0; i < RAM_SQRT; ++i) {
        for(int j = 0; j < RAM_SQRT; ++j) {
            if(proc_t -> RAM[i*RAM_SQRT + j] == 0) {
                printf(".");
            } else {
                printf("*");
            }
        }
        printf("\n           ");
    }
    printf("\n");
}

int GetArgPush(struct SPU* proc_t) {
    int* code = proc_t -> code;
    int* temp_ip = &(proc_t -> ip);
    int* regs = proc_t -> registers;
    (*temp_ip)++;
    int arg_type = code[(*temp_ip)++];
    int result = 0;

    if(arg_type & 1) {
        result += code[(*temp_ip)++];
    }
    if(arg_type & 2) {
        result += regs[code[(*temp_ip)++]];
    }
    if(arg_type & 4) {
        result = proc_t -> RAM[result];
    }
    return result;
}

int* GetArgPop(struct SPU* proc_t) {
    int* code = proc_t -> code;
    int* temp_ip = &(proc_t -> ip);
    int* regs = proc_t -> registers;
    (*temp_ip)++;
    int arg_type = code[(*temp_ip)++];
    int result = 0;

    if(arg_type & 4) {
        if(arg_type & 1) {
            result = code[(*temp_ip)++];
        } 
        if(arg_type & 2) {
            result += regs[code[(*temp_ip)++]];
        }
        return proc_t -> RAM + result;
    } else if(arg_type & 2) {
        return regs + code[(*temp_ip)++]; 
    } 
    return 0;
}

Error_proc InitProcessor(struct SPU* proc_t, char** code_txt, FILE* fp, const char* file_read) {
    if(proc_t == NULL) {
        printf("ERROR IN ADDRESS OF PROCESSOR!\n");
        return ERROR_ADDRESS_PROCESSOR;
    }
    if(code_txt == NULL) {
        printf("ERROR IN ADDRESS OF CODE ARRAY!\n");
        return ERROR_ADDRESS_CODE;
    }
    if(fp == NULL) {
        printf("ERROR IN ADDRESS OF FILE!\n");
        return ERROR_ADDRESS_FILE;
    }

    struct stat buf = {};
    stat(file_read, &buf);
    long int code_len = buf.st_size;
    printf("code len: %ld\n", code_len);
    
    StackCtor(&proc_t -> stk);
    StackCtor(&proc_t -> stk_adr);
    proc_t -> code = (elem_t*) calloc(code_len, sizeof(elem_t));
    *code_txt = (char*) calloc(code_len, sizeof(char));
    proc_t -> ip = 0;
    proc_t -> size = 0;
    proc_t -> RAM = (elem_t*) calloc(RAM_MEMORY, sizeof(elem_t));
    proc_t -> max_len_code = MAX_LEN_CODE_START;
    proc_t -> registers = (elem_t*) calloc(REG_SIZE, sizeof(elem_t));

    fread(*code_txt, code_len, 1, fp);

    struct Header hdr = {};

    char* code_command = strtok(*code_txt, " \r\n");
    hdr.signature = strtok(NULL, " \r\n");

    code_command = strtok(NULL, " \r\n");
    hdr.version = strtok(NULL, " \r\n");

    code_command = strtok(NULL, " \r\n");
    hdr.size = strtok(NULL, " \r\n");

    return ERROR_OK;
}

Error_proc GetCode(struct SPU* proc_t, const char* file_read) {
    if(proc_t == NULL) {
        return ERROR_STRUCT;
    }
    char* code_txt = 0;
    FILE* fp = fopen(file_read, "rb");
    if (InitProcessor(proc_t, &code_txt, fp, file_read)) {
        return ERROR_GETCODE;
    }

    if(fp) {
        printf("File has opened!\n");

        char* code_command = strtok(NULL, " \r\n");
        
        while(code_command != NULL) {
            int code_int = CharToInt(code_command);
            char code_x_format[MAX_LEN_X_FORMAT] = {};
            sprintf(code_x_format, "%X", code_int);

            if (proc_t -> max_len_code < strlen(code_x_format)) {
                proc_t -> max_len_code = strlen(code_x_format);
            }
            proc_t -> code[proc_t -> size] = code_int;
            proc_t -> size++;
            code_command = strtok(NULL, " \r\n");
        }

    } else {
        printf("File hasn`t opened!");
        return ERROR_MASSIVE;
    }
    fclose(fp);
    free(code_txt);
    return ERROR_OK;
} 

Error_proc DumpProcessor(struct SPU* proc_t) {

    printf("----------");
    for(size_t i = 0; i < (proc_t -> max_len_code+1)*(proc_t -> size); ++i) {
        printf("-");
    }
    printf("\n");

    printf("code:     ");
    DUMP_PRINTF(proc_t -> size, proc_t -> max_len_code, i)
    printf("          ");
    DUMP_PRINTF(proc_t -> size, proc_t -> max_len_code, proc_t -> code[i])

    printf("          ");
    for(size_t i = 0; i < proc_t -> ip*(proc_t -> max_len_code + 1); ++i) {
        printf(" ");
    }
    char arrow = '^';
    printf("%*c\n", proc_t -> max_len_code + 1, arrow);

    printf("registers:");
    DUMP_PRINTF(REG_SIZE, proc_t -> max_len_code, i)
    printf("          ");
    DUMP_PRINTF(REG_SIZE, proc_t -> max_len_code, proc_t -> registers[i])

    printf("stack:    ");
    for(size_t i = 0; i < proc_t -> stk.size; ++i) {
        printf(" %0*X", proc_t -> max_len_code, i);
    }
    printf("\n          ");
    for(size_t i = 0; i < proc_t -> stk.size; ++i) {
        printf(" %0*d", proc_t -> max_len_code, proc_t -> stk.data[i+1]);
    }
    printf("\n");
    printf("VideoCard: ");
    VideoCard(proc_t);

    printf("----------");
    for(size_t i = 0; i < (proc_t -> max_len_code+1)*(proc_t -> size); ++i) {
        printf("-");
    }
    printf("\n");


    return ERROR_OK;
}

Error_proc DtorProcessor(struct SPU* proc_t) {
    if(proc_t == NULL) {
        printf("ERROR IN ADDRESS OF PROCESSOR!\n");
        return ERROR_ADDRESS_PROCESSOR;
    }
    if(proc_t -> code == NULL) {
        printf("ERROR IN ADDRESS OF PROCESSOR`S ARRAY!\n");
        return ERROR_ADDRESS_PROC_CODE;
    }

    free(proc_t -> code);
    free(proc_t -> RAM);
    free(proc_t -> registers);
    StackDtor(&proc_t -> stk);
    printf("konec!");

    return ERROR_OK;
}

int CharToInt(char* word) {
    int res_numb = 0;
    if (word[0] == '-') {
        return -1;
    }
    for(size_t i = 0; i < strlen(word); ++i) {
        res_numb = res_numb*10 + word[i] - '0';
    }
    return res_numb;
}