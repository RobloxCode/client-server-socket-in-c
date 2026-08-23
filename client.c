#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int create_socket();
void connect_to_server(int *client_fd);

int main(void) {
    char client_msg[1024] = "hello from client";
    char server_buf[1024] = {0};

    int client_fd = create_socket();
    int server_fd = create_socket();

    connect_to_server(&client_fd);

    while (1) {
        if (send(client_fd, client_msg, sizeof client_msg - 1, 0) == -1) {
            fprintf(stderr, "Error sending message to client\n");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_recieved =
            recv(client_fd, server_buf, sizeof server_buf - 1, 0);

        if (bytes_recieved > 0) {
            printf("Recieved from client: %s\n", server_buf);
        } else if (bytes_recieved == 0) {
            printf("End of connection\n");
            break;
        } else {
            fprintf(stderr, "Error connecting to client\n");
            exit(EXIT_FAILURE);
        }
    }

    close(client_fd);

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

void connect_to_server(int *client_fd) {
    struct sockaddr_in server_addr = {.sin_family = AF_INET,
                                      .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
                                      .sin_port = htons(8080)};

    if (connect(*client_fd, (struct sockaddr *)&server_addr, sizeof server_addr)
        == -1) {
        fprintf(stderr, "Error sending message to client\n");
        exit(EXIT_FAILURE);
    }
}
