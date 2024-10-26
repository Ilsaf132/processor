#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <TXLib.h>
#include <math.h>
#include <ProcessorConstants.h>
#include "Stack.h"

#define DUMP_PRINTF(size, len, arr)     for(int i = 0; i < size; ++i) {\
                                            printf(" %0*X", len, arr);\
                                        }\
                                        printf("\n");

#define DEF_CMD(name, num, arg, code)   case name##_CODE: code;

struct Header {
    char sign[HEADER_SIGN_LEN];
    uint32_t version;
    uint32_t size;
};

struct SPU {
    elem_t* code;
    elem_t* registers;
    struct Stack_t stk; 
    struct Stack_t stk_adr;
    elem_t* RAM;
    int ip;
    int size_code;
    size_t max_len_code;
};

Error_proc GetCode(struct SPU* proc_t, const char* file_read);
Error_proc RunCode(struct SPU* proc_t);
Error_proc InitProcessor(struct SPU* proc_t, char** code_txt, FILE* fp, const char* file_read);
Error_proc DumpProcessor(struct SPU* proc_t);
Error_proc DtorProcessor(struct SPU* proc_t);
Header TakeHeader(FILE* fp);
void VideoCard(struct SPU* proc_t);
int CharToInt(char* word);
int GetArgPush(struct SPU* proc_t);
int* GetArgPop(struct SPU* proc_t);

int main() {
    struct SPU proc_t = {}; //инициализация
    const char* file_read = "./Text_files/program_code.txt";
    if (GetCode(&proc_t, file_read)) {
        printf("ERROR IN GETCODE FUNCTION!\n");
        return -1;
    }
    for(int i = 0; i < proc_t.size_code; ++i) {
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
        if (proc_t -> size_code < proc_t -> ip) {
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
            #include "Commands.h"

            default:        printf("PIZDEC V KODE! NAHUI NAPISAL?\n");
                            return ERROR_ARRAY_CODE;
        }
    }
    return ERROR_OK;
}
#undef DEF_CMD

void VideoCard(struct SPU* proc_t) {
    for(int i = 0; i < TABLE_RAM_LEN; ++i) {
        for(int j = 0; j < TABLE_RAM_LEN; ++j) {
            if((j-5)*(j-5) + (i-5)*(i-5) <= 25) {
                printf("*");
            } else {
                printf(".");
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

    Header header = TakeHeader(fp);

    struct stat buf = {};
    stat(file_read, &buf);
    long int code_len = buf.st_size;
    
    StackCtor(&proc_t -> stk);
    StackCtor(&proc_t -> stk_adr);
    proc_t -> code = (elem_t*) calloc(code_len, sizeof(elem_t));
    *code_txt = (char*) calloc(code_len, sizeof(char));
    proc_t -> ip = 0;
    proc_t -> size_code = 0;
    proc_t -> RAM = (elem_t*) calloc(RAM_MEMORY, sizeof(elem_t));
    proc_t -> max_len_code = MAX_LEN_CODE_START;
    proc_t -> registers = (elem_t*) calloc(REG_SIZE, sizeof(elem_t));

    fread(*code_txt, code_len - sizeof(Header), 1, fp);
    return ERROR_OK;
}

Header TakeHeader(FILE* fp) {
    Header header = {};
    fread(&header, sizeof(Header), 1, fp);

    return header;
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

        char* code_command = strtok(code_txt, " \r\n");
        
        while(code_command != NULL) {
            int code_int = CharToInt(code_command);
            char code_x_format[MAX_LEN_X_FORMAT] = {};
            sprintf(code_x_format, "%X", code_int);

            if (proc_t -> max_len_code < strlen(code_x_format)) {
                proc_t -> max_len_code = strlen(code_x_format);
            }

            proc_t -> code[proc_t -> size_code] = code_int;
            proc_t -> size_code++;
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
    for(size_t i = 0; i < (proc_t -> max_len_code+1)*(proc_t -> size_code); ++i) {
        printf("-");
    }
    printf("\n");

    printf("code:     ");
    DUMP_PRINTF(proc_t -> size_code, proc_t -> max_len_code, i)
    printf("          ");
    DUMP_PRINTF(proc_t -> size_code, proc_t -> max_len_code, proc_t -> code[i])

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
    for(size_t i = 0; i < (proc_t -> max_len_code+1)*(proc_t -> size_code); ++i) {
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
        for(size_t i = 1; i < strlen(word); ++i) {
            res_numb = res_numb*10 + word[i] - '0';
        }
        return -res_numb;
    }
    for(size_t i = 0; i < strlen(word); ++i) {
        res_numb = res_numb*10 + word[i] - '0';
    }
    return res_numb;
}