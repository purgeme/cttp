#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include<unistd.h>
#include<string.h>
#include "server.h"
#include "filehandler.h"

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, u_long interface, void(*launch)(struct Server *server)) {
    struct Server server;

    server.domain = domain;
    server.port = port;
    server.service = service;
    server.protocol = protocol;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    if(server.socket < 0) {
        perror("Failed to initialize/connect to socket...\n");
        exit(EXIT_FAILURE);
    }

    if(bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(+EXIT_FAILURE);
    }

    server.launch = launch;
    return server;

}

void launch(struct Server *server) {

    char buffer[BUFFER_SIZE];
    while(1) {
        printf(" === WAITING FOR CONNECTION === \n");
        int addlen = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addlen);
        ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);

        if(bytesRead >= 0) {
            buffer[bytesRead] = '\0';
            puts(buffer);
        } else {
            perror("Error reading buffer...\n");
        }
        
        // Get filename from request
        char filename[2048];
        int init_pos=4;
        int size=0;
        if(buffer[init_pos+1]!= ' ') {
            while(buffer[init_pos+size]!=' '){
                size++;
            }
            memcpy(filename, buffer+init_pos+1, size-1);
            memset(filename+size-1, '\0', sizeof(char));
        } else {
            memcpy(filename, DEFAULT_FILENAME, sizeof(DEFAULT_FILENAME));
            memset(filename+strlen(DEFAULT_FILENAME), '\0', sizeof(char));
        }
        // Get datatype from filename, uses first dot as datatype starter.
        char datatype[2048];
        size=0;
        while(filename[size] != '.'){
            size++;
        }
        memcpy(datatype, filename+size+1, strlen(filename)-size); 
        memset(datatype+size, '\0', sizeof(char));

        struct FileHandler filehandler = fileHandler_Constructor(filename, readFile);
        filehandler.readFile(&filehandler);

        // Get file text from filename

        char response[BUFFER_SIZE];
        memset(response, '\0', sizeof(response));

        // Add header
        char header[2048];
        sprintf(header, "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/%s; charset=UTF-8\r\n\r\n", datatype);
        memcpy(response, header, strlen(header));
        
        // Add body
        memcpy(response+strlen(header), filehandler.filetext, strlen(filehandler.filetext));

        puts(response);
    
        // Send response
        write(new_socket, response, strlen(response));
        close(new_socket);
    }
}
