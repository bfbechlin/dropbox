CC=g++ -std=gnu++11
INCLUDE=-I../util/ -I../server/ -I../client/ -I../config/
FLAGS=-O2 -Wall
CFLAGS=$(FLAGS) $(INCLUDE)

UTIL=../util/timestamp.o ../util/file.o ../util/foldermanager.o ../util/filediff.o\
 ../util/communication.o ../util/activeprocess.o ../util/passiveprocess.o

SERVER=../server/servercomm.o ../server/database.o
CLIENT=../client/clientcomm.o

TESTS=commserver.cpp commclient.cpp

.PHONY: all clean

all: $(TESTS:.cpp=.out)

%.out: %.cpp $(UTIL) $(SERVER) $(CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ../*/*.o *.out
