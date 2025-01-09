# Serial Digital Transmission System

The design for this project is taken from the block diagram provided below which illustrates a one way messaging system that transmits data through the use of a structured and modular design. 

This design essentially splits the system into two notable parts: the transmitter side and receiver side. On the transmitter side, operations such as format, source coding, framing, channel coding, line coding and UART-TX are represented as blocks. The receiver side continues from the end of the transmitter side starting from UART-RX, line decoding, framing, channel decoding, source decoding and format. The channel used in this system is implemented  using a null-modem emulator .

![image](https://github.com/user-attachments/assets/76ce0111-ab1c-426f-bdab-c9f9bc02cfba)

Application: one way messaging <br>
Line coding: BAMI-HDB3 <br>
Source coding: Compression (huffman) <br> 
Channel coding: CRC-8 <br>
Maximum data block size: Compression (huffman) <br>
Sequence Number: Maximum sequence number: 8000 <br>
