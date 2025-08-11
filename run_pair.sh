#!/bin/bash

SEND_NAME=$1
RECV_NAME=$2

if [ -z "$SEND_NAME" ] || [ -z "$RECV_NAME" ]; then
    echo "Usage: $0 <send_shm_name> <recv_shm_name>"
    exit 1
fi

# Start C++ program in the background
bin/game "$SEND_NAME" "$RECV_NAME" &

# Give it a small delay to ensure shm is created before Python connects
sleep 0.2

# Start Python program
python3 Network/client.py "$SEND_NAME" "$RECV_NAME"
