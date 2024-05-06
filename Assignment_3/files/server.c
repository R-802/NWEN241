/**
 * Skeleton file for server.c
 *
 * You are free to modify this file to implement the server specifications
 * as detailed in Assignment 3 handout.
 *
 * As a matter of good programming habit, you should break up your imple-
 * mentation into functions. All these functions should be contained in this
 * file as you are only allowed to submit this file.
 */

 // Include necessary header files
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

void error(const char* msg) {
    perror(msg);
    exit(-1);
}

/**
 * The main function should be able to accept a command-line argument
 * argv[0]: program name
 * argv[1]: port number
 *
 * Read the assignment handout for more details about the server program
 * design specifications.
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);
    if (port < 1024) {
        fprintf(stderr, "Invalid port number. Please use a port number greater than 1023.\n");
        return -1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) error("Error creating socket");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        error("Error binding socket");

    if (listen(server_socket, 5) < 0) error("Error listening");

    printf("Server is listening on port %d\n", port);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) error("Error accepting connection");

        // Send HELLO message
        char* hello_msg = "HELLO\n";
        int n = write(client_socket, hello_msg, strlen(hello_msg));
        if (n < 0) error("Error writing to socket");

        // Wait for a message from the client
        char buffer[BUFFER_SIZE];
        n = read(client_socket, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            error("Error reading from client");
        }
        else if (n == 0) {
            printf("Client closed the connection\n");
        }
        else {
            buffer[n] = '\0'; // Null terminate the string
            printf("Received message: %s\n", buffer);
        }

        close(client_socket); // Close the client socket after handling
    }

    // Close server socket (although we never reach here in this loop)
    close(server_socket);

    return 0;
}