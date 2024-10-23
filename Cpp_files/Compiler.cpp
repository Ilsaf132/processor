#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "ErrorsProcessor.h"

struct Labels_t {
    int address;
    char* name;
};

struct Table_labels {
    struct Labels_t* array;
    int size;
    size_t max_len;
};

struct Header {
    const char* signature;
    const char* version;
    char size[SIZE_ASM];
    uint32_t reserved;
};

Error_proc InitAssembler(struct Table_labels* labels, char** array_text, long int* text_len, const char* file_read);
Error_proc Assembler(char* array_text, long int text_len, struct Table_labels* labels, int iteration, const char* file_read, const char* file_write);
Error_proc DumpLabels(struct Table_labels* labels);
Error_proc DtorAssembler(struct Table_labels* labels, char* array_text);
Error_proc FullingLabels(struct Table_labels* labels, int* cnt, char* word);
void CreateSignature(FILE* fp, long int size);
void JumpCode(Code_const jump, int* cnt, Table_labels* labels, FILE* fw);
int Register(char* word);
int FindLabel(char* word, struct Table_labels* labels);
void FprintfLabelAddress(FILE* fp, struct Table_labels* labels, char* word);
void GetLabel(char* name, char word[20]);
void GetCodePush(FILE* fp, int* cnt);
void PutWordsPush(FILE* fp, char* word, int* cnt, int code_plus, int code_reg, int code_numb);
void PushWithPlus(FILE* fp, char* word, int* cnt, char* plus);


int main() {

    char* array_text = {};
    long int text_len = 0;
    struct Table_labels labels = {};
    const char* file_read = "./Text_files/program.asm";
    const char* file_write = "./Text_files/program_code.txt";

    if(InitAssembler(&labels, &array_text, &text_len, file_read)) {
        return -1;
    }
    printf("text len: %ld\n", text_len);

    if(Assembler(array_text, text_len, &labels, 0, file_read, file_write)) {
        return -1;
    }
    DumpLabels(&labels);
    if(Assembler(array_text, text_len, &labels, 1, file_read, file_write)) {
        return -1;
    }

    if(DtorAssembler(&labels, array_text)) {
        return -1;
    }
}

Error_proc InitAssembler(struct Table_labels* labels, char** array_text, long int* text_len, const char* file_read) {
    if(labels == NULL) {
        printf("labels address is null!\n");
        return ERROR_ADDRESS_LABELS;
    } 
    if(text_len == NULL) {
        printf("text_len address is null!\n");
        return ERROR_ADDRESS_TEXT_LEN;
    }
    if(array_text == NULL) {
        printf("text address is null!\n");
        return ERROR_ADDRESS_TEXT;
    }  

    struct stat buf = {};
    stat(file_read,&buf);
    *text_len = buf.st_size;

    labels -> array = (struct Labels_t*) calloc(LABELS_SIZE_MAX, sizeof(struct Labels_t));
    *array_text = (char*) calloc(*text_len, sizeof(char));
    labels -> max_len = MAX_LEN_NAME;
    labels -> size = 0;

    printf("array_text: %p\n", *array_text);
    return ERROR_OK;
}

Error_proc DumpLabels(struct Table_labels* labels) {
    if(labels == NULL) {
        printf("labels address is null!\n");
        return ERROR_ADDRESS_LABELS;
    } 


    for(int i = 0; i < LABELS_SIZE_MAX*(MAX_LEN_NAME + 1) + 8; ++i) {
        printf("-");
    }
    printf("\n");

    printf("labels size: %d\n", labels -> size);

    printf("name:   ");
    for(int i = 0; i < LABELS_SIZE_MAX; ++i) {
        printf("%*s ", labels -> max_len, labels -> array[i].name);
    }
    printf("\n");

    printf("address:");
    for(int i = 0; i < LABELS_SIZE_MAX; ++i) {
        printf("%*x ", labels -> max_len, labels -> array[i].address);
    }
    printf("\n");

    for(int i = 0; i < LABELS_SIZE_MAX*(MAX_LEN_NAME + 1) + 8; ++i) {
        printf("-");
    }
    printf("\n");

    return ERROR_OK;
}

Error_proc DtorAssembler(struct Table_labels* labels, char* array_text) {
    if(labels == NULL) {
        printf("labels address is null!\n");
        return ERROR_ADDRESS_LABELS;
    } 
    if(array_text == NULL) {
        printf("text address is null!\n");
        return ERROR_ADDRESS_TEXT;
    } 

    free(labels);
    free(array_text);
    return ERROR_OK;
}

Error_proc Assembler(char* array_text, long int text_len, struct Table_labels* labels, int iteration, const char* file_read, const char* file_write) {

    if(labels == NULL) {
        printf("labels address is null!\n");
        return ERROR_ADDRESS_LABELS;
    } 
    if(array_text == NULL) {
        printf("text address is null!\n");
        return ERROR_ADDRESS_TEXT;
    } 

    FILE* fr = fopen(file_read, "rb");
    FILE* fw = fopen(file_write, "w");
    fread(array_text, text_len, 1, fr);

    CreateSignature(fw, text_len);

    if(fw && fr) {
        char* word = strtok(array_text, " \r\n");
        int cnt = 0;

        while(word != NULL) {
            if(strcmp(word, "push") == 0) {

                fprintf(fw, "%d ", PUSH_CODE);
                GetCodePush(fw, &cnt);

            } else if(strcmp(word, "add") == 0) {

                fprintf(fw, "%d\n", ADD_CODE);

            } else if(strcmp(word, "sub") == 0) {

                fprintf(fw, "%d\n", SUB_CODE);

            } else if(strcmp(word, "div") == 0) {

                fprintf(fw, "%d\n", DIV_CODE);

            } else if(strcmp(word, "mul") == 0) {

                fprintf(fw, "%d\n", MUL_CODE);

            } else if(strcmp(word, "out") == 0) {

                fprintf(fw, "%d\n", OUT_CODE);

            } else if(strcmp(word, "in") == 0) {

                fprintf(fw, "%d\n", IN_CODE);

            } else if(strcmp(word, "dump") == 0) {

                fprintf(fw, "%d\n", DUMP_CODE);

            } else if(strcmp(word, "sqrt") == 0) {

                fprintf(fw, "%d\n", SQRT_CODE);

            } else if(strcmp(word, "sin") == 0) {

                fprintf(fw, "%d\n", SIN_CODE);

            } else if(strcmp(word, "cos") == 0) {

                fprintf(fw, "%d\n", COS_CODE);

            } else if (strcmp(word, "pop") == 0) {

                fprintf(fw, "%d ", POP_CODE);
                GetCodePush(fw, &cnt);

            } else if (strcmp(word, "ja") == 0) {

                JumpCode(JA_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "jae") == 0) {

                JumpCode(JAE_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "jb") == 0) {

                JumpCode(JB_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "jbe") == 0) {

                JumpCode(JBE_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "je") == 0) {

                JumpCode(JE_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "jne") == 0) {

                JumpCode(JNE_CODE, &cnt, labels, fw);

            } else if (strcmp(word, "jmp") == 0) {

                JumpCode(JMP_CODE, &cnt, labels, fw);

            } else if(strcmp(word, "hlt") == 0) {
                fprintf(fw, "%d\n", HLT_CODE);

            } else if (strchr(word, ':')) {

                if (iteration == 0) {
                    if(FullingLabels(labels, &cnt, word)) {
                        return ERROR_LABELS;
                    }
                }

            } else {
                printf("ERROR COMMAND!\n");
                fclose(fw);
                fclose(fr);
                return ERROR_WRONG_COMMAND;
            }

            word = strtok(NULL, " \r\n");
            cnt++;
        }
    } else {
        printf("File hasn`t opened!\n");
    }

    printf("END assembler\n");
    fclose(fw);
    fclose(fr);
    return ERROR_OK;
}

Error_proc FullingLabels(struct Table_labels* labels, int* cnt, char* word) {
    if (labels -> max_len < strlen(word)) labels -> max_len = strlen(word);

    printf("cnt: %d\n", cnt);
    printf("label: %s\n", word);

    if (labels -> size >= LABELS_SIZE_MAX) {
        printf("Not enough labels ->  Realloc memory!\n");
        return ERROR_LABELS;
    }

    labels -> array[labels -> size].address = *cnt; 
    labels -> array[labels -> size].name = word;
    labels -> size++;

    *cnt--;
    return ERROR_OK;
}

void CreateSignature(FILE* fp, long int size) {
    struct Header hdr = {};
    hdr.signature = "ILSAF";
    hdr.version = "22.10.2024";
    sprintf(hdr.size, "%ld", size);
    fprintf(fp, "signature: %s\n", hdr.signature);
    fprintf(fp, "version:   %s\n", hdr.version);
    fprintf(fp, "size:      %s\n", hdr.size);
}

void GetCodePush(FILE* fp, int* cnt) {

    char* word = strtok(NULL, " \r\n");
    char* bracket_open = strchr(word, '[') ;
    char* bracket_close = strchr(word, ']');
    char between_brack[REG_LEN + NUMBERS_LEN + 1] = {};

    if (bracket_open && bracket_close) {
        memcpy(between_brack, word + 1, strlen(word) - 2);
        PutWordsPush(fp, between_brack, cnt, PUSH_BRACK_PLUS_CODE, PUSH_BRACK_REG_CODE, PUSH_BRACK_NUMB_CODE);
    } else {
        PutWordsPush(fp, word, cnt, PUSH_PLUS_CODE, PUSH_REG_CODE, PUSH_NUMB_CODE);
    }
}

void PutWordsPush(FILE* fp, char* word, int* cnt, int code_plus, int code_reg, int code_numb) {
    char* plus = strchr(word, '+');
    if (plus) {
        fprintf(fp, "%d ", code_plus);
        PushWithPlus(fp, word, cnt, plus);  
    } else {

        if(strchr(word, 'x')) {
            fprintf(fp, "%d %d\n", code_reg, Register(word));
        } else {
            fprintf(fp, "%d %d\n", code_numb, atoi(word));
        }
        *cnt += 2;

    }
}

void PushWithPlus(FILE* fp, char* word, int* cnt, char* plus) {
    char reg_push[REG_LEN] = {};
    char numb_push[NUMBERS_LEN] = {};
    memcpy(reg_push, word, plus - word);
    memcpy(numb_push, plus + 1, strlen(word) - strlen(reg_push) - 1);

    fprintf(fp, "%d ", atoi(numb_push));
    fprintf(fp, "%d\n", Register(reg_push));
    *cnt += 3;
}

void JumpCode(Code_const jump, int* cnt, Table_labels* labels, FILE* fw) {
    fprintf(fw, "%d ", jump);
    char* word = strtok(NULL, " \r\n");
    FprintfLabelAddress(fw, labels, word);
    cnt++;
}

void FprintfLabelAddress(FILE* fp, struct Table_labels* labels, char* word) {
    int temp_label = 0;

    temp_label = FindLabel(word, labels);
    if(temp_label == -1) {
        fprintf(fp, "%s\n", word);
    } else {
        fprintf(fp, "%d\n", temp_label);   
    }
}

int FindLabel(char* word, struct Table_labels* labels) {
    for(int i = 0; i < labels -> size; ++i) {
        char temp_word[20] = {};
        GetLabel(labels -> array[i].name, temp_word);
        printf("temp_word: %s\n", temp_word);
        if(!strcmp(word, temp_word)) {
            printf("VERNI: %s!\n", temp_word);
            return labels -> array[i].address;
        }
    }
    return LABEL_EMPTY;
}

void GetLabel(char* name, char word[20]) {
    int i = 0;
    while (name[i] != ':') {
        word[i] = name[i];
        ++i;
    }
    word[i] = ':';
}

int Register(char* word) {
    if (!strcmp(word, "ax")) {
        return AX;
    } else if(!strcmp(word, "bx")) {
        return BX;
    } else if(!strcmp(word, "cx")) {
        return CX;
    } else if(!strcmp(word, "dx")) {
        return DX;
    } else {
        printf("WRONG REGISTER!\n");
        assert(0);
    }
}