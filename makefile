CC = gcc
CFLAGS = -Wall

all: proxy client1 server client2

proxy: proxy.o log_functions.o
	$(CC) $(CFLAGS) -o proxy proxy.o log_functions.o

client1: client1.o log_functions.o
	$(CC) $(CFLAGS) -o client1 client1.o log_functions.o

server: server.o log_functions.o
	$(CC) $(CFLAGS) -o server server.o log_functions.o

client2: client2.o log_functions.o
	$(CC) $(CFLAGS) -o client2 client2.o log_functions.o

log_functions.o: log_functions.c
	$(CC) $(CFLAGS) -c log_functions.c

clean:
	rm -f *.o proxy client1 server client2 