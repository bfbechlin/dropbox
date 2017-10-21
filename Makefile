CC=g++ -std=c++11
INCLUDE=
FLAGS=-O2 -Wall

CFLAGS=$(FLAGS) $(INCLUDE)

.PHONY: dropboxServer dropboxClient all clean

all: dropboxServer dropboxClient

dropboxServer: util/communication.o server/serverComm.o server/dropboxServer.o
	$(CC) $(CFLAGS) -o $@ $^

dropboxClient: util/communication.o client/dropboxClient.o client/clientComm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ./*/*.o dropboxClient dropboxServer
