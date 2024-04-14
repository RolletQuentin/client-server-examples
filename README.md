# Client Server Examples

This repository contains two examples of Client-Server programs in C. The first example demonstrates the use of shared memory, while the second example illustrates socket communication.

## Installation

Clone the repository:
```shell
git clone https://github.com/RolletQuentin/client-server-examples
cd client-server-examples
```

## Shared Memory

### Usage

You have to open two terminals before starting, one for the server, one for the client.

**Build and Run the Server:**

```shell
cd 1-sharedMemory/server
make
make run
```

**Build and Run the Client:**

```shell
cd 1-sharedMemory/client
make
./bin/main "Hello, world! "
```

### How it Works

Shared memory allows multiple processes to share a region of memory, enabling communication between them. In the shared memory example, the server and client processes communicate by reading and writing to a shared memory segment. The server receives a message from the client, reverses it, and sends it back as a response.

## Socket

### Usage

You have to open two terminals before starting, one for the server, one for the client.

**Build and Run the Server:**

```shell
cd 2-socket/server
make
make run
```

**Build and Run the Client:**

```shell
cd 2-socket/server
make
make run
```

You can customize the calculation request in the client program by modifying the regular expression pattern in the `2-socket/client/src/main.c` file (line 51). The regular expression pattern allows for any simple calculation involving two terms and one operation. Simply adjust the pattern to suit your desired calculation format.


### How It Works

Sockets are communication endpoints that allow processes to communicate over a network. In the socket example, the server and client communicate using a TCP/IP socket connection. The client sends a mathematical expression to the server, which evaluates it and sends the result back to the client. This example demonstrates how sockets facilitate inter-process communication over a network, enabling distributed computing.