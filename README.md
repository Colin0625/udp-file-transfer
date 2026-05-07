# UDP File Transfer

This is my first networking project! It is a very 
simple file transfer program that operates over UDP.
This was a small project, so I told Codex to write
me a README, because it would do a better job explaining
the code. Everything below this point is from Codex, and it definitely explains things pretty accurately.

---

## Codex README

This project is a small C++20 file-transfer program built directly on UDP sockets. It includes a server, a client, a custom binary packet format, and session logic for requesting a file from the server and reconstructing it on the client.

The code is organized as a learning-oriented transport layer rather than a production file-transfer tool. It demonstrates packet serialization, socket address handling, a receive thread with a blocking queue, simple client/server state machines, and binary file chunking.

## What It Does

- Runs a UDP server that listens on port `5000`.
- Runs a UDP client that connects to a configured server address.
- Performs a basic `SYN` / `SYNACK` / `ACK` handshake.
- Lets the client request a file with a `GET` packet.
- Looks for requested files under `test-files/` on the server.
- Sends file metadata before the file body.
- Splits file contents into fixed-size `DATA` packets.
- Writes the received file under `test-output/` on the client.

## Project Layout

```text
include/
  net/          UDP socket and IPv4 address wrappers
  packaging/   File chunking and file reassembly helpers
  protocol/    Packet header, message types, and serialization
  transfer/    Client/server session state machines and endpoint logic
  utility/     Thread-safe queue used by the receive loop

src/
  client_main.cpp
  server_main.cpp
  net/
  packaging/
  protocol/
  transfer/
  utility/

test-files/    Sample files served by the server
test-output/   Files reconstructed by the client
```

## Packet Format

Each packet has an 11-byte header followed by an optional payload.

```text
byte 0      message type
bytes 1-2   payload size
bytes 3-6   sequence number
bytes 7-10  checksum
bytes 11+   payload
```

Supported message types are:

```text
SYN, ACK, SYNACK, GET, METADATA, DATA, COMPLETE, RETRANSMIT, ERROR, CLOSE, CLOSEACK
```

The active file-transfer path currently uses `SYN`, `SYNACK`, `ACK`, `GET`, `METADATA`, `DATA`, and `ERROR`.

## Build

The makefile uses `g++-13` and C++20.

```sh
make
```

This builds:

- `server.out`
- `client.out`

To remove build artifacts:

```sh
make clean
```

## Run

Start the server first:

```sh
./server.out
```

Then run the client with the name of a file available in `test-files/`:

```sh
./client.out cpp.png
```

The client writes the result to:

```text
test-output/cpp.png
```

## Configuration Notes

The server binds to UDP port `5000`.

The client server address is currently hardcoded in `src/client_main.cpp`:

```cpp
SocketAddress server_addr(5000, "XXX.XXX.XXX.XXX");
```

For local testing on one machine, change that to:

```cpp
SocketAddress server_addr = SocketAddress::localhost(5000);
```

## Tests and Utilities

The makefile includes a `test` target for the file packaging and reassembly path:

```sh
make test
./test.out
```

There is also a standalone queue test source at `src/utility/queue_test.cpp`, but it is not currently wired into the makefile.

## Current Limitations

This project uses UDP, so packet loss, packet reordering, and duplicate packets are possible. The code includes message types for retransmission and completion, but the current transfer flow does not implement a full reliability layer. In normal use it sends data packets sequentially and writes them by sequence number.

Other current constraints:

- One active client session is tracked by the server.
- The client output directory must exist.
- The client server address is compiled in.
- The server only serves files from `test-files/`.
- The transfer loop does not currently verify that every packet arrived before finishing.

## Requirements

- Linux or another POSIX-like environment with UDP socket APIs
- `make`
- `g++-13` or another C++20-capable compiler, with the makefile adjusted if needed
