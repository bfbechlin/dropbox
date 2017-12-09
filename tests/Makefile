CC=g++ -std=gnu++11
INCLUDE=-I../util/ -I../server/ -I../client/ -I../config/
FLAGS=-O2 -Wall
CFLAGS=$(FLAGS) $(INCLUDE)

UTIL=../util/timestamp.o ../util/file.o ../util/foldermanager.o ../util/folderdiff.o\
 ../util/communication.o ../util/activeprocess.o ../util/passiveprocess.o ../util/device.o\
 ../util/action.o ../util/actionqueue.o

FILE_AC=../fac/fileaccesscontrol.o ../fac/filesmanager.o


SERVER=../server/servercomm.o ../server/database.o
CLIENT=../client/clientcomm.o

TESTS=folder_manager.cpp

.PHONY: all clean

all: $(TESTS:.cpp=.out)

%.out: %.cpp $(UTIL) $(SERVER) $(CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.bak *. ../*/*.o *.out
