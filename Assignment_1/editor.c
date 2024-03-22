#include <stdio.h>
#include <string.h>

#include "editor.h"

// CORE 1
int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos) {
    if (pos < 0 || pos >= editing_buflen) {
        return 0; // If out of bounds return 0 
    }

    char* destination = &editing_buffer[0] + pos + 1; // Address of the character to the right of the position
    char* source = &editing_buffer[0] + pos; // The address of the character at the position
    int length = editing_buflen - pos - 1; // Calculate the length of the buffer from the position to the end

    // Move the characters from the position to the end of the buffer one position to the right
    memmove(destination, source, length);

    // Insert the character at the position
    editing_buffer[pos] = to_insert;

    return 1;
}

// CORE 2
int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset) {
    // Starting from the offset, iterate to the end of the buffer
    for (int i = offset; i < editing_buflen; i++) {
        if (editing_buffer[i] == to_delete) { // If we find the chararcter to delete
            // Get the address of it
            char* destination = editing_buffer + i;
            printf("    Index: %d\n", i);
            printf("    Destination: %c\n", *destination);

            // Get the address of the character to the right of it
            char* source = destination + 1;
            printf("    Source: %c\n", *source);

            // Calculate the length of the buffer from the position of the char to delete to end
            int length = editing_buflen - i - 1;

            // Move the characters from the position to the end of the buffer one position to the left
            memmove(destination, source, length);
            return 1;
        }
    }

    return 0;
}

// COMPLETION
int editor_replace_str(char editing_buffer[], int editing_buflen, const char* str, const char* replacement, int offset) {
    if (offset < 0 || offset >= editing_buflen) {
        return -1; // If out of bounds return -1
    }

    char* found = strstr(editing_buffer + offset, str); // Find the first occurence of the string starting from the offset

    if (!found) {
        return -1; // If the string is not found return -1
    }

    // Calculate the number of characters to move
    int num_chars_to_move = editing_buflen - (found - editing_buffer) - strlen(str);

    memmove(found, found + strlen(str), num_chars_to_move); // Move the characters to the right of the string to the left

    for (int i = 0; i < strlen(replacement); i++) { // Insert the replacement string
        editor_insert_char(editing_buffer, editing_buflen, replacement[i], found - editing_buffer + i);
    }

    // Return the index of the first character of the replacement string
    return found - editing_buffer + strlen(replacement) - 1;
}


// DID NOT ATTEMPT CHALLENGE
void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap) {
    // 
    // for (int i = 0; i < rows; i++) {
    //     for (int j = 0; j < cols; j++) {
    //         viewing_buffer[i][j] = '\0';
    //     }
    // }
}
