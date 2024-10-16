#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>

int main() {

    struct stat buf = {};
    stat("program.asm",&buf);
    long int text_len = buf.st_size;

    char* array_text = (char*) calloc(text_len, sizeof(char));
    printf("text len: %d\n", text_len);
    FILE* fr = fopen("program.asm", "rb");
    FILE* fw = fopen("program_code.txt", "w");

    if(fr && fw) {
        fread(array_text, text_len, 1, fr);

        char* word = strtok(array_text, " \r\n");
        printf("word length: %d\n", strlen(word));
        while(word != NULL) {

            if(strcmp(word, "push") == 0) {
                word = strtok(NULL, " \r\n");
                fputs("1 ", fw);
                fputs(word, fw);
                fputs("\n", fw);
            } else if(strcmp(word, "add") == 0) {
                fputs("2\n", fw);
            } else if(strcmp(word, "sub") == 0) {
                fputs("3\n", fw);
            } else if(strcmp(word, "div") == 0) {
                fputs("4\n", fw);
            } else if(strcmp(word, "mul") == 0) {
                fputs("5\n", fw);
            } else if(strcmp(word, "out") == 0) {
                fputs("6\n", fw);
            } else if(strcmp(word, "in") == 0) {
                fputs("7\n", fw);
            } else if(strcmp(word, "dump") == 0) {
                fputs("8\n", fw);
            } else if(strcmp(word, "sqrt") == 0) {
                fputs("9\n", fw);
            } else if(strcmp(word, "sin") == 0) {
                fputs("10\n", fw);
            } else if(strcmp(word, "cos") == 0) {
                fputs("11\n", fw);
            } else if(strcmp(word, "hlt") == 0) {
                fputs("0", fw);
            } else {
                printf("ERROR!\n");
                assert(0);
            }

            word = strtok(NULL, " \r\n");
        }
    }
    fclose(fr);
    fclose(fw);


}