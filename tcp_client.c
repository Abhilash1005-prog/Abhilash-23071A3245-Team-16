
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");

    // Communication loop
    while (1) {
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(sock_fd, buffer, strlen(buffer));

        memset(buffer, 0, sizeof(buffer));
        int n = read(sock_fd, buffer, sizeof(buffer) - 1);
        if (n == 0) {
            printf("Server disconnected\n");
            break;
        } else if (n == -1) {
            perror("read");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}
