#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_REQUEST_SIZE 1024
#define POST "POST"
#define DELETE "DELETE"
#define PUT "PUT"
#define GET "GET"

void send_200(int client_socket)
{
    char http_header[] = "HTTP/1.1 200 OK\r\n\n";
    send(client_socket, http_header, sizeof(http_header), 0);
}

void send_500(int client_socket)
{
    char http_header[] = "HTTP/1.1 500 Internal Server Error\r\n\n";
    send(client_socket, http_header, sizeof(http_header), 0);
}

struct parsed_request
{
    char method[16];
    char body[512];
};

struct parsed_request parse_request(char *request)
{
    struct parsed_request req;
    int i = 0;
    while ( i < 15 && request[i] != 47) {
        req.method[i] = request[i];
        i++;
    }
    req.method[i-1] = '\0';
    
    return req;
}

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

    // Reading the HTTP request
    recv(client_socket, request, sizeof(request), 0);

    struct parsed_request req = parse_request(request);

    printf("%s", req.method);

    if (strcmp(req.method, POST) == 0) {
        send_200(client_socket);
    } else {
        send_500(client_socket);
    }
    close(server_socket);
    return 0;
}
