#include <stdio.h>

int main() {
    FILE *input_file, *output_file;
    char input_filename[] = "rx_format_input.txt";
    char output_filename[] = "tx_format_output.txt";
    char hex_string[10000];
    char *ptr = hex_string;
    char c;

    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Could not open input file %s\n", input_filename);
        return 1;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open output file %s\n", output_filename);
        fclose(input_file);
        return 1;
    }

    // Read hex string from input file
    fgets(hex_string, sizeof(hex_string), input_file);

    // Convert and write to output file
    while (*ptr != '\0' && *(ptr + 1) != '\0') {
        char hex_pair[3] = {*ptr, *(ptr + 1), '\0'};
        int ascii_byte = strtol(hex_pair, NULL, 16);
        fprintf(output_file, "%c", ascii_byte);
        ptr += 2;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

