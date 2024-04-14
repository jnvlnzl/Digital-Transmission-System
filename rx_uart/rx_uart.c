#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM8"  // INPUT Port (Current set to COM8 but need to change to respective virtual output port from com0com) 
#define OUTPUT_FILE "rx_uart_output.txt"  // File to store received data

// Function to receive data from the virtual serial port and print it to stdout
void receive_and_print_data() {
    HANDLE serial_handle; // File descriptor for the serial port

    // Open the virtual serial port for reading
    serial_handle = CreateFile(VIRTUAL_SERIAL_PORT, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serial_handle == INVALID_HANDLE_VALUE) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Read data from the serial port and print it to stdout
    char received_buffer[256];
    DWORD bytes_read;
    while (1) {
        if (!ReadFile(serial_handle, received_buffer, sizeof(received_buffer), &bytes_read, NULL)) {
            perror("Error reading from virtual serial port");
            CloseHandle(serial_handle);
            exit(EXIT_FAILURE);
        }
        
        // Print the received data
        fwrite(received_buffer, 1, bytes_read, stdout);
        fflush(stdout); // Flush output buffer to ensure data is written immediately
    }

    // Close the serial port
    CloseHandle(serial_handle);
}

int main() {
    receive_and_print_data(); // Receive data from the virtual serial port and print it to stdout
    return 0;
}

