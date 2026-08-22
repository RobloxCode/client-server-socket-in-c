#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        fprintf(stderr, "error creating socket\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htonl(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        fprintf(stderr, "error creating socket\n");
        return EXIT_FAILURE;
    }

    printf("bound to 127.0.0.1:8080");

    return EXIT_SUCCESS;
}
