#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pid, ret_exec, status;
  pid = fork();

  switch (pid) {
  case -1:
    // Code for error in fork
    perror("Error");
    exit(EXIT_FAILURE);

  case 0:
    // Code for child process
    ret_exec = execl("/bin/ps", "ps", "-A", NULL);
    if (ret_exec == -1) {
      perror("Error executing exec");
      exit(1); // Exiting with status 1 if execl fails
    }
    break;

  default:
    // Code for parent process
    pid = wait(&status); // Correctly using wait to get child status and pid
    if (pid == -1) {
      perror("Wait failed");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      printf("Parent process ID: %d\n", getpid());
      printf("Child process ID: %d\n", pid);
      printf("Child termination status: %d\n", WEXITSTATUS(status));
    }
    break;
  }

  return 0;
}
