#include "filehandler.h"
#include <string.h>

struct FileHandler fileHandler_Constructor(char* filename, void(*readFile)(struct FileHandler *fileHandler)) {
    struct FileHandler filehandler;
    filehandler.filename = filename;

    filehandler.readFile = readFile;

    return filehandler;
}

void readFile(struct FileHandler *filehandler) {

    printf("Opening file: %s\n", filehandler->filename);
    FILE *file = fopen(filehandler->filename, "r");
    char* filetext = filehandler->filetext;
    char* linetext = filehandler->linetext;
    if(file!=NULL){
        int total_size=0;
        while(fgets(linetext, 2048, file) != NULL) {
            memcpy(filetext+strlen(filetext), linetext, strlen(linetext));
            total_size += strlen(linetext);
        }
        memset(filetext+total_size, '\0', sizeof(char));
        memset(&filetext+total_size, '\0', sizeof(char));
        fclose(file);
    } else {
        sprintf(filetext, "<!DOCTYPE html><html><head><title>404 - not found</title></head><body>404 - Not found</body></html>");
    }
}

