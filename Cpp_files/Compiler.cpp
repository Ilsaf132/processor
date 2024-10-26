#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include "ProcessorConstants.h"

#define DEF_CMD(name, num, arg, ...) if (mystricmp(word, #name) == 0) {\
                                            if(arg == 2) {\
                                                JumpCode(name##_CODE, &cnt, labels, fw);\
                                            } else {\
                                                fprintf(fw, "%d ", name##_CODE);\
                                                if(arg == 1) {\
                                                    if(GetCodePush(fw, &cnt)) {\
                                                        return ERROR_ARG_NAME;\
                                                    }\
                                                } else {\
                                                    fprintf(fw, "\n");\
                                                }\
                                            }\
                                    } else

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
    char sign[HEADER_SIGN_LEN];
    uint32_t version;
    uint32_t size;
};

Error_proc InitAssembler(struct Table_labels* labels, char** array_text, long int* text_len, const char* file_read);
Error_proc Assembler(char* array_text, long int text_len, struct Table_labels* labels, int iteration, const char* file_read, const char* file_write);
Error_proc DumpLabels(struct Table_labels* labels);
Error_proc DtorAssembler(struct Table_labels* labels, char* array_text);
Error_proc FullingLabels(struct Table_labels* labels, int* cnt, char* word);
Error_proc CheckLabel(char* word, char* colon);
Error_proc CheckNumName(char* word);
Error_proc CheckRegisterName(char* word);
void CreateSignature(FILE* fp, Header header);
void JumpCode(Code_const jump, int* cnt, Table_labels* labels, FILE* fw);
int Register(char* word);
int FindLabel(char* word, struct Table_labels* labels);
void FprintfLabelAddress(FILE* fp, struct Table_labels* labels, char* word);
void GetLabel(char* name, char word[20]);
int mystricmp(const char* word1, const char* word2);
int StrToDouble(char numb_push[NUMBERS_LEN]);
Error_proc GetCodePush(FILE* fp, int* cnt);
Error_proc PutWordsPush(FILE* fp, char* word, int* cnt, int code_plus, int code_reg, int code_numb);
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

    labels -> array = (struct Labels_t*) calloc(LABELS_MAX_SIZE, sizeof(struct Labels_t));
    *array_text = (char*) calloc(*text_len, sizeof(char));
    labels -> max_len = REG_LEN;
    labels -> size = 0;

    printf("array_text: %p\n", *array_text);
    return ERROR_OK;
}

Error_proc DumpLabels(struct Table_labels* labels) {
    if(labels == NULL) {
        printf("labels address is null!\n");
        return ERROR_ADDRESS_LABELS;
    } 


    for(int i = 0; i < LABELS_MAX_SIZE*(REG_LEN + 1) + 8; ++i) {
        printf("-");
    }
    printf("\n");

    printf("labels size: %d\n", labels -> size);

    printf("name:   ");
    for(int i = 0; i < LABELS_MAX_SIZE; ++i) {
        printf("%*s ", labels -> max_len, labels -> array[i].name);
    }
    printf("\n");

    printf("address:");
    for(int i = 0; i < LABELS_MAX_SIZE; ++i) {
        printf("%*x ", labels -> max_len, labels -> array[i].address);
    }
    printf("\n");

    for(int i = 0; i < LABELS_MAX_SIZE*(REG_LEN + 1) + 8; ++i) {
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

    Header header = {"IBB", 1, (uint32_t)text_len};
    CreateSignature(fw, header);  

    if(fw && fr) {
        char* word = strtok(array_text, " \r\n");
        int cnt = 0;

        while(word != NULL) {
            #include "Commands.h" 
            if (strchr(word, ':')) {

                if (iteration == 0) {
                    if(CheckLabel(word, strchr(word, ':'))) {
                        return ERROR_LABELS;
                    }
                    if(FullingLabels(labels, &cnt, word)) {
                        return ERROR_LABELS;
                    }
                }

            } else {
                printf("WRONG COMMAND!\n");
                fclose(fw);
                fclose(fr);
                return ERROR_WRONG_COMMAND;
            }
            word = strtok(NULL, " \r\n");
            printf("words: %s\n", word);
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
#undef DEF_CMD

int mystricmp(const char* word1, const char* word2) {

    while (*word1 && toupper(*word2) == toupper(*word1)){
        word1++;
        word2++;
    }
    return *word1 - *word2;

}

void CreateSignature(FILE* fp, Header header) { 
    fwrite(&header, sizeof(Header), 1, fp);
}

Error_proc GetCodePush(FILE* fp, int* cnt) {

    char* word = strtok(NULL, " \r\n");
    char* bracket_open = strchr(word, '[') ;
    char* bracket_close = strchr(word, ']');
    char between_brack[REG_LEN + NUMBERS_LEN + 1] = {};

    if (bracket_open && bracket_close) {
        if((bracket_open == word) && (bracket_close == word + strlen(word) - 1)) {
            memcpy(between_brack, word + 1, strlen(word) - 2);
            if(PutWordsPush(fp, between_brack, cnt, PUSH_BRACK_PLUS_CODE, PUSH_BRACK_REG_CODE, PUSH_BRACK_NUMB_CODE)) { 
                printf("ERROR IN ARGS OF FUNCTION!\n");
                return ERROR_ARG_NAME;
            }
        } else {
            printf("WRONG NAME OF BRACKETS!\n");
            return ERROR_BRACKETS;
        }
    } else {
        if(PutWordsPush(fp, word, cnt, PUSH_PLUS_CODE, PUSH_REG_CODE, PUSH_NUMB_CODE)) {
            printf("ERROR IN ARGS OF FUNCTION!\n");
            return ERROR_ARG_NAME;
        }
    }
    return ERROR_OK;
}

Error_proc PutWordsPush(FILE* fp, char* word, int* cnt, int code_plus, int code_reg, int code_numb) {
    char* plus = strchr(word, '+');
    if (plus) {
        char reg_name[REG_LEN] = {};
        char num_name[NUMBERS_LEN] = {};
        memcpy(reg_name, word, plus - word);
        memcpy(num_name, plus+1, strlen(word) - strlen(num_name) - 1);

        if(CheckRegisterName(reg_name) || CheckNumName(num_name)) {
            printf("WRONG ARGUMENT NAME WITH PLUS!\n");
            return ERROR_ARG_NAME;
        }
        fprintf(fp, "%d ", code_plus);
        PushWithPlus(fp, word, cnt, plus);  
    } else {
        if(CheckRegisterName(word) && CheckNumName(word)) {
            printf("WRONG ARGUMENT NAME!\n");
            return ERROR_ARG_NAME;
        }

        if(strchr(word, 'x')) {
            fprintf(fp, "%d %d\n", code_reg, Register(word));
        } else {
            fprintf(fp, "%d %d\n", code_numb, StrToDouble(word));
        }
        *cnt += 2;

    }
    return ERROR_OK;
}

Error_proc CheckNumName(char* word) {
    if(strlen(word) == 0 || word[0] == '.') {
        return ERROR_ARG_NAME;
    }

    for(size_t c = 0; c < strlen(word); ++c) {
        if(word[c] == '-' && c == 0) {
            continue;
        }
        if(!isdigit(word[c]) && word[c] != '.') {
            return ERROR_ARG_NAME;
        }
    }
    return ERROR_OK;
}

Error_proc CheckRegisterName(char* word) {
    if(!strcmp(word, "ax")) {
        return ERROR_OK;
    } else if(!strcmp(word, "bx")) {
        return ERROR_OK;
    } else if(!strcmp(word, "cx")) {
        return ERROR_OK;
    } else if(!strcmp(word, "dx")) {
        return ERROR_OK;
    } else {
        return ERROR_ARG_NAME;
    }
}

void PushWithPlus(FILE* fp, char* word, int* cnt, char* plus) {
    char reg_push[REG_LEN] = {};
    char numb_push[NUMBERS_LEN] = {};
    memcpy(reg_push, word, plus - word);
    memcpy(numb_push, plus + 1, strlen(word) - strlen(reg_push) - 1);

    fprintf(fp, "%d ", StrToDouble(numb_push));
    fprintf(fp, "%d\n", Register(reg_push));
    *cnt += 3;
}

int StrToDouble(char numb_push[NUMBERS_LEN]) {
    int res = 0;
    for(size_t c = 0; c < strlen(numb_push); ++c) {
        if(numb_push[c] != '.' && numb_push[c] != '-') {
            res = res*10 + (numb_push[c] - '0');
        }
    }
    if(numb_push[0] == '-') {
        return -res;
    }
    return res;
}

void JumpCode(Code_const jump, int* cnt, Table_labels* labels, FILE* fw) {
    fprintf(fw, "%d ", jump);
    char* word = strtok(NULL, " \r\n");
    FprintfLabelAddress(fw, labels, word);
    (*cnt)++;
}

Error_proc CheckLabel(char* word, char* colon) {
    if (word + strlen(word) - 1 == colon) {
        return ERROR_OK;
    } else {
        printf("Error in label name!\n");
        return ERROR_LABEL_NAME;
    }
}

Error_proc FullingLabels(struct Table_labels* labels, int* cnt, char* word) {
    if (labels -> max_len < strlen(word)) labels -> max_len = strlen(word);

    printf("cnt: %d\n", *cnt);
    printf("label: %s\n", word);

    if (labels -> size >= LABELS_MAX_SIZE) {
        printf("Not enough labels ->  Realloc memory!\n");
        return ERROR_LABELS;
    }

    if(FindLabel(word, labels) != -1) {
        printf("Entering label one more tome!\n");
        return ERROR_TWICE_LABEL;
    }

    labels -> array[labels -> size].address = *cnt; 
    labels -> array[labels -> size].name = word;
    labels -> size++;

    (*cnt)--;
    return ERROR_OK;
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