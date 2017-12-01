CC=g++
INCLUDE=-I./util
FLAGS=-O2 -Wall -pthread -std=gnu++11 -Wl,--no-as-needed
CFLAGS=$(FLAGS) $(INCLUDE)

UTIL=util/timestamp.o util/file.o util/foldermanager.o util/filediff.o\
 util/communication.o util/activeprocess.o util/passiveprocess.o util/device.o\
 util/action.o util/actionqueue.o

.PHONY: all clean

all: dropboxServer dropboxClient

dropboxServer: $(UTIL) server/database.o server/serveruser.o server/loggedusers.o\
 server/servercomm.o server/dropboxserver.o
	$(CC) $(CFLAGS) -o $@ $^

dropboxClient: $(UTIL) client/clientuser.o client/dropboxclient.o client/clientcomm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ./*/*.o dropboxClient dropboxServer
