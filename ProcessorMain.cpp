#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "ErrorsProcessor.h"
#include "Stack.h"

Error_proc Create_massive(struct Processor_t* code);
int CharToInt(char* word);

typedef int elem_t;

struct Processor_t {
    elem_t* code_data;
    int size;
};

int main() {
    struct Processor_t code = {};
    if (!Create_massive(&code)) {
       for(int i = 0; i < code.size; ++i) {
        printf("%d ");
       }  
    } else {
        printf("ERROR IN MASSIVE!\n");
        assert(0);
    }

}

Error_proc Create_massive(struct Processor_t* code) {
    struct stat buf = {};
    stat("program_code.txt", &buf);
    long int code_len = buf.st_size;
    printf("code len: %ld\n", code_len);
    
    code -> code_data = (elem_t*) calloc(code_len, sizeof(elem_t));

    char* code_txt = (char*) calloc(code_len, sizeof(char));

    FILE* fp = fopen("program_code.txt", "rb");
    fread(code_txt, code_len, 1, fp);
    printf("%s\n", code_txt);
    if(fp) {
        printf("File has opened!\n");
        code -> size = 0;

        // fscanf закончить!
        while(code_command != NULL) {
            code->code_data[code -> size] = CharToInt(code_command);
            
            code -> size++;
            code_command = strtok(NULL, " \r\n");
        }
    } else {
        printf("File hasn`t opened!");
        return ERROR_MASSIVE;
    }
    fclose(fp);
    return ERROR_OK;
} 

int CharToInt(char* word) {
    int res_numb = 0;
    for(int i = 0; i < strlen(word); ++i) {
        res_numb = res_numb*10 + word[i] - '0';
    }
    return res_numb;
}