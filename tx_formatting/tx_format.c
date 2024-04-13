#include <stdio.h>

int main() {
    FILE *file, *output_file;
    char filename[] = "tx_format_input.txt";
    char output_filename[] = "tx_format_output.txt";
    char hex_string[10000];
    char *ptr = hex_string; 
    char c;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return 1;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not create file %s\n", output_filename);
        return 1;
    }

    //Format (Send) -------------------------------------------------
    int i = 0;
    while ((c = fgetc(file)) != EOF) {
        sprintf(&hex_string[i], "%02X", (unsigned char)c);
        i += 2;
    }
    hex_string[i] = '\0';
    
    fprintf(output_file, "%s", hex_string);

    fclose(file);
    fclose(output_file);
    return 0;
}
