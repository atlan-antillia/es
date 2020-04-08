CC            = g++
CFLAGS        = -DDEBUG -O4 -Wall -I/usr/local/include -I../../include/
#CFLAGS        = -O4 -Wall -I/usr/local/include -I../../include/
DEST          = /usr/local/bin
LDFLAGS       = -L/usr/local/lib
LIBS          = -lrt -lm -lpthread
TARGET1 = Writer
TARGET2 = Reader
all:   $(TARGET1) $(TARGET2) 

$(TARGET1): $(TARGET1).cpp
	$(CC) $(CFLAGS) $(TARGET1).cpp $(LDFLAGS) $(LIBS) -o $(TARGET1) 

$(TARGET2): $(TARGET2).cpp
	$(CC) $(CFLAGS) $(TARGET2).cpp $(LDFLAGS) $(LIBS) -o $(TARGET2) 

clean:;         rm -f *.o *~ $(TARGET1) $(TARGET2)



