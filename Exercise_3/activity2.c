#include <stdio.h>

int main(void)
{
    int a, b;
    FILE *in; // use for handling input file
    FILE *out; // use for handling output file

    // Open raw.txt for reading
    in = fopen("raw.txt", "r");
    if (in == NULL) {
        printf("raw.txt open failed");
        return -1;
    }

    // Open processed.txt for writing
    out = fopen("processed.txt", "w");
    if (out == NULL) {
        printf("processed.txt open failed");
        return -1;
    }

    while (fscanf(in, "%d %d", &a, &b) == 2) {
        int sum = a + b;
        fprintf(out, "%d %d %d\n", a, b, sum);
    }

    fclose(in);
    fclose(out);
    
    return 0;
}
