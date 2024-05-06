#include <stdio.h>
#include <string.h>

void reverse(char* arr) {
    int len = strlen(arr);
    for (int i = 0; i < len / 2; i++) {
        char t = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = t;
    }
}

int main() {
    char string[] = "Hello World!";

    char text_buffer[24];
    char reversed_text_buffer[24];

    sprintf(text_buffer, "%12.5s", string);
    printf(text_buffer);

    reverse(string);
    sprintf(reversed_text_buffer, "%5s", string);
    printf("\n%s", reversed_text_buffer);

    return 0;
}