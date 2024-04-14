#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// INPUT: Sequence of Frame Bits as '0' and '1' FROM: Line Decoding 
// OUTPUT: Sequence of Frame Bytes in HEX To: Channel Decoding

#define CRC8_POLYNOMIAL 0x07
#define MAX_BLOCK_SIZE 256
#define MAX_FRAME_SIZE 1024 // Maximum size of a frame
#define MAX_DATA_BLOCK_SIZE 38
#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_DATA_BLOCK_SIZE 38 // Maximum size of data block
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence
#define FRAME_SIZE 18;

// INPUT: Sequence of 0s and 1s from Source Coding
// OUTPUT: Framed sequence in binary in HEX
// Preamble = FF
// Frame Type = 01
// Payload Size = 0002

// Function to calculate CRC-8
uint8_t crc8(uint8_t *data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

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

void hex_string_to_bytes(const char *hex_string, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex_string + 2*i, "%2hhx", &bytes[i]);
    }
}

int main() {
    char input[MAX_BINARY_SEQUENCE * 10]; // Buffer to store binary sequence
    char strDataWord[2], strPayload_size[4], strFrame_type[2], strSequence_number[4], result[14];
    char* binaryResult;

    // Pipe operation
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }
    
    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';
    char* hexadecimal = binaryToHexadecimal(input);
    
    int len = strlen(input);

    char preamble[] = "FF";
    int sequence_number = 1;
    int frame_type = 1;
    int payload_size = 2;
	
    int length = strlen(hexadecimal);
    
    // Iterate through the word, printing two characters at a time
    for (int i = 0; i < length; i += 2) {
        int remaining_chars = length - i;  // Calculate remaining characters
        
        printf("%s%04X%02d%04d", preamble, sequence_number, frame_type, payload_size);
        
        // Convert integers to strings
	    sprintf(strSequence_number, "%04X", sequence_number);
	    sprintf(strFrame_type, "%02d", frame_type);
	    sprintf(strPayload_size, "%04d", payload_size);
	
	    // Concatenate strings
	    strcpy(result, preamble);
	    strcat(result, strSequence_number); 
        
        // Handle trailing spaces if fewer than payload_size characters remaining
        if (remaining_chars < 2) {
            for (int k = 0; k < 2 - remaining_chars; k++) {
                printf("0");  // Print spaces for missing characters
                strcat(result, "0");
            }
        }
        
        // Print up to payload_size characters in hexadecimal
        for (int j = 0; j < 2 && i + j < length; j++) {
            printf("%c", hexadecimal[i + j]);
            sprintf(strDataWord, "%c", hexadecimal[i + j]);
            strcat(result, strDataWord);
        }
        
        strcat(result, "0000");
		size_t len = 9;
        uint8_t *bytes = malloc(len);
        hex_string_to_bytes(result, bytes, len - 2); // Exclude the FCS part

        uint8_t crc = crc8(bytes, len - 2); // Calculate CRC-8 for the frame
		char fcs = crc;

        strcpy(result, ""); // Clear result string
        printf("%04X", fcs); // Print FCS
        
        sequence_number++;  
		sequence_number %= 0x1F41; // Wrap around when sequence_number reaches 1F40 
    }

    return 0;
}
