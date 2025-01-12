#ifndef fileHandler_h
#define fileHandler_h

#include <stdio.h>

#define HTML_BUFFER_SIZE 15000
#define MAX_LINE_LENGTH 2048

struct FileHandler {
    char* filename;

    char filetext[HTML_BUFFER_SIZE];
    char linetext[HTML_BUFFER_SIZE];

    void (*readFile)();
};

struct FileHandler fileHandler_Constructor(char* filename, void(*readFile)(struct FileHandler *fileHandler));

void readFile(struct FileHandler *fileHandler);

#endif // !FileHandler_h
#define fileHandler_h

