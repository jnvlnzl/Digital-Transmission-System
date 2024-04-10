#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Define the virtual serial port device
#define VIRTUAL_SERIAL_PORT "COM8"  // INPUT Port (Current set to COM8 but need to change to respective virtual output port from com0com) 
#define OUTPUT_FILE "received_data.txt"  // File to store received data

// Function to receive data from the virtual serial port and store it in a file
void receive_and_store_data() {
    HANDLE serial_handle; // File descriptor for the serial port

    // Open the virtual serial port for reading
    serial_handle = CreateFile(VIRTUAL_SERIAL_PORT, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serial_handle == INVALID_HANDLE_VALUE) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    // Configure serial port settings
    // Need this for data tranmission to work with Command Prompt, settings taken to match PUTTY program
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(serial_handle, &dcbSerialParams)) {
        perror("Error getting serial port state");
        CloseHandle(serial_handle);
        exit(EXIT_FAILURE);
    }
    dcbSerialParams.BaudRate = CBR_9600; // Set baud rate to 9600
    dcbSerialParams.ByteSize = 8;        // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity = NOPARITY;     // No parity
    if (!SetCommState(serial_handle, &dcbSerialParams)) {
        perror("Error setting serial port state");
        CloseHandle(serial_handle);
        exit(EXIT_FAILURE);
    }

    // Open the output file for writing (Can edit this to take inputs from keyboard)
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        CloseHandle(serial_handle);
        exit(EXIT_FAILURE);
    }

    // Read data from the serial port and write it to the output file
    char received_buffer[256];
    DWORD bytes_read;
    while (1) {
        if (!ReadFile(serial_handle, received_buffer, sizeof(received_buffer), &bytes_read, NULL)) {
            perror("Error reading from virtual serial port");
            fclose(output_file);
            CloseHandle(serial_handle);
            exit(EXIT_FAILURE);
        }
        
        // Print the received data and port information
        printf("Received data from virtual serial port '%s': %.*s\n", VIRTUAL_SERIAL_PORT, (int)bytes_read, received_buffer);
        
        fwrite(received_buffer, 1, bytes_read, output_file); // Write received data to file
        fflush(output_file); // Flush output buffer to ensure data is written immediately
    }

    // Close the output file and the serial port
    fclose(output_file);
    CloseHandle(serial_handle);
}

/*
int main() {
    receive_and_store_data(); // Receive data from the virtual serial port and store it in a file
    return 0;
}
*/

