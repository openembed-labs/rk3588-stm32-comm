#!/bin/bash

# Set default server address and port
SERVER_ADDRESS="127.0.0.1"
SERVER_PORT=5000

# Start the server
echo "Starting server program..."
./build/server_program $SERVER_ADDRESS $SERVER_PORT &
SERVER_PID=$!

# Wait for the server to start
sleep 2

# Start the client
echo "Starting client program..."
./build/client_program $SERVER_ADDRESS $SERVER_PORT <<EOF
----------Hello, server!------------
quit
EOF

# Stop the server
echo "Stopping server program..."
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null

echo "Done!"
