UDP File Transfer
---
This repo is currently in development
---
Here are the types of messages the program sends for conection and data transfer.
It is laid out with this format, where each [] is a byte, | separates bytes within
a block ([ Description | ... | ... ] is 3 bytes), and each {} is variable size.

type > [Message type]
---

CONNECT > [CONNECT]\n
OK      > [OK]\n
GET     > [GET]{Filename}\n
SIZE    > [SIZE][Amount of packets to be sent | ... ][Size of file in bytes | ... | ... | ... ]\n
DATA    > [DATA][Number of packet in sequence | ... ][Size of payload | ... ][Checksum | ... ]{Payload}\n
CLOSE   > [CLOSE]\n
ERROR   > [ERROR]\n