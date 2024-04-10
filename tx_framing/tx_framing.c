#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number

char* decimalToBinary(int decimal) {
    // Calculate the number of bits required to represent the decimal
    int numBits = 0;
    int temp = decimal;
    while (temp > 0) {
        temp /= 2;
        numBits++;
    }

    // Allocate memory for the binary string
    char* binary = (char*)malloc((numBits + 1) * sizeof(char)); // +1 for null terminator
    if (binary == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Convert decimal to binary
    for (int i = numBits - 1; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0'; // Convert remainder to character
        decimal /= 2;
    }
    binary[numBits] = '\0'; // Null terminate the string

    // Return the binary string
    return binary;
}

int main() {
    char input[MAX_SEQUENCE_NUMBER];
    
    // Read the input from tx_input_framing.txt
    FILE* inputFile = fopen("tx_input_framing.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening input_framing file");
        return 1;
    }

    // Create and open the output file
    FILE* outputFile = fopen("tx_output_framing.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating output_framing file");
        fclose(inputFile); // Close the input file if output file creation failed
        return 1;
    }

    // Read input from the file
    if (fgets(input, sizeof(input), inputFile) == NULL) {
        perror("Error reading input from file");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    // Close the input file as it's no longer needed
    fclose(inputFile);

    char preamble[] = "11111111";
    int sequence_number = 1;
    int frame_type = 1;
    int payload_size = 1000;
    int fcs = 0;
    
    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';
    
    int len = strlen(input);
    
    // Iterate through the word, printing two characters at a time
    for (int i = 0; i < len; i += 8) {
        int remaining_chars = len - i;  // Calculate remaining characters
        char* binary = decimalToBinary(sequence_number);
        
        fprintf(outputFile, "%s%08s%02d%04d", preamble, binary, frame_type, payload_size);
        
        // Handle trailing spaces if fewer than payload_size characters remaining
        if (remaining_chars < 8) {
            for (int k = 0; k < 8 - remaining_chars; k++) {
                fprintf(outputFile, "0");  // Print spaces for missing characters
            }
        }
        
        // Print up to payload_size characters
        for (int j = 0; j < 8 && i + j < len; j++) {
            fprintf(outputFile, "%02c", input[i + j]); // Print characters in hexadecimal
        }
        
        fprintf(outputFile, "%04d", fcs); // Print FCS and newline
        
        sequence_number++;   
    }

    // Close the output file
    fclose(outputFile);
    
    return 0;
}
