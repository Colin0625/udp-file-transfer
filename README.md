UDP File Transfer
---
This repo is currently in development
---
Here are the types of messages the program sends for conection and data transfer.
It is laid out with this format, where each [] is a byte, | separates bytes within
a block ([ Description | ... | ... ] is 3 bytes), and each {} is variable size.  
  
This program sends all additional data in the headers in little endian form.  
  
type > [Message type]... 
---

CONNECT > [CONNECT]  
OK      > [OK]  
GET     > [GET] [Size of filename] [Size of file extension] {Filename} {File extension}  
SIZE    > [SIZE] [Amount of packets to be sent | ... ] [Size of file in bytes | ... | ... | ... ] [Size of filename] [Size of file extension] {Filename} {File extension}  
DATA    > [DATA] [Number of packet in sequence | ... ] [Size of payload | ... ] [Checksum | ... | ... | ... ] {Payload}  
CLOSE   > [CLOSE]  
ERROR   > [ERROR] {Error message}  
