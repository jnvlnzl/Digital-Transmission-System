// LINE CODING: BAMI-HDB3 

#include <stdio.h>
#include <string.h>

// INPUT: Sequence of half-bit symbols (+,-,0) FROM UART-RX
// OUTPUT: Sequence of '0' and '1' TO: FRAMING

#define MAX_BLOCK_SIZE 38 * 8

void decode_BAMI_HDB3(char *input, char *output) {
    int i = 0; // Index for input string
    int pulses_since_last_sub = 0;
    char last_polarity = '+'; 

    // Find the first '1' in the input and set last_polarity accordingly
    for (int j = 0; j < strlen(input); j++) {
        if (input[j] == '+' || input[j] == '-') {
            last_polarity = input[j];
            break;
        }
    }

    while (i < strlen(input)) {
        // Check for patterns "000-", "000+", "-00-", and "+00+"
        if ((i + 3 < strlen(input) && strncmp(input + i, "000-", 4) == 0) || 
            (i + 3 < strlen(input) && strncmp(input + i, "000+", 4) == 0)) {
            // Decoded as '0000' or '0001' depending on the last polarity
            if ((last_polarity == '+') && (pulses_since_last_sub % 2 == 0)) {
                strncpy(output + i, "0001", 4);
            } else {
                strncpy(output + i, "0000", 4);
            }
            pulses_since_last_sub++;
            i += 4;
            continue; // Skip to next iteration
        } else if ((i + 3 < strlen(input) && strncmp(input + i, "-00-", 4) == 0) || 
                   (i + 3 < strlen(input) && strncmp(input + i, "+00+", 4) == 0)) {
            // Decoded as '0000'
            strncpy(output + i, "0000", 4);
            i += 4;
            continue; // Skip to next iteration
        } else {
            // Regular bit decoding
            if (input[i] == '-' || input[i] == '+') {
                output[i] = '1'; 
                last_polarity = input[i]; // Update the last polarity
                pulses_since_last_sub++;
            } else if (input[i] == '0') {
                output[i] = '0'; 
            }
            i++; 
        }
    }
    output[i] = '\0'; 
}


int main() {
    //char input[100] = "-+-+-+-+-00-+00+-00-000+000+000-000-+00+-00-00+0-+0-+0-+000+-00-+00+-00-000+0-+0";
    //char output[100];

    char input[MAX_BLOCK_SIZE + 1], output[MAX_BLOCK_SIZE + 1];

    fgets(input, MAX_BLOCK_SIZE, stdin);
    decode_BAMI_HDB3(input, output);

    printf("%s\n", output);

    return 0;
}