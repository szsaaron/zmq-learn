#! /bin/sh
gcc client.c -g -o client.out \
-I /usr/include -L /usr/lib/x86_64-linux-gnu -lzmq -lczmq
gcc server.c -g -o server.out \
-I /usr/include -L /usr/lib/x86_64-linux-gnu -lzmq -lczmq
