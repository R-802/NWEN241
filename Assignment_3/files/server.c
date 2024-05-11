#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * Print an error message and exit the program.
 * @param msg The error message to print.
 */
void error(const char *msg) {
  perror(msg);
  exit(-1);
}

/**
 * Setup a server socket and bind it to the specified port.
 * @param port The port number to bind the server socket to.
 * @return The server socket file descriptor.
 */
int setup_server_socket(int port) {
  // Create a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    error("Error creating socket");
  }

  // Bind the socket to the specified port
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port); // Host To Network Short
  server_address.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address and port
  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    close(server_socket);
    error("Error binding socket");
  }

  // Listen for incoming connections
  if (listen(server_socket, SOMAXCONN) < 0) {
    close(server_socket);
    error("Error listening");
  }
  printf("Server is listening on port %d\n", port);

  return server_socket;
}

/**
 * Send the contents of a file to the client.
 * @param client_socket The client socket file descriptor.
 * @param file The file to send to the client.
 */
void send_file_contents(int client_socket, FILE *file) {
  char buffer[BUFFER_SIZE];
  char *okMsg = "SERVER 200 OK\n\n";
  write(client_socket, okMsg, strlen(okMsg));

  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    write(client_socket, buffer, strlen(buffer));
  }
  write(client_socket, "\n\n", 3); // Two newlines after file contents
}

/**
 * Handle the GET command from the client.
 * @param client_socket The client socket file descriptor.
 * @param filename The name of the file to send to the client.
 */
void handle_get_command(int client_socket, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    char *errMsg = "SERVER 404 Not Found\n";
    write(client_socket, errMsg, strlen(errMsg));
    return;
  }

  send_file_contents(client_socket, file);
  fclose(file);
}

/**
 * Parse the message received from the client.
 * @param client_socket The client socket file descriptor.
 * @param buffer The message received from the client.
 */
void parse_message(int port, int client_socket, char *buffer) {
  if (strcasecmp(buffer, "BYE") == 0) {
    close(client_socket);
    char *hello_msg = "goodbye\n";
    write(client_socket, hello_msg, strlen(hello_msg));

    printf("BYE command received\n");

    return; // Exit the function after handling "BYE"
  } else if (strncasecmp(buffer, "GET ", 4) == 0) {
    char *filename = buffer + 4; // Skip "GET " to get the filename
    printf("GET request for file: %s\n", filename);

    if (*filename == '\0') {
      char *errMsg = "SERVER 500 Get Error\n";
      write(client_socket, errMsg, strlen(errMsg));
    } else {
      handle_get_command(client_socket, filename);
    }
  } else {
    char errMsg[256]; // Allocate a buffer for the error message
    snprintf(errMsg, sizeof(errMsg), "Command %s not recognized!\n",
             buffer); // Safely format the string
    write(client_socket, errMsg,
          strlen(errMsg)); // Send the formatted error message over the socket
    parse_message(
        port, client_socket,
        buffer); // Recursively call parse_message to handle the next message
  }
}

/**
 * Main function to start the server.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 if the program exits successfully, -1 otherwise.
 */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return -1;
  }

  int port = atoi(argv[1]);
  if (port < 1024 || port > 65535) {
    printf("Invalid port number. Please use a port number between 1024 and "
           "65535.\n");
    return -1;
  }

  int server_socket = setup_server_socket(port);

  // Main server loop
  while (1) {
    // Accept a new client connection
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_address,
               (socklen_t *)&client_address_len);
    if (client_socket < 0) {
      error("Error accepting connection");
    }
    printf("Accept successful\n");

    // Send a hello message to the client
    char *hello_msg = "HELLO\n";
    write(client_socket, hello_msg, strlen(hello_msg));

    // Read the message from the client
    char buffer[BUFFER_SIZE] = {0};
    int n = read(client_socket, buffer, BUFFER_SIZE - 1);
    printf("Received message: %s\n", buffer);

    if (n < 0) {
      error("Error reading from client");
    } else if (n == 0) {
      printf("Client closed the connection.\n");
      close(client_socket);
    } else {
      buffer[n] = '\0'; // Ensure string is null-terminated
    }
  }

  return 0; // Unreachable
}
