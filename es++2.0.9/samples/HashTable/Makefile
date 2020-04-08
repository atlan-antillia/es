CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib
#LIB = -lXm -lXt -lX11
LIB = 
TARGET = HashTable

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFALG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

