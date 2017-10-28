CC=g++ -std=gnu++11
INCLUDE=
FLAGS=-O2 -Wall
CFLAGS=$(FLAGS) $(INCLUDE)

UTIL=util/timestamp.o util/file.o util/communication.o

.PHONY: all clean

all: dropboxServer dropboxClient

dropboxServer: $(UTIL) server/serverComm.o server/dropboxServer.o
	$(CC) $(CFLAGS) -o $@ $^

dropboxClient: $(UTIL) client/dropboxClient.o client/clientComm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ./*/*.o dropboxClient dropboxServer
