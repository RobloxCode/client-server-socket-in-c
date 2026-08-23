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
void bind_socket(int server_fd);
void listen_socket(int server_fd, int n);
int accept_client(int server_fd);
void start_server(void);

int main(void) {
    start_server();
    return EXIT_SUCCESS;
}

int create_socket(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        HANDLE_ERR("Couldn't create socket!");
    }

    return server_fd;
}

void bind_socket(int server_fd) {
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        HANDLE_ERR("Couldn't bind socket!");
    }
}

void listen_socket(int server_fd, int n) {
    if (listen(server_fd, n) == -1) {
        HANDLE_ERR("Couldn't listening to client");
    }
}

int accept_client(int server_fd) {
    int client_fd = accept(server_fd, NULL, NULL);

    if (client_fd == -1) {
        HANDLE_ERR("Couldn't accept client!");
    }

    return client_fd;
}

void start_server(void) {
    char client_buf[BUFCAP] = {0};
    char server_buf[BUFCAP] = "hello from server";

    int server_fd = create_socket();

    bind_socket(server_fd);
    listen_socket(server_fd, 10);

    printf("waiting for client...\n");

    int client_fd = accept_client(server_fd);

    printf("client connected\n");

    while (1) {
        ssize_t bytes_recieved =
            recv(client_fd, client_buf, sizeof client_buf - 1, 0);

        if (bytes_recieved > 0) {
            client_buf[bytes_recieved] = '\0';
            printf("Recieved from client: %s\n", client_buf);
        } else if (bytes_recieved == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            printf("recv stopped\n");
            break;
        }

        if (send(client_fd, server_buf, strlen(server_buf), 0) == -1) {
            perror("Error sending message to client");
            break;
        }

        printf("Sent to client: %s\n", server_buf);
    }

    close(server_fd);
    close(client_fd);
}
