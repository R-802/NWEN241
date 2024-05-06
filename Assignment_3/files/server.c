#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

/**
 * Print an error message and exit the program.
 * @param msg The error message to print.
*/
void error(const char* msg) {
    perror(msg);
    exit(-1);
}

/**
 * Setup a server socket and bind it to the specified port.
 * @param port The port number to bind the server socket to.
 * @return The server socket file descriptor.
*/
int setup_server_socket(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("Error creating socket");
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        error("Error binding socket");
    }

    if (listen(server_socket, 5) < 0) {
        error("Error listening");
    }

    return server_socket;
}

/**
 * Handle the GET command from the client.
 * @param client_socket The client socket file descriptor.
 * @param filename The name of the file to send to the client.
*/
void handle_get_command(int client_socket, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        char* errMsg = "SERVER 404 Not Found\n";
        write(client_socket, errMsg, strlen(errMsg));
        return;
    }

    char* okMsg = "SERVER 200 OK\n\n";
    write(client_socket, okMsg, strlen(okMsg));

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        write(client_socket, buffer, strlen(buffer));
    }
    write(client_socket, "\n\n", 3);

    fclose(file);
}

/**
 * Parse the message received from the client.
 * @param client_socket The client socket file descriptor.
 * @param buffer The message received from the client.
*/
void parse_message(int client_socket, char* buffer) {
    if (strcasecmp(buffer, "BYE") == 0) {
        printf("BYE received, closing connection.\n");
        return;
    }
    else if (strncasecmp(buffer, "GET ", 4) == 0) {
        char* filename = buffer + 4;  // Skip "GET " to get the filename
        if (*filename == '\0') {
            char* getErrMsg = "USAGE: GET <filename>\n";
            write(client_socket, getErrMsg, strlen(getErrMsg));
            return;
        }
        handle_get_command(client_socket, filename);
    }
    else {
        char* errMsg = "SERVER 500 Get Error\n";
        write(client_socket, errMsg, strlen(errMsg));
    }
}

/**
 * Main function to start the server.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 if the program exits successfully, -1 otherwise.
*/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);
    if (port < 1024) {
        printf("Invalid port number. Please use a port number greater than 1023.\n");
        return -1;
    }

    int server_socket = setup_server_socket(port);
    printf("Server is listening on port %d\n", port);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) {
            error("Error accepting connection");
        }

        // Send HELLO message
        char* hello_msg = "HELLO\n";
        write(client_socket, hello_msg, strlen(hello_msg));

        // Wait for a message from the client
        char buffer[BUFFER_SIZE] = { 0 };
        int n = read(client_socket, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            error("Error reading from client");
        }
        buffer[n] = '\0'; // Null terminate the string
        parse_message(client_socket, buffer);
    }

    return 0;
}