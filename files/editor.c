#include <stdio.h>
#include <string.h>
#include "editor.h"


int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos) {
    // Check if the position is out of bounds
    if (pos < 0 || pos >= editing_buflen) {
        return 0; // If out of bounds, return 0 indicating failure to insert
    }

    // Calculate the number of characters to move to the right, excluding the new insertion
    int num_chars_to_move = editing_buflen - pos - 1;

    // Destination pointer: Address to start moving characters to (one position to the right of the insertion point)
    char* destination = &editing_buffer[0] + pos + 1;

    // Source pointer: Address of the current position where new character is to be inserted
    char* source = &editing_buffer[0] + pos;

    // Move the block of memory starting from 'source' to 'destination' to make space for the new character
    memmove(destination, source, num_chars_to_move);

    // Insert the new character at the specified position
    editing_buffer[pos] = to_insert;

    // Return 1 to indicate success
    return 1;
}


int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset) {
    if (offset < 0 || offset >= editing_buflen) {
        return 0;
    }

    char* found = strchr(editing_buffer + offset, to_delete);
    if (!found) {
        return 0;
    }

    int num_chars_to_move = editing_buflen - (found - editing_buffer) - 1;
    memmove(found, found + 1, num_chars_to_move);

    return 1;
}


int editor_replace_str(char editing_buffer[], int editing_buflen, const char* str, const char* replacement, int offset) {
    if (offset < 0 || offset >= editing_buflen) {
        return -1;
    }

    char* found = strstr(editing_buffer + offset, str);

    if (!found) {
        return -1;
    }

    int num_chars_to_move = editing_buflen - (found - editing_buffer) - strlen(str);

    memmove(found, found + strlen(str), num_chars_to_move);

    for (int i = 0; i < strlen(replacement); i++) {
        editor_insert_char(editing_buffer, editing_buflen, replacement[i], found - editing_buffer + i);
    }

    return found - editing_buffer + strlen(replacement) - 1;
}


void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap) {


}
