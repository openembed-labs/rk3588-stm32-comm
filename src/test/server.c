#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PADDING 10
#define MAX_LINE 256

#define DEFAULT_ADDRESS "0.0.0.0" // Default to bind all network interfaces
#define DEFAULT_PORT 5000         // Default port number

int main(int argc, char *argv[])
{
    int s_fd, c_fd;
    char buf[MAX_LINE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;          // Use socklen_t instead of int
    ssize_t bytes_received; // Use ssize_t to receive return values

    // Use default values
    const char *address = (argc > 1) ? argv[1] : DEFAULT_ADDRESS;
    int port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    printf("Server address: %s:%d\n", address, port);

    int status = bind(s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (status == -1)
    {
        perror("Bind error");
        close(s_fd);
        exit(1);
    }

    if (listen(s_fd, MAX_PADDING) == -1)
    {
        perror("Listen error");
        close(s_fd);
        exit(1);
    }

    printf("Server waiting for connection on port: %d...\n", port);

    while (1)
    {
        len = sizeof(client_addr);

        c_fd = accept(s_fd, (struct sockaddr *)&client_addr, &len);
        if (c_fd == -1)
        {
            perror("Accept error");
            close(s_fd);
            exit(1);
        }

        printf("Connection established with client address: %s, port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while (1)
        {
            bytes_received = recv(c_fd, buf, MAX_LINE, 0);
            if (bytes_received > 0)
            {
                buf[bytes_received] = '\0';
                printf("Received message: %s", buf);

                if (send(c_fd, buf, bytes_received, 0) == -1)
                {
                    perror("Send error");
                    close(c_fd);
                    close(s_fd);
                    exit(1);
                }
            }
            else if (bytes_received == -1)
            {
                perror("Receive error");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(s_fd);
    return 0;
}
