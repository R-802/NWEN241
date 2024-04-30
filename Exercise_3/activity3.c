#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int sum;

    for (int i = 0; i < argc; i++) {
        if (argv[i] != NULL) {
            sum += atoi(argv[i]);
        } else {
            return 1;
        }
    }

    printf("%d\n", sum);

    return 0;
}
