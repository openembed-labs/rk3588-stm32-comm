# RK3588-STM32 Communication

This repository contains code for a simple TCP client-server communication setup between an RK3588 running Ubuntu and an STM32 microcontroller. The RK3588 acts as both the server and client, facilitating communication with the STM32.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Building and Running](#building-and-running)
- [Testing](#testing)
- [License](#license)

## Overview

This project demonstrates a basic TCP client-server communication setup. The RK3588 runs a program that can act as both a client and a server to communicate with an STM32 microcontroller over a TCP connection.

## Features

- **TCP Server**: Listens for incoming connections and echoes received messages.
- **TCP Client**: Connects to a server and sends messages.
- **Cross-Compilation**: Supports cross-compilation for ARM architectures.
- **Simple Communication Protocol**: Sends and receives messages between the RK3588 and STM32.

## Setup

### Prerequisites

- GCC Compiler
- Make
- Ubuntu running on RK3588
- STM32 Microcontroller
- Network setup to allow communication between RK3588 and STM32

## Usage

### Building and Running

1. **Clone the repository**

   ```
   git clone git@github.com:openembed-labs/rk3588-STM32-comm.git
   cd rk3588-STM32-comm
   ```

2. **Build the programs**

   ```
   make
   ```

3. **Run the server and client**

   First, start the server program:

   ```
   ./build/server_program
   ```

   In another terminal, run the client program:

   ```
   ./build/client_program <server_address> <port> [--daemon]
   ```

## Testing

### Running the Test Script

A test script is provided to automate the process of starting the server and client programs. The script starts the server, waits for it to initialize, and then runs the client.

1. **Make the script executable**

   ```
   chmod +x test.sh
   ```

2. **Run the script**

   ```
   ./test.sh
   ```

## Daemonize & Log
```shell
ps aux | grep combined_program
```

kill
```shell
pkill combined_program # kill 339504
```

Log
```shell
sudo journalctl -t mydaemon
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

