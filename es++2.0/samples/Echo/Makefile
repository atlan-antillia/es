CC            = g++
CFLAGS        = -DDEBUG -O4 -Wall -I/usr/local/include -I../../include/
#CFLAGS        = -O4 -Wall -I/usr/local/include -I../../include/
DEST          = /usr/local/bin
LDFLAGS       = -L/usr/local/lib
LIBS          = -lm -lpthread

all:    Server Client

Server: Server.cpp
	$(CC) $(CFLAGS) Server.cpp $(LDFLAGS) $(LIBS) -o Server

Client: Client.cpp
	$(CC) $(CFLAGS) Client.cpp $(LDFLAGS) $(LIBS) -o Client

clean:;         rm -f *.o *~ Server Client 


