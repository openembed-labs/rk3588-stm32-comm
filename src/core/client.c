#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"

void run_client(const char *server_address, int port)
{
    int c_fd;
    char buf[MAX_LINE];
    struct sockaddr_in client_addr;
    int len, bytes_sent;

    if ((c_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(server_address);
    client_addr.sin_port = htons(port);

    int status = connect(c_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (status == -1)
    {
        perror("Connect error");
        close(c_fd);
        exit(1);
    }

    printf("Connected to server %s on port %d\n", server_address, port);

    while (1)
    {
        printf("Enter message to send to server (type 'quit' to exit): ");
        fgets(buf, MAX_LINE, stdin);

        if (strcmp(buf, "quit\n") == 0)
            break;

        bytes_sent = send(c_fd, buf, strlen(buf), 0);
        if (bytes_sent == -1)
        {
            perror("Send error");
            close(c_fd);
            exit(1);
        }
        printf("Sent %d bytes to server\n", bytes_sent);

        len = recv(c_fd, buf, MAX_LINE - 1, 0);
        if (len == -1)
        {
            perror("Receive error");
            close(c_fd);
            exit(EXIT_FAILURE);
        }

        buf[len] = '\0';
        printf("Received reply from server: %s", buf);
    }

    close(c_fd);
}