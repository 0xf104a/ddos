rm -f ddos
gcc main.c ddos.c util.c message.c socket.c -lpthread -o ddos
