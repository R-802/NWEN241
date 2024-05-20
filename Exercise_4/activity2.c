#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 23456
#define BUFFER_SIZE 1024

// Function to reverse the string
void reverseString(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = temp;
  }
}

int main() {
  int server_fd, client_fd;
  struct sockaddr_in address;
  int addr_len = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to the port 23456
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Put the server socket in a passive mode, where it waits for the client to
  // approach the server to make a connection
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d\n", PORT);

  while (1) {
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addr_len)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    int read_size = read(client_fd, buffer, BUFFER_SIZE);
    if (read_size > 0) {
      buffer[read_size] = '\0'; // Null-terminate string
      reverseString(buffer);
      send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd); // Close the client socket
  }

  return 0;
}
