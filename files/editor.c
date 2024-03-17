#include <stdio.h>
#include <string.h>
#include "editor.h"

int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos) {
    return pos < 0 || pos >= editing_buflen ? 0 : (memmove(editing_buffer + pos + 1, editing_buffer + pos, editing_buflen - pos - 1), editing_buffer[pos] = to_insert, 1);
}

int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset) {
    for (int i = offset; i < editing_buflen; i++) {
        if (editing_buffer[i] == to_delete) {
            memmove(editing_buffer + i, editing_buffer + i + 1, editing_buflen - i - 1);
            return 1;
        }
    }
    return 0;
}

int editor_replace_str(char editing_buffer[], int editing_buflen, const char* str, const char* replacement, int offset) {
    char* p = strstr(editing_buffer + offset, str);
    if (p) {
        memmove(p + strlen(replacement), p + strlen(str), editing_buflen - (p - editing_buffer) - strlen(str));
        memcpy(p, replacement, strlen(replacement));
        return p - editing_buffer + strlen(replacement) - 1;
    }
    return -1;
}

void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap) {

}
