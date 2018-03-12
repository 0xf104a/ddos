#!/bin/bash
echo "Building..."
rm -f ddos
gcc ddos.c util.c message.c socket.c main.c -lpthread -o ddos
