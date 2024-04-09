// LINE CODING: BAMI-HDB3 

#include <stdio.h>
#include <string.h>

// INPUT: Sequence of Frame Bits as '0' and '1'FROM: Channel Coding
// OUTPUT: Sequence of as Half-Bit Symbols (+, -, 0)

void encode_BAMI_HDB3(char *input, char *output) {
    int count = 0; 
    int pulses_since_last_sub = 0; 
    char last_polarity = '+'; // Can be '+' or '-'

    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == '1') { 
            // Alternate positive and negative polarities for '1'
            if (last_polarity == '+') {
                output[i] = '-';
                last_polarity = '-';
            } else {
                output[i] = '+';
                last_polarity = '+';
            }
            pulses_since_last_sub++; // Increment pulses since last substitution
            count = 0; 
        } 
        if (input[i] == '0') { 
            count++; // Increment count of consecutive zeros
            if (count == 4) { // If four consecutive zeros
                if (pulses_since_last_sub % 2 == 0) {
                    // Substitute with "B00V"
                    strcpy(output + i - 3, (last_polarity == '+') ? "-00-" : "+00+");
                    last_polarity = (last_polarity == '+') ? '-' : '+';
                } else {
                    // Substitute with "000V"
                    strcpy(output + i - 3, (last_polarity == '+') ? "000+" : "000-");
                }
                // Reset counts
                count = 0;
                pulses_since_last_sub = 0;
            } else {
                output[i] = '0'; // No substitution yet
            }
        }
    }
    output[strlen(input)] = '\0'; // Null-terminate
}

//int main() {
//    char input[];
//    char output[];
    
//    encode_BAMI_HDB3(input, output);
//    printf("%s\n", output);
    
//    return 0;
//}