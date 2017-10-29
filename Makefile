CC=g++ -std=gnu++11
INCLUDE=-I./util
FLAGS=-O2 -Wall
CFLAGS=$(FLAGS) $(INCLUDE)

UTIL=util/timestamp.o util/file.o util/foldermanager.o util/filediff.o\
 util/communication.o util/activeprocess.o util/passiveprocess.o

.PHONY: all clean

all: dropboxServer dropboxClient

dropboxServer: $(UTIL) server/database.o server/servercomm.o server/dropboxserver.o
	$(CC) $(CFLAGS) -o $@ $^

dropboxClient: $(UTIL) client/dropboxclient.o client/clientcomm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ./*/*.o dropboxClient dropboxServer
