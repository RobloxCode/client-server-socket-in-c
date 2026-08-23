#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define HANDLE_ERR(msg)                                                        \
    do {                                                                       \
        perror("send");                                                        \
        exit(EXIT_FAILURE);                                                    \
    } while (0)

#define BUFCAP 1024
#define PORT   8080

int create_socket(void);
void connect_to_server(int client_fd);
void start_client(void);

int main(void) {
    start_client();
    return EXIT_SUCCESS;
}

int create_socket(void) {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {
        HANDLE_ERR("Couldn't create socket!");
    }

    return client_fd;
}

void connect_to_server(int client_fd) {
    struct sockaddr_in server_addr = {.sin_family = AF_INET,
                                      .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
                                      .sin_port = htons(PORT)};

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof server_addr)
        == -1) {
        HANDLE_ERR("Couldn't connect to server!");
    }
}

void start_client(void) {
    char client_buf[] = "hello from client";
    char server_buf[BUFCAP] = {0};

    int client_fd = create_socket();

    connect_to_server(client_fd);

    printf("Connected to server!\n");

    while (1) {
        if (send(client_fd, client_buf, strlen(client_buf), 0) == -1) {
            HANDLE_ERR("Couldn't send message to server");
        }

        printf("Sent to server: %s\n", client_buf);

        ssize_t bytes_received =
            recv(client_fd, server_buf, sizeof server_buf - 1, 0);

        if (bytes_received > 0) {
            server_buf[bytes_received] = '\0';
            printf("Received from server: %s\n", server_buf);
        } else if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }

    close(client_fd);
}
