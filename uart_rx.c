#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM8"  // INPUT Port (Current set to COM8 but need to change to respective virtual output port from com0com) 

int main() {
    int serial_fd; // File descriptor for the serial port

    // Open the virtual serial port for reading
    serial_fd = open(VIRTUAL_SERIAL_PORT, O_RDONLY);
    
    //Check or test for error in opening virtual serial port
    if (serial_fd == -1) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Read data from the serial port and print it to stdout
    char received_buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = read(serial_fd, received_buffer, sizeof(received_buffer))) > 0) {
        printf("Received: %.*s", (int)bytes_read, received_buffer); // Print received data
        fflush(stdout); // Flush stdout to ensure data is printed immediately
    }

    // Close the serial port
    close(serial_fd);

    return 0;
}

