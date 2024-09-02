# RK3588-STM32 Communication

This repository contains code for a TCP client-server communication setup between an RK3588 running Ubuntu and an STM32 microcontroller. The RK3588 can act as both a server and a client, facilitating communication with the STM32 over a TCP connection.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Testing](#testing)
- [Daemonize & Log](#daemonize--log)
- [License](#license)

## Overview

This project demonstrates a TCP client-server communication setup where the RK3588 can function as either a client or a server. The communication is established over TCP with an STM32 microcontroller. The project includes support for running in interactive mode, daemon mode, and test modes for both sending and receiving data.

## Features

- **TCP Server**: Listens for incoming connections and processes received messages.
- **TCP Client**: Connects to a server and sends messages.
- **Interactive Mode**: Provides an interactive interface for managing communication.
- **Cross-Compilation Support**: Enables cross-compilation for ARM architectures.
- **Communication Modes**: Supports sending and receiving test modes (`--send-test` and `--recv-test`).
- **Daemon Mode**: Allows the program to run as a daemon process.
- **Logging**: Supports both console and syslog logging.
- **Command File Processing**: Processes commands from a file in server mode.

## Setup

### Prerequisites

- GCC Compiler
- Make
- Ubuntu running on RK3588
- STM32 Microcontroller
- Network setup to allow communication between RK3588 and STM32

### Dependencies

Make sure you have the necessary packages installed on your Ubuntu system:

```
sudo apt-get update
sudo apt-get install build-essential
```

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

   - **Start the server program:**

     ```
     ./build/combined_program server [--send-test | --recv-test] [--daemon]
     ```

   - **Run the client program in another terminal:**

     ```
     ./build/combined_program client <server_address> <port> [--send-test | --recv-test] [--daemon]
     ```

   - **Options:**

     - `--send-test`: Enable sending test mode.
     - `--recv-test`: Enable receiving test mode.
     - `--daemon`: Run the program as a daemon.

   **Example:**

   ```
   ./build/combined_program client 192.168.1.100 8080 --send-test
   ```

### Interactive Mode

To enter interactive mode, use:

```
./build/combined_program interactive
```

In interactive mode, the program provides a command-line interface for managing communication.

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

   The script will handle starting the server and client with default parameters.

## Daemonize & Log

### Running as a Daemon

To run the program as a daemon:

```
./build/combined_program server --daemon
```

### Checking the Process

To check if the program is running as a daemon:

```
ps aux | grep combined_program
```

### Stopping the Daemon

To stop the running daemon:

```
pkill combined_program
```

### Viewing Logs

If the program is running in daemon mode with syslog enabled, view the logs using:

```
sudo journalctl -t mydaemon
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

------

