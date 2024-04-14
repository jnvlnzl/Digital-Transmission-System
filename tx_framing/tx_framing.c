#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_DATA_BLOCK_SIZE 38 // Maximum size of data block
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence

// INPUT: Sequence of 0s and 1s from Source Coding
// OUTPUT: Framed sequence in binary in HEX
// Preamble = FF
// Frame Type = 01
// Payload Size = 0002
// FCS = 0000

// Function to convert a binary string to its hexadecimal representation
char* binaryToHexadecimal(const char* input) {
    size_t binaryLength = strlen(input) - 1;
    size_t hexLength = (binaryLength + 3) / 4; // Calculate the number of hexadecimal characters required
    char* hexadecimal = (char*)malloc((hexLength + 1) * sizeof(char));

    for (size_t i = 0; i < hexLength; i++) {
        int byte = 0;
        for (int j = 0; j < 4 && i * 4 + j < binaryLength; j++) {
            byte = (byte << 1) | (input[i * 4 + j] - '0');
        }
        sprintf(hexadecimal + i, "%X", byte);
    }

    return hexadecimal;
}

int main() {
    char input[MAX_BINARY_SEQUENCE]; // Buffer to store binary sequence
    char hexOutput[MAX_SEQUENCE_NUMBER * 2]; // Buffer to store hexadecimal output

    // Pipe operation
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }
	
    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';
    char* hexadecimal = binaryToHexadecimal(input);
    
    char preamble[] = "FF";
    int sequence_number = 1;
    int frame_type = 1;
    int payload_size = 2;
    int fcs = 0;

    int length = strlen(hexadecimal);
	        
    // Iterate through the word, printing two characters at a time
    for (int i = 0; i < length; i += 2) {
        int remaining_chars = length - i;  // Calculate remaining characters
        
        printf("%s%04X%02d%04d", preamble, sequence_number, frame_type, payload_size);
        
        // Handle trailing spaces if fewer than payload_size characters remaining
        if (remaining_chars < 2) {
            for (int k = 0; k < 2 - remaining_chars; k++) {
                printf("0");  // Print spaces for missing characters
            }
        }
        
        // Print up to payload_size characters in hexadecimal
        for (int j = 0; j < 2 && i + j < length; j++) {
            printf("%c", hexadecimal[i + j]);
        }
        
        printf("%04d", fcs); // Print FCS 
        
        sequence_number++;
        sequence_number %= 0x1F41; // Wrap around when sequence_number reaches 1F40   
    }

    return 0;
}
