CC            = g++
CFLAGS        = -DDEBUG -O4 -Wall -I/usr/local/include -I../../include/
#CFLAGS        = -O4 -Wall -I/usr/local/include -I../../include/
DEST          = /usr/local/bin
LDFLAGS       = -L/usr/local/lib
LIBS          = -lm -lpthread
TARGET = URLEncoder
all:   $(TARGET) 

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp $(LDFLAGS) $(LIBS) -o $(TARGET)


clean:;         rm -f *.o *~ $(TARGET)


