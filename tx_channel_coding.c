// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdint.h>

// INPUT: Sequence of Frame Bytes in HEX FROM: Framing
// OUTPUT: Sequence of Frame Bits as '0' and '1' TO: Line Coding 

#define CRC8_POLYNOMIAL 0x07

// Function to calculate CRC-8
uint8_t crc8(const uint8_t *data, size_t len) {
    uint8_t crc = 0;
    size_t i, j;
    for (i = 0; i < len; ++i) {
        crc ^= data[i];
        for (j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int main() {
    int c;
    while ((c = getchar()) != EOF) {
        uint8_t data = (uint8_t)c;

        for (int i = 7; i >= 0; --i) {
            putchar(((data >> i) & 0x01) ? '1' : '0');
        }

        uint8_t crc = crc8(&data, sizeof(uint8_t));
        
        for (int i = 7; i >= 0; --i) {
            putchar(((crc >> i) & 0x01) ? '1' : '0');
        }
    }
    return 0;
}