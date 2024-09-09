# RK3588-STM32 Communication

This repository contains code for a TCP client-server communication setup between an RK3588 running Ubuntu and an STM32 microcontroller. The RK3588 can act as both a client and a server, facilitating communication with the STM32 over a TCP connection.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Testing](#testing)
- [Daemonize & Log](#daemonize--log)
- [Interactive Mode](#interactive-mode)
- [License](#license)

## Overview

This project demonstrates a TCP client-server communication setup where the RK3588 can function as either a client or a server. The communication is established over TCP with an STM32 microcontroller. The project includes support for running in interactive mode, daemon mode, and test modes for both sending and receiving data.

## Features

- **TCP Server**: Listens for incoming connections and processes received messages.
- **TCP Client**: Connects to a server and sends messages.
- **Interactive Mode**: Provides an interactive interface for managing communication and performing upgrades.
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

```sh
sudo apt-get update
sudo apt-get install build-essential
```

## Usage

### Building and Running

1. **Clone the repository**

   ```sh
   git clone git@github.com:openembed-labs/rk3588-STM32-comm.git
   cd rk3588-STM32-comm
   ```

2. **Build the programs**

   Run the following command to build the project:

   ```sh
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

   ```sh
   ./build/combined_program client 192.168.1.100 8080 --send-test
   ```

### Interactive Mode

To enter interactive mode, use:

```sh
./build/combined_program interactive
```

In interactive mode, the program provides a command-line interface for managing communication. You can perform various actions, including executing upgrade commands.

#### Upgrade Commands in Interactive Mode

While in interactive mode, you can manage firmware upgrades using the following commands:

1. **Prepare for Upgrade**

   Make sure the new firmware file is available and accessible. The firmware file should be placed in the designated directory where the interactive mode can access it.

2. **Initiate Upgrade**

   To start an upgrade, use the following command within the interactive mode:

   ```sh
   /upgrade <firmware_file_path>
   ```

   Replace `<firmware_file_path>` with the path to the new firmware file.

   Example command:

   ```sh
   /upgrade /path/to/new_firmware.bin
   ```

3. **Monitor and Confirm**

   The interactive mode will display the status and progress of the upgrade. You can monitor the process and confirm successful completion.

4. **Post-Upgrade**

   After the upgrade is completed, you may need to restart the system or application to apply the changes.

### Note

- Ensure that the firmware file is compatible with your system to avoid any issues during the upgrade process.
- Backup your current firmware and settings before initiating an upgrade.

## Testing

### Running the Test Script

A test script is provided to automate the process of starting the server and client programs. The script starts the server, waits for it to initialize, and then runs the client.

1. **Make the script executable**

   ```sh
   chmod +x test.sh
   ```

2. **Run the script**

   ```sh
   ./test.sh
   ```

   The script will handle starting the server and client with default parameters.

## Daemonize & Log

### Running as a Daemon

To run the program as a daemon:

```sh
./build/combined_program server --daemon
```

### Checking the Process

To check if the program is running as a daemon:

```sh
ps aux | grep combined_program
```

### Stopping the Daemon

To stop the running daemon:

```sh
pkill combined_program
```

### Viewing Logs

If the program is running in daemon mode with syslog enabled, view the logs using:

```sh
sudo journalctl -t mydaemon
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
