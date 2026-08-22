#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int create_socket();
void bind_socket(int *server_fd);
void listen_socket(int *server_fd, int n);

int main(void) {
    int server_fd = create_socket();

    bind_socket(&server_fd);

    listen_socket(&server_fd, 10);

    return EXIT_SUCCESS;
}

int create_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        fprintf(stderr, "error creating socket\n");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void bind_socket(int *server_fd) {
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htonl(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(*server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        fprintf(stderr, "error binding socket\n");
        exit(EXIT_FAILURE);
    }
}

void listen_socket(int *server_fd, int n) {
    if (listen(*server_fd, n) == -1) {
        fprintf(stderr, "error binding socket\n");
        exit(EXIT_FAILURE);
    }
}
