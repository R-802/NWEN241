#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

//----------Helper functions----------//

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

  printf("Server setup on port %d, waiting for connections...\n", port);

  return server_socket;
}

//----------Command handlers----------//

/**
 * Handles the GET command from the client.
 * @param client_socket The client socket file descriptor.
 * @param filename The name of the file to send to the client.
 */
void handle_get_command(int client_socket, char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    char *errMsg = "SERVER 404 Not Found\n";
    write(client_socket, errMsg, strlen(errMsg));
    return;
  }

  char *okMsg = "SERVER 200 OK\n\n";
  write(client_socket, okMsg, strlen(okMsg));

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file)) {
    write(client_socket, buffer, strlen(buffer));
  }
  char *endMsg = "\n\n";
  write(client_socket, endMsg, strlen(endMsg));

  fclose(file);
}

/**
 * Handles the PUT command from the client.
 * @param client_socket The client socket file descriptor.
 * @param filename The name of the file to write to.
 */
void handle_put_command(int client_socket, char *filename) {
  FILE *file = fopen(filename, "w"); // Overwrite file
  if (!file) {
    char *errMsg = "SERVER 501 Put Error\n";
    write(client_socket, errMsg, strlen(errMsg));
    return;
  }

  char buffer[BUFFER_SIZE];
  int newline_count = 0;
  int last_char_was_newline = 0;

  while (1) {
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read < 0) { // Read error
      fclose(file);
      char *errMsg = "SERVER 500 Read Error\n";
      write(client_socket, errMsg, strlen(errMsg));
      return;
    } else if (bytes_read == 0) { // Client disconnected
      fclose(file);
      return;
    }

    // Write to file and check for double newlines
    fwrite(buffer, 1, bytes_read, file);
    for (int i = 0; i < bytes_read; ++i) {
      if (buffer[i] == '\n') {
        if (last_char_was_newline) {
          newline_count++;
          if (newline_count == 2) {
            fclose(file);
            char *okMsg = "SERVER 201 Created\n";
            write(client_socket, okMsg, strlen(okMsg));
            return;
          }
        } else {
          last_char_was_newline = 1;
        }
      } else {
        last_char_was_newline = 0;
      }
    }
  }
}

/**
 * Parses the client message and calls the appropriate handler.
 * @param client_socket The client socket file descriptor.
 * @param buffer The message received from the client.
 */
void parse_message(int client_socket, char *buffer) {
  if (!buffer || *buffer == '\0') {
    char *errMsg = "SERVER 502 Command Error\n";
    write(client_socket, errMsg, strlen(errMsg));
    return;
  }

  if (strncasecmp(buffer, "BYE", 3) == 0) {
    printf("Received BYE command: Closing client...\n");
    close(client_socket);
    return;
  } else if (strncasecmp(buffer, "GET ", 4) == 0) {
    char *filename = buffer + 4;
    filename[strcspn(filename, "\r\n")] = 0;
    if (*filename) {
      handle_get_command(client_socket, filename);
    } else {
      char *errMsg = "SERVER 500 Get Error\n";
      write(client_socket, errMsg, strlen(errMsg));
    }
    printf("Received GET request: %s\n", filename);
    return;
  } else if (strncasecmp(buffer, "PUT ", 4) == 0) {
    char *filename = buffer + 4;
    filename[strcspn(filename, "\r\n")] = 0;
    if (*filename) {
      handle_put_command(client_socket, filename);
    } else {
      char *errMsg = "SERVER 501 Put Error\n";
      write(client_socket, errMsg, strlen(errMsg));
    }
    printf("Received PUT request: %s\n", filename);
    return;
  } else {
    char errMsg[] = "SERVER 502 Command Error\n";
    write(client_socket, errMsg, sizeof(errMsg) - 1);
    return;
  }
}

//----------Main function----------//

/**
 * Main function that sets up a server socket and listens for client
 * connections.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
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

  while (1) { // Accept client connections
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(
        server_socket, (struct sockaddr *)&client_address, &client_address_len);

    if (client_socket < 0) {
      perror("Error accepting connection");
      continue;
    }

    printf("Client connected from %s\n", inet_ntoa(client_address.sin_addr));

    char *hello_msg = "HELLO\n";
    write(client_socket, hello_msg, strlen(hello_msg));

    while (1) { // Handle client messages
      char buffer[BUFFER_SIZE] = {0};
      int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

      if (bytes_read < 0) {
        close(client_socket);
        break;
      } else if (bytes_read == 0) {
        close(client_socket);
        break;
      }

      buffer[bytes_read] = '\0';
      parse_message(client_socket, buffer);
    }
  }

  return 0;
}
