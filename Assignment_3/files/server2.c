#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define COMMAND_ERROR "SERVER 502 Command Error\n"
#define GET_ERROR "SERVER 500 Get Error\n"
#define NOT_FOUND "SERVER 404 Not Found\n"
#define FOUND "SERVER 200 OK\n"

/**
 * Prints an error message and exits the program.
 * @param msg The error message to print.
 */
void error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

/**
 * Sets up a server socket on the specified port.
 * @param port The port number to listen on.
 * @return The server socket file descriptor.
 */
int setup_server_socket(int port) {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    error("Error creating socket");
  }

  struct sockaddr_in server_address = {.sin_family = AF_INET,
                                       .sin_port = htons(port),
                                       .sin_addr.s_addr = INADDR_ANY};

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    error("Error binding socket");
  }

  if (listen(server_socket, SOMAXCONN) < 0) {
    error("Error listening");
  }
  printf("Server setup on port %d, waiting for connections...\n\n", port);

  return server_socket;
}

/**
 * Handles the GET command from the client.
 * @param client_socket The client socket file descriptor.
 * @param filename The name of the file to send to the client.
 */
void handle_get(int client_socket, char *filename) {
  if (filename == NULL || strlen(filename) == 0) {
    write(client_socket, GET_ERROR, strlen(GET_ERROR));
    return;
  }

  FILE *file = fopen(filename, "r");
  if (!file) { // File not found
    write(client_socket, NOT_FOUND, strlen(NOT_FOUND));
    return;
  }

  // File found
  write(client_socket, FOUND, strlen(FOUND));
  write(client_socket, "\n", 2); // Send newline

  char buffer[BUFFER_SIZE]; // Read file line by line
  while (fgets(buffer, BUFFER_SIZE, file)) {
    write(client_socket, buffer, strlen(buffer));
  }
  write(client_socket, "\n\n", 3); // Send double newline

  fclose(file);
}

/**
 * Handles a client connection. Reads client messages and sends responses.
 * @param client_socket The client socket file descriptor.
 * @param client_number The client number.
 */
void handle_client(int client_socket, int client_number) {
  // Send initial HELLO message
  write(client_socket, "HELLO\n", 6);

  char buffer[BUFFER_SIZE];
  while (1) { // Read client messages
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) { // Client disconnected or error occurred
      printf("Client_%d disconnected\n", client_number);
      close(client_socket);
      exit(0);
    }

    buffer[bytes_read] = '\0';

    // Handle commands
    if (strncasecmp(buffer, "BYE", 3) == 0) { // BYE command
      printf("Client_%d sent BYE command. Closing Socket.\n", client_number);
      close(client_socket);
      exit(0);
    } else if (strncasecmp(buffer, "GET ", 4) == 0) { // GET command
      char *filename = buffer + 4;
      filename[strcspn(filename, "\r\n")] = '\0';
      handle_get(client_socket, filename);
      printf("Client_%d GET request: %s\n", client_number, filename);
    } else {
      write(client_socket, COMMAND_ERROR, strlen(COMMAND_ERROR));
    }
  }
}

/**
 * Handles a fork failure by sending a HELLO message and reading client
 * messages. This function is used when a fork fails to create a child process.
 * @param client_socket The client socket file descriptor.
 */
void handle_fork_failure(int client_socket) {
  write(client_socket, "HELLO\n", 6);
  char buffer[BUFFER_SIZE];

  while (1) { // Read client messages
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) { // Client disconnected or error occurred
      printf("Client disconnected or error occurred\n");
      close(client_socket);
      break;
    }

    buffer[bytes_read] = '\0'; // Null-terminate the received string
    printf("Received message: %s\n", buffer); // Print received messages
  }
}

/**
 * The main server function that sets up the server socket and handles client
 * connections. The server listens for incoming connections and forks a child
 * process to handle each client connection. The child process reads client
 * messages and sends responses.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return The exit status.
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
  int client_number = 0;

  while (1) { // Loop to accept multiple client connections
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(
        server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
      perror("Error accepting connection");
      continue; // Continue to accept next connection
    }

    pid_t pid = fork();
    if (pid < 0) { // Error forking process
      perror("Error forking process");
      handle_fork_failure(client_socket);
    } else if (pid == 0) {  // Child process
      close(server_socket); // Child does not need access to the listener
      handle_client(client_socket, client_number);
      exit(0);
    } else { // Parent process
      printf("--------------------------------------------------\n");
      printf("Client_%d connected from %s with PID %d\n", client_number,
             inet_ntoa(client_address.sin_addr), pid);
      printf("--------------------------------------------------\n");
      client_number++;

      close(client_socket);
      waitpid(-1, NULL, WNOHANG); // Non-blocking wait for any child process
    }
  }

  close(server_socket);
  return 0;
}
