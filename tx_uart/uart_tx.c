#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM7"  // INPUT Port (Current set to COM7 but need to change to respective virtual input port from com0com) 

// Function to transmit data through the virtual serial port
void transmit_data(const char *input_filename) {
    int serial_fd; // File descriptor for the virtual serial port

    // Open the virtual serial port for writing
    serial_fd = open(VIRTUAL_SERIAL_PORT, O_WRONLY);
    if (serial_fd == -1) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Open input file and read input (Can change this to read from keyboard entry and save to .txt file)
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        close(serial_fd);
        exit(EXIT_FAILURE);
    }

    // Read input from the file and send it to the virtual serial port
    char input_buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file)) > 0) {
    	
        // Print the word being transmitted from input port
     	printf("Transmitting data: \"%.*s\" from virtual serial port '%s'\n", (int)bytes_read, input_buffer, VIRTUAL_SERIAL_PORT);

        if (write(serial_fd, input_buffer, bytes_read) != bytes_read) {
            perror("Error writing to virtual serial port");
            fclose(input_file);
            close(serial_fd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the input file and the virtual serial port
    fclose(input_file);
    close(serial_fd);
}

/*
int main() {
    transmit_data("tx_line_coding_output.txt"); // Transmit data from input.txt or line coding output text file.
    return 0;
}
*/

