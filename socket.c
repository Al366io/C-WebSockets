#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int close(int fd);
#define MAX_REQUEST_SIZE 1024

int main()
{
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("Listening failed");
        exit(1);
    }

    printf("Server listening on port 8080\n");

    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    char request[MAX_REQUEST_SIZE];
    memset(request, 0, sizeof(request));

    // Read the incoming HTTP request
    recv(client_socket, request, sizeof(request), 0);

    printf("Received HTTP Request:\n%s\n", request);

    char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: text/plain\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
    close(server_socket);
    return 0;
}
