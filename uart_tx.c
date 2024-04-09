#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM7"  // INPUT Port (Current set to COM7 but need to change to respective virtual input port from com0com) 

int main() {
	
    int serial_fd; // Integer that holds the file descriptor for the virtual port
    
    
    //Check or test for error in opening virtual serial port
    serial_fd = open(VIRTUAL_SERIAL_PORT, O_WRONLY);
    if (serial_fd == -1) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Opens input file and reads input from "input.txt" atm
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        close(serial_fd);
        exit(EXIT_FAILURE);
    }

    // Read input from the file and send it to the virtual serial port
    char input_buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file)) > 0) {
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

    return 0;
}

