#include "server.h"

void main() {
    struct Server server = server_Constructor(AF_INET, 800, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
    server.launch(&server);
}
