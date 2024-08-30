# RK3588-STM32 Communication

This repository contains code for a simple TCP client-server communication setup between an RK3588 running Ubuntu and an STM32 microcontroller. The RK3588 can operate as both the server and client, facilitating communication with the STM32 over a TCP connection.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Building and Running](#building-and-running)
- [Testing](#testing)
- [Daemonize & Log](#daemonize--log)
- [License](#license)

## Overview

This project demonstrates a basic TCP client-server communication setup where the RK3588 can act as a client or server, enabling communication with an STM32 microcontroller over a TCP connection.

## Features

- **TCP Server**: Listens for incoming connections and processes received messages.
- **TCP Client**: Connects to a server and sends messages.
- **Cross-Compilation Support**: Enables cross-compilation for ARM architectures.
- **Communication Modes**: Supports both sending and receiving test modes (`--send-test` and `--recv-test`).
- **Daemon Mode**: Allows the program to run as a daemon process.
- **Logging**: Supports both console and syslog logging.

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

   Run the following command to build the project:

   ```
   make
   ```
   
   This will compile the source files and generate the binaries in the `build/` directory.

3. **Run the server and client**

   First, start the server program:

   ```
   ./build/combined_program server [--send-test | --recv-test] [--daemon]
   ```
   
   In another terminal, run the client program:

   ```
   ./build/combined_program client <server_address> <port> [--send-test | --recv-test] [--daemon]
   ```
   
   - `--send-test`: Enable sending test mode.
   - `--recv-test`: Enable receiving test mode.
   - `--daemon`: Run the program as a daemon.

   Example:
   
   ```
   ./build/combined_program client 192.168.1.100 8080 --send-test
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
   
   The script will handle starting the server and client, running them with default parameters.

## Daemonize & Log

### Running as a Daemon

To run the program as a daemon:

```
./build/combined_program server --daemon
```

### Checking the Process

You can check if the program is running as a daemon by using:

```
ps aux | grep combined_program
```

### Stopping the Daemon

To stop the running daemon:

```
pkill combined_program
```

### Viewing Logs

If the program is running in daemon mode with syslog enabled, you can view the logs using:

```
sudo journalctl -t mydaemon
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

------

This expanded documentation should provide users with clear instructions and details for setting up, using, and testing the communication system between the RK3588 and STM32.