// LINE CODING: BAMI-HDB3 

#include <stdio.h>
#include <string.h>

// INPUT: Sequence of half-bit symbols (+,-,0) FROM UART-RX
// OUTPUT: Sequence of '0' and '1' TO: FRAMING

#define MAX_BLOCK_SIZE 256 

void decode_BAMI_HDB3(char *input, char *output) {
    int i = 0; // Index for input string

    while (i < strlen(input)) {
        // Check for patterns "000-", "000+", "-00-", and "+00+"
        if ((i + 3 < strlen(input) && strncmp(input + i, "000-", 4) == 0) || 
            (i + 3 < strlen(input) && strncmp(input + i, "000+", 4) == 0) || 
            (i + 3 < strlen(input) && strncmp(input + i, "-00-", 4) == 0) || 
            (i + 3 < strlen(input) && strncmp(input + i, "+00+", 4) == 0)) {
            // Decoded as '0000'
            strncpy(output + i, "0000", 4);
            i += 4;
            continue; // Skip to next iteration
        } else {
            // Regular bit decoding
            if (input[i] == '-' || input[i] == '+') {
                output[i] = '1'; 
            } else if (input[i] == '0') {
                output[i] = '0'; 
            }
            i++; 
        }
    }
    output[i] = '\0'; 



int main() {
    char input[MAX_BLOCK_SIZE + 1], output[MAX_BLOCK_SIZE + 1];

    fgets(input, MAX_BLOCK_SIZE, stdin);
    decode_BAMI_HDB3(input, output);

    printf("%s\n", output);

    return 0;
}