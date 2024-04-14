#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM7"  // INPUT Port (Current set to COM7 but need to change to respective virtual input port from com0com) 

// Function to transmit data through the virtual serial port
void transmit_data() {
    int serial_fd; // File descriptor for the virtual serial port

    // Open the virtual serial port for writing
    serial_fd = open(VIRTUAL_SERIAL_PORT, O_WRONLY);
    if (serial_fd == -1) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Read input from stdin and send it to the virtual serial port
    char input_buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, input_buffer, sizeof(input_buffer))) > 0) {
    	
        // Print the word being transmitted from input port
     	printf("Transmitting data: %s", input_buffer);

        if (write(serial_fd, input_buffer, bytes_read) != bytes_read) {
            perror("Error writing to virtual serial port");
            close(serial_fd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the virtual serial port
    close(serial_fd);
}

int main() {
    transmit_data(); // Transmit data from input.txt or line coding output text file.
    return 0;
}

