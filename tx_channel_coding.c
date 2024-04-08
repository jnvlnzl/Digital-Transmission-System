// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdlib.h>

// INPUT: Sequence of Frame Bytes in HEX
// FROM: FRAMING 

typedef unsigned char uint8_t;
#define CRC_POLYNOMIAL 0x07; 
// sabi ng ppt ni sir 0x8F pero sabi ng google at ng mga crc calculators online ay 0x07

uint8_t crc8(const uint8_t *data, size_t len) {
    uint8_t crc = 0;
    size_t i, j;

    for (i = 0; i < len; ++i) {
        crc ^= data[i];

        for (j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc; 
}

// to be removed

int main() {
    uint8_t data[] = {0x01, 0x02, 0x03}; // sample input to test operation
    size_t len = sizeof(data) / sizeof(data[0]); 

    uint8_t crc = crc8(data, len); 

    printf("CRC-8: "); // display binary result
    for (int i = 7; i >= 0; --i) {
        printf("%d", (crc >> i) & 1);
    }

    return 0; 
}
