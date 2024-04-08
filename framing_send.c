#include <stdio.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number

char* int_to_hex(int sequence_number) {
  static char hex_string[9];  // Buffer to store hex digits (8 + null terminator)
  int i = 0;
  
  if (sequence_number == 0) {
    hex_string[0] = '0';
    hex_string[1] = '\0';
    return hex_string;
  }
  
  // Handle negative values (convert to positive and prepend '-')
  if (sequence_number < 0) {
    sequence_number = -sequence_number;
    hex_string[i++] = '-';
  }
  
  // Convert integer to hex digits (using nibbles)
  for (; sequence_number > 0; sequence_number /= 16) {
    int digit = sequence_number % 16;
    hex_string[i++] = (digit < 10) ? ('0' + digit) : ('A' - 10 + digit);
  }
  
  // Terminate the string
  hex_string[i] = '\0';
  
  // Reverse the string (optional, for human readability)
  // strrev(hex_string);  // Uncomment to reverse the string

  return hex_string;
}

int main() {
    char input[MAX_SEQUENCE_NUMBER];
    
    // Accept input from the user
    fgets(input, sizeof(input), stdin);
    
	char preamble[] = "CCCCCCCC";
	int sequence_number = 0001;
	int frame_type = 01;
	int payload_size = 0002;
	int fcs = 0000;
	
	// Remove trailing newline (if any)
	  input[strcspn(input, "\n")] = '\0';
	
	  int len = strlen(input);
	
	  // Iterate through the word, printing two characters at a time
	  for (int i = 0; i < len; i += 2) {
	  	char* hex_value = int_to_hex(sequence_number);
	  	if(i + 1 < len){
	      printf("%s%04s%02d%04d%08c%08c%04d", preamble, hex_value, frame_type, payload_size, input[i], input[i + 1], fcs);
		} else {
		  // Handle single character case (odd length word)
	      printf("%s%04s%02d%04d%08c%084c%04d", preamble, hex_value, frame_type, payload_size, input[i], fcs);
		}	
	    sequence_number++;   
	  }
		
	  printf("\n");
    
    return 0;
}


