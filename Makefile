CC=g++
INCLUDE=
FLAGS=-O2 -Wall

CFLAGS=$(FLAGS) $(INCLUDE)

.PHONY: dropboxServer dropboxClient util all clean

all: dropboxServer dropboxClient

dropboxServer: server/dropboxServer.o server/serverComm.o
	$(CC) $(CFLAGS) -o $@ $^

dropboxClient: client/dropboxClient.o client/clientComm.o
	$(CC) $(CFLAGS) -o $@ $^

util: util/communication.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ./*/*.o dropboxClient dropboxServer
