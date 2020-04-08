CC            = g++
CFLAGS        = -DDEBUG -O4 -Wall -I/usr/local/include -I../../include/
#CFLAGS        = -O4 -Wall -I/usr/local/include -I../../include/
DEST          = /usr/local/bin
LDFLAGS       = -L/usr/local/lib
LIBS          = -lm -lpthread

all:   Folder 

Folder: Folder.cpp
	$(CC) $(CFLAGS) Folder.cpp $(LDFLAGS) $(LIBS) -o Folder


clean:;         rm -f *.o *~ Folder


