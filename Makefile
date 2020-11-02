TOP_DIR := $(shell pwd)
CLIENT_APP = client.out

CC = gcc
CFLAGS = -g 
LIBS = -lzmq -lczmq 
LIBS_DIR = -L/usr/lib/x86_64-linux-gnu
HEADERFILE_DIR = -I/usr/include
CLIENT_SRC = demo.c obstreamntp.c md5.c

#all:  $(OBJS)
all:
	$(CC) -g -o $(CLIENT_APP) $(CLIENT_SRC) $(HEADERFILE_DIR) $(LIBS) $(LIBS_DIR) 

clean:
	rm -f *.o *.out 

